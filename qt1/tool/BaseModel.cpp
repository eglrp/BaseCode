#pragma once
#include "BaseModel.h"
#include "stdafx.h"

/*����.off�ļ�*/
void BaseModel::readOffFile(char *fileName){
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.is_open() == false){
		printf("can not open OFF file: %s\n", fileName);
		return;
	}
	string fileType;
	ifile >> fileType;
	if (fileType != "OFF"){
		printf("Please confirm that this is an OFF type file, the first line should be 'OFF'!(when read file %s )\n", fileName);
		return;
	}
	int verticesNumber, facesNumber, nedgesNumber;
	ifile >> verticesNumber >> facesNumber >> nedgesNumber;
	printf("there are %d vertices  and %d faces in the OFF file!\n", verticesNumber, facesNumber);
	for (int i = 0; i < verticesNumber; i++){
		double _x, _y, _z;
		ifile >> _x >> _y >> _z;
		//printf("vertex%d: %f %f %f\n", i, _x, _y, _z);
		BaseModel::vertexs.push_back(Point3D(_x, _y, _z));
	}
	int pointNumberError = 0;
	for (int i = 0; i < facesNumber; i++){
		int pointNumber;
		ifile >> pointNumber;
		if (pointNumber != 3){
			if (pointNumberError == 0)
				printf("Our program only accept face that contain 3 points and this text will only hint once!\n");
			pointNumberError = 1;
			continue;
		}
		Face face;
		for (int i = 0; i < pointNumber; i++){
			ifile >> face.vertex[i];
		}
		//printf("face %d: %d %d %d\n", i, face.vertex[0], face.vertex[1], face.vertex[2]);
		BaseModel::faces.push_back(face);
	}
	ifile.close();
	printf("read over OFF file ! %s \n", fileName);
	printf("totally, %d vertices and %d face have been push into vector!\n", vertexs.size(), faces.size());
}

/*����.obj�ļ�*/
void BaseModel::readObjFile(char *fileName){
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.is_open() == false){
		printf("can not open obj file: %s\n", fileName);
		return;
	}

	string buffer;
	int flag = 0;
	while (!ifile.eof()){
		getline(ifile, buffer);
		//cout << buffer << endl;
		if (buffer[0] == 'v' && buffer[1] == ' '){
			Point3D p;
			sscanf_s(buffer.c_str(), "v %lf %lf %lf", &p.x, &p.y, &p.z);
			//cout << p.x << " " << p.y << " " << p.z << endl;
			vertexs.push_back(p);
		}
		else if (buffer[0] == 'f' && buffer[1] == ' '){
			Face f;
			vector<int> id;
			int u = 0;
			for (int i = 2; i < buffer.length(); i++){
				if (buffer[i] == '/'){
					id.push_back(u);
					u = 0;
					while (i < buffer.length() && buffer[i] != ' ')
						i++;
				}
				else {
					u = u * 10 + buffer[i] - '0';
				}
			}
			for (int i = 0; i < 3 && i < id.size(); i++){
				f.vertex[i] = id[i];
			}
			if (id.size() > 3 && flag == 0){
				flag = 1;
				printf("Our program only accept face that contain 3 points and this text will only hint once!\n");
			}
			faces.push_back(f);
		}
	}

	ifile.close();
	printf("read over obj file ! %s \n", fileName);
	printf("totally, %d vertices and %d face have been push into vector!\n", vertexs.size(), faces.size());
}

