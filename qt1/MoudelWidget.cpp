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
	isWireframe = false;//是否显示网格线
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
	glEnable(GL_DEPTH_TEST);//深度测试，深度值有效
	glEnable(GL_BLEND);		//颜色混合
	//glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);//启用颜色追踪

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//设置深度范围为最大

	GLfloat lightPosition[] = { 1.0, 1.0, 100.0, 0.0 };		//光源位置,xyz,dis
	GLfloat lightAmbient[] = { 0.7, 0.7, 0.7, 1.0 };		//全局光属性,光强度
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//指定需要颜色追踪的材料属性
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//指定第0号光源的位置 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //漫反射后 
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//镜面反射后
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);


	//glEnable(GL_DEPTH_TEST);//深度测试，深度值有效
	////glEnable(GL_BLEND);//颜色混合
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glEnable(GL_COLOR_MATERIAL);//启用颜色追踪
	//glShadeModel(GL_SMOOTH);
	//glClearDepth(1.0f);		//设置深度范围为最大

	//GLfloat position[] = { 1.0, 20.0, 20.0, 1.0 };		//光源位置
	//GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };	//全局光属性

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//指定需要颜色追踪的材料属性
	//glLightfv(GL_LIGHT0, GL_POSITION, position);//指定第0号光源的位置 
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void MoudelWidget::initWidget()
{

}

void MoudelWidget::paintGL()
{
	if (isOpen)
		glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);				//关闭光照渲染

	glClearColor(0.0, 0.0, 0.0, 1.0);			//背景颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (viewMode == ViewMode::MODE_FILL)		//实体模式
	{
		DrawModelFill(GL_RENDER);
	}
	else if (isICP)									//是否显示配准点云，QQQ
	{
		drawPointsICP(M_model, T_model);			//绘制配准点云,QQQ
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

	//gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);	//投影方式：透视投影

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

//计算法向量，根据三个点，计算面的法向量。使用叉积计算，n=a×b。
void MoudelWidget::ComputeNormal(double v1[], double v2[], double v3[], Point3D &normal)
{
	double vc1[3], vc2[3];//三角形内两个不平行的向量
	double a, b, c;
	double r;
	vc1[0] = v2[0] - v1[0]; vc1[1] = v2[1] - v1[1]; vc1[2] = v2[2] - v1[2];
	vc2[0] = v3[0] - v1[0]; vc2[1] = v3[1] - v1[1]; vc2[2] = v3[2] - v1[2];
	a = vc1[1] * vc2[2] - vc2[1] * vc1[2];
	b = vc2[0] * vc1[2] - vc1[0] * vc2[2];
	c = vc1[0] * vc2[1] - vc2[0] * vc1[1];
	r = sqrt(a * a + b * b + c * c);
	normal.x = a / r;	//归一化
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
		// 计算鼠标移动距离，旋转图像
		deltY = e->x() - mouseX;
		deltX = e->y() - mouseY;
		// 更新当前鼠标位置，使图像实时旋转
		mouseX = e->x();
		mouseY = e->y();

		// rotate  
		rotX += deltX;
		rotY += deltY;

		update();// 刷新窗口
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
		clearSelected();//清空上次的选择
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
	int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
	double numSteps = numDegrees / 120.0;//滚动的步数，*15就是鼠标滚动的角度
	//zoom += numSteps;
	scale += numSteps;
	if (scale < 0.005) scale = 0.05;
	event->accept();      //接收该事件
	update();
}

//void MoudelWidget::DrawModelPoints(GLenum mode)
//{
//	glPushMatrix();
//
//	glTranslatef(translateX, translateY, zoom);		// 平移 
//	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// 绕Y轴旋转
//	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// 绕X轴旋转
//	glScaled(scale, scale, scale);					// 缩放
//
//	glBegin(GL_POINTS);					// 开始绘制各个点  
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

////随机分配顶点，随机生成模型顶点，每个面随机一个点
//void MoudelWidget::SetVertexRandom()
//{
//	/**
//	* 利用面的顶点vecFaceTriangle计算，结果存至vecVertexRand。
//	* vecFaceTriangle中三个顶点对应vecVertexRand中一个顶点。
//	*/
//	vecVertexRand.clear();
//	for (int i = 0, pos = 0; i < ply.getTotalFace(); i++, pos += 9)//对每个面；
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
////随机生成模型的顶点，使用点绘制模型
//void MoudelWidget::DrawModelRand(GLenum mode)
//{
//	glPushMatrix();
//
//	glTranslatef(translateX, translateY, zoom);		// 平移 
//	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// 绕Y轴旋转
//	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// 绕X轴旋转
//	glScaled(scale, scale, scale);					// 缩放
//
//	glBegin(GL_POINTS);					// 开始绘制各个点  
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

	GLdouble centerX = GLdouble(x) + rw / 2;//拾取区域中心
	GLdouble centerY = GLdouble(y) + rh / 2;
	const int PICK_SIZE = 8192;
	GLuint pickBuffer[PICK_SIZE];
	GLint hits, view[4];

	glSelectBuffer(PICK_SIZE, pickBuffer);	//设置拾取缓冲区
	glRenderMode(GL_SELECT);				//切换拾取模式
	glInitNames();							//初始化名字堆栈
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//glGetIntegerv(GL_VIEWPORT, view);
	view[0] = this->rect().x();
	view[1] = this->rect().y();
	view[2] = this->rect().width();
	view[3] = this->rect().height();

	gluPickMatrix(centerX, (view[3] - centerY), rw, rh, view);	//定义一个选择区域

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

	hits = glRenderMode(GL_RENDER);		//结束拾取

	if (hits > 0)
	{
		if (isICP)
		{
			ProcessPicks(hits, pickBuffer);
		}
	}
	else
	{
		QMessageBox::warning(this, T_CHAR("选取失败"), T_CHAR("选取的点集过多！"));
	}
	update();// 刷新窗口
}

