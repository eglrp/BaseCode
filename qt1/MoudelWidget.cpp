#include "MoudelWidget.h"
#include <QWheelEvent>
#include <gl\freeglut.h>

using namespace std;

MoudelWidget::MoudelWidget(QWidget* parent)
:QOpenGLWidget(parent)
{
	initializeGL();
	mouseX = mouseY = 0;
	isMouseDown = false;
	rotX = rotY = 0;
	translateX = 0.0;
	translateY = 0.0;
	scale = 1.0;
	zoom = -1.0;
	isOpen = true;
	/*cwh*/
	mean = 0;
	variance = 0;

	//************QQQ
	icpM = NULL;
	icpT = NULL;
	isWireframe = false;//�Ƿ���ʾ������
	isICP = true;
	isLockView = false;
	num_m = 0;
	num_t = 0;
	fittingShape = Shape::PLANE;
	viewMode = ViewMode::MODE_POINTS;
	for (int i = 0; i < 5; i++)
	{
		fittingCoe[i] = 0.0;
	}
}

void MoudelWidget::setOpen(){
	isOpen = true;
}
void MoudelWidget::setClose(){
	isOpen = false;
}

void MoudelWidget::initializeGL()
{
	glEnable(GL_DEPTH_TEST);//��Ȳ��ԣ����ֵ��Ч
	glEnable(GL_BLEND);		//��ɫ���
	//glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);//������ɫ׷��

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//������ȷ�ΧΪ���

	GLfloat lightPosition[] = { 1.0, 1.0, 100.0, 0.0 };		//��Դλ��,xyz,dis
	GLfloat lightAmbient[] = { 0.7, 0.7, 0.7, 1.0 };		//ȫ�ֹ�����,��ǿ��
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//ָ����Ҫ��ɫ׷�ٵĲ�������
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//ָ����0�Ź�Դ��λ�� 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //������� 
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//���淴���
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);


	//glEnable(GL_DEPTH_TEST);//��Ȳ��ԣ����ֵ��Ч
	////glEnable(GL_BLEND);//��ɫ���
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glEnable(GL_COLOR_MATERIAL);//������ɫ׷��
	//glShadeModel(GL_SMOOTH);
	//glClearDepth(1.0f);		//������ȷ�ΧΪ���

	//GLfloat position[] = { 1.0, 20.0, 20.0, 1.0 };		//��Դλ��
	//GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };	//ȫ�ֹ�����

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//ָ����Ҫ��ɫ׷�ٵĲ�������
	//glLightfv(GL_LIGHT0, GL_POSITION, position);//ָ����0�Ź�Դ��λ�� 
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void MoudelWidget::initWidget()
{

}

void MoudelWidget::paintGL()
{
	if (isOpen)
		glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);				//�رչ�����Ⱦ

	glClearColor(0.0, 0.0, 0.0, 1.0);			//������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (viewMode == ViewMode::MODE_FILL)		//ʵ��ģʽ
	{
		DrawModelFill(GL_RENDER);
	}
	else if (isICP)									//�Ƿ���ʾ��׼���ƣ�QQQ
	{
		drawPointsICP(M_model, T_model);			//������׼����,QQQ
	}

}

void MoudelWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	paintGL();
}

void MoudelWidget::resizeGL(int width, int height)
{
	if (0 == height) {
		height = 1;
	}
	iWinWidth = (GLint)width;
	iWinHeight = (GLint)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);	//ͶӰ��ʽ��͸��ͶӰ

	GLdouble aspectRatio = (GLfloat)width / (GLfloat)height;
	GLdouble zNear = 0.1;
	GLdouble zFar = 100.0;

	GLdouble rFov = 45.0 * 3.14159265 / 180.0;
	glFrustum(-zNear * tan(rFov / 2.0) * aspectRatio,
		zNear * tan(rFov / 2.0) * aspectRatio,
		-zNear * tan(rFov / 2.0),
		zNear * tan(rFov / 2.0),
		zNear, zFar);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void MoudelWidget::GetNormal(BaseModel model)
{
	triNormals.clear();
	for (int i = 0; i < model.faces.size(); i++)
	{
		Point3D normal; normal.x = 0.0; normal.y = 0.0; normal.z = 0.0;
		double v1[3], v2[3], v3[3];

		v1[0] = model.vertexs[model.faces[i].vertex[0]].x;
		v1[1] = model.vertexs[model.faces[i].vertex[0]].y;
		v1[2] = model.vertexs[model.faces[i].vertex[0]].z;

		v2[0] = model.vertexs[model.faces[i].vertex[1]].x;
		v2[1] = model.vertexs[model.faces[i].vertex[1]].y;
		v2[2] = model.vertexs[model.faces[i].vertex[1]].z;

		v3[0] = model.vertexs[model.faces[i].vertex[2]].x;
		v3[1] = model.vertexs[model.faces[i].vertex[2]].y;
		v3[2] = model.vertexs[model.faces[i].vertex[2]].z;

		ComputeNormal(v1, v2, v3, normal);

		triNormals.push_back(normal);
	}
}

//���㷨���������������㣬������ķ�������ʹ�ò�����㣬n=a��b��
void MoudelWidget::ComputeNormal(double v1[], double v2[], double v3[], Point3D &normal)
{
	double vc1[3], vc2[3];//��������������ƽ�е�����
	double a, b, c;
	double r;
	vc1[0] = v2[0] - v1[0]; vc1[1] = v2[1] - v1[1]; vc1[2] = v2[2] - v1[2];
	vc2[0] = v3[0] - v1[0]; vc2[1] = v3[1] - v1[1]; vc2[2] = v3[2] - v1[2];
	a = vc1[1] * vc2[2] - vc2[1] * vc1[2];
	b = vc2[0] * vc1[2] - vc1[0] * vc2[2];
	c = vc1[0] * vc2[1] - vc2[0] * vc1[1];
	r = sqrt(a * a + b * b + c * c);
	normal.x = a / r;	//��һ��
	normal.y = b / r;
	normal.z = c / r;
}

void MoudelWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_W:
		translateY += (GLfloat)0.2;
		update();
		break;

	case Qt::Key_S:
		translateY -= (GLfloat)0.2;
		update();
		break;

	case Qt::Key_Up:
		translateY += (GLfloat)0.2;
		update();
		break;

	case Qt::Key_Down:
		translateY -= (GLfloat)0.2;
		update();
		break;

	case Qt::Key_Right:
		translateX -= (GLfloat)0.2;
		update();
		break;

	case Qt::Key_Left:
		translateX += (GLfloat)0.2;
		update();
		break;

	case Qt::Key_A:
		translateX -= (GLfloat)0.2;
		update();
		break;

	case Qt::Key_D:
		translateX += (GLfloat)0.2;
		update();
		break;


	case Qt::Key_Escape:
		close();
		break;

	}
}

void MoudelWidget::mouseMoveEvent(QMouseEvent *e){
	if (isMouseDown&&isLockView == false)
	{
		int deltX, deltY;
		// ��������ƶ����룬��תͼ��
		deltY = e->x() - mouseX;
		deltX = e->y() - mouseY;
		// ���µ�ǰ���λ�ã�ʹͼ��ʵʱ��ת
		mouseX = e->x();
		mouseY = e->y();

		// rotate  
		rotX += deltX;
		rotY += deltY;

		update();// ˢ�´���
	}

}
void MoudelWidget::mousePressEvent(QMouseEvent *e){
	mouseX = e->x();
	mouseY = e->y();
	isMouseDown = true;
}
void MoudelWidget::mouseReleaseEvent(QMouseEvent *e){
	if (isLockView == true)
	{
		clearSelected();//����ϴε�ѡ��
		int w = abs(e->x() - mouseX);
		int h = abs(e->y() - mouseY);
		PickMouse(mouseX, mouseY, w, h);
	}
	isMouseDown = false;

}
void MoudelWidget::mouseDoubleClickEvent(QMouseEvent *e){
	PickMouse(e->x(), e->y());
}
void MoudelWidget::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;//�����ĽǶȣ�*8�����������ľ���
	double numSteps = numDegrees / 120.0;//�����Ĳ�����*15�����������ĽǶ�
	//zoom += numSteps;
	scale += numSteps;
	if (scale < 0.005) scale = 0.05;
	event->accept();      //���ո��¼�
	update();
}

//void MoudelWidget::DrawModelPoints(GLenum mode)
//{
//	glPushMatrix();
//
//	glTranslatef(translateX, translateY, zoom);		// ƽ�� 
//	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// ��Y����ת
//	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// ��X����ת
//	glScaled(scale, scale, scale);					// ����
//
//	glBegin(GL_POINTS);					// ��ʼ���Ƹ�����  
//	if (cr.size() != ply.getTotalVertex())
//	{
//		for (int i = 0; i < ply.getTotalVertex(); i++)
//		{
//			//glColor3f(vecVertexColor8[i].r, vecVertexColor8[i].g, vecVertexColor8[i].b);
//			glColor3f(1.0, 0.0, 0.0);
//			glVertex3f(ply.vecVertex[i].x, ply.vecVertex[i].y, ply.vecVertex[i].z);
//		}
//	}
//	glEnd();
//	glPopMatrix();
//}

void MoudelWidget::setFill(){
	viewMode = ViewMode::MODE_FILL;
	update();
}
void MoudelWidget::setPoint(){
	viewMode = ViewMode::MODE_POINTS;
	isICP = true;
	update();
}

////������䶥�㣬�������ģ�Ͷ��㣬ÿ�������һ����
//void MoudelWidget::SetVertexRandom()
//{
//	/**
//	* ������Ķ���vecFaceTriangle���㣬�������vecVertexRand��
//	* vecFaceTriangle�����������ӦvecVertexRand��һ�����㡣
//	*/
//	vecVertexRand.clear();
//	for (int i = 0, pos = 0; i < ply.getTotalFace(); i++, pos += 9)//��ÿ���棻
//	{
//		int r = (rand() % 3) * 3;
//		vertex ver;
//		ver.x = ply.vecFaceTriangle[pos + r];
//		ver.y = ply.vecFaceTriangle[pos + r + 1];
//		ver.z = ply.vecFaceTriangle[pos + r + 2];
//		ver.r = ply.vecTriangleColor[pos + r];
//		ver.g = ply.vecTriangleColor[pos + r + 1];
//		ver.b = ply.vecTriangleColor[pos + r + 2];
//
//		vecVertexRand.push_back(ver);
//	}
//}
////�������ģ�͵Ķ��㣬ʹ�õ����ģ��
//void MoudelWidget::DrawModelRand(GLenum mode)
//{
//	glPushMatrix();
//
//	glTranslatef(translateX, translateY, zoom);		// ƽ�� 
//	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// ��Y����ת
//	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// ��X����ת
//	glScaled(scale, scale, scale);					// ����
//
//	glBegin(GL_POINTS);					// ��ʼ���Ƹ�����  
//	if (vecVertexRand.size() == ply.getTotalFace())
//	{
//		for (int i = 0; i < ply.getTotalFace(); i++)
//		{
//			glColor3f(vecVertexRand[i].r, vecVertexRand[i].g, vecVertexRand[i].b);
//			glVertex3f(vecVertexRand[i].x, vecVertexRand[i].y, vecVertexRand[i].z);
//		}
//	}
//	glEnd();
//	glPopMatrix();
//}