/*����.ply�ļ�*/
void BaseModel::readPlyFile(const char *fileName){
	ifstream ifile(fileName, ios_base::in | ios_base::binary);
	if (ifile.is_open() == false){
		printf("can not open ply file: %s\n", fileName);
		return;
	}
	string fileType;
	ifile >> fileType;
	if (fileType != "ply"){
		printf("Please confirm that this is an ply type file, the first line should be 'ply'!(when read file %s )\n", fileName);
		return;
	}
	string buffer;
	do{
		getline(ifile, buffer);
		transform(buffer.begin(), buffer.end(), buffer.begin(), tolower);
	} while (buffer.find("format") == string::npos);
	ifile.close();
	if (buffer.find("ascii") != string::npos){
		printf("this is an ascii type fly file\n");
		ReadAsciiPly(fileName);
	}
	else if (buffer.find("binary") != string::npos){
		printf("this is a %s type fly file\n", buffer.c_str());
		if (buffer.find("little") != string::npos)
			readBinaryPly(fileName, "little");
		else
			readBinaryPly(fileName, "big");
	}
	else {

	}
}
/*ply�ı��ļ�����*/
void BaseModel::ReadAsciiPly(const char *fileName){
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.is_open() == false){
		printf("can not open ply file: %s\n", fileName);
		return;
	}
	string fileType;
	ifile >> fileType;
	if (fileType != "ply"){
		printf("Please confirm that this is an ply type file, the first line should be 'ply'!(when read file %s )\n", fileName);
		return;
	}
	string buffer;
	int verticesNumber = 0, facesNumber = 0;
	do{
		getline(ifile, buffer);
		//printf("%s\n", buffer.c_str());
		if (buffer.find("element vertex") != string::npos){
			sscanf_s(buffer.c_str(), "element vertex %d", &verticesNumber);
		}
		if (buffer.find("element face") != string::npos){
			sscanf_s(buffer.c_str(), "element face %d", &facesNumber);
		}
	} while (buffer != "end_header");
	printf("there are %d vertices  and %d faces in the ply file!\n", verticesNumber, facesNumber);
	char buff[128];
	for (int i = 0; i < verticesNumber; i++){
		double _x, _y, _z;
		ifile.getline(buff, 128);
		sscanf_s(buff, "%lf%lf%lf", &_x, &_y, &_z);
		//ifile >> _x >> _y >> _z;
		BaseModel::vertexs.push_back(Point3D(_x / 1000, _y / 1000, _z / 1000));//---------------1000
		//BaseModel::vertexs.push_back(Point3D(_x, _y, _z));
	}
	int pointNumberError = 0;
	for (int i = 0; i < facesNumber; i++){
		int pointNumber;
		ifile >> pointNumber;
		if (pointNumber != 3){
			if (pointNumberError == 0)
				printf("Our program only accept face that contain 3 points and this text will only hint once!\n");
			pointNumberError = 1;
			continue;
		}
		Face face;
		for (int i = 0; i < pointNumber; i++){
			ifile >> face.vertex[i];
		}
		//printf("face %d: %d %d %d\n", i, face.vertex[0], face.vertex[1], face.vertex[2]);
		BaseModel::faces.push_back(face);
	}
	ifile.close();
	printf("read over ply file ! %s \n", fileName);
	printf("totally, %d vertices and %d face have been push into vector!\n", vertexs.size(), faces.size());
}
/*ply�������ļ�����*/
void BaseModel::readBinaryPly(const char * filename, const char* format){
	ifstream in(filename, ios_base::in | ios_base::binary);
	if (in.fail())
	{
		throw "fail to read file";
	}
	map<string, int> types;
	types["char"] = 1;
	types["uchar"] = 1;
	types["short"] = 2;
	types["ushort"] = 2;
	types["int"] = 4;
	types["uint"] = 4;
	types["float"] = 4;
	types["double"] = 8;
	types["int8"] = 1;
	types["int16"] = 2;
	types["int32"] = 4;
	types["uint8"] = 1;
	types["uint16"] = 2;
	types["uint32"] = 4;
	types["float32"] = 4;
	types["float64"] = 8;

	int vertNum(0);
	int faceNum(0);
	while (!in.eof())
	{
		string word;
		ReadIntoWord(in, word);
		if (word == "element")
		{
			ReadIntoWord(in, word);
			if (word == "vertex")
			{
				in >> vertNum;
				break;
			}
		}
	}

	bool isFloatCoordinate(true);
	int sizeOfOneVert(0);
	string word;
	ReadIntoWord(in, word);
	if (!in.eof() && word == "property")
	{
		ReadIntoWord(in, word);
		transform(word.begin(), word.end(), word.begin(), tolower);
		if (types.find(word) != types.end())
		{
			if (types[word] == 4)
			{
				isFloatCoordinate = true;
			}
			else if (types[word] == 8)
			{
				isFloatCoordinate = false;
			}
			else
			{
				throw "read file error";
			}
			sizeOfOneVert += types[word];
		}
		else
		{
			throw "read file error";
		}
		ReadUntilNextLine(in);
	}
	ReadIntoWord(in, word);
	while (word == "property")
	{
		ReadIntoWord(in, word);
		transform(word.begin(), word.end(), word.begin(), tolower);
		sizeOfOneVert += types[word];
		ReadUntilNextLine(in);
		ReadIntoWord(in, word);
	}
	while (word != "element" || (ReadIntoWord(in, word), word != " " && word != "face"))
	{
		ReadUntilNextLine(in);
		ReadIntoWord(in, word);
	}

	in >> faceNum;
	if (word == "tristrips")
		faceNum = 0;
	ReadUntilNextLine(in);
	ReadIntoWord(in, word);
	while (word != "property" || (ReadIntoWord(in, word), word != "list"))
	{
		ReadUntilNextLine(in);
		ReadIntoWord(in, word);
	}
	ReadIntoWord(in, word);
	transform(word.begin(), word.end(), word.begin(), tolower);
	if (types.find(word) == types.end())
	{
		throw "file read error";
	}
	int szOfNumInFace = types[word];
	ReadIntoWord(in, word);
	transform(word.begin(), word.end(), word.begin(), tolower);
	if (types.find(word) == types.end())
	{
		throw "file read error";
	}
	int szOfVertIndex = types[word];
	do
	{
		ReadIntoWord(in, word);
		ReadUntilNextLine(in);
	} while (word != "end_header");   //�ļ�ͷ����

	map<string, int> uniqueVerts;
	vertexs.reserve(vertNum);
	faces.reserve(faceNum);
	vector<int> vertCorrespondence(vertNum);

	for (int i = 0; i < vertNum; ++i)
	{
		ostringstream outStr;
		int szRemaining = sizeOfOneVert;
		Point3D pt;
		if (isFloatCoordinate)
		{
			float temp[3];
			szRemaining -= 12;
			for (int j = 0; j < 3; ++j)
			{
				float tmp;
				in.read((char *)&tmp, 4);
				if (format == "big")
				{
					SwapOrder((char*)&tmp, 4);
				}
				if (fabs(tmp) < FLT_EPSILON)
					tmp = 0;
				temp[j] = tmp;
			}
			pt.x = temp[0];
			pt.y = temp[1];
			pt.z = temp[2];
		}
		else
		{
			double temp[3];
			szRemaining -= 24;
			for (int j = 0; j < 3; ++j)
			{
				double tmp;
				in.read((char *)&tmp, 8);
				if (format == "big")
				{
					SwapOrder((char*)&tmp, 8);
				}
				if (fabs(tmp) < FLT_EPSILON)
					tmp = 0;
				temp[j] = tmp;
			}
			pt.x = temp[0];
			pt.y = temp[1];
			pt.z = temp[2];
		}
		//int oldSize = (int)uniqueVerts.size();
		//vertCorrespondence[i] = uniqueVerts[outStr.str()] = oldSize;
		vertCorrespondence[i] = i;
		vertexs.push_back(pt);
		/*map<string, int>::iterator pos = uniqueVerts.find(outStr.str());
		if (pos == uniqueVerts.end())
		{
		int oldSize = (int)uniqueVerts.size();
		vertCorrespondence[i] = uniqueVerts[outStr.str()] = oldSize;
		vertexs.push_back(pt);
		}
		else
		{
		vertCorrespondence[i] = pos->second;
		}*/
		//vertexs�������ȥ�غ�ĵ� 
		//uniqueVerts��ʾ����ÿ���㣨���ظ�����vertexs�ж�Ӧ��λ�� 
		//vertCorrespondence��ʾ���������ļ��ĵ�i������vertexs�ж�Ӧ��λ��
		if (szRemaining == 0)
			continue;
		char buf[64];
		in.read((char*)&buf, szRemaining);
	}

	vertexs.swap(vector<Point3D>(vertexs));
	//��vector��swap�����ͷ��ڴ� ʹ�ڴ�����������

	if (faceNum != 0)
	{
		for (int i = 0; i < faceNum; ++i)
		{
			int num(0);
			if (szOfNumInFace == 1)
			{
				char ch;
				in.read(&ch, 1);
				num = ch;
			}
			else if (szOfNumInFace == 2)
			{
				short ch;
				in.read((char*)&ch, 2);
				if (format == "big")
				{
					SwapOrder((char*)&ch, 2);
				}
				num = ch;
			}
			else if (szOfNumInFace == 4)
			{
				int ch;
				in.read((char*)&ch, 4);
				if (format == "big")
				{
					SwapOrder((char*)&ch, 4);
				}
				num = ch;
			}
			vector<int> indices(num);
			for (int j = 0; j < num; ++j)
			{
				int index(0);
				if (szOfVertIndex == 2)
				{
					short ch;
					in.read((char*)&ch, 2);
					if (format == "big")
					{
						SwapOrder((char*)&ch, 2);
					}
					index = ch;
				}
				else if (szOfVertIndex == 4)
				{
					int ch;
					in.read((char*)&ch, 4);
					if (format == "big")
					{
						SwapOrder((char*)&ch, 4);
					}
					index = ch;
				}
				indices[j] = vertCorrespondence[index];
			}
			if (num == 0){
				i--;
				continue;
			}
			for (int j = 1; j < num - 1; ++j)
			{
				faces.push_back(Face(indices[0], indices[j], indices[j + 1]));
			}
		}
	}
	else
	{
		if (szOfNumInFace == 1)
		{
			char ch;
			in.read((char*)&ch, 1);
			faceNum = ch;
		}
		else if (szOfNumInFace == 2)
		{
			short ch;
			in.read((char*)&ch, 2);
			if (format == "big")
			{
				SwapOrder((char*)&ch, 2);
			}
			faceNum = ch;
		}
		else if (szOfNumInFace == 4)
		{
			int ch;
			in.read((char*)&ch, 4);
			if (format == "big")
			{
				SwapOrder((char*)&ch, 4);
			}
			faceNum = ch;
		}
		vector<int> magicBox;
		int minPos(0);
		bool fInverse(false);
		for (int i = 0; i < faceNum; ++i)
		{
			int curIndex;
			if (szOfVertIndex == 1)
			{
				char ch;
				in.read((char*)&ch, 1);
				curIndex = ch;
			}
			else if (szOfVertIndex == 2)
			{
				short ch;
				in.read((char*)&ch, 2);
				if (format == "big")
				{
					SwapOrder((char*)&ch, 2);
				}
				curIndex = ch;
			}
			else if (szOfVertIndex == 4)
			{
				int ch;
				in.read((char*)&ch, 4);
				if (format == "big")
				{
					SwapOrder((char*)&ch, 4);
				}
				curIndex = ch;
			}

			if (curIndex == -1)
			{
				magicBox.clear();
				minPos = 0;
				fInverse = false;
				continue;
			}
			else
			{
				curIndex = vertCorrespondence[curIndex];
			}
			if (magicBox.size() < 3)
			{
				magicBox.push_back(curIndex);
			}
			else
			{
				magicBox[minPos] = curIndex;
				fInverse = !fInverse;
			}
			minPos = (minPos + 1) % 3;
			if (magicBox.size() >= 3)
			{
				if (fInverse)
					faces.push_back(Face(magicBox[0], magicBox[2], magicBox[1]));
				else
					faces.push_back(Face(magicBox[0], magicBox[1], magicBox[2]));
			}
		}
	}
	in.close();
	faces.swap(vector<Face>(faces));
	printf("read over ply file ! %s \n", filename);
	printf("totally, %d vertices and %d face have been push into vector!\n", vertexs.size(), faces.size());
}