void  MoudelWidget::ProcessPicks(GLint tPicks, GLuint *pickBuffer)
{
	GLuint nameTotal;//拾取到的元素名字数量，先假设只有1个
	GLuint zNear, zFar;
	GLuint name;//本次拾取的所有名字(这里名字表示序号)
	selectedPoints.clear();

	for (GLint i = 0; i < tPicks; i++)//分析本次拾取数据
	{
		nameTotal = *pickBuffer++;		//选中图元在堆栈中的位置
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
	for (GLint i = 0; i < num_t; i++)//分析本次拾取数据
	{

		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}

	update();
}

//每次绘制一个面，每个面对应一个名字，名字用于拾取选择
void MoudelWidget::DrawModelFill(GLenum mode)
{
	glPushMatrix();

	glTranslatef(translateX, translateY, zoom);		// 平移 
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// 绕Y轴旋转
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// 绕X轴旋转
	glScaled(scale, scale, scale);					// 缩放

	for (int i = 0; i < M_model.orgfaces.size(); i++)
	{
		glBegin(GL_TRIANGLES);					// 开始绘制各个面
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
输入：
M点云，M顶点数量
T点云，T点云数量
*/
void MoudelWidget::drawPointsICP(BaseModel &M, BaseModel &T, GLenum mode)
{
	glPushMatrix();

	glTranslatef(translateX, translateY, zoom);		// 平移 
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);				// 绕Y轴旋转
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);				// 绕X轴旋转
	glScaled(scale, scale, scale);					// 缩放

	//配准T-M
	glBegin(GL_POINTS);//M点云
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
			glBegin(GL_POINTS);//T点云
			glColor3f(T_color[j + 0], T_color[j + 1], T_color[j + 2]);
		}
		else
		{
			glBegin(GL_POINTS);//T点云
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
		glBegin(GL_LINES);//M点云
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
	//绘制拟合的模型
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
						 //绘制圆
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

			Point3D normal_p(fittingCoe[0], fittingCoe[1], fittingCoe[2]);//椭圆平面法向量
			Point3D normal_xy(0, 0, 1);//xy平面法向量

			double angle_p_xy = -angleBetween(normal_p, normal_xy);//旋转角
			Point3D rotationAxis = normal_p * normal_xy;//旋转轴

			Point3D o((fittingCoe[13] + fittingCoe[10]) / 2, (fittingCoe[14] + fittingCoe[11]) / 2, (fittingCoe[15] + fittingCoe[12]) / 2);
			PointModel::createEllipse(fittingCoe[6], fittingCoe[7], o, ellipse, rotationAxis, angle_p_xy, 0.05);

			glPushMatrix();
		
			glBegin(GL_LINE_LOOP);
			glColor4f(1.0, 1.0, 0.8, 0.5);
			//绘制圆
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

/*点云去噪
* 输入：	points ----- 点云
*			p_num  ----- 点数量
*			indist ----- 阈值，两点间最大距离
* 输出：	points ----- 点云
*			p_num  ----- 点数量
* 返回：	去掉的点的数量
*/
int MoudelWidget::denoising(double* points, int &p_num, const double indist)
{
	int sum_num = p_num;

	std::shared_ptr<kdtree::KDTree>     pointsTree;		// 点云的KD树
	kdtree::KDTreeArray					pointsData;		// 点云的数据
	kdtree::KDTreeResultVector			neighbors;		// 邻接点查询结果

	// 将顶点拷贝到pointsData
	pointsData.resize(p_num);
	for (int i = 0; i < p_num; i++)//分配内存
	{
		pointsData[i].resize(3);
	}
	for (int32_t m = 0; m < p_num; m++)				// 获取点云的所有顶点
	for (int32_t n = 0; n < 3; n++)
	{
		pointsData[m][n] = (float)points[m * 3 + n];
	}
	pointsTree = make_shared<kdtree::KDTree>(pointsData);	// 建立点云顶点的KD树

	//------------------遍历所有邻域，分割点集---------------
	bool* visited = new bool[p_num];	// 顶点访问标志数组
	vector<int> vecPoints;				// 新的点云
	for (int unv = 0; unv < p_num; unv++)
	{
		if (visited[unv] != true)//如果当前顶点未访问过，则从当前遍历
		{

			stack<int> s;							// 深度遍历栈
			s.push(unv);							// 从顶点unv开始遍历
			visited[unv] = true;
			vector<int> idxPoints;					// 同一阈值内的点集
			idxPoints.push_back(unv);				// 访问unv，将顶点unv存入点集
			int	i_cur;								// 当前点

			while (!s.empty())						// 深度优先遍历点云
			{

				i_cur = s.top();
				pointsTree->r_nearest_around_point(i_cur, 0, indist, neighbors);// 搜索当前顶点阈值内的邻点
				int i, v;										// i:邻点索引序号；v:已访问计数器
				for (i = 0, v = 0; i < neighbors.size(); i++)	// 遍历邻点数组，找到一个未访问的邻点
				{
					int i_neighbor = neighbors[i].idx;	// 当前点的邻点索引
					if (visited[i_neighbor] != true)	// 如果当前邻点未访问过
					{
						idxPoints.push_back(i_neighbor);// 访问
						visited[i_neighbor] = true;
						s.push(i_neighbor);
					}
					else
					{
						v++;
					}

				} // while
				if (v == neighbors.size())// 如果当前顶点的所有邻点均被访问过
				{
					s.pop();
				}
			}
			if (vecPoints.size() < idxPoints.size())//保留顶点数量最大的点集
			{
				vecPoints = idxPoints;
			}
		} // if
	} // for

	//------------------保存新点云------------------
	p_num = vecPoints.size();//更新点云的点数量
	double* _points = new double[p_num * 3];
	for (int i = 0, j = 0; i < p_num; i++, j += 3)
	{
		_points[j + 0] = points[vecPoints[i] * 3 + 0];
		_points[j + 1] = points[vecPoints[i] * 3 + 1];
		_points[j + 2] = points[vecPoints[i] * 3 + 2];
	}
	for (int i = 0; i < p_num * 3; i++)//更新点云数据
	{
		points[i] = _points[i];
	}

	return sum_num - p_num;
}

//计算方向包围盒
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
//	cout << "体积：" << v << endl;
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

/*点云变换，旋转和平移
输入：
angle  ----- 旋转角度
direct ----- 旋转方向（x轴y轴z轴）
P	   ----- 旋转点云
num_p  ----- 点云顶点数量
tx,ty,tz----- 平移向量
*/
void MoudelWidget::transformPoints(double angle, int direct, double* P, int num_p, double tx, double ty, double tz)
{
	float r = angle / 180.0f * 3.141592f;//T旋转弧度值

	double r00 = 1; double r01 = 0; double r02 = 0;//旋转矩阵
	double r10 = 0; double r11 = 1; double r12 = 0;
	double r20 = 0; double r21 = 0; double r22 = 1;

	switch (direct)
	{
	case 0:{
			   r00 = 1; r01 = 0; r02 = 0;//旋转矩阵
			   r10 = 0; r11 = cos(r); r12 = -sin(r);
			   r20 = 0; r21 = sin(r); r22 = cos(r);
	}break;
	case 1:{
			   r00 = cos(r); r01 = 0; r02 = sin(r);//旋转矩阵
			   r10 = 0; r11 = 1; r12 = 0;
			   r20 = -sin(r); r21 = 0; r22 = cos(r);

	}break;
	case 2:{
			   r00 = cos(r); r01 = -sin(r);  r02 = 0;//旋转矩阵
			   r10 = sin(r);  r11 = cos(r);  r12 = 0;
			   r20 = 0;  r21 = 0;  r22 = 1;
	}break;
	default:
		break;
	}

	double q[3];
	for (int i = 0, k = 0; i < num_p; i++, k += 3)
	{
		P[k + 0] += tx;//平移
		P[k + 1] += ty;
		P[k + 2] += tz;

		q[0] = r00*P[k + 0] + r01*P[k + 1] + r02*P[k + 2];//旋转
		q[1] = r10*P[k + 0] + r11*P[k + 1] + r12*P[k + 2];
		q[2] = r20*P[k + 0] + r21*P[k + 1] + r22*P[k + 2];

		P[k + 0] = q[0];//更新T
		P[k + 1] = q[1];
		P[k + 2] = q[2];
	}
}

/*
* 对场景点云T和模型点云M进行配准，如果一次执行不成功则向x/y/z方向旋转一定角度继续配准
*/
void MoudelWidget::runICP()
{
	if (T_model.vertexs.size() < 5 || M_model.vertexs.size() < 5)
	{
		QMessageBox::warning(this, T_CHAR("配准失败"), T_CHAR("配准失败！模型点云或场景点云不存在。"));
		return;
	}

	if (QMessageBox::question(this, T_CHAR("点云配准"), T_CHAR("是否开始配准？"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) != QMessageBox::Ok)
		return;

	DWORD begin = ::GetTickCount();//计算运行时间，起点

	cout << endl << "(IcpPointToPlane) Running ICP..." << endl;
	int32_t dim = 3;

	//**************************初次配准*****************************
	Matrix R = Matrix::eye(3);
	Matrix t(3, 1);

	IcpPointToPoint icp(icpM, M_model.denseRelation, M_model.relation, M_model.vertexs, M_model.orgfaces, num_m, dim);
	//IcpPointToPlane icp(icpM, M_model.denseRelation, M_model.relation, M_model.vertexs, M_model.orgfaces, num_m, dim);
	icp.fit(icpT, num_t, R, t, -1, offset);

	// 计算完成够的矩阵和向量
	double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
	double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
	double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
	double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

	// ICP计算结果
	cout << endl << "Transformation results:" << endl;
	//cout << "R:" << endl << R << endl << endl;
	cout << "t:" << endl << t0 << "," << t1 << "," << t2 << endl << endl;
	cout << "delta:" << endl << icp.getDelta() << endl << endl;

	double q[3];

	// 变换点云T
	for (int i = 0; i < 3 * num_t; i += 3)
	{
		q[0] = r00*icpT[i + 0] + r01*icpT[i + 1] + r02*icpT[i + 2] + t0;//计算旋转平移变换
		q[1] = r10*icpT[i + 0] + r11*icpT[i + 1] + r12*icpT[i + 2] + t1;
		q[2] = r20*icpT[i + 0] + r21*icpT[i + 1] + r22*icpT[i + 2] + t2;

		icpT[i + 0] = q[0];//更新模型坐标
		icpT[i + 1] = q[1];
		icpT[i + 2] = q[2];
	}

	for (int i = 0; i < num_t; i++)
		mean += offset[i];
	mean /= num_t;						//平均误差
	for (int i = 0; i < num_t; i++)
		variance += (offset[i] - mean)*(offset[i] - mean);
	variance = sqrt(variance / num_t);	//方差

	printf("点到面的平均误差：%lf 方差：%lf\n", mean, variance);

	if (mean < 0.002)
	{
		DWORD sumTime = ::GetTickCount() - begin;// ICP耗时，计算运行时间，终点
		cout << endl << "结果：" << endl << "总耗时： " << sumTime / 1000 << " s " << sumTime % 1000 << " ms " << endl;

		return;
	}

	//*****************判断是否配准成功********************
	double* _T;//初次配准后的场景点云的拷贝	
	_T = new double[3 * num_t];//拷贝场景点云icpT
	for (int i = 0; i < 3 * num_t; i += 3)
	{
		_T[i + 0] = icpT[i + 0];
		_T[i + 1] = icpT[i + 1];
		_T[i + 2] = icpT[i + 2];
	}
	int direct = 2;//当前旋转的方向
	double minMean = mean;//最小误差
	int minMeanDirect = 3;//最佳的旋转方向，误差最小

	while (direct > -1)
	{
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			_T[i + 0] = icpT[i + 0];
			_T[i + 1] = icpT[i + 1];
			_T[i + 2] = icpT[i + 2];
		}
		transformPoints(170, direct, _T, num_t);//变换

		Matrix _R = Matrix::eye(3);
		Matrix _t(3, 1);
		icp.fit(_T, num_t, _R, _t, -1, offset);//二次配准
		// 计算完成够的矩阵和向量
		double r00 = _R.val[0][0]; double r01 = _R.val[0][1]; double r02 = _R.val[0][2];
		double r10 = _R.val[1][0]; double r11 = _R.val[1][1]; double r12 = _R.val[1][2];
		double r20 = _R.val[2][0]; double r21 = _R.val[2][1]; double r22 = _R.val[2][2];
		double t0 = _t.val[0][0]; double t1 = _t.val[1][0]; double t2 = _t.val[2][0];
		double q[3];
		// 变换点云T
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			q[0] = r00*_T[i + 0] + r01*_T[i + 1] + r02*_T[i + 2] + t0;//计算旋转平移变换
			q[1] = r10*_T[i + 0] + r11*_T[i + 1] + r12*_T[i + 2] + t1;
			q[2] = r20*_T[i + 0] + r21*_T[i + 1] + r22*_T[i + 2] + t2;
			_T[i + 0] = q[0];//更新模型坐标
			_T[i + 1] = q[1];
			_T[i + 2] = q[2];
		}
		for (int i = 0; i < num_t; i++)
			mean += offset[i];
		mean /= num_t;						//平均误差
		for (int i = 0; i < num_t; i++)
			variance += (offset[i] - mean)*(offset[i] - mean);
		variance = sqrt(variance / num_t);	//方差

		printf("点到面的平均误差：%lf 方差：%lf\n", mean, variance);

		if (mean < minMean)//记录最优变换
		{
			minMean = mean;
			minMeanDirect = direct;
		}

		if (mean < 0.002)//平均误差小于阈值，退出循环
		{
			icpT = _T;
			break;
		}
		direct--;
	}

	//如果当前不是最佳变换，则使用最佳变换
	if (direct == -1 && minMean != mean)
	{
		transformPoints(170, minMeanDirect, icpT, num_t);//变换

		icp.fit(icpT, num_t, R, t, -1, offset);

		// 计算完成够的矩阵和向量
		double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
		double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
		double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
		double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

		double q[3];

		// 变换点云icpT
		for (int i = 0; i < 3 * num_t; i += 3)
		{
			q[0] = r00*icpT[i + 0] + r01*icpT[i + 1] + r02*icpT[i + 2] + t0;//计算旋转平移变换
			q[1] = r10*icpT[i + 0] + r11*icpT[i + 1] + r12*icpT[i + 2] + t1;
			q[2] = r20*icpT[i + 0] + r21*icpT[i + 1] + r22*icpT[i + 2] + t2;

			icpT[i + 0] = q[0];//更新模型坐标
			icpT[i + 1] = q[1];
			icpT[i + 2] = q[2];
		}
		for (int i = 0; i < num_t; i++)
			mean += offset[i];
		mean /= num_t;						//平均误差
		for (int i = 0; i < num_t; i++)
			variance += (offset[i] - mean)*(offset[i] - mean);
		variance = sqrt(variance / num_t);	//方差
	}

	//将变换后的场景点云保存
	for (int i = 0; i < num_t; i++)
	{
		T_model.vertexs[i].x = icpT[i * 3 + 0];
		T_model.vertexs[i].y = icpT[i * 3 + 1];
		T_model.vertexs[i].z = icpT[i * 3 + 2];
	}


	DWORD sumTime = ::GetTickCount() - begin;// ICP耗时，计算运行时间，终点
	cout << endl << "结果：" << endl << "总耗时： " << sumTime / 1000 << " s " << sumTime % 1000 << " ms " << endl;

	printf("点到面的平均误差：%lf 方差：%lf\n", mean, variance);

	QMessageBox::about(this, T_CHAR("配准"), T_CHAR("配准完成。平均误差：%1；方差：%2。耗时：%3 s %4 ms").arg(mean).arg(variance).arg(sumTime / 1000).arg(sumTime % 1000));

	update();
}

/*
* 读取ply和txt模型到T_model和M_model，并将顶点保存到配准点云M和T中
* 对于模型点云M，可以使用T_model的一般顶点或是密集均匀顶点
*/
void MoudelWidget::readPointsICP()
{
	//////cwh////////
	M_model.getRelation(M_model.vertexs, M_model.faces);  /*得到每一个模型点对模型三角网格的对应关系*/
	//////cwh////////

	M_model.denseVertices(0.01);//取密集点云
	M_model.orgfaces = M_model.faces;
	M_model.refineTriangles(0.001, M_model.vertexs, M_model.faces);//细分三角，注：不要放在取密集点之前,否则可能崩溃，原因未知

	num_t = T_model.vertexs.size();
	num_m = M_model.denseVertex.size();

	// M,T点云数据
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

	double c[3] = { 0, 0, 0 };//质心
	for (int i = 0; i < num_t; i++)//计算icpT质心
	{
		c[0] += icpT[i * 3 + 0];
		c[1] += icpT[i * 3 + 1];
		c[2] += icpT[i * 3 + 2];
	}
	c[0] /= num_t; c[1] /= num_t; c[2] /= num_t;
	for (int i = 0; i < num_t; i++)//icpT移动到原点
	{
		icpT[i * 3 + 0] -= c[0];
		icpT[i * 3 + 1] -= c[1];
		icpT[i * 3 + 2] -= c[2];

		T_model.vertexs[i].x -= c[0];
		T_model.vertexs[i].y -= c[1];
		T_model.vertexs[i].z -= c[2];
	}
}

//执行点云拟合
bool MoudelWidget::fitting(double *coe)
{
	if (selectedPoints.size() < 3)
	{
		QMessageBox::warning(this, T_CHAR("点云拟合失败"), T_CHAR("没有选择点云！\n请双击选择点云，或锁定视图选择点云。"));
		return false;
	}

	return pointCloudFitting(T_model.vertexs, selectedPoints, coe, fittingShape);
}

//点云拟合
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
					 //将选择的点云的质心移至原点
					 for (int i = 0; i < vertexs.size(); i++)
					 {
						 _vertexs.push_back(vertexs[i] -= cc);
					 }
					 //使用在原点附近的点进行拟合
					 Fitting::circularFitting(_vertexs, idxFit, coe, 1000);
					 //将拟合的方程移动至原始点处
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
					 //将选择的点云的质心移至原点
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
				 //将选择的点云的质心移至原点
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

//创建一个圆柱点云
void MoudelWidget::createCylinderPoints()
{
	vector<Point3D> cylinder;//点云
	PointModel::createCylinder(0.1, 0.8, cylinder, Point3D(5, 1, 0), 0.01, 0.01);

	if (cylinder.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = cylinder;//显示点云

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}

//创建一个椭圆点云
void MoudelWidget::createEllipsePoints()
{
	vector<Point3D> ellipse;//椭圆点云
	PointModel::createEllipse(0.24, 0.12, Point3D(0.0, 0.0, 0.0), ellipse, Point3D(1, 1, 0), 1.8, 0.05);

	if (ellipse.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = ellipse;//显示点云

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}

//创建一个圆锥点云
void MoudelWidget::createConePoints()
{
	vector<Point3D> cone;//圆锥点云
	PointModel::createCone(Point3D(0, 0.3, 0), Point3D(0.1, -0.3, 0), 0.3, 0.5, cone, 0.01, 0.01);

	if (cone.empty())
		return;

	T_model.vertexs.clear();
	T_model.vertexs = cone;//显示点云

	num_t = T_model.vertexs.size();
	T_color = new double[T_model.vertexs.size() * 3];
	for (int i = 0; i < T_model.vertexs.size(); i++)
	{
		T_color[i * 3 + 0] = 0.9;
		T_color[i * 3 + 1] = 0.9;
		T_color[i * 3 + 2] = 0.9;
	}
}