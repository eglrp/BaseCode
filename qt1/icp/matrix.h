#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


/*说明：
矩阵数据为公有;
设矩阵M，则元素调用方法:M.val[m][n];行列数调用方法：M.m，M.n
使用方法：	
	Matrix R = Matrix::eye(3);创建一个三阶单位矩阵
	Matrix t(3, 1);创建一个3*1阶矩阵(向量)
*/
class Matrix {
public:

	// 构造函数和析构函数
	Matrix();                                           // 构造空矩阵，赋值0
	Matrix(const int m, const int n);                   // 构造m*n阶矩阵，初值为0
	Matrix(const int m, const int n, const double* val_); // 构造m*n阶矩阵，并用数组val按行赋值,
	Matrix(const Matrix &M);                             // 拷贝构造函数
	~Matrix();

	// assignment operator, copies contents of M，重载赋值运算符
	Matrix& operator= (const Matrix &M);

	// create identity matrix，创建并返回一个m阶的单位矩阵
	static Matrix eye(const int m);

	// create matrix with ones，创建并返回一个m*n的矩阵，并用1赋值全部元素
	static Matrix ones(const int m, const int n);

	// 基本方法
	Matrix  operator+ (const Matrix &M); // 矩阵加
	Matrix  operator- (const Matrix &M); // 矩阵减
	Matrix  operator* (const Matrix &M); // 矩阵乘法
	Matrix  operator/ (const double &s);  // 矩阵数除，矩阵缩放.M=M/s

	Matrix  operator~ ();                // 返回当前矩阵的转置矩阵
	double   l2norm();                   // 返回矩阵的欧氏范数（向量）或 矩阵范数（矩阵）

	// 主要方法
	bool   solve(const Matrix &M, double eps = 1e-20); // 解线性方程并替换原矩阵， (设当前矩阵*this为B：M*x=B, 计算完成替换 *this and M)
	void   svd(Matrix &U, Matrix &W, Matrix &V);       // SVD分解, *this = U*diag(W)*V^T

	// 数据元素
	double   **val;//矩阵的值
	int   m, n;//矩阵的行m和列n

	/*私有函数*/
private:
	// set or get submatrices of current matrix
	Matrix getMat(int i1, int j1, int i2 = -1, int j2 = -1);
	void   setMat(const Matrix &M, const int i, const int j);

	void allocateMemory(const int m_, const int n_);
	void releaseMemory();
	inline double pythag(double a, double b);

};

#endif // MATRIX_H
