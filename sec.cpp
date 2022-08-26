#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include"tool.h"
#include <iostream>
using namespace std;

void secMul1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1) {
	a2[ii] = x_1 - a_1;
	a3[ii] = y_1 - b_1;
	ptext1[ii] = a2[ii];
	ptext2[ii] = a3[ii];
	//cout << "ii: " << ii << "   x_1::" << x_1 << "  a_1  " << a_1  << "  y_1   " << y_1 << "  b_1   " << b_1 << endl;
}
void secMul1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1) {
	//cout << "111111  ii: " << ii << "   ptext1[ii]::" << ptext1[ii] << " ptext2[ii]  " << ptext2[ii] << endl;

	a1[ii] = ptext1[ii] + a2[ii];//e
	a2[ii] = ptext2[ii] + a3[ii];//f
	//cout << "ii: " << ii << "   a1[ii]::" << a1[ii] << "  a2[ii]  " << a2[ii] << "  a3[ii]" << a3[ii] << endl;
	//cout << "ii: " << ii << "   ptext1[ii]::" << ptext1[ii] << " ptext2[ii]  " << ptext2[ii] << endl;

	a3[ii] = c_1 + b_1 * a1[ii] + a_1 * a2[ii] + a1[ii] * a2[ii];

}

void secMul2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2) {
	b2[ii] = x_2 - a_2;//e_2
	b3[ii] = y_2 - b_2;//f_2
	//cout << "ii: " << ii << "   x_2::" << x_2 << "  a_2  " << a_2 << "  y_2   " << y_2 << "  b_2   " << b_2 << endl;

	b1[ii] = b2[ii] + ptext1[ii];//e
	ptext1[ii] = b2[ii];
	b2[ii] = b3[ii] + ptext2[ii];//f
	ptext2[ii] = b3[ii];
	//cout << "22222222   ii: " << ii << "   ptext1[ii]::" << ptext1[ii] << " ptext2[ii]  " << ptext2[ii] << endl;

	//cout << "ii: " << ii << "  b1[ii]  " << b1[ii] << "  b2[ii]  " << b2[ii] << "  b3[ii]  " << b3[ii] << endl;

	b3[ii] = c_2 + b_2 * b1[ii] + a_2 * b2[ii];

}

void secCmp1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1, double& t_1) {
	a1[ii] = x_1 - y_1;
	secMul1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, t_1, a1[ii]);
}
void secCmp2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2, double& t_2) {
	b1[ii] = x_2 - y_2;
	secMul2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, t_2, b1[ii]);
}
void secCmp1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1) {
	secMul1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1);
	ptext1[ii] = a3[ii];

}
void secCmp2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3) {
	b1[ii] = sgn(ptext1[ii] + b3[ii]);//sgn_xy
	ptext1[ii] = b3[ii];

}
void secCmp1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3) {
	a1[ii] = sgn(ptext1[ii] + a3[ii]);//sgn_xy
}

