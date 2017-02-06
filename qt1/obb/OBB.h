// OBB.h: interface for the OBB class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "ObbMatrix.h"

class OBB:public ObbMatrix          
{
public:
	OBB();
	OBB(int,double **);
	OBB(int,double **,double);
	virtual ~OBB();

public:
	int getdirection();
	double getarea(double mi[],double ma[]);
	int getobb();
	void getAABB(double mi[],double ma[],double **a);
    void rotate(double x,double y,double z,double a,double temp[]);
	void rotate(double *x,double *y,double a);
	double getvol();
	double **xyz;	//存储点云数组
	int num;		//点个数
	double angle[3],dangle[3];
    double min[3],max[3];
};
