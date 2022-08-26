#include"NB.h"
#include"tool.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<fstream>
#include <iostream>
#include<iomanip>
#include <sys/time.h>
using namespace std;

void write_result(double* result, const char* file) {
	ofstream dout(file);
	for (int i = 0; i < QUERY_NUM; i++) {
		dout << result[i] << endl;
	}
	dout.close();
}


void write_Ptable(vector<PPT>& PPtable, vector<CPT>& CPtable, const char* Pfile, const char* Cfile)
{
	ofstream dout1(Pfile);
	dout1 << PPtable.size() << endl;
	for (int i = 0; i < (int)PPtable.size(); i++)
	{
		//cout << "classes  " << Ptable[i].classes << "     attribute  " << Ptable[i].attribute << "     attribute_category  " << Ptable[i].attribute_category << "     probability  " << Ptable[i].probability << endl;

		dout1 << PPtable[i].classes << " " << PPtable[i].probability << endl;
	}
	dout1.close();

	ofstream dout2(Cfile);
	dout2 << CPtable.size() << endl;
	for (int i = 0; i < (int)CPtable.size(); i++)
	{
		//cout << "classes  " << Ptable[i].classes << "     attribute  " << Ptable[i].attribute << "     attribute_value  " << Ptable[i].attribute_value << "     probability  " << Ptable[i].probability << endl;

		dout2 <<  CPtable[i].classes << " " << CPtable[i].attribute << " " << CPtable[i].attribute_value << " " << CPtable[i].probability << endl;
	}
	dout2.close();
}
void read_Ptable(vector<PPT>& PPtable, vector<CPT>& CPtable, const char* Pfile, const char* Cfile) {
	ifstream in1(Pfile);
	int PPtable_size;
	in1 >> PPtable_size;
	for (int i = 0; i < PPtable_size; i++) {
		PPT pro;
		in1 >> pro.classes >> pro.probability;
		PPtable.push_back(pro);
	}
	in1.close();

	ifstream in2(Cfile);
	int CPtable_size;
	in2 >> CPtable_size;
	for (int i = 0; i < CPtable_size; i++) {
		CPT pro;
		in2 >> pro.classes >> pro.attribute >> pro.attribute_value >>  pro.probability;
		CPtable.push_back(pro);
	}
	in2.close();
}