/*����.m�ļ�*/
void BaseModel::readMFile(char *fileName){
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.is_open() == false){
		printf("can not open .m file: %s\n", fileName);
		return;
	}

	string buffer;
	int flag = 0;
	map<int, int> pid;
	while (!ifile.eof()){
		getline(ifile, buffer);
		//cout << buffer << endl;
		if (buffer.find("Vertex") != string::npos){
			Point3D p;
			int id;
			sscanf_s(buffer.c_str(), "Vertex %d %lf %lf %lf", &id, &p.x, &p.y, &p.z);
			//cout << p.x << " " << p.y << " " << p.z << endl;
			pid[id] = vertexs.size();
			vertexs.push_back(p);
		}
		else if (buffer.find("Face") != string::npos){
			Face f;
			vector<int> id;
			int u = -1;
			for (int i = 4; i < buffer.size(); i++){
				if (buffer[i] == ' '){
					if (u != -1)
						id.push_back(u);
					u = -1;
				}
				else {
					if (u == -1) u = 0;
					u = u * 10 + buffer[i] - '0';
				}
			}
			if (u != -1) id.push_back(u);
			for (int i = 1; i < 4 && i < id.size(); i++){
				if (pid.find(id[i]) != pid.end())
					f.vertex[i - 1] = pid[id[i]];
				else{
					f.vertex[i - 1] = -1;
					cout << id[i] << endl;
				}
			}
			if (id.size() > 4 && flag == 0){
				flag = 1;
				printf("Our program only accept face that contain 3 points and this text will only hint once!\n");
			}
			faces.push_back(f);
		}
	}

	ifile.close();
	printf("read over .m file ! %s \n", fileName);
	printf("totally, %d vertices and %d face have been push into vector!\n", vertexs.size(), faces.size());
}