void MoudelWidget::PickMouse(int x, int y, GLdouble rw, GLdouble rh)
{
	if (rw == 0 || rh == 0)
		return;

	GLdouble centerX = GLdouble(x) + rw / 2;//ʰȡ��������
	GLdouble centerY = GLdouble(y) + rh / 2;
	const int PICK_SIZE = 8192;
	GLuint pickBuffer[PICK_SIZE];
	GLint hits, view[4];

	glSelectBuffer(PICK_SIZE, pickBuffer);	//����ʰȡ������
	glRenderMode(GL_SELECT);				//�л�ʰȡģʽ
	glInitNames();							//��ʼ�����ֶ�ջ
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//glGetIntegerv(GL_VIEWPORT, view);
	view[0] = this->rect().x();
	view[1] = this->rect().y();
	view[2] = this->rect().width();
	view[3] = this->rect().height();

	gluPickMatrix(centerX, (view[3] - centerY), rw, rh, view);	//����һ��ѡ������

	GLdouble aspectRatio = (GLfloat)iWinWidth / (GLfloat)iWinHeight;
	GLdouble zNear = 0.1;
	GLdouble zFar = 3.0;
	GLdouble rFov = 45.0 * 3.14159265 / 180.0;
	glFrustum(-zNear * tan(rFov / 2.0) * aspectRatio,
		zNear * tan(rFov / 2.0) * aspectRatio,
		-zNear * tan(rFov / 2.0),
		zNear * tan(rFov / 2.0),
		zNear, zFar);

	if (isICP)
		drawPointsICP(M_model, T_model, GL_SELECT);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	hits = glRenderMode(GL_RENDER);		//����ʰȡ

	if (hits > 0)
	{
		if (isICP)
		{
			ProcessPicks(hits, pickBuffer);
		}
	}
	else
	{
		QMessageBox::warning(this, T_CHAR("ѡȡʧ��"), T_CHAR("ѡȡ�ĵ㼯���࣡"));
	}
	update();// ˢ�´���
}

void  MoudelWidget::ProcessPicks(GLint tPicks, GLuint *pickBuffer)
{
	GLuint nameTotal;//ʰȡ����Ԫ�������������ȼ���ֻ��1��
	GLuint zNear, zFar;
	GLuint name;//����ʰȡ����������(�������ֱ�ʾ���)
	selectedPoints.clear();

	for (GLint i = 0; i < tPicks; i++)//��������ʰȡ����
	{
		nameTotal = *pickBuffer++;		//ѡ��ͼԪ�ڶ�ջ�е�λ��
		zNear = *pickBuffer++;
		zFar = *pickBuffer++;
		name = *pickBuffer++;
		selectedPoints.push_back(name);


		if (name == 237 || name == 238)
		{
			T_color[name * 3 + 0] = 0.0;
			T_color[name * 3 + 1] = 0.0;
			T_color[name * 3 + 2] = 1.0;
		}
		else
		{
			T_color[name * 3 + 0] = 1.0;
			T_color[name * 3 + 1] = 0.0;
			T_color[name * 3 + 2] = 0.0;
		}

	}


}

void MoudelWidget::clearSelected()
{
	selectedPoints.clear();
	mean = 0.0;
	for (GLint i = 0; i < num_t; i++)//��������ʰȡ����
	{

		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}

	update();
}

