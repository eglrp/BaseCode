#pragma once
#include"stdafx.h"
#include"Point3D.h"
#include"Face.h"

/*基本模型类*/
class BaseModel{

public:
	/*半边结构*/
	struct HalfEdge{
		/*该半边的长度*/
		double length;
		/*对边的id*/
		int idOfOppositeEdge;
		/*起点的id*/
		int idOfStartPoint;

		/*无参数构造函数*/
		HalfEdge() {
			length = 0;
			idOfOppositeEdge = -1;
			idOfStartPoint = -1;
		}

		/*带参数的构造函数*/
		HalfEdge(double _length, int _idOfOppositeEdge, int _idOfStartPoint) {
			length = _length;
			idOfOppositeEdge = _idOfOppositeEdge;
			idOfStartPoint = _idOfStartPoint;
		}
	};

public:
	/*导入文件名*/
	char inputFileName[256];
	/*点集*/
	vector<Point3D> vertexs;
	/*面集*/
	vector<Face> faces;
	vector<Face> orgfaces;
	/*半边集*/
	vector<HalfEdge> halfEdges;

	/*******QQQ***s*****/
public:
	Point3D centroid; // 点云质心
	vector<Point3D> denseVertex;//密集顶点
	
	/********cwh****s****/
	vector<int> denseRelation; //密集点和面的对应关系
	/*******cwh*****e****/

	/*读写TXT点云文件*/
	void readTxtPointCloudFile(const char *fileName);
	void wirteTxtPointCloudFile(char *fileName);
	/*对点云均匀采样*/
	void uniformSampling(double ratio);
	void uniformSampling(int num);
	/*对三角网格采样，取密集点云*/
	void denseVertices(double s);
	/*对三角网格细化，面积大于阈值的三角形一分为四*/
	void refineTriangles(double area, vector<Point3D> &rvertexs, vector<Face> &rfaces);
	void clear();
	/*****QQQ****e*****/
	
	/********cwh***s*******/
public:
	/*得到一个点对三角网格的对应关系*/
	vector<vector<int>> relation;		//保存点与面之间的关系
	void getRelation(vector<Point3D> gvertexs, vector<Face> gfaces);
	/********cwh***e*******/

	/*构造函数*/
public:

	/*无参数的构造函数，将点集和面集清空*/
	BaseModel() {
		vertexs.clear();
		faces.clear();
		halfEdges.clear();
		denseVertex.clear();
	}

	/*带参数的构造函数，参数是导入的文件名，并将点集和面集清空*/
	BaseModel(char *fileName) {
		memcpy(inputFileName, fileName, strlen(fileName) + 1);
		vertexs.clear();
		faces.clear();
		denseVertex.clear();
	}

	/*文件导入的类方法*/
public:
	/*导入.obj文件*/
	void readObjFile(char *fileName);

	/*导入.ply文件*/
	void readPlyFile(const char *fileName);
	/*ply文本文件读入*/
	void ReadAsciiPly(const char *fileName);
	/*ply二进制文件读入*/
	void readBinaryPly(const char * filename, const char* format);

	/*导入.m文件*/
	void readMFile(char *fileName);

	/*导入.off文件*/
	void readOffFile(char *fileName);

	/*导入点云文件，点云文件的后缀可能有多种（.stl, .ply, .obj, .x3d），在实现的时候再具体细分且调用相关方法*/
	void readPointCloudFile(char *fileName);

	/*导入.stl文件*/
	void readStlFile(char *fileName);

	/*导入.X3D文件*/
	void readX3DFile(char *fileName);

	/*导出文件*/
	void outputFile(char *fileName);

	void ReadUntilNextLine(ifstream& in) const;
	void ReadIntoWord(ifstream &in, string& word) const;
	void SwapOrder(char *buf, int sz);

	/*对应的半边结构的方法和操作*/
public:

	/*创建半边结构*/
	void creatHalfEdge();

	/*所在面的id, 参数eid代表该半边所在模型半边集中的id*/
	int idOfFace(int eid) {
		return eid % 3;
	}

	/*下一条半边的id， 参数eid代表该半边所在模型半边集中的id*/
	int idOfNextHalfEdge(int eid) {
		return eid / 3 + (eid + 1) % 3;
	}

	/*上一条半边的id， 参数eid代表该半边所在模型半边集中的id*/
	int idOfPreHalfEdge(int eid) {
		return eid / 3 + (eid + 2) % 3;
	}

	/*该半边的起点*/
	Point3D startOfHalfEdge(int eid) {
		return vertexs[halfEdges[eid].idOfStartPoint];
	}

	/*该半边的终点*/
	Point3D endOfHalfEdge(int eid) {
		return vertexs[halfEdges[idOfNextHalfEdge(eid)].idOfStartPoint];
	}

	/*遍历一个顶点所关联的所有顶点, 参数vid为该点的id， eid为以vid为起点的一条半边id。返回邻接点的id*/
	vector<int> findAllAdjVer(int vid, int eid);
};
