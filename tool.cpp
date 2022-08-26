#include <sys/sem.h>
#include<fstream>

#include"NB.h"
#include"tool.h"
#include<iomanip>
#include <iostream>
using namespace std;

void generate_random(double* a, double* a_1, double* a_2, double* b, double* b_1, double* b_2, double* c, double* c_1, double* c_2,
	double* t, double* t_1, double* t_2, double* r_1, double* r_2, double* cc, double* cc_1, double* cc_2, double* k, double* k_1,
	double* k_2, double* rr, double* rr_1, double* rr_2, double* sgn_rr_1 , double* sgn_rr_2) {
    for (int i = 0; i < 3; i++) {
        a[i] = 0 + rand() % 25;
        a_1[i] = 0 + rand() % 25;
        a_2[i] = a[i] - a_1[i];
        b[i] = 0 + rand() % 25;
        b_1[i] = 0 + rand() % 25;
        b_2[i] = b[i] - b_1[i];
        c[i] = a[i] * b[i];
        c_1[i] = 0 + rand() % 25;
        c_2[i] = c[i] - c_1[i];
        t[i] = 1 + rand() % 25;
        t_1[i] = 0 + rand() % 25;
        t_2[i] = t[i] - t_1[i];

        r_1[i] = 1 + rand() % 18;
        r_2[i] = 1 + rand() % 20;
        cc[i] = r_1[i] * r_2[i];
        cc_1[i] = 0 + rand() % 15;
        cc_2[i] = cc[i] - cc_1[i];


        k[i] = rand() % 25;
        k_1[i] = 0 + rand() % 25;
        k_2[i] = k[i] - k_1[i];
        
        rr[i] = 1 + rand() % 25;
        rr[i] = k[i]* rr[i];
        rr_1[i] = 1 + rand() % 25;
        rr_2[i] = rr[i] - rr_1[i];
        sgn_rr_1[i] = 1 + rand() % 14; 
        sgn_rr_2[i] = sgn(rr[i]) - sgn_rr_1[i];
    }
}



int getmax(double* arr) {
	double max = arr[0];

	int result = max;
	for (int i = 1; i < ATTRIBUTE_NUM; i++) {
		if (max < arr[i]) {
			max = arr[i];
            result = (int)max;
			
		}
	}

	return result;

}



int sgn(double xy) {
    if (xy > 0) return 1;
    else if (xy < 0) return -1;
    else return 0;
}



void read_features(double* features, double* features1, double* features2, const char* file, int n, int dims)
{
	ifstream in(file);

	for (int i = 0; i < n; i++)
	{

		for (int j = 0; j < dims; j++)
		{
			in >> features[i * dims + j];
			features1[i * dims + j] = 0 + rand() % 255;
			features2[i * dims + j] = features[i * dims + j] - features1[i * dims + j];
		}
	}
	in.close();
    
}

void Scrambling_category(double* category_num, double* Label_table1, double* Label_table2) {
	for (int j = 0; j < ATTRIBUTE_NUM; j++) {
		Label_table1[j] = category_num[j];
		Label_table2[j] = category_num[j];
		for (int i = 1; i <= category_num[j]; i++) {
			Label_table1[i * ATTRIBUTE_NUM + j] = 1 + rand() % 15;
			Label_table2[i * ATTRIBUTE_NUM + j] = i - Label_table1[i * ATTRIBUTE_NUM + j];
           // cout << Label_table1[i * ATTRIBUTE_NUM + j] << " " << Label_table2[i * ATTRIBUTE_NUM + j]<<" " << Label_table1[i * ATTRIBUTE_NUM + j] + Label_table2[i * ATTRIBUTE_NUM + j] << endl;
		}
	}

}


bool CSEM::init(key_t key)
{
    // 获取信号灯。
    if ((sem_id = semget(key, 1, 0640)) == -1)
    {
        // 如果信号灯不存在，创建它。
        if (errno == 2)
        {
            if ((sem_id = semget(key, 1, 0640 | IPC_CREAT)) == -1) { perror("init 1 semget()"); return false; }

            // 信号灯创建成功后，还需要把它初始化成可用的状态。
            union semun sem_union;
            sem_union.val = 1;
            if (semctl(sem_id, 0, SETVAL, sem_union) < 0) { perror("init semctl()"); return false; }
        }
        else
        {
            perror("init 2 semget()"); return false;
        }
    }

    return true;
}

bool CSEM::destroy()
{
    if (semctl(sem_id, 0, IPC_RMID) == -1) { perror("destroy semctl()"); return false; }

    return true;
}

bool CSEM::wait()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) { perror("wait semop()"); return false; }

    return true;
}

bool CSEM::post()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) { perror("post semop()"); return false; }

    return true;
}