//ÿ�λ���һ���棬ÿ�����Ӧһ�����֣���������ʰȡѡ��
void MoudelWidget::DrawModelFill(GLenum mode)
{
	glPushMatrix();

	glTranslatef(translateX, translateY, zoom);		// ƽ�� 
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// ��Y����ת
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// ��X����ת
	glScaled(scale, scale, scale);					// ����

	for (int i = 0; i < M_model.orgfaces.size(); i++)
	{
		glBegin(GL_TRIANGLES);					// ��ʼ���Ƹ�����
		glNormal3d(triNormals[i].x, triNormals[i].y, triNormals[i].z);
		glColor4f(0.5, 0.5, 0.5, 1.0);
		glVertex3d(M_model.vertexs[M_model.orgfaces[i].vertex[0]].x, M_model.vertexs[M_model.orgfaces[i].vertex[0]].y, M_model.vertexs[M_model.orgfaces[i].vertex[0]].z);
		glVertex3d(M_model.vertexs[M_model.orgfaces[i].vertex[1]].x, M_model.vertexs[M_model.orgfaces[i].vertex[1]].y, M_model.vertexs[M_model.orgfaces[i].vertex[1]].z);
		glVertex3d(M_model.vertexs[M_model.orgfaces[i].vertex[2]].x, M_model.vertexs[M_model.orgfaces[i].vertex[2]].y, M_model.vertexs[M_model.orgfaces[i].vertex[2]].z);
		glEnd();
	}

	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		if (mode == GL_SELECT)
		{
			glLoadName(i);
		}
		glBegin(GL_POINTS);
		glColor3d(T_color[i * 3 + 0], T_color[i * 3 + 1], T_color[i * 3 + 2]);
		glVertex3d(T_model.vertexs[i].x, T_model.vertexs[i].y, T_model.vertexs[i].z);
		glEnd();
	}

	glPopMatrix();
}
//*s****************************QQQ
/*
���룺
M���ƣ�M��������
T���ƣ�T��������
*/
void MoudelWidget::drawPointsICP(BaseModel &M, BaseModel &T, GLenum mode)
{
	glPushMatrix();

	glTranslatef(translateX, translateY, zoom);		// ƽ�� 
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// ��Y����ת
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// ��X����ת
	glScaled(scale, scale, scale);					// ����

	//��׼T-M
	glBegin(GL_POINTS);//M����
	for (int i = 0; i < M.denseVertex.size(); i++)
	{
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(M.denseVertex[i].x, M.denseVertex[i].y, M.denseVertex[i].z);

	}
	glEnd();

	for (int i = 0, j = 0; i < T.vertexs.size(); i++, j += 3)
	{
		if (mode == GL_SELECT)
		{
			glLoadName(i);
			glBegin(GL_POINTS);//T����
			glColor3f(T_color[j + 0], T_color[j + 1], T_color[j + 2]);
		}
		else
		{
			glBegin(GL_POINTS);//T����
			if (fabs(mean)<1e-9)
			{
				glColor3f(T_color[j + 0], T_color[j + 1], T_color[j + 2]);
			}
			else
			{
				double top = mean * 5, val;
				if (offset[i] > top)
					val = 0;
				else val = 0.9 - offset[i] / top*0.9;
				glColor3f(0.9, val, val);
			}
		}
		glVertex3d(T.vertexs[i].x, T.vertexs[i].y, T.vertexs[i].z);
		glEnd();
	}
	if (isWireframe){
		glBegin(GL_LINES);//M����
		for (int i = 0, j = 0; i < M.faces.size(); i++, j += 3)
		{
			glColor4f(0.0, 0.0, 1.0, 1.0);
			glVertex3d(M.vertexs[M.faces[i][0]].x, M.vertexs[M.faces[i][0]].y, M.vertexs[M.faces[i][0]].z);
			glVertex3d(M.vertexs[M.faces[i][1]].x, M.vertexs[M.faces[i][1]].y, M.vertexs[M.faces[i][1]].z);
			glVertex3d(M.vertexs[M.faces[i][2]].x, M.vertexs[M.faces[i][2]].y, M.vertexs[M.faces[i][2]].z);

			glVertex3d(M.vertexs[M.faces[i][0]].x, M.vertexs[M.faces[i][0]].y, M.vertexs[M.faces[i][0]].z);
			glVertex3d(M.vertexs[M.faces[i][1]].x, M.vertexs[M.faces[i][1]].y, M.vertexs[M.faces[i][1]].z);
			glVertex3d(M.vertexs[M.faces[i][2]].x, M.vertexs[M.faces[i][2]].y, M.vertexs[M.faces[i][2]].z);

		}
		glEnd();
	}
	//������ϵ�ģ��
	if (fittingCoe[0] != 0.0 || fittingCoe[1] != 0.0 || fittingCoe[2] != 0.0 || fittingCoe[3] != 0.0 || fittingCoe[4] != 0.0 || fittingCoe[5] != 0.0)
	{

		switch (fittingShape)
		{
		case NONE:
			break;
		case LINE:
		{
					 glBegin(GL_LINES);
					 glColor4f(1.0, 1.0, 0.8, 0.5);
					 glVertex3d(fittingCoe[0] - fittingCoe[3] * 2, fittingCoe[1] - fittingCoe[4] * 2, fittingCoe[2] - fittingCoe[5] * 2);
					 glVertex3d(fittingCoe[0] + fittingCoe[3] * 2, fittingCoe[1] + fittingCoe[4] * 2, fittingCoe[2] + fittingCoe[5] * 2);
					 glEnd();
		}
			break;
		case PLANE:
			break;
		case CIRCULAR:
		{
						 Point3D c_normal(fittingCoe[4], fittingCoe[5], fittingCoe[6]);
						 Point3D xy_normal(0, 0, 1);
						 double rotationAngle = M_PI - angleBetween(c_normal, xy_normal);
						 Point3D rotationAxis = c_normal * xy_normal;

						 glPushMatrix();
						 //glRotated(rotationAngle * 180 / M_PI, rotationAxis.x, rotationAxis.y, rotationAxis.z);
						 glTranslatef(fittingCoe[0], fittingCoe[1], fittingCoe[2]);
						 glBegin(GL_LINE_LOOP);
						 glColor4f(1.0, 1.0, 0.8, 0.5);
						 //����Բ
						 int N = 200;
						 for (int i = 0; i < N; i++)
						 {
							 double x = fittingCoe[3] * cos(2 * M_PI / N*i);
							 double y = fittingCoe[3] * sin(2 * M_PI / N*i);
							 double z = 0;
							 Quaternion qua(x, y, z);
							 qua = qua.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
							 glVertex3d(qua.x, qua.y, qua.z);
							 //glVertex3d(x, y, z);
						 }

						 glEnd();
						 glPopMatrix();
		}
			break;
		case ELLIPSE:
		{
			vector<Point3D> ellipse;

			Point3D normal_p(fittingCoe[0], fittingCoe[1], fittingCoe[2]);//��Բƽ�淨����
			Point3D normal_xy(0, 0, 1);//xyƽ�淨����

			double angle_p_xy = -angleBetween(normal_p, normal_xy);//��ת��
			Point3D rotationAxis = normal_p * normal_xy;//��ת��

			Point3D o((fittingCoe[13] + fittingCoe[10]) / 2, (fittingCoe[14] + fittingCoe[11]) / 2, (fittingCoe[15] + fittingCoe[12]) / 2);
			PointModel::createEllipse(fittingCoe[6], fittingCoe[7], o, ellipse, rotationAxis, angle_p_xy, 0.05);

			glPushMatrix();
		
			glBegin(GL_LINE_LOOP);
			glColor4f(1.0, 1.0, 0.8, 0.5);
			//����Բ
			for (int i = 0; i < ellipse.size(); i++)
			{
				glVertex3d(ellipse[i].x, ellipse[i].y, ellipse[i].z);
			}

			glEnd();
			glPopMatrix();
		}
			break;
		case CYLINDER:
		{
						 Point3D c_normal(fittingCoe[3], fittingCoe[4], fittingCoe[5]);
						 Point3D xy_normal(0, 0, 1);
						 double rotationAngle = M_PI - angleBetween(c_normal, xy_normal);
						 Point3D rotationAxis = c_normal * xy_normal;

						 glPushMatrix();
						 glTranslatef(fittingCoe[0], fittingCoe[1], fittingCoe[2]);
						 glRotated(rotationAngle * 180 / M_PI, rotationAxis.x, rotationAxis.y, rotationAxis.z);
						 GLUquadricObj *quadricObj = gluNewQuadric();
						 gluQuadricDrawStyle(quadricObj, GLU_LINE);

						 //glBegin(GL_POINTS);
						 glColor4f(1.0, 1.0, 0.5, 0.5);

						 gluCylinder(quadricObj, fittingCoe[6], fittingCoe[6], 0.3, 50, 30);

						 //vector<Point3D> vs;
						 //PointModel::createCylinder(fittingCoe[6], 0.5, vs, c_normal); 
						 //for (int i = 0; i < vs.size(); i++)
						 //{
						 // glVertex3d(vs[i].x, vs[i].y, vs[i].z);
						 //}
						 //glEnd();

						 glPopMatrix();
		}
			break;
		case CONE:
		{
					 Point3D c_normal(fittingCoe[0] - fittingCoe[3]  , fittingCoe[1] - fittingCoe[4]  ,  fittingCoe[2] - fittingCoe[5]);
					 Point3D xy_normal(0, 0, 1);
					 double rotationAngle = M_PI - angleBetween(c_normal, xy_normal);
					 Point3D rotationAxis = c_normal * xy_normal;

					 glPushMatrix();
					 glTranslatef(fittingCoe[0], fittingCoe[1], fittingCoe[2]);
					 glRotated(rotationAngle * 180 / M_PI, rotationAxis.x, rotationAxis.y, rotationAxis.z);
					 glColor4f(1.0, 1.0, 0.5, 0.5);
					 double r = 0.35*tan(fittingCoe[6]);
					 GLUquadricObj *quadricObj = gluNewQuadric();
					 gluQuadricDrawStyle(quadricObj, GLU_LINE);
					 gluCylinder(quadricObj, 0, r, 0.35, 20, 20);
					 glPopMatrix();
		}
			break;
		case SPHERE:{
						glPushMatrix();
						glTranslatef(fittingCoe[0], fittingCoe[1], fittingCoe[2]);
						GLUquadricObj *quadricObj = gluNewQuadric();
						gluQuadricDrawStyle(quadricObj, GLU_LINE);
						glColor4f(1.0, 1.0, 0.8, 0.5);
						gluSphere(quadricObj, fittingCoe[3], fittingCoe[3] * 500, fittingCoe[3] * 500);
						glPopMatrix();
		}
			break;
		default:
			break;
		}
	}
	glPopMatrix();
}

