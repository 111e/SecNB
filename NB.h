#pragma once
#ifndef NB_H
#define NB_H
#include<vector>
#include"tool.h"
using namespace std;


#define DATA_NUM 489//12960//625
#define ATTRIBUTE_NUM 10//9  //5
#define QUERY_NUM 7

struct PPT {
	double classes;		//目标类别
	double classes_num;		//目标类别计数
	double probability;		//概率
};
struct CPT {

	double attribute;		//属性
	double attribute_num;	 //属性计数
	double attribute_value;		//属性类别
	double classes;		//目标类别
	double classes_num;		//目标类别计数
	double probability;		//概率
};


void write_Ptable(vector<PPT>& PPtable, vector<CPT>& CPtable, const char* Pfile, const char* Cfile);
void read_Ptable(vector<PPT>& PPtable, vector<CPT>& CPtable, const char* Pfile, const char* Cfile);
void write_result(double* result, const char* file);

void Training1(CSEM sem, double* ptext1, double* ptext2, double* features1, double* Label_table1, vector<PPT>& PPtable1, vector<CPT>& CPtable1, double* a_1, double* b_1, double* c_1, double* rr_1, double* sgn_rr_1,double* k_1, double* t_1);
void Training2(CSEM sem, double* ptext1, double* ptext2, double* features2, double* Label_table2, vector<PPT>& PPtable2, vector<CPT>& CPtable2, double* a_2, double* b_2, double* c_2, double* rr_2, double* sgn_rr_2, double* k_2, double* t_2);

void Classification1(CSEM sem, double* ptext1, double* ptext2, vector<PPT>& PPtable1, vector<CPT>& CPtable1, double* Label_table1, double* query1, double* result1, double* a_1, double* b_1, double* c_1, double* cc_1, double* r_1, double* t_1);
void Classification2(CSEM sem, double* ptext1, double* ptext2, vector<PPT>& PPtable2, vector<CPT>& CPtable2, double* Label_table2, double* query2, double* result2, double* a_2, double* b_2, double* c_2, double* cc_2, double* r_2, double* t_2);

void argmax1(CSEM sem, double* ptext1, double* ptext2, double* posterior_probability1, double* Label_table1, double* result1, double* a_1, double* b_1, double* c_1, double* t_1);
void argmax2(CSEM sem, double* ptext1, double* ptext2, double* posterior_probability2, double* Label_table2, double* result2, double* a_2, double* b_2, double* c_2, double* t_2);
#endif