/*��������ļ��������ļ��ĺ�׺�����ж��֣�.stl, .ply, .obj, .x3d������ʵ�ֵ�ʱ���پ���ϸ���ҵ�����ط���*/
void BaseModel::readPointCloudFile(char *fileName){
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.is_open() == false){
		printf("can not open this file: %s\n", fileName);
		return;
	}
	string type;
	ifile >> type;
	transform(type.begin(), type.end(), type.begin(), tolower);
	if (type == "ply"){
		readPlyFile(fileName);
	}
	else {
		printf("can not identify this file type [%s] in %s \n", type.c_str(), fileName);
	}
}

/*����.stl�ļ�*/
void BaseModel::readStlFile(char *fileName){

}

/*����.X3D�ļ�*/
void BaseModel::readX3DFile(char *fileName){

}

void BaseModel::ReadIntoWord(ifstream &in, string& word) const
{
	word.clear();
	char ch;
	while (in.read(&ch, 1), (ch == 0xA || ch == 0xD || ch == ' ' || ch == '\t' || ch == '\n') && !in.eof());
	//0xA=10 ��ASCII�б�ʾ���м�<����> ����Ƶ����������   0xD=13 ��ASCII�б�ʾ�س���<�س�> ����Ƶ���һ��
	while (!(ch == 0xA || ch == 0xD || ch == ' ' || ch == '\t' || ch == '\n') && !in.eof())
	{
		word.push_back(ch);
		in.read(&ch, 1);
	}
	if (in.eof())
		return;
	in.putback(ch);
}