/*����ȥ��
* ���룺	points ----- ����
*			p_num  ----- ������
*			indist ----- ��ֵ�������������
* �����	points ----- ����
*			p_num  ----- ������
* ���أ�	ȥ���ĵ������
*/
int MoudelWidget::denoising(double* points, int &p_num, const double indist)
{
	int sum_num = p_num;

	std::shared_ptr<kdtree::KDTree>     pointsTree;		// ���Ƶ�KD��
	kdtree::KDTreeArray					pointsData;		// ���Ƶ�����
	kdtree::KDTreeResultVector			neighbors;		// �ڽӵ��ѯ���

	// �����㿽����pointsData
	pointsData.resize(p_num);
	for (int i = 0; i < p_num; i++)//�����ڴ�
	{
		pointsData[i].resize(3);
	}
	for (int32_t m = 0; m < p_num; m++)				// ��ȡ���Ƶ����ж���
	for (int32_t n = 0; n < 3; n++)
	{
		pointsData[m][n] = (float)points[m * 3 + n];
	}
	pointsTree = make_shared<kdtree::KDTree>(pointsData);	// �������ƶ����KD��

	//------------------�����������򣬷ָ�㼯---------------
	bool* visited = new bool[p_num];	// ������ʱ�־����
	vector<int> vecPoints;				// �µĵ���
	for (int unv = 0; unv < p_num; unv++)
	{
		if (visited[unv] != true)//�����ǰ����δ���ʹ�����ӵ�ǰ����
		{

			stack<int> s;							// ��ȱ���ջ
			s.push(unv);							// �Ӷ���unv��ʼ����
			visited[unv] = true;
			vector<int> idxPoints;					// ͬһ��ֵ�ڵĵ㼯
			idxPoints.push_back(unv);				// ����unv��������unv����㼯
			int	i_cur;								// ��ǰ��

			while (!s.empty())						// ������ȱ�������
			{

				i_cur = s.top();
				pointsTree->r_nearest_around_point(i_cur, 0, indist, neighbors);// ������ǰ������ֵ�ڵ��ڵ�
				int i, v;										// i:�ڵ�������ţ�v:�ѷ��ʼ�����
				for (i = 0, v = 0; i < neighbors.size(); i++)	// �����ڵ����飬�ҵ�һ��δ���ʵ��ڵ�
				{
					int i_neighbor = neighbors[i].idx;	// ��ǰ����ڵ�����
					if (visited[i_neighbor] != true)	// �����ǰ�ڵ�δ���ʹ�
					{
						idxPoints.push_back(i_neighbor);// ����
						visited[i_neighbor] = true;
						s.push(i_neighbor);
					}
					else
					{
						v++;
					}

				} // while
				if (v == neighbors.size())// �����ǰ����������ڵ�������ʹ�
				{
					s.pop();
				}
			}
			if (vecPoints.size() < idxPoints.size())//���������������ĵ㼯
			{
				vecPoints = idxPoints;
			}
		} // if
	} // for

	//------------------�����µ���------------------
	p_num = vecPoints.size();//���µ��Ƶĵ�����
	double* _points = new double[p_num * 3];
	for (int i = 0, j = 0; i < p_num; i++, j += 3)
	{
		_points[j + 0] = points[vecPoints[i] * 3 + 0];
		_points[j + 1] = points[vecPoints[i] * 3 + 1];
		_points[j + 2] = points[vecPoints[i] * 3 + 2];
	}
	for (int i = 0; i < p_num * 3; i++)//���µ�������
	{
		points[i] = _points[i];
	}

	return sum_num - p_num;
}

//���㷽���Χ��
//double MoudelWidget::minBoundingBox(double *points, int num)
//{
//	double **__points = new double *[num];
//	for (int i = 0; i < num; i++)
//		__points[i] = new double[3];
//
//	for (int i = 0; i < num; i++)
//	{
//		__points[i][0] = points[i * 3 + 0];
//		__points[i][1] = points[i * 3 + 1];
//		__points[i][2] = points[i * 3 + 2];
//	}
//
//	OBB pobb(num, __points);
//	int ci = pobb.getobb();
//
//	double v = 1;
//	for (int i = 0; i < 3; i++)
//	{
//		v = v*(pobb.max[i] - pobb.min[i]);
//	}
//
//	cout << "�����" << v << endl;
//
//	obb[0][0] = pobb.max[0];
//	obb[0][1] = pobb.max[1];
//	obb[0][2] = pobb.max[2];
//
//	obb[1][0] = pobb.max[0];
//	obb[1][1] = pobb.min[1];
//	obb[1][2] = pobb.max[2];
//
//	obb[2][0] = pobb.min[0];
//	obb[2][1] = pobb.min[1];
//	obb[2][2] = pobb.max[2];
//
//	obb[3][0] = pobb.min[0];
//	obb[3][1] = pobb.max[1];
//	obb[3][2] = pobb.max[2];
//
//	obb[4][0] = pobb.min[0];
//	obb[4][1] = pobb.max[1];
//	obb[4][2] = pobb.min[2];
//
//	obb[5][0] = pobb.max[0];
//	obb[5][1] = pobb.max[1];
//	obb[5][2] = pobb.min[2];
//
//	obb[6][0] = pobb.max[0];
//	obb[6][1] = pobb.min[1];
//	obb[6][2] = pobb.min[2];
//
//	obb[7][0] = pobb.min[0];
//	obb[7][1] = pobb.min[1];
//	obb[7][2] = pobb.min[2];
//	//transformPoints(ci*0.18, direc, points, num);
//
//	return v;
//}

