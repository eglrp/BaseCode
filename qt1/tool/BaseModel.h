#pragma once
#include"stdafx.h"
#include"Point3D.h"
#include"Face.h"

/*����ģ����*/
class BaseModel{

public:
	/*��߽ṹ*/
	struct HalfEdge{
		/*�ð�ߵĳ���*/
		double length;
		/*�Աߵ�id*/
		int idOfOppositeEdge;
		/*����id*/
		int idOfStartPoint;

		/*�޲������캯��*/
		HalfEdge() {
			length = 0;
			idOfOppositeEdge = -1;
			idOfStartPoint = -1;
		}

		/*�������Ĺ��캯��*/
		HalfEdge(double _length, int _idOfOppositeEdge, int _idOfStartPoint) {
			length = _length;
			idOfOppositeEdge = _idOfOppositeEdge;
			idOfStartPoint = _idOfStartPoint;
		}
	};

public:
	/*�����ļ���*/
	char inputFileName[256];
	/*�㼯*/
	vector<Point3D> vertexs;
	/*�漯*/
	vector<Face> faces;
	vector<Face> orgfaces;
	/*��߼�*/
	vector<HalfEdge> halfEdges;

	/*******QQQ***s*****/
public:
	Point3D centroid; // ��������
	vector<Point3D> denseVertex;//�ܼ�����
	
	/********cwh****s****/
	vector<int> denseRelation; //�ܼ������Ķ�Ӧ��ϵ
	/*******cwh*****e****/

	/*��дTXT�����ļ�*/
	void readTxtPointCloudFile(const char *fileName);
	void wirteTxtPointCloudFile(char *fileName);
	/*�Ե��ƾ��Ȳ���*/
	void uniformSampling(double ratio);
	void uniformSampling(int num);
	/*���������������ȡ�ܼ�����*/
	void denseVertices(double s);
	/*����������ϸ�������������ֵ��������һ��Ϊ��*/
	void refineTriangles(double area, vector<Point3D> &rvertexs, vector<Face> &rfaces);
	void clear();
	/*****QQQ****e*****/
	
	/********cwh***s*******/
public:
	/*�õ�һ�������������Ķ�Ӧ��ϵ*/
	vector<vector<int>> relation;		//���������֮��Ĺ�ϵ
	void getRelation(vector<Point3D> gvertexs, vector<Face> gfaces);
	/********cwh***e*******/

	/*���캯��*/
public:

	/*�޲����Ĺ��캯�������㼯���漯���*/
	BaseModel() {
		vertexs.clear();
		faces.clear();
		halfEdges.clear();
		denseVertex.clear();
	}

	/*�������Ĺ��캯���������ǵ�����ļ����������㼯���漯���*/
	BaseModel(char *fileName) {
		memcpy(inputFileName, fileName, strlen(fileName) + 1);
		vertexs.clear();
		faces.clear();
		denseVertex.clear();
	}

	/*�ļ�������෽��*/
public:
	/*����.obj�ļ�*/
	void readObjFile(char *fileName);

	/*����.ply�ļ�*/
	void readPlyFile(const char *fileName);
	/*ply�ı��ļ�����*/
	void ReadAsciiPly(const char *fileName);
	/*ply�������ļ�����*/
	void readBinaryPly(const char * filename, const char* format);

	/*����.m�ļ�*/
	void readMFile(char *fileName);

	/*����.off�ļ�*/
	void readOffFile(char *fileName);

	/*��������ļ��������ļ��ĺ�׺�����ж��֣�.stl, .ply, .obj, .x3d������ʵ�ֵ�ʱ���پ���ϸ���ҵ�����ط���*/
	void readPointCloudFile(char *fileName);

	/*����.stl�ļ�*/
	void readStlFile(char *fileName);

	/*����.X3D�ļ�*/
	void readX3DFile(char *fileName);

	/*�����ļ�*/
	void outputFile(char *fileName);

	void ReadUntilNextLine(ifstream& in) const;
	void ReadIntoWord(ifstream &in, string& word) const;
	void SwapOrder(char *buf, int sz);

	/*��Ӧ�İ�߽ṹ�ķ����Ͳ���*/
public:

	/*������߽ṹ*/
	void creatHalfEdge();

	/*�������id, ����eid����ð������ģ�Ͱ�߼��е�id*/
	int idOfFace(int eid) {
		return eid % 3;
	}

	/*��һ����ߵ�id�� ����eid����ð������ģ�Ͱ�߼��е�id*/
	int idOfNextHalfEdge(int eid) {
		return eid / 3 + (eid + 1) % 3;
	}

	/*��һ����ߵ�id�� ����eid����ð������ģ�Ͱ�߼��е�id*/
	int idOfPreHalfEdge(int eid) {
		return eid / 3 + (eid + 2) % 3;
	}

	/*�ð�ߵ����*/
	Point3D startOfHalfEdge(int eid) {
		return vertexs[halfEdges[eid].idOfStartPoint];
	}

	/*�ð�ߵ��յ�*/
	Point3D endOfHalfEdge(int eid) {
		return vertexs[halfEdges[idOfNextHalfEdge(eid)].idOfStartPoint];
	}

	/*����һ�����������������ж���, ����vidΪ�õ��id�� eidΪ��vidΪ����һ�����id�������ڽӵ��id*/
	vector<int> findAllAdjVer(int vid, int eid);
};