//eat off the remainder of the current line.
void BaseModel::ReadUntilNextLine(ifstream& in) const
{
	if (in.eof())
		return;
	char ch;
	while (in.read(&ch, 1), !(ch == 0x0A || ch == 0x0D || in.eof()));
	if (in.eof())
		return;
	while (in.read(&ch, 1), (ch == 0x0A || ch == 0x0D) && !in.eof());
	if (in.eof())
		return;
	in.putback(ch);
}

void BaseModel::SwapOrder(char *buf, int sz)
{
	for (int i = 0; i < sz / 2; ++i)
	{
		char temp = buf[i];
		buf[i] = buf[sz - 1 - i];
		buf[sz - 1 - i] = temp;
	}
}

void BaseModel::outputFile(char *fileName){
	ofstream out;
	out.open(fileName);
	if (out.is_open() == false){
		printf("open file %s failed!\n", fileName);
		return;
	}
	out << vertexs.size() << " " << faces.size() << endl;
	char buffer[256];
	for (int i = 0; i < vertexs.size(); i++){
		out << vertexs[i].x << " " << vertexs[i].y << " " << vertexs[i].z << endl;
	}
	for (int i = 0; i < faces.size(); i++){
		for (int j = 0; j < 3; j++){
			if (j != 0) out << " ";
			out << faces[i].vertex[j];
		}
		out << endl;
	}
}

