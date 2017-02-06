#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


/*˵����
��������Ϊ����;
�����M����Ԫ�ص��÷���:M.val[m][n];���������÷�����M.m��M.n
ʹ�÷�����	
	Matrix R = Matrix::eye(3);����һ�����׵�λ����
	Matrix t(3, 1);����һ��3*1�׾���(����)
*/
class Matrix {
public:

	// ���캯������������
	Matrix();                                           // ����վ��󣬸�ֵ0
	Matrix(const int m, const int n);                   // ����m*n�׾��󣬳�ֵΪ0
	Matrix(const int m, const int n, const double* val_); // ����m*n�׾��󣬲�������val���и�ֵ,
	Matrix(const Matrix &M);                             // �������캯��
	~Matrix();

	// assignment operator, copies contents of M�����ظ�ֵ�����
	Matrix& operator= (const Matrix &M);

	// create identity matrix������������һ��m�׵ĵ�λ����
	static Matrix eye(const int m);

	// create matrix with ones������������һ��m*n�ľ��󣬲���1��ֵȫ��Ԫ��
	static Matrix ones(const int m, const int n);

	// ��������
	Matrix  operator+ (const Matrix &M); // �����
	Matrix  operator- (const Matrix &M); // �����
	Matrix  operator* (const Matrix &M); // ����˷�
	Matrix  operator/ (const double &s);  // ������������������.M=M/s

	Matrix  operator~ ();                // ���ص�ǰ�����ת�þ���
	double   l2norm();                   // ���ؾ����ŷ�Ϸ������������� ������������

	// ��Ҫ����
	bool   solve(const Matrix &M, double eps = 1e-20); // �����Է��̲��滻ԭ���� (�赱ǰ����*thisΪB��M*x=B, ��������滻 *this and M)
	void   svd(Matrix &U, Matrix &W, Matrix &V);       // SVD�ֽ�, *this = U*diag(W)*V^T

	// ����Ԫ��
	double   **val;//�����ֵ
	int   m, n;//�������m����n

	/*˽�к���*/
private:
	// set or get submatrices of current matrix
	Matrix getMat(int i1, int j1, int i2 = -1, int j2 = -1);
	void   setMat(const Matrix &M, const int i, const int j);

	void allocateMemory(const int m_, const int n_);
	void releaseMemory();
	inline double pythag(double a, double b);

};

#endif // MATRIX_H
