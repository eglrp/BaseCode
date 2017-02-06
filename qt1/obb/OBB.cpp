// OBB.cpp: implementation of the OBB class.
//
//////////////////////////////////////////////////////////////////////

#include "OBB.h"
#include<string>
#include<math.h>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OBB::OBB()
{
	num=0;
	xyz=NULL;
	
}
OBB::OBB(int a,double **A)
{
	num=a;
	xyz=A;
	for(int i=0;i<3;i++)
	{
		angle[i]=0;
		dangle[i]=0.001*3.1415926;
	}
}
OBB::OBB(int a,double **A,double temp)
{
	num=a;
	xyz=A;
	for(int i=0;i<3;i++)
	{
		angle[i]=0;
		dangle[i]=temp;
	}
}
OBB::~OBB()
{

}
void OBB::getAABB(double mi[],double ma[],double **a)
{
	for(int i=0;i<3;i++)
	{
		mi[i]=a[0][i];
		ma[i]=a[0][i];
	}
	
	for(int i=0;i<num;i++)
		for(int j=0;j<3;j++)
		{
			if(a[i][j]<=mi[j])
				mi[j]=a[i][j];
			if(a[i][j]>=ma[j])
				ma[j]=a[i][j];
		}
}
double OBB::getarea(double mi[],double ma[])
{
	double area=1;
	for(int i=0;i<2;i++)
		area=area*(ma[i]-mi[i]);
	return area;
}
double OBB::getvol()
{
	double vv=1;
	for(int i=0;i<3;i++)
		vv=vv*(max[i]-min[i]);
	return vv;
}
void OBB::rotate(double x,double y,double z,double a,double temp[])
{
	temp[0]=x*cos(a)-y*sin(a);
	temp[1]=x*sin(a)+y*cos(a);
	temp[2]=z;
}
void OBB::rotate(double *x,double *y,double a)
{
	double temp1=*x,temp2=*y;
	*x=temp1*cos(a)-temp2*sin(a);
	*y=temp1*sin(a)+temp2*cos(a);
}

int OBB::getobb()
{
	int i,j,n=0,m=0;
	double area,temp;
	double xyztemp[3],mintemp[3],maxtemp[3];
	getAABB(min,max,xyz);
	area=getarea(min,max);
	for(i=-20;i<=20;i++)
	{
			rotate(xyz[0][0],xyz[0][1],xyz[0][2],angle[0]+dangle[0]*i*20,xyztemp);
			for(int k=0;k<3;k++)
			{
				mintemp[k]=xyztemp[k];
				maxtemp[k]=xyztemp[k];
			}

		for(j=1;j<num;j++)
		{
			rotate(xyz[j][0],xyz[j][1],xyz[j][2],angle[0]+dangle[0]*i*20,xyztemp);
			for(int k=0;k<3;k++)
			{
				if(xyztemp[k]<=mintemp[k])
					mintemp[k]=xyztemp[k];
				if(xyztemp[k]>=maxtemp[k])
					maxtemp[k]=xyztemp[k];
			}
		}
		temp=getarea(mintemp,maxtemp);
		if(area>=temp)
		{
			area=temp;
			n=i;
			for(int k=0;k<3;k++)
			{
				min[k]=mintemp[k];
				max[k]=maxtemp[k];
			}
		}
	}
    for(i=20*n-10;i<=20*n+10;i++)
	{
			rotate(xyz[0][0],xyz[0][1],xyz[0][2],angle[0]+dangle[0]*i,xyztemp);
			for(int k=0;k<3;k++)
			{
				mintemp[k]=xyztemp[k];
				maxtemp[k]=xyztemp[k];
			}

		for(j=1;j<num;j++)
		{
			rotate(xyz[j][0],xyz[j][1],xyz[j][2],angle[0]+dangle[0]*i,xyztemp);
			for(int k=0;k<3;k++)
			{
				if(xyztemp[k]<=mintemp[k])
					mintemp[k]=xyztemp[k];
				if(xyztemp[k]>=maxtemp[k])
					maxtemp[k]=xyztemp[k];
			}
		}
		temp=getarea(mintemp,maxtemp);
		if(area>=temp)
		{
			area=temp;
			m=i;
			for(int k=0;k<3;k++)
			{
				min[k]=mintemp[k];
				max[k]=maxtemp[k];
			}
		}
	}
  return m;
}

int OBB::getdirection()
{
	double a[3];
	int j;
	for(int i=0;i<3;i++)
		a[i]=max[i]-min[i];
	if(a[0]>a[1] && a[0]>a[2])
		j=0;
	if(a[1]>a[0] && a[1]>a[2])	
		j=1;
	if(a[2]>a[0] && a[2]>a[1])
		j=2;
	return j;
}