/*������߽ṹ*/
void BaseModel::creatHalfEdge() {
	halfEdges.clear();
	int cntFaces = faces.size();
	int halfEdgeIdByFace[3];
	map<pair<int, int>, int> halfEdgeByPoint;
	halfEdgeByPoint.clear();

	for (int i = 0; i < cntFaces; i++) {
		for (int j = 0; j < 3; j++) {
			int startPointId = faces[i][j];
			int endPointId = faces[i][(j + 1) % 3];
			HalfEdge newHalfEdge = HalfEdge();
			newHalfEdge.idOfOppositeEdge = -1;
			newHalfEdge.idOfStartPoint = startPointId;
			newHalfEdge.length = dist(vertexs[startPointId], vertexs[endPointId]);
			halfEdges.push_back(newHalfEdge);
			halfEdgeIdByFace[j] = halfEdges.size() - 1;
			int nowId = halfEdgeIdByFace[j];
			if (halfEdgeByPoint.find(make_pair(endPointId, startPointId)) != halfEdgeByPoint.end()) {
				int otherHalfEdgeId = halfEdgeByPoint[make_pair(endPointId, startPointId)];
				halfEdges[nowId].idOfOppositeEdge = otherHalfEdgeId;
				halfEdges[otherHalfEdgeId].idOfOppositeEdge = nowId;
			}
			else {
				halfEdgeByPoint[make_pair(startPointId, endPointId)] = nowId;
			}
		}
	}
}

/*����һ�����������������ж���, ����vidΪ�õ��id�� eidΪ��vidΪ����һ�����id�������ڽӵ��id*/
vector<int> BaseModel::findAllAdjVer(int vid, int eid) {
	vector<int> adjVers;
	adjVers.clear();
	int nextId, tid = eid;
	while (1) {
		int idOfOppositeEdge = halfEdges[tid].idOfOppositeEdge;
		adjVers.push_back(halfEdges[idOfOppositeEdge].idOfStartPoint);
		nextId = halfEdges[idOfPreHalfEdge(tid)].idOfOppositeEdge;
		if (nextId == eid) {
			break;
		}
		else {
			tid = nextId;
		}
	}
	while (1) {
		int idOfNextEdge = BaseModel::idOfNextHalfEdge(tid);
		adjVers.push_back(halfEdges[idOfNextEdge].idOfStartPoint);
		nextId = halfEdges[idOfPreHalfEdge(BaseModel::idOfNextHalfEdge(idOfNextEdge))].idOfOppositeEdge;
		if (nextId == -1) {
			int idOfOppositeEdge = halfEdges[eid].idOfOppositeEdge;
			while (idOfOppositeEdge != -1) {
				nextId = BaseModel::idOfNextHalfEdge(idOfOppositeEdge);
				adjVers.push_back(halfEdges[BaseModel::idOfPreHalfEdge(idOfOppositeEdge)].idOfStartPoint);
				idOfOppositeEdge = halfEdges[nextId].idOfOppositeEdge;
			}
			break;
		}
		else if (nextId == eid) {
			break;
		}
		else {
			tid = nextId;
		}
	}
	return adjVers;
}

/*�洢Txt*/
void BaseModel::wirteTxtPointCloudFile(char *fileName)
{
	ofstream fout;
	fout.open(fileName, ios::out);//ֻ���ķ�ʽ���ļ���

	for (int i = 0; i < vertexs.size(); i++)
	{
		fout << vertexs[i].x << ' ' << vertexs[i].y << ' ' << vertexs[i].z << endl;
	}

	fout.close();
}
void BaseModel::readTxtPointCloudFile(const char *fileName)
{
	ifstream fin;
	fin.open(fileName, ios::in);//ֻ���ķ�ʽ���ļ���
	if (!fin)
	{
		//cout << ERROR << "�ļ���ʧ�ܣ�" << "(" << this->path.data() << ")" << endl;
		return;
	}

	const int BUFFER_SIZE = 300;
	char buffer[BUFFER_SIZE];	//����һ�����ݵ�buffer��

	//-------------------------------------�����ļ�ͷ����ȡ�ļ���Ϣ-------------------------------------
	//��ȡ�ļ�ͷ����
	int skip = 0;	//�Թ��ļ�ͷ
	for (int i = 0; i < skip; i++)
	{
		fin.getline(buffer, 300);
	}
	//-------------------------------------�����������꣬��ȡ������Ϣ-------------------------------------
	//���������걣����vertexs��
	while (!fin.eof())	//��ȡ��������
	{
		Point3D verTemp;

		fin.getline(buffer, 300);
		if (buffer[0] != '\0')
		{
			sscanf_s(buffer, "%lf%lf%lf", &verTemp.x, &verTemp.y, &verTemp.z);	//ȡ��������
			verTemp.x /= 1000; verTemp.y /= 1000; verTemp.z /= 1000;//--------------��С1000��
			vertexs.push_back(verTemp);

			centroid.x += verTemp.x;
			centroid.y += verTemp.y;
			centroid.z += verTemp.z;
		}

	}

	fin.close();

	centroid.x = centroid.x / vertexs.size();//����M������
	centroid.y = centroid.y / vertexs.size();//����M������
	centroid.z = centroid.z / vertexs.size();//����M������
}
void BaseModel::uniformSampling(double ratio)
{
	if (vertexs.size() < 5)
	{
		return;
	}
	if (ratio < 0 || ratio > 1)
	{
		return;
	}
	int skip;
	int totalVertexs;
	totalVertexs = vertexs.size();				//ԭʼ�������
	skip = totalVertexs / (totalVertexs*ratio);	//�ɼ�Ƶ��

	vector<Point3D> _vertexs;					//������ĵ�

	for (int i = 0; i < totalVertexs; i += skip)
	{
		_vertexs.push_back(vertexs[i]);
	}

	vertexs.clear();
	vertexs = _vertexs;
}
void BaseModel::uniformSampling(int num)
{
	if (vertexs.size() < 5)
	{
		return;
	}
	if (num < 1 || num > vertexs.size())
	{
		return;
	}
	int skip;
	int totalVertexs;
	totalVertexs = vertexs.size();				//ԭʼ�������
	skip = totalVertexs / num;					//�ɼ�Ƶ��

	vector<Point3D> _vertexs;					//������ĵ�

	for (int i = 0; i < totalVertexs; i += skip)
	{
		_vertexs.push_back(vertexs[i]);
	}

	vertexs.clear();
	vertexs = _vertexs;
}

