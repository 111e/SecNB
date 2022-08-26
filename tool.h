#pragma once
#ifndef TOOL_H
#define TOOL_H
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include<vector>


using namespace std;

class CSEM
{
private:
    union semun  // 用于信号灯操作的共同体。
    {
        int val;
        struct semid_ds* buf;
        unsigned short* arry;
    };

    int  sem_id;  // 信号灯描述符。
public:
    bool init(int key); // 如果信号灯已存在，获取信号灯；如果信号灯不存在，则创建信号灯并初始化。
    bool wait();          // 等待信号灯挂出。
    bool post();          // 挂出信号灯。
    bool destroy();       // 销毁信号灯。
};




void generate_random(double* a, double* a_1, double* a_2, double* b, double* b_1, double* b_2, double* c, double* c_1, double* c_2,
    double* t, double* t_1, double* t_2, double* r_1, double* r_2, double* cc, double* cc_1, double* cc_2, double* k, double* k_1,
    double* k_2, double* rr, double* rr_1, double* rr_2, double* sgn_rr_1, double* sgn_rr_2);
int getmax(double* arr);
int sgn(double xy);
void read_features(double* features, double* features1, double* features2, const char* file, int n, int dims);
void Scrambling_category(double* category_num, double* Label_table1, double* Label_table2);


void secMul1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1);
void secMul1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1);
void secMul2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2);
void secCmp1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1, double& t_1);
void secCmp1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1);
void secCmp1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3);
void secCmp2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2, double& t_2);
void secCmp2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3);
void secAddRes1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& cc_1);
void secAddRes2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& cc_2);
void secAddRes1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& r_1);
void secAddRes2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& r_2);
void secDiv1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& t_1);
void secDiv1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double* tx_1, double& y_1, double& t_1);
void secDiv1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1);
void secDiv1_4(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double* tx_1);
void secDiv2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double* tx_2, double& x_2, double& t_2);
void secDiv2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& y_2, double& t_2);
void secDiv2_3(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double* tx_2);
void secLog1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& cc_1);
void secLog1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& r_1);
void secLog2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& cc_2);
void secLog2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& r_2);
void secCmp_closet1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1, double& rr_1);
void secCmp_closet1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& k_1);
void secCmp_closet1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& sgn_rr_1);
void secCmp_closet2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2, double& rr_2, double& k_2);
void secCmp_closet2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& sgn_rr_2);
#endif