void Training1(CSEM sem, double* ptext1, double* ptext2, double* features1, double* Label_table1, vector<PPT>& PPtable1, vector<CPT>& CPtable1, double* a_1, double* b_1, double* c_1, double* rr_1, double* sgn_rr_1, double* k_1, double* t_1) {
	PPT prior1;
	CPT condition1;
	double classes_num1, attribute_num1;
	int maxtype =  getmax(Label_table1);
	double a1[(int)(Label_table1[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];
	double a2[(int)(Label_table1[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];
	double a3[(int)(Label_table1[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];

	struct timeval start, end;
	gettimeofday(&start, NULL);

	cout << fixed << setprecision(8);
	for (int d = 1; d <= Label_table1[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table1[n]; j++) {
				for (int k = 0; k < DATA_NUM; k++) {
					//cout << "features1[k * ATTRIBUTE_NUM]      " << features1[k * ATTRIBUTE_NUM] << "   features1[k * ATTRIBUTE_NUM + n]" << features1[k * ATTRIBUTE_NUM + n] << endl;
					//先验概率计数
					secCmp_closet1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], features1[k * ATTRIBUTE_NUM], Label_table1[d * ATTRIBUTE_NUM], rr_1[0]);
					secCmp_closet1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], Label_table1[d * ATTRIBUTE_NUM], features1[k * ATTRIBUTE_NUM], rr_1[0]);
					//条件概率计数
					secCmp_closet1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], features1[k * ATTRIBUTE_NUM + n], Label_table1[j * ATTRIBUTE_NUM + n], rr_1[0]);
					secCmp_closet1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], Label_table1[j * ATTRIBUTE_NUM + n], features1[k * ATTRIBUTE_NUM + n], rr_1[0]);
				}
			}
		}
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int d = 1; d <= Label_table1[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table1[n]; j++) {
				for (int k = 0; k < DATA_NUM; k++) {
					secCmp_closet1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], k_1[0]);
					secCmp_closet1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], k_1[0]);
					secCmp_closet1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], k_1[0]);
					secCmp_closet1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0], k_1[0]);
				}
			}
		}
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int d = 1; d <= Label_table1[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table1[n]; j++) {
				for (int k = 0; k < DATA_NUM; k++) {
					secCmp_closet1_3(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, a1, a2, a3, sgn_rr_1[0]);
					secCmp_closet1_3(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, a1, a2, a3, sgn_rr_1[0]);
					secCmp_closet1_3(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, a1, a2, a3, sgn_rr_1[0]);
					secCmp_closet1_3(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3,
						ptext1, ptext2, a1, a2, a3, sgn_rr_1[0]);
					//两次比较结果相乘，等于+1
					secMul1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0],
						a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0],
						a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1]);
					secMul1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
							ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0],
							a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2],
							a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3]);
				}
			}
		}
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int d = 1; d <= Label_table1[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table1[n]; j++) {
				classes_num1 = 0;
				for (int k = 0; k < DATA_NUM; k++) {
					secMul1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0]);
					classes_num1 += a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4];
					secMul1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0]);
					//条件概率，用乘积表示嵌套关系
					secMul1_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0],
						a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0],
						a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2]);
				}
			}
		}
		cout << "classes_num1  " << classes_num1 << endl;
		prior1.classes = Label_table1[d * ATTRIBUTE_NUM];
		prior1.probability = classes_num1 / DATA_NUM;
		PPtable1.push_back(prior1);
		cout << "classes1  " << prior1.classes << "     probability  " << prior1.probability << endl;
	
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int d = 1; d <= Label_table1[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table1[n]; j++) {
				classes_num1 = 0;
				attribute_num1 = 0;
				for (int k = 0; k < DATA_NUM; k++) {
					classes_num1 += a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4];
					secMul1_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, a1, a2, a3, a_1[0], b_1[0], c_1[0]);
					attribute_num1 += a3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1];
				}
				condition1.attribute = n;
				condition1.attribute_value = Label_table1[j * ATTRIBUTE_NUM + n];
				condition1.classes = Label_table1[d * ATTRIBUTE_NUM];
				condition1.attribute_num = attribute_num1;
				condition1.classes_num = classes_num1;
				CPtable1.push_back(condition1);
				//cout << "classes1  " << condition1.classes << "     attribute  " << condition1.attribute << "     attribute_category  " << condition1.attribute_value << "     attribute_num  " << condition1.attribute_num << endl;

			}
		}
	}
	double thresh = 0.5;
	for (int i = 0; i < CPtable1.size(); i++) {
		secCmp1_1(i, ptext1, ptext2, a1, a2, a3, a_1[1], b_1[1], c_1[1], CPtable1[i].attribute_num, thresh, t_1[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int i = 0; i < CPtable1.size(); i++) {
		secCmp1_2(i, ptext1, ptext2, a1, a2, a3, a_1[1], b_1[1], c_1[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int i = 0; i < CPtable1.size(); i++) {
		secCmp1_3(i, ptext1, ptext2, a1, a2, a3);
		if (a1[i] == -1)  CPtable1[i].attribute_num = 0.00005;
	}

	for (int i = 0; i < CPtable1.size(); i++) {
		secDiv1_1(i, ptext1, ptext2, a1, a2, a3, a_1[1], b_1[1], c_1[1], CPtable1[i].attribute_num, t_1[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	double tx_1[CPtable1.size()];
	for (int i = 0; i < CPtable1.size(); i++) {
		secDiv1_2(i, ptext1, ptext2, a1, a2, a3, a_1[1], b_1[1], c_1[1], tx_1, CPtable1[i].classes_num, t_1[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int i = 0; i < CPtable1.size(); i++) {
		secDiv1_3(i, ptext1, ptext2, a1, a2, a3, a_1[1], b_1[1], c_1[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int i = 0; i < CPtable1.size(); i++) {
		secDiv1_4(i, ptext1, ptext2, a1, a2, a3, tx_1);
		CPtable1[i].probability = a1[i];
		//cout << "classes1  " << CPtable1[i].classes << "     attribute  " << CPtable1[i].attribute << "     attribute_category  " << CPtable1[i].attribute_value << "     attribute_num  " << CPtable1[i].attribute_num << "  probability: " << CPtable1[i].probability << endl;

	}
	gettimeofday(&end, NULL);
	printf("Training   us: %ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

}

void Training2(CSEM sem, double* ptext1, double* ptext2, double* features2, double* Label_table2, vector<PPT>& PPtable2, vector<CPT>& CPtable2, double* a_2, double* b_2, double* c_2, double* rr_2, double* sgn_rr_2, double* k_2, double* t_2) {
	PPT prior2;
	CPT condition2;
	double classes_num2, attribute_num2;
	int maxtype = getmax(Label_table2);
	double b1[(int)(Label_table2[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];
	double b2[(int)(Label_table2[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];
	double b3[(int)(Label_table2[0] + 1) * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4];
	cout << fixed << setprecision(8);

	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int d = 1; d <= Label_table2[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table2[n]; j++) {
				for (int k = 0; k < DATA_NUM; k++) {
					//先验概率计数
					secCmp_closet2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0], features2[k * ATTRIBUTE_NUM], Label_table2[d * ATTRIBUTE_NUM], rr_2[0], k_2[0]);
					secCmp_closet2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0], Label_table2[d * ATTRIBUTE_NUM], features2[k * ATTRIBUTE_NUM], rr_2[0],  k_2[0]);
					//条件概率计数
					secCmp_closet2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0], features2[k * ATTRIBUTE_NUM + n], Label_table2[j * ATTRIBUTE_NUM + n], rr_2[0], k_2[0]);
					secCmp_closet2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0], Label_table2[j * ATTRIBUTE_NUM + n], features2[k * ATTRIBUTE_NUM + n], rr_2[0], k_2[0]);
				}
			}
		}
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int d = 1; d <= Label_table2[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table2[n]; j++) {
				for (int k = 0; k < DATA_NUM; k++) {
					secCmp_closet2_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, b1, b2, b3, sgn_rr_2[0]);
					secCmp_closet2_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, b1, b2, b3, sgn_rr_2[0]);
					secCmp_closet2_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, b1, b2, b3, sgn_rr_2[0]);
					secCmp_closet2_2(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3,
						ptext1, ptext2, b1, b2, b3, sgn_rr_2[0]);
				}
			}
		}
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int d = 1; d <= Label_table2[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table2[n]; j++) {
				classes_num2 = 0;
				attribute_num2 = 0;
				for (int k = 0; k < DATA_NUM; k++) {
					//两次比较结果相乘，等于+1
					secMul2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1]);
					classes_num2 += b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0];
					secMul2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 3]);
				}
			}
		}
		//cout <<"classes_num2  " << classes_num2 << endl;
		prior2.classes = Label_table2[d * ATTRIBUTE_NUM];
		prior2.probability = classes_num2 / DATA_NUM;
		PPtable2.push_back(prior2);
		cout << "classes2  " << prior2.classes  << "     probability  " << prior2.probability << endl;

	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int d = 1; d <= Label_table2[0]; d++) {
		for (int n = 1; n < ATTRIBUTE_NUM; n++) {
			for (int j = 1; j <= Label_table2[n]; j++) {
				classes_num2 = 0;
				attribute_num2 = 0;
				for (int k = 0; k < DATA_NUM; k++) {
					//条件概率，用乘积表示嵌套关系
					secMul2_1(d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1,
						ptext1, ptext2, b1, b2, b3, a_2[0], b_2[0], c_2[0],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0],
						b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 2]);
					attribute_num2 += b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 1];
					classes_num2 += b3[d * ATTRIBUTE_NUM * (maxtype + 1) * DATA_NUM * 4 + n * (maxtype + 1) * DATA_NUM * 4 + j * DATA_NUM * 4 + k * 4 + 0];
				}
				condition2.attribute = n;
				condition2.attribute_value = Label_table2[j * ATTRIBUTE_NUM + n];
				condition2.classes = Label_table2[d * ATTRIBUTE_NUM];
				condition2.attribute_num = attribute_num2;
				condition2.classes_num = classes_num2;
				CPtable2.push_back(condition2);
				//cout << "classes2  " << condition2.classes << "     attribute  " << condition2.attribute << "     attribute_category  " << condition2.attribute_value<< "     attribute_num  " << condition2.attribute_num << endl;

			}
		}
		
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	double thresh = 0.5;
	for (int i = 0; i < CPtable2.size(); i++) {
		secCmp2_1(i, ptext1, ptext2, b1, b2, b3, a_2[1], b_2[1], c_2[1], CPtable2[i].attribute_num, thresh,t_2[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int i = 0; i < CPtable2.size(); i++) {
		secCmp2_2(i, ptext1, ptext2, b1, b2, b3);
		if (b1[i] == -1)  CPtable2[i].attribute_num = 0.00005;

	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	double tx_2[CPtable2.size()];
	for (int i = 0; i < CPtable2.size(); i++) {
		secDiv2_1(i, ptext1, ptext2, b1, b2, b3, a_2[1], b_2[1], c_2[1], tx_2, CPtable2[i].attribute_num, t_2[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int i = 0; i < CPtable2.size(); i++) {
		secDiv2_2(i, ptext1, ptext2, b1, b2, b3, a_2[1], b_2[1], c_2[1], CPtable2[i].classes_num, t_2[1]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int i = 0; i < CPtable2.size(); i++) {
		secDiv2_3(i, ptext1, ptext2, b1, b2, b3, tx_2);
		CPtable2[i].probability = b1[i];
		//cout << "classes2  " << CPtable2[i].classes << "     attribute  " << CPtable2[i].attribute << "     attribute_category  " << CPtable2[i].attribute_value<< "     attribute_num  " << CPtable2[i].attribute_num << "  probability: " << CPtable2[i].probability<< endl;

	}
	
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
}

void Classification1(CSEM sem, double* ptext1, double* ptext2, vector<PPT>& PPtable1, vector<CPT>& CPtable1, double* Label_table1, double* query1, double* result1, double* a_1, double* b_1, double* c_1, double* cc_1, double* r_1, double* t_1) {
	double* posterior_probability1 = new double[QUERY_NUM * ((int)Label_table1[0] + 1)];
	double a1[QUERY_NUM * ((int)Label_table1[0] + 1) * CPtable1.size() * ATTRIBUTE_NUM * 3];
	double a2[QUERY_NUM * ((int)Label_table1[0] + 1) * CPtable1.size() * ATTRIBUTE_NUM * 3];
	double a3[QUERY_NUM * ((int)Label_table1[0] + 1) * CPtable1.size() * ATTRIBUTE_NUM * 3];

	struct timeval start, end;
	gettimeofday(&start, NULL);

	for (int i = 0; i < PPtable1.size(); i++) {
		secLog1_1(i, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], PPtable1[i].probability, cc_1[2]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int i = 0; i < PPtable1.size(); i++) {
		secLog1_2(i, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], r_1[2]);
		PPtable1[i].probability = a2[i];
		//cout << "class1  " << PPtable1[i].classes << " probability   " << PPtable1[i].probability << endl;

	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");

	for (int i = 0; i <  CPtable1.size(); i++) {
		secLog1_1(i, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], CPtable1[i].probability, cc_1[2]);
	}

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	
	for (int i = 0; i <  CPtable1.size(); i++) {
		secLog1_2(i, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], r_1[2]);
		CPtable1[i].probability = a2[i];
		//cout << "class1  " << " probability   " << CPtable1[i].probability << endl;
		//cout << "classes1  " << CPtable1[i].classes << "     attribute  " << CPtable1[i].attribute << "     attribute_category  " << CPtable1[i].attribute_value << "     attribute_num  " << CPtable1[i].attribute_num << "  probability: " << CPtable1[i].probability << endl;

	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int q = 0; q < QUERY_NUM; q++) {
		for (int d = 1; d <= Label_table1[0]; d++) {
			posterior_probability1[q * ((int)Label_table1[0] + 1) + d] = PPtable1[d - 1].probability;
			//cout << "1--posterior  "<< q * ((int)Label_table1[0] + 1) + d <<"   probability  " << PPtable1[d - 1].probability << endl;
			for (int n = 1; n < ATTRIBUTE_NUM; n++) {
				double nn = n / 1;
				for (int i = 0; i < CPtable1.size(); i++) {
					secCmp1_1(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 0, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], CPtable1[i].classes, Label_table1[d * ATTRIBUTE_NUM], t_1[2]);
					secCmp1_1(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 1, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], CPtable1[i].attribute, nn, t_1[2]);
					secCmp1_1(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 2, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], CPtable1[i].attribute_value, query1[q * ATTRIBUTE_NUM + (int)n], t_1[2]);
				}
			}
		}
	}
	
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int q = 0; q < QUERY_NUM; q++) {
		for (int d = 1; d <= Label_table1[0]; d++) {
			posterior_probability1[q * ((int)Label_table1[0] + 1) + d] = PPtable1[d - 1].probability;
			for (int n = 1; n < ATTRIBUTE_NUM; n++) {
				for (int i = 0; i < CPtable1.size(); i++) {
					secCmp1_2(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 0, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2]);
					secCmp1_2(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 1, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2]);
					secCmp1_2(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 2, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2]);

				}
			}
		}
	}

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("A  sem.wait ok\n");
	for (int q = 0; q < QUERY_NUM; q++) {
		for (int d = 1; d <= Label_table1[0]; d++) {
			posterior_probability1[q * ((int)Label_table1[0] + 1) + d] = PPtable1[d - 1].probability;
			for (int n = 1; n < ATTRIBUTE_NUM; n++) {
				for (int i = 0; i < CPtable1.size(); i++) {
					secCmp1_3(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 0, ptext1, ptext2, a1, a2, a3);
					secCmp1_3(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 1, ptext1, ptext2, a1, a2, a3);
					secCmp1_3(q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 2, ptext1, ptext2, a1, a2, a3);
					if (a1[q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 0] == 0 &&
						a1[q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 1] == 0 &&
						a1[q * ((int)Label_table1[0] + 1) * ATTRIBUTE_NUM * CPtable1.size() * 3 + d * ATTRIBUTE_NUM * CPtable1.size() * 3 + n * CPtable1.size() * 3 + i * 3 + 2] == 0) {
						posterior_probability1[q * ((int)Label_table1[0] + 1) + d] += CPtable1[i].probability;
						//cout << "1--d:" << d << "   n:" << n << "  CPtable1[i].attribute_value:" << CPtable1[i].attribute_value << "    " << CPtable1[i].probability << endl;
					}
				}
			}
			//cout << q * ((int)Label_table1[0] + 1) + d << "    " << posterior_probability1[q * ((int)Label_table1[0] + 1) + d] << endl;
		}
	}

	argmax1(sem, ptext1, ptext2, posterior_probability1, Label_table1, result1, a_1, b_1, c_1, t_1);

	gettimeofday(&end, NULL);
	printf("Classification   us: %ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("A  sem.post ok\n");
}
void Classification2(CSEM sem, double* ptext1, double* ptext2, vector<PPT>& PPtable2, vector<CPT>& CPtable2, double* Label_table2, double* query2, double* result2, double* a_2, double* b_2, double* c_2, double* cc_2, double* r_2, double* t_2) {
	double* posterior_probability2 = new double[QUERY_NUM * ((int)Label_table2[0] + 1)];
	double b1[QUERY_NUM * ((int)Label_table2[0] + 1) * CPtable2.size() * ATTRIBUTE_NUM * 3];
	double b2[QUERY_NUM * ((int)Label_table2[0] + 1) * CPtable2.size() * ATTRIBUTE_NUM * 3];
	double b3[QUERY_NUM * ((int)Label_table2[0] + 1) * CPtable2.size() * ATTRIBUTE_NUM * 3];

	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int i = 0; i < PPtable2.size(); i++) {
		secLog2_1(i, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], PPtable2[i].probability, cc_2[2]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("---B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int i = 0; i < PPtable2.size(); i++) {
		secLog2_2(i, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], r_2[2]);

		PPtable2[i].probability = b2[i];
		//cout << "class2  " << PPtable2[i].classes << " probability   " << PPtable2[i].probability << endl;

	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("---B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");

	for (int i = 0; i <  CPtable2.size(); i++) {
		secLog2_1(i, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], CPtable2[i].probability, cc_2[2]);
	}
	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("---B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	
	for (int i = 0; i <  CPtable2.size(); i++) {
		secLog2_2(i, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], r_2[2]);
		CPtable2[i].probability = b2[i];
		//cout << "class2  " << "  attribute_num   "<< CPtable2[i] .attribute_num << "   probability   " << CPtable2[i].probability << endl;
		//cout << "classes2  " << CPtable2[i].classes << "     attribute  " << CPtable2[i].attribute << "     attribute_category  " << CPtable2[i].attribute_value<< "     attribute_num  " << CPtable2[i].attribute_num << "  probability: " << CPtable2[i].probability<< endl;

	}

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int q = 0; q < QUERY_NUM; q++) {
		for (int d = 1; d <= Label_table2[0]; d++) {
			posterior_probability2[q * ((int)Label_table2[0] + 1) + d] = PPtable2[d - 1].probability;
			//cout << "2--posterior  " << q * ((int)Label_table2[0] + 1) + d << "   probability  " << PPtable2[d - 1].probability << endl;
			for (int n = 1; n < ATTRIBUTE_NUM; n++) {
				double nn = n / 1;
				for (int i = 0; i < CPtable2.size(); i++) {
					secCmp2_1(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 0, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], CPtable2[i].classes, Label_table2[d * ATTRIBUTE_NUM], t_2[2]);
					secCmp2_1(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 1, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], CPtable2[i].attribute, nn, t_2[2]);
					secCmp2_1(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 2, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], CPtable2[i].attribute_value, query2[q * ATTRIBUTE_NUM + n], t_2[2]);

				}
			}
		}
	}

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
	//else printf("B  sem.wait ok\n");
	for (int q = 0; q < QUERY_NUM; q++) {
		for (int d = 1; d <= Label_table2[0]; d++) {
			posterior_probability2[q * ((int)Label_table2[0] + 1) + d] = PPtable2[d - 1].probability;
			for (int n = 1; n < ATTRIBUTE_NUM; n++) {
				for (int i = 0; i < CPtable2.size(); i++) {
					secCmp2_2(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 0, ptext1, ptext2, b1, b2, b3);
					secCmp2_2(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 1, ptext1, ptext2, b1, b2, b3);
					secCmp2_2(q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 2, ptext1, ptext2, b1, b2, b3);
					if (b1[q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 0] == 0 &&
						b1[q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 1] == 0 &&
						b1[q * ((int)Label_table2[0] + 1) * ATTRIBUTE_NUM * CPtable2.size() * 3 + d * ATTRIBUTE_NUM * CPtable2.size() * 3 + n * CPtable2.size() * 3 + i * 3 + 2] == 0) {
						posterior_probability2[q * ((int)Label_table2[0] + 1) + d] += CPtable2[i].probability;
						//cout << "d:" << d << "   n:" << n << "  CPtable2[i].attribute_value:" << CPtable2[i].attribute_value << "    " << CPtable2[i].probability << endl;
					}
				}
			}
			//cout << q * ((int)Label_table2[0] + 1) + d << "    " << posterior_probability2[q * ((int)Label_table2[0] + 1) + d] << endl;
		}
	}

	if (sem.post() == false)    ;//printf("sem.post failed.\n");
	//else printf("B  sem.post ok\n");
	argmax2(sem,ptext1, ptext2,posterior_probability2, Label_table2, result2, a_2, b_2, c_2, t_2);
	
}