void secAddRes1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& cc_1) {


	secMul1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, x_1, cc_1);

}
void secAddRes2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& cc_2) {
	secMul2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, x_2, cc_2);

}
void secAddRes1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& r_1) {
	secMul1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1);
	ptext1[ii] = a3[ii];
	a1[ii] = 1 / r_1;
}
void secAddRes2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& r_2) {
	b1[ii] = (ptext1[ii] + b3[ii]) / r_2;

}
void secPRE1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& cc_1) {
	secAddRes1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, x_1, cc_1);
}
void secPRE2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& cc_2) {
	secAddRes2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, x_2, cc_2);

}
void secPRE1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& r_1, double& power) {

	secAddRes1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, r_1);

	a2[ii] = pow(fabs(a1[ii]), power);

	a3[ii] = a2[ii] / r_1;
	//cout << "a3[ii]:" << a3[ii] << endl;
}
void secPRE2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& r_2, double& power) {
	secAddRes2_2(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, r_2);
	b2[ii] = pow(fabs(b1[ii]), power);
	b3[ii] = b2[ii] / r_2;
	//cout << "b3[ii]:" << b3[ii] << endl;
	ptext1[ii] = b3[ii];
}
void secPRE1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& cc_1) {
	a1[ii] = a3[ii] * ptext1[ii] * cc_1;
	//cout<< a3[ii] * ptext1[ii] <<endl;
	ptext1[ii] = a3[ii];
}
void secPRE2_3(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& cc_2) {
	b1[ii] = ptext1[ii] * b3[ii] * cc_2;
	//cout <<"---"<< b3[ii] * ptext1[ii] << endl;
}
void secDiv1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& t_1) {
	secMul1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, t_1, x_1);
}
void secDiv2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double* tx_2, double& x_2, double& t_2) {
	secMul2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, t_2, x_2);
	tx_2[ii] = b3[ii];
}
void secDiv1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double* tx_1, double& y_1, double& t_1) {
	secMul1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1);
	tx_1[ii] = a3[ii];
	secMul1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, t_1, y_1);
}
void secDiv2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& y_2, double& t_2) {
	secMul2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, t_2, y_2);
}
void secDiv1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1) {
	secMul1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1);
	ptext1[ii] = a3[ii];
}
void secDiv2_3(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double* tx_2) {
	b1[ii] = tx_2[ii] / (ptext1[ii] + b3[ii]);
	ptext1[ii] = b3[ii];
	//cout << "b1[ii]" << b1[ii] << endl;
}
void secDiv1_4(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double* tx_1) {
	a1[ii] = tx_1[ii] / (a3[ii] + ptext1[ii]);
}
void secLog1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& cc_1) {

	secAddRes1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, x_1, cc_1);
}
void secLog1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& r_1) {
	secAddRes1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, r_1);

	a2[ii] = log(fabs(a1[ii])) / log(2);
	
	//cout << "log   " << a2[ii] << endl;
}
void secLog2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& cc_2) {
	secAddRes2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, x_2, cc_2);

}
void secLog2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& r_2) {
	secAddRes2_2(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, r_2);
	//cout << "b1  " << b1[ii] << endl;
	b2[ii] = log(fabs(b1[ii])) / log(2);
	//cout << "log   " << b2[ii] << endl;
}
void secCmp_closet1_1(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& x_1, double& y_1, double& rr_1) {
	
	a1[ii] = x_1 - y_1;
	//cout << "ii:    " << ii << "                  x_1" << x_1 << " " << y_1 << "              a1[ii]" << a1[ii] << endl;
	secMul1_1(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1, rr_1, a1[ii]);
	
}
void secCmp_closet1_2(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& a_1, double& b_1, double& c_1, double& k_1) {
	//cout << "ii: " << ii << "  e2[ii]  " << ptext1[ii] << "  f2[ii]  " << ptext2[ii] << endl;
	secMul1_2(ii, ptext1, ptext2, a1, a2, a3, a_1, b_1, c_1);
	//cout << "111111111111111    ii: " << ii << "   a1[ii]::" << a1[ii] << "  a2[ii]  " << a2[ii] <<"  a3[ii]" << a3[ii] << endl;

	a1[ii] = a3[ii] + k_1;
	ptext1[ii] = a1[ii];
}
void secCmp_closet1_3(int ii, double* ptext1, double* ptext2, double* a1, double* a2, double* a3, double& sgn_rr_1) {
	a2[ii] = ptext1[ii] + a1[ii];
	
	if (a2[ii] < 0) a3[ii] = 0.5 - sgn_rr_1;
	else a3[ii] = sgn_rr_1;
	//cout << "ii: " << ii << "   a3[ii]::" << a3[ii] << "  a2[ii]  " << a2[ii] << endl;
}
void secCmp_closet2_1(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& a_2, double& b_2, double& c_2, double& x_2, double& y_2, double& rr_2, double& k_2) {
	
	b1[ii] = x_2 - y_2;
	//cout << "ii: " << ii << "                x_2" << x_2 << " " << y_2 << "                b1[ii]" << b1[ii] << endl;
	secMul2_1(ii, ptext1, ptext2, b1, b2, b3, a_2, b_2, c_2, rr_2, b1[ii]);
	//cout << "ii: " << ii << "  e2[ii]  " << ptext1[ii] << "  f2[ii]  " << ptext2[ii]<< endl;

	//cout << "22222222222    ii: " << ii << "  b1[ii]  " << b1[ii] << "  b2[ii]  " << b2[ii] << "  b3[ii]  " << b3[ii] << endl;
	b1[ii] = b3[ii] + k_2;
}
void secCmp_closet2_2(int ii, double* ptext1, double* ptext2, double* b1, double* b2, double* b3, double& sgn_rr_2 ) {
	b2[ii] = ptext1[ii] + b1[ii];
	ptext1[ii] = b1[ii];
	
	if (b2[ii] < 0) b3[ii] = 0.5 - sgn_rr_2;
	else b3[ii] = sgn_rr_2;
	//cout <<"ii: " <<ii << "    b2[ii]::" << b2 [ii]<<"     b3[ii]::" << b3[ii]<< endl;

}