/*���Ʊ任����ת��ƽ��
���룺
angle  ----- ��ת�Ƕ�
direct ----- ��ת����x��y��z�ᣩ
P	   ----- ��ת����
num_p  ----- ���ƶ�������
tx,ty,tz----- ƽ������
*/
void MoudelWidget::transformPoints(double angle, int direct, double* P, int num_p, double tx, double ty, double tz)
{
	float r = angle / 180.0f * 3.141592f;//T��ת����ֵ

	double r00 = 1; double r01 = 0; double r02 = 0;//��ת����
	double r10 = 0; double r11 = 1; double r12 = 0;
	double r20 = 0; double r21 = 0; double r22 = 1;

	switch (direct)
	{
	case 0:{
			   r00 = 1; r01 = 0; r02 = 0;//��ת����
			   r10 = 0; r11 = cos(r); r12 = -sin(r);
			   r20 = 0; r21 = sin(r); r22 = cos(r);
	}break;
	case 1:{
			   r00 = cos(r); r01 = 0; r02 = sin(r);//��ת����
			   r10 = 0; r11 = 1; r12 = 0;
			   r20 = -sin(r); r21 = 0; r22 = cos(r);

	}break;
	case 2:{
			   r00 = cos(r); r01 = -sin(r);  r02 = 0;//��ת����
			   r10 = sin(r);  r11 = cos(r);  r12 = 0;
			   r20 = 0;  r21 = 0;  r22 = 1;
	}break;
	default:
		break;
	}

	double q[3];
	for (int i = 0, k = 0; i < num_p; i++, k += 3)
	{
		P[k + 0] += tx;//ƽ��
		P[k + 1] += ty;
		P[k + 2] += tz;

		q[0] = r00*P[k + 0] + r01*P[k + 1] + r02*P[k + 2];//��ת
		q[1] = r10*P[k + 0] + r11*P[k + 1] + r12*P[k + 2];
		q[2] = r20*P[k + 0] + r21*P[k + 1] + r22*P[k + 2];

		P[k + 0] = q[0];//����T
		P[k + 1] = q[1];
		P[k + 2] = q[2];
	}
}

/*
* �Գ�������T��ģ�͵���M������׼�����һ��ִ�в��ɹ�����x/y/z������תһ���Ƕȼ�����׼
*/
void MoudelWidget::runICP()
{
	if (T_model.vertexs.size() < 5 || M_model.vertexs.size() < 5)
	{
		QMessageBox::warning(this, T_CHAR("��׼ʧ��"), T_CHAR("��׼ʧ�ܣ�ģ�͵��ƻ򳡾����Ʋ����ڡ�"));
		return;
	}

	if (QMessageBox::question(this, T_CHAR("������׼"), T_CHAR("�Ƿ�ʼ��׼��"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) != QMessageBox::Ok)
		return;

	DWORD begin = ::GetTickCount();//��������ʱ�䣬���

	cout << endl << "(IcpPointToPlane) Running ICP..." << endl;
	int32_t dim = 3;

	//**************************������׼*****************************
	Matrix R = Matrix::eye(3);
	Matrix t(3, 1);

	IcpPointToPoint icp(icpM, M_model.denseRelation, M_model.relation, M_model.vertexs, M_model.orgfaces, num_m, dim);
	//IcpPointToPlane icp(icpM, M_model.denseRelation, M_model.relation, M_model.vertexs, M_model.orgfaces, num_m, dim);
	icp.fit(icpT, num_t, R, t, -1, offset);

	// ������ɹ��ľ��������
	double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
	double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
	double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
	double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

	// ICP������
	cout << endl << "Transformation results:" << endl;
	//cout << "R:" << endl << R << endl << endl;
	cout << "t:" << endl << t0 << "," << t1 << "," << t2 << endl << endl;
	cout << "delta:" << endl << icp.getDelta() << endl << endl;

	double q[3];

	// �任����T
	for (int i = 0; i < 3 * num_t; i += 3)
	{
		q[0] = r00*icpT[i + 0] + r01*icpT[i + 1] + r02*icpT[i + 2] + t0;//������תƽ�Ʊ任
		q[1] = r10*icpT[i + 0] + r11*icpT[i + 1] + r12*icpT[i + 2] + t1;
		q[2] = r20*icpT[i + 0] + r21*icpT[i + 1] + r22*icpT[i + 2] + t2;

		icpT[i + 0] = q[0];//����ģ������
		icpT[i + 1] = q[1];
		icpT[i + 2] = q[2];
	}

	for (int i = 0; i < num_t; i++)
		mean += offset[i];
	mean /= num_t;						//ƽ�����
	for (int i = 0; i < num_t; i++)
		variance += (offset[i] - mean)*(offset[i] - mean);
	variance = sqrt(variance / num_t);	//����

	printf("�㵽���ƽ����%lf ���%lf\n", mean, variance);

	if (mean < 0.002)
	{
		DWORD sumTime = ::GetTickCount() - begin;// ICP��ʱ����������ʱ�䣬�յ�
		cout << endl << "�����" << endl << "�ܺ�ʱ�� " << sumTime / 1000 << " s " << sumTime % 1000 << " ms " << endl;

		return;
	}

	//*****************�ж��Ƿ���׼�ɹ�********************
	double* _T;//������׼��ĳ������ƵĿ���	
	_T = new double[3 * num_t];//������������icpT
	for (int i = 0; i < 3 * num_t; i += 3)
	{
		_T[i + 0] = icpT[i + 0];
		_T[i + 1] = icpT[i + 1];
		_T[i + 2] = icpT[i + 2];
	}
	int direct = 2;//��ǰ��ת�ķ���
	double minMean = mean;//��С���
	int minMeanDirect = 3;//��ѵ���ת���������С

	while (direct > -1)
	{
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			_T[i + 0] = icpT[i + 0];
			_T[i + 1] = icpT[i + 1];
			_T[i + 2] = icpT[i + 2];
		}
		transformPoints(170, direct, _T, num_t);//�任

		Matrix _R = Matrix::eye(3);
		Matrix _t(3, 1);
		icp.fit(_T, num_t, _R, _t, -1, offset);//������׼
		// ������ɹ��ľ��������
		double r00 = _R.val[0][0]; double r01 = _R.val[0][1]; double r02 = _R.val[0][2];
		double r10 = _R.val[1][0]; double r11 = _R.val[1][1]; double r12 = _R.val[1][2];
		double r20 = _R.val[2][0]; double r21 = _R.val[2][1]; double r22 = _R.val[2][2];
		double t0 = _t.val[0][0]; double t1 = _t.val[1][0]; double t2 = _t.val[2][0];
		double q[3];
		// �任����T
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			q[0] = r00*_T[i + 0] + r01*_T[i + 1] + r02*_T[i + 2] + t0;//������תƽ�Ʊ任
			q[1] = r10*_T[i + 0] + r11*_T[i + 1] + r12*_T[i + 2] + t1;
			q[2] = r20*_T[i + 0] + r21*_T[i + 1] + r22*_T[i + 2] + t2;
			_T[i + 0] = q[0];//����ģ������
			_T[i + 1] = q[1];
			_T[i + 2] = q[2];
		}
		for (int i = 0; i < num_t; i++)
			mean += offset[i];
		mean /= num_t;						//ƽ�����
		for (int i = 0; i < num_t; i++)
			variance += (offset[i] - mean)*(offset[i] - mean);
		variance = sqrt(variance / num_t);	//����

		printf("�㵽���ƽ����%lf ���%lf\n", mean, variance);

		if (mean < minMean)//��¼���ű任
		{
			minMean = mean;
			minMeanDirect = direct;
		}

		if (mean < 0.002)//ƽ�����С����ֵ���˳�ѭ��
		{
			icpT = _T;
			break;
		}
		direct--;
	}

	//�����ǰ������ѱ任����ʹ����ѱ任
	if (direct == -1 && minMean != mean)
	{
		transformPoints(170, minMeanDirect, icpT, num_t);//�任

		icp.fit(icpT, num_t, R, t, -1, offset);

		// ������ɹ��ľ��������
		double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
		double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
		double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
		double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

		double q[3];

		// �任����icpT
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			q[0] = r00*icpT[i + 0] + r01*icpT[i + 1] + r02*icpT[i + 2] + t0;//������תƽ�Ʊ任
			q[1] = r10*icpT[i + 0] + r11*icpT[i + 1] + r12*icpT[i + 2] + t1;
			q[2] = r20*icpT[i + 0] + r21*icpT[i + 1] + r22*icpT[i + 2] + t2;

			icpT[i + 0] = q[0];//����ģ������
			icpT[i + 1] = q[1];
			icpT[i + 2] = q[2];
		}
		for (int i = 0; i < num_t; i++)
			mean += offset[i];
		mean /= num_t;						//ƽ�����
		for (int i = 0; i < num_t; i++)
			variance += (offset[i] - mean)*(offset[i] - mean);
		variance = sqrt(variance / num_t);	//����
	}

	//���任��ĳ������Ʊ���
	for (int i = 0; i < num_t; i++)
	{
		T_model.vertexs[i].x = icpT[i * 3 + 0];
		T_model.vertexs[i].y = icpT[i * 3 + 1];
		T_model.vertexs[i].z = icpT[i * 3 + 2];
	}


	DWORD sumTime = ::GetTickCount() - begin;// ICP��ʱ����������ʱ�䣬�յ�
	cout << endl << "�����" << endl << "�ܺ�ʱ�� " << sumTime / 1000 << " s " << sumTime % 1000 << " ms " << endl;

	printf("�㵽���ƽ����%lf ���%lf\n", mean, variance);

	QMessageBox::about(this, T_CHAR("��׼"), T_CHAR("��׼��ɡ�ƽ����%1�����%2����ʱ��%3 s %4 ms").arg(mean).arg(variance).arg(sumTime / 1000).arg(sumTime % 1000));

	update();
}