//ȡ�ܼ����㣬���룺��������
void BaseModel::denseVertices(double s)
{
	denseVertex.clear();
	denseRelation.clear();
	Point3D v[3];//�����εĶ���
	Point3D left, right, top;
	for (int i = 0; i < faces.size(); i++)//����ÿ�������Σ�ȡ��
	{
		v[0] = vertexs[faces[i][0]];
		v[1] = vertexs[faces[i][1]];
		v[2] = vertexs[faces[i][2]];

		Point3D v10;//����
		Point3D v20;//����
		Point3D v21;//����
		v10 = v[1] - v[0];
		v20 = v[2] - v[0];
		v21 = v[2] - v[1];

		//if (v10.length() < v20.length())//��������ױ�
		//{
		//	if (v20.length() < v21.length())
		//	{
		//		left = v[2];
		//		right = v[1];
		//		top = v[0];
		//	}
		//	else
		//	{
		//		left = v[2];
		//		right = v[0];
		//		top = v[1];
		//	}
		//}
		//else
		//{
		//	if (v10.length() < v21.length())
		//	{
		//		left = v[2];
		//		right = v[1];
		//		top = v[0];
		//	}
		//	else
		//	{

		left = v[0];
		right = v[1];
		top = v[2];

		//	}
		//}

		Point3D b_vector;//�ױ�����
		Point3D l_vector;//�������
		Point3D r_vector;//�ұ�����
		Point3D bu_vector;//�ױߵ�λ����
		Point3D lu_vector;//��ߵ�λ����
		Point3D ru_vector;//�ұߵ�λ����
		b_vector = right - left;
		l_vector = top - left;
		r_vector = top - right;
		bu_vector = b_vector.normalize();
		lu_vector = l_vector.normalize();
		ru_vector = r_vector.normalize();

		double height;		//�����θ�
		height = 2 * getTriangleArea(left, right, top) / b_vector.length();

		int n_b;//�ײ����ܶ�
		int n_h;//�߲����ܶ�

		n_h = height / s;
		n_h = n_h == 0 ? 1 : n_h;
		double sl, sr;
		sl = l_vector.length() / n_h;
		sr = r_vector.length() / n_h;

		double bottom;//�ױ߳���

		//�Բ�������������ȡ��,���ױ�����һ�ξ���
		for (int h = 0; h <= n_h; h++)
		{
			Point3D new_lvertex;//���󶥵�
			Point3D new_rvertex;//���Ҷ���
			new_lvertex.x = left.x + lu_vector.x*sl*h;
			new_lvertex.y = left.y + lu_vector.y*sl*h;
			new_lvertex.z = left.z + lu_vector.z*sl*h;

			new_rvertex.x = right.x + ru_vector.x*sr*h;
			new_rvertex.y = right.y + ru_vector.y*sr*h;
			new_rvertex.z = right.z + ru_vector.z*sr*h;

			b_vector = new_rvertex - new_lvertex;//���¼���ױ�����
			bu_vector = b_vector.normalize();

			bottom = b_vector.length();
			n_b = bottom / s;
			//�Եױ������������ȡ��
			for (int j = 0; j < n_b; j++)
			{
				Point3D new_vertex;
				new_vertex.x = new_lvertex.x + bu_vector.x*s*j;
				new_vertex.y = new_lvertex.y + bu_vector.y*s*j;
				new_vertex.z = new_lvertex.z + bu_vector.z*s*j;

				denseVertex.push_back(new_vertex);//�洢�����Ķ���
				denseRelation.push_back(i);
			}
		}
	}

}

