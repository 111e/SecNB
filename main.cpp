#include <cstdio>
#include <unistd.h>
#include <time.h>
#include"NB.h"
#include"tool.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


using namespace std;


int main()
{
    int shmid1, shmid2, i;
    pid_t pid;
    CSEM sem;

	double features[DATA_NUM * ATTRIBUTE_NUM], features1[DATA_NUM * ATTRIBUTE_NUM],features2[DATA_NUM * ATTRIBUTE_NUM];
	//double category_num[9] = { 5,3,5,4,4,3,2,3,3 };//nursery 
	//double category_num[5] = { 3,5,5,5,5 };  //balance-scale
	//double category_num[23] = { 2,6,4,9,2,9,2,2,2,12,2,5,4,4,9,9,1,4,3,5,9,6,7 };  //Mushroom
	double category_num[10] = { 2,10,10,10,10,10,10,10,10,10 }; //breast-cancer-wisconsin
	//double category_num[7] = { 4,4,4,4,3,3,3 };//car
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);

	double Label_table1[(getmax(category_num)+1) * ATTRIBUTE_NUM], Label_table2[(getmax(category_num) + 1) * ATTRIBUTE_NUM];
	read_features(features, features1, features2, "/home/amax/zxl/projects/secNB/Data/breast-cancer-wisconsin.txt", DATA_NUM, ATTRIBUTE_NUM);
	Scrambling_category(category_num, Label_table1, Label_table2);
	
	gettimeofday(&end1, NULL);
	printf("train user   us: %ld\n", 2 * ((end1.tv_sec * 1000000 + end1.tv_usec) - (start1.tv_sec * 1000000 + start1.tv_usec)));

	struct timeval start, end;
	gettimeofday(&start, NULL);

	double  query[QUERY_NUM* ATTRIBUTE_NUM], query1[QUERY_NUM * ATTRIBUTE_NUM], query2[QUERY_NUM * ATTRIBUTE_NUM];
	read_features(query, query1, query2, "/home/amax/zxl/projects/secNB/Data/q.txt", QUERY_NUM, ATTRIBUTE_NUM);

	gettimeofday(&end, NULL);
	printf("user   us: %ld\n", 2*((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec )));


	double  a[3], a_1[3], a_2[3], b[3], b_1[3], b_2[3], c[3], c_1[3], c_2[3], t[3], t_1[3], t_2[3],
			r_1[3], r_2[3], cc[3], cc_1[3], cc_2[3], k[3], k_1[3], k_2[3], rr[3], rr_1[3], rr_2[3],sgn_rr_1[3], sgn_rr_2[3] ;
	generate_random(a, a_1, a_2, b, b_1, b_2, c, c_1, c_2, t, t_1, t_2, r_1, r_2, cc, cc_1, cc_2, k, k_1, k_2, rr, rr_1, rr_2, sgn_rr_1, sgn_rr_2);

	if ((shmid1 = shmget((key_t)0x5006, 99995955000, 0666 | IPC_CREAT)) == -1)
	{
		printf("shmat(0x5006) failed\n"); return -1;
	}
	if ((shmid2 = shmget((key_t)0x5007, 99995955000, 0666 | IPC_CREAT)) == -1)
	{
		printf("shmat(0x5007) failed\n"); return -1;
	}
	double* ptext1 = 0;   // 用于指向共享内存的指针
	ptext1 = (double*)shmat(shmid1, 0, 0);
	double* ptext2 = 0;   // 用于指向共享内存的指针
	ptext2 = (double*)shmat(shmid2, 0, 0);

	if (sem.init(0x5000) == false)  printf("sem.init failed.\n");
	else printf("sem.init ok\n");


	for (i = 0; i < 2; i++)
	{
		if ((pid = fork()) == 0)
		{
			break;//子进程出口
		}
	}


	if (i == 0)   // 兄进程读数据
	{
		if (sem.wait() == false)  printf("sem.wait failed.\n");
		else printf("1A  sem.wait ok\n");

		vector<PPT> PPtable1;
		vector<CPT> CPtable1;
		//训练阶段
		Training1(sem, ptext1, ptext2, features1, Label_table1, PPtable1, CPtable1, a_1, b_1, c_1, rr_1, sgn_rr_1, k_1, t_1);
		cout << "Training 1  **********" << endl;

		//write_Ptable(PPtable1, CPtable1, "/home/amax/zxl/projects/secNB/Ptable/PPtable1.txt", "/home/amax/zxl/projects/secNB/Ptable/CPtable1.txt");

		//预测阶段
		//read_Ptable(PPtable1, CPtable1, "/home/amax/zxl/projects/secNB/Ptable/PPtable1.txt", "/home/amax/zxl/projects/secNB/Ptable/CPtable1.txt");
		double result1[QUERY_NUM];

		Classification1(sem, ptext1, ptext2, PPtable1, CPtable1, Label_table1, query1, result1, a_1, b_1, c_1, cc_1, r_1, t_1);
		//for (int q = 0; q < QUERY_NUM; q++) cout << "q:  " << q << "   result1[q]  " << result1[q] << endl;
		write_result(result1, "/home/amax/zxl/projects/secNB/Data/result1.txt");


		
	}
	else if (i == 1)
	{
		
		vector<PPT> PPtable2;
		vector<CPT> CPtable2;
		//训练阶段

		
		Training2(sem, ptext1, ptext2, features2, Label_table2, PPtable2, CPtable2, a_2, b_2, c_2, rr_2, sgn_rr_2, k_2, t_2);
		cout << "Training 2  **********" << endl;
		
		//write_Ptable(PPtable2, CPtable2, "/home/amax/zxl/projects/secNB/Ptable/PPtable2.txt", "/home/amax/zxl/projects/secNB/Ptable/CPtable2.txt");

		//预测阶段
		//read_Ptable(PPtable2, CPtable2, "/home/amax/zxl/projects/secNB/Ptable/PPtable2.txt", "/home/amax/zxl/projects/secNB/Ptable/CPtable2.txt");
		double result2[QUERY_NUM];


		Classification2(sem, ptext1, ptext2, PPtable2, CPtable2, Label_table2, query2, result2, a_2, b_2, c_2, cc_2, r_2, t_2);
		//for (int q = 0; q < QUERY_NUM; q++) cout << "q:  " << q << "   result2[q]  " << result2[q] << endl;
		write_result(result2, "/home/amax/zxl/projects/secNB/Data/result2.txt");
		

	}







	if (shmctl(shmid1, IPC_RMID, 0) == -1)
	{
		printf("shmctl(0x5006) failed\n"); return -1;
	}
	if (shmctl(shmid2, IPC_RMID, 0) == -1)
	{
		printf("shmctl(0x5007) failed\n"); return -1;
	}
    return 0;
}