/*
* ��ȡply��txtģ�͵�T_model��M_model���������㱣�浽��׼����M��T��
* ����ģ�͵���M������ʹ��T_model��һ�㶥������ܼ����ȶ���
*/
void MoudelWidget::readPointsICP()
{
	//////cwh////////
	M_model.getRelation(M_model.vertexs, M_model.faces);  /*�õ�ÿһ��ģ�͵��ģ����������Ķ�Ӧ��ϵ*/
	//////cwh////////

	M_model.denseVertices(0.01);//ȡ�ܼ�����
	M_model.orgfaces = M_model.faces;
	M_model.refineTriangles(0.001, M_model.vertexs, M_model.faces);//ϸ�����ǣ�ע����Ҫ����ȡ�ܼ���֮ǰ,������ܱ�����ԭ��δ֪

	num_t = T_model.vertexs.size();
	num_m = M_model.denseVertex.size();

	// M,T��������
	icpM = new double[3 * num_m];
	icpT = new double[3 * num_t];
	T_color = new double[3 * num_t];

	for (int i = 0, j = 0; i < num_t; i++, j += 3)
	{
		icpT[j + 0] = T_model.vertexs[i].x;
		icpT[j + 1] = T_model.vertexs[i].y;
		icpT[j + 2] = T_model.vertexs[i].z;

		T_color[j + 0] = 0.9;
		T_color[j + 1] = 0.9;
		T_color[j + 2] = 0.9;
	}

	for (int i = 0, j = 0; i < num_m; i++, j += 3)
	{
		icpM[j + 0] = M_model.denseVertex[i].x;
		icpM[j + 1] = M_model.denseVertex[i].y;
		icpM[j + 2] = M_model.denseVertex[i].z;
	}

	double c[3] = { 0, 0, 0 };//����
	for (int i = 0; i < num_t; i++)//����icpT����
	{
		c[0] += icpT[i * 3 + 0];
		c[1] += icpT[i * 3 + 1];
		c[2] += icpT[i * 3 + 2];
	}
	c[0] /= num_t; c[1] /= num_t; c[2] /= num_t;
	for (int i = 0; i < num_t; i++)//icpT�ƶ���ԭ��
	{
		icpT[i * 3 + 0] -= c[0];
		icpT[i * 3 + 1] -= c[1];
		icpT[i * 3 + 2] -= c[2];

		T_model.vertexs[i].x -= c[0];
		T_model.vertexs[i].y -= c[1];
		T_model.vertexs[i].z -= c[2];
	}
}