/*
ϸ����������
���룺
area     ----- �����ֵ����������
rvertexs ----- ��ϸ���㼯
rfaces   ----- ��ϸ���漯
�����
rvertexs ----- ϸ���ĵ㼯
rfaces   ----- ϸ�����漯
*/
void BaseModel::refineTriangles(double area, vector<Point3D> &rvertexs, vector<Face> &rfaces)
{
	/*����ϸ���ĵ㼯*/
	vector<Point3D> newVertexs;
	/*����ϸ�����漯*/
	vector<Face> newFaces;

	int idxVertexs = rvertexs.size();//������ĵ�ǰ����
	double curArea = 0.0;//��ǰ�����ε����
	for (int i = 0; i < rfaces.size(); i++)//��ÿ�������λ���
	{
		//���浱ǰ�����εĻ�����
		vector<Face> tFaces;

		Point3D left, right, top;//��ǰ�����εĶ���
		int lidx, ridx, tidx;	//�����ζ�������
		lidx = rfaces[i][0];
		ridx = rfaces[i][1];
		tidx = rfaces[i][2];

		left = rvertexs[lidx];
		right = rvertexs[ridx];
		top = rvertexs[tidx];

		curArea = getTriangleArea(left, right, top);

		//�����ǰ���������������ֵ���򻮷�
		if (curArea > area)
		{
			Point3D cleft, cright, cbottom;	//�����ı��е�
			int clidx, cridx, cbidx;		//�����ı��е��ڶ����б��е�����

			cleft = (left + top) /= 2;
			cright = (right + top) /= 2;
			cbottom = (left + right) /= 2;

			clidx = idxVertexs + 0;
			cridx = idxVertexs + 1;
			cbidx = idxVertexs + 2;

			rvertexs.push_back(cleft);//����µ�
			rvertexs.push_back(cright);
			rvertexs.push_back(cbottom);

			tFaces.push_back(Face(tidx, clidx, cridx));//�������
			tFaces.push_back(Face(clidx, lidx, cbidx));
			tFaces.push_back(Face(clidx, cridx, cbidx));
			tFaces.push_back(Face(cridx, cbidx, ridx));

			refineTriangles(area, rvertexs, tFaces);//�ݹ黮������

			idxVertexs = rvertexs.size();//���µ�ǰ������������
		}
		else
		{
			tFaces.push_back(Face(tidx, lidx, ridx));
		}
		//�������εĻ�������ӵ����漯����
		newFaces.insert(newFaces.end(), tFaces.begin(), tFaces.end());
	}
	rfaces = newFaces;
}

/*�õ�ÿһ�������������Ķ�Ӧ��ϵ*/
void BaseModel::getRelation(vector<Point3D> gvertexs, vector<Face> gfaces)
{
	int i, j, n = gvertexs.size(), m = gfaces.size();
	relation.clear();
	vector<int> tmp;
	tmp.clear();
	for (i = 0; i < n; i++)
		relation.push_back(tmp);
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < 3; j++)
			relation[gfaces[i][j]].push_back(i);
	}
	int sum = 0;
	for (i = 0; i < n; i++)
	{
		sum += relation[i].size();
	}
	cout << sum << endl;
}

void BaseModel::clear()
{
	char inputFileName[256] = "";

	vertexs.clear();
	faces.clear();
	orgfaces.clear();
	halfEdges.clear();
	centroid = Point3D(0.0, 0.0, 0.0);
	denseVertex.clear();
	denseRelation.clear();
	relation.clear();
}