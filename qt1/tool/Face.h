#pragma once
#include"stdafx.h"

/*���������е���*/
class Face {
public:
	
	/*���ϵ��������id*/
	int vertex[3];
	
public:

	/*�޲����Ĺ��캯��*/
	Face(){}

	/*�������Ĺ��캯��*/
	Face(int id1, int id2, int id3) {
		vertex[0] = id1;
		vertex[1] = id2;
		vertex[2] = id3;
	}
	
	/*�������������ͨ����������ķ�ʽ����*/
	int& operator[](int index) {
		return vertex[index];
	}

	/*�������������ͨ����������ķ�ʽ����*/
	int operator[](int index) const {
		return vertex[index];
	}
};