//ִ�е������
bool MoudelWidget::fitting(double *coe)
{
	if (selectedPoints.size() < 3)
	{
		QMessageBox::warning(this, T_CHAR("�������ʧ��"), T_CHAR("û��ѡ����ƣ�\n��˫��ѡ����ƣ���������ͼѡ����ơ�"));
		return false;
	}

	return pointCloudFitting(T_model.vertexs, selectedPoints, coe, fittingShape);
}

//�������
bool MoudelWidget::pointCloudFitting(vector<Point3D> &vertexs, vector<int> &idxFit, double *coe, Shape shape)
{
	if (idxFit.empty() == true)
		return false;

	switch (shape)
	{
	case NONE:
		break;
	case LINE:
		Fitting::lineFitting(vertexs, idxFit, coe, 1000);
		break;
	case PLANE:
		Fitting::planeFitting(vertexs, idxFit, coe, 1000);
		break;
	case CIRCULAR:
	{
					 vector<Point3D> _vertexs;
					 Point3D cc(0.0, 0.0, 0.0);
					 for (int i = 0; i < idxFit.size(); i++)
					 {
						 int index = idxFit[i];
						 cc.x = cc.x + vertexs[index].x;
						 cc.y = cc.y + vertexs[index].y;
						 cc.z = cc.z + vertexs[index].z;
					 }
					 cc = cc /= idxFit.size();
					 //��ѡ��ĵ��Ƶ���������ԭ��
					 for (int i = 0; i < vertexs.size(); i++)
					 {
						 _vertexs.push_back(vertexs[i] -= cc);
					 }
					 //ʹ����ԭ�㸽���ĵ�������
					 Fitting::circularFitting(_vertexs, idxFit, coe, 1000);
					 //����ϵķ����ƶ���ԭʼ�㴦
					 coe[0] += cc.x;
					 coe[1] += cc.y;
					 coe[2] += cc.z;

					 //Fitting::cylinderFitting1(vertexs, idxFit, coe, 1000);
	}
		break;
	case SPHERE:
		Fitting::sphereFitting(vertexs, idxFit, coe, 1000);
		break;
	case ELLIPSE:
		Fitting::ellipseFitting(vertexs, idxFit, coe, 1000);
		break;
	case CYLINDER:
	{
					 vector<Point3D> _vertexs;
					 Point3D cc(0.0, 0.0, 0.0);
					 for (int i = 0; i < idxFit.size(); i++)
					 {
						 int index = idxFit[i];
						 cc.x = cc.x + vertexs[index].x;
						 cc.y = cc.y + vertexs[index].y;
						 cc.z = cc.z + vertexs[index].z;
					 }
					 cc = cc /= idxFit.size();
					 //��ѡ��ĵ��Ƶ���������ԭ��
					 for (int i = 0; i < vertexs.size(); i++)
					 {
						 _vertexs.push_back(vertexs[i] -= cc);
					 }
					 //Fitting::cylinderFitting(_vertexs, idxFit, coe, 1000);
					 Fitting::cylinderFitting1(_vertexs, idxFit, coe, 1000);
					 coe[0] += cc.x;
					 coe[1] += cc.y;
					 coe[2] += cc.z;

					 //T = Fitting::cylinderFitting2(vertexs, idxFit, coe, 1000);
					 //num_t = idxFit.size();
					 //T_model.vertexs.clear();
					 //for (int i = 0; i < num_t; i++)
					 //{
					 // T_model.vertexs.push_back(Point3D(T[i * 3 + 0], T[i * 3 + 1], T[i * 3 + 2]));
					 //}


	}
		break;
	case CONE:
	{
				 vector<Point3D> _vertexs;
				 Point3D cc(0.0, 0.0, 0.0);
				 for (int i = 0; i < idxFit.size(); i++)
				 {
					 int index = idxFit[i];
					 cc.x = cc.x + vertexs[index].x;
					 cc.y = cc.y + vertexs[index].y;
					 cc.z = cc.z + vertexs[index].z;
				 }
				 cc = cc /= idxFit.size();
				 //��ѡ��ĵ��Ƶ���������ԭ��
				 for (int i = 0; i < vertexs.size(); i++)
				 {
					 _vertexs.push_back(vertexs[i] -= cc);
				 }
				 Fitting::coneFitting(_vertexs, idxFit, coe, 2000);
				 coe[0] += cc.x;
				 coe[1] += cc.y;
				 coe[2] += cc.z;
				 coe[3] += cc.x;
				 coe[4] += cc.y;
				 coe[5] += cc.z;
	}
		break;
	default:
		break;
	}

	for (int i = 0; i < num_t; i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
	for (auto idx : idxFit)
	{
		T_color[idx * 3 + 0] = 0.9;
		T_color[idx * 3 + 1] = 0.0;
		T_color[idx * 3 + 2] = 0.0;
	}

	if (idxFit.empty())
		return false;

	if (coe[0] != 0.0 || coe[1] != 0.0 || coe[2] != 0.0 || coe[3] != 0.0 || coe[4] != 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//����һ��Բ������
void MoudelWidget::createCylinderPoints()
{
	vector<Point3D> cylinder;//����
	PointModel::createCylinder(0.1, 0.8, cylinder, Point3D(5, 1, 0), 0.01, 0.01);

	if (cylinder.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = cylinder;//��ʾ����

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}

//����һ����Բ����
void MoudelWidget::createEllipsePoints()
{
	vector<Point3D> ellipse;//��Բ����
	PointModel::createEllipse(0.24, 0.12, Point3D(0.0, 0.0, 0.0), ellipse, Point3D(1, 1, 0), 1.8, 0.05);

	if (ellipse.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = ellipse;//��ʾ����

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}

//����һ��Բ׶����
void MoudelWidget::createConePoints()
{
	vector<Point3D> cone;//Բ׶����
	PointModel::createCone(Point3D(0, 0.3, 0), Point3D(0.1, -0.3, 0), 0.3, 0.5, cone, 0.01, 0.01);

	if (cone.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = cone;//��ʾ����

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}