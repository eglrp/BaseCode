// ObbMatrix.h: interface for the ObbMatrix class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class ObbMatrix          
{
public:
	ObbMatrix();
	ObbMatrix(int ,int);
	virtual ~ObbMatrix();
	
public:
   void TransPose(int m,int n,double **B,double **BT);
   void GetBTPB(int m,int n,double **B,double **BT,double **NBB);
   void GetW(int m,int n,double **BT,double **L,double **W);
   bool Inverse(double **A,int N);
   void InitObbMatrix(int row,int col,double **Mat);
   void Getx(int m,int n,double **InvNbb,double **W,double **x);
   bool CopyObbMatrix(ObbMatrix &src,ObbMatrix &des);
   double **GetObbMatrix();
   double **m_ObbMatrix;
   int Rows,Cols;
}; 
