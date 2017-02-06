// ObbMatrix.cpp: implementation of the ObbMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "ObbMatrix.h"
#include<string>
#include<math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ObbMatrix::ObbMatrix()
{

 Rows=0;
 Cols=0;
 m_ObbMatrix=NULL;
}

ObbMatrix::ObbMatrix(int m,int n)
{
 m_ObbMatrix=new double *[m];
 for (int i=0;i<m;i++) m_ObbMatrix[i]=new double [n];
 InitObbMatrix(m,n, m_ObbMatrix);
 Rows=m;
 Cols=n;
}

ObbMatrix::~ObbMatrix()
{
if(Rows)
	 {
		for (int i=0;i<Rows;i++)
		delete m_ObbMatrix[i];
		delete  m_ObbMatrix;
	}
}
double** ObbMatrix::GetObbMatrix()
	{
		return m_ObbMatrix;
		
		}

void ObbMatrix::TransPose(int m,int n,double **B,double **BT)

{

for (int i=0;i<m;i++)
for (int j=0;j<n;j++)

{
BT[j][i]=B[i][j];

}

}




void ObbMatrix::GetBTPB(int m,int n,double **B,double **BT,double **NBB)
{
for (int i=0;i<n;i++)
for (int j=0;j<n;j++)
{NBB[i][j]=0;
for (int k=0;k<m;k++)
NBB[i][j]+=BT[i][k]*B[k][j];

}

}

 void ObbMatrix::GetW(int m,int n,double **BT,double **L,double **W)
 {
 for (int i=0;i<m;i++)
	 
	 {W[i][0]=0;
	 for (int j=0;j<n;j++)

		 W[i][0]+=BT[i][j]*L[j][0];
	 
	 }
 
 
 
 }

 void ObbMatrix::Getx(int m,int n,double **InvNbb,double **W,double **x)
 {
 for (int i=0;i<m;i++)
	 
	 {x[i][0]=0;
	 for (int j=0;j<n;j++)

		 x[i][0]+=InvNbb[i][j]*W[j][0];
	 
	 }
 
 
 
 }

bool ObbMatrix::Inverse(double **A,int N)
{
	if(A==NULL) return false;
	
	int *is,*js,i,j,k;
	long double d,p;
	is=new int [N];
	js=new int [N];


	for (k=0; k<N; k++)
	{ 
		d=0.0;
		for (i=k; i<N; i++)
			for (j=k; j<N; j++)
			{ 
				p=fabs(A[i][j]);
				if (p>d) { d=p; is[k]=i; js[k]=j;}
			}
			if (d+1.0==1.0)
			{ 
				delete []is;
				delete []js;
		

				return false;
			}
			if (is[k]!=k)
				for (j=0; j<N; j++)
				{ 
					p=A[k][j]; A[k][j]=A[is[k]][j]; A[is[k]][j]=p;
				}
				if (js[k]!=k)
					for (i=0; i<N; i++)
					{ 
						p=A[i][k]; A[i][k]=A[i][js[k]]; A[i][js[k]]=p;
					}
					A[k][k]=1.0/A[k][k];
					for (j=0; j<N; j++)
						if (j!=k)
						{ 
							A[k][j]=A[k][j]*A[k][k];
						}
						for (i=0; i<N; i++)
							if (i!=k)
								for (j=0; j<N; j++)
									if (j!=k)
									{ 
										A[i][j]=A[i][j]-A[i][k]*A[k][j];
									}
									for (i=0; i<N; i++)
										if (i!=k)
										{ 
											A[i][k]=-A[i][k]*A[k][k];
										}
	}
	for (k=N-1; k>=0; k--)
	{ 
		if (js[k]!=k)
			for (j=0; j<N; j++)
			{ 
				p=A[k][j]; A[k][j]=A[js[k]][j]; A[js[k]][j]=p;
			}
			if (is[k]!=k)
				for (i=0; i<N; i++)
				{ 
					p=A[i][k]; A[i][k]=A[i][is[k]]; A[i][is[k]]=p;
				}
	}
	delete []is;
	delete []js;


	return true;
}
	

void ObbMatrix::InitObbMatrix(int row,int col, double **Mat)
{
	if(Mat==NULL) return;
    
	int i,j;
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
               Mat[i][j]=0.0;

}