void argmax1(CSEM sem, double* ptext1, double* ptext2, double* posterior_probability1, double* Label_table1, double* result1, double* a_1, double* b_1, double* c_1, double* t_1) {
	double a1[QUERY_NUM ];
	double a2[QUERY_NUM];
	double a3[QUERY_NUM];
	double max[QUERY_NUM];
	for (int q = 0; q < QUERY_NUM; q++) {
		result1[q] = 0;
		max[q] = posterior_probability1[q * ((int)Label_table1[0] + 1) + 1];
	} 
	for (int d = 2; d <= Label_table1[0]; d++) {
		for (int q = 0; q < QUERY_NUM; q++) {
			secCmp1_1(q, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2], max[q], posterior_probability1[q * ((int)Label_table1[0] + 1) + d], t_1[2]);
		}
		if (sem.post() == false)    ;//printf("sem.post failed.\n");
		//else printf("A  sem.post ok\n");
		if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
		//else printf("A  sem.wait ok\n");
		for (int q = 0; q < QUERY_NUM; q++) {
			secCmp1_2(q, ptext1, ptext2, a1, a2, a3, a_1[2], b_1[2], c_1[2]);
		}
		if (sem.post() == false)    ;//printf("sem.post failed.\n");
		//else printf("A  sem.post ok\n");
		if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
		//else printf("****A  sem.wait ok\n");
		for (int q = 0; q < QUERY_NUM; q++) {
			secCmp1_3(q, ptext1, ptext2, a1, a2, a3);
			if (a1[q] == -1) {
				max[q] = posterior_probability1[q * ((int)Label_table1[0] + 1) + d];
				result1[q] = Label_table1[d * ATTRIBUTE_NUM];
			}
		}
	}
	
}
void argmax2(CSEM sem, double* ptext1, double* ptext2, double* posterior_probability2, double* Label_table2, double* result2, double* a_2, double* b_2, double* c_2, double* t_2) {
	double b1[QUERY_NUM];
	double b2[QUERY_NUM];
	double b3[QUERY_NUM];
	double max[QUERY_NUM];
	for (int q = 0; q < QUERY_NUM; q++) {
		result2[q] =1;
		max[q] = posterior_probability2[q * ((int)Label_table2[0] + 1) + 1];
	} 
	for (int d = 2; d <= Label_table2[0]; d++) {
		if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
		//else printf("B  sem.wait ok\n");
		for (int q = 0; q < QUERY_NUM; q++) {
			secCmp2_1(q, ptext1, ptext2, b1, b2, b3, a_2[2], b_2[2], c_2[2], max[q], posterior_probability2[q * ((int)Label_table2[0] + 1) + d], t_2[2]);
		}
		if (sem.post() == false)    ;//printf("sem.post failed.\n");
		//else printf("B  sem.post ok\n");
		if (sem.wait() == false)    ;//printf("sem.wait failed.\n");
		//else printf("B  sem.wait ok\n");
		for (int q = 0; q < QUERY_NUM; q++) {
			secCmp2_2(q, ptext1, ptext2, b1, b2, b3);
			//cout << "max[q]:  " << max[q] << "  posterior_probability2[q * ((int)Label_table2[0] + 1) + d]:  " << posterior_probability2[q * ((int)Label_table2[0] + 1) + d] << endl;
			//cout << "b1[q]:  " << b1[q] << endl;
			if (b1[q] == -1) {
				
				max[q] = posterior_probability2[q * ((int)Label_table2[0] + 1) + d];
				result2[q] = Label_table2[d * ATTRIBUTE_NUM];
			} 
		}
		if (sem.post() == false)    ;//printf("sem.post failed.\n");
		//else printf("****B  sem.post ok\n");
	}
}