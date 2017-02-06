#ifndef DATAMODEL_H
#define DATAMODEL_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
using namespace std;
typedef struct{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
} vertex;

class datamodel
{
public:
	datamodel();
	datamodel(std::string path);
	~datamodel();
	void setPath(string path);
	bool plyRead();
	void plyWrite();
	bool clear();
	int  getTotalVertex();
	int  getTotalFace();
	int  getTotalFaceVertex3();
	const char* getPath();
	ifstream fin;
	string path;
	int m_totalVertexProperty;	//������������
	int m_colorFlag;			//������ɫ���,0:�ޣ�3:RGB��4:RGBA
	int m_colorPropertyPos;		//��ɫ����λ�ã�����ڼ�������ΪRGBֵ
	int m_totalVertex;			//��������
	int m_totalFace;			//������

public:
	vector<vertex> vecVertex;			//�������飬�洢ģ�����ж������ꡣ
	vector<float> vecTriangleColor;		//��ɫ���飬�洢������ɫ���ݡ�
	vector<float> vecFaceTriangle;		//�����飬�洢�����ĸ����������ꡣ
	vector<int>   vecFaceTriangleIndex;	//���������飬�洢�����ĸ������������

};

#endif //DATAMODEL