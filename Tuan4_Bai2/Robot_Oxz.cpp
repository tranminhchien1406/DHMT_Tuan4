#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <stdio.h>
#include <math.h>

//hang qui uoc trang thai di chuyen cua tay va chan
const char FORWARD_STATE = 1;
const char BACKWARD_STATE = 0;

//Index qui uoc cho mang (goc xoay cua tay va chan)
const char LEFT = 0;
const char RIGHT = 1;

//Trang thai di chuyen hien tai cua tay va chan
char legStates[2];
char armStates[2];

//Goc xuay hien tai cua tay va chan
float legAngles[2];
float armAngles[2];

//Goc xoay hien tai cua toan bo robot
float angle;

//Cac huong di chuyen
const char MOVE_LEFT = 0;
const char MOVE_RIGHT = 1;
const char MOVE_UP = 3;
const char MOVE_DOWN = 4;
//const char MOVE_JUMP = 1;

//goc xoay tuong ung voi nut dieu khien
const int TURN_LEFT = -90;
const int TURN_RIGHT = 90;
const int TURN_UP = 180;
const int TURN_DOWN = 0;

const int GROUND_SIZE = 200;		//kich thuoc mat san
const float FLOOR_HEIGHT = -2;		//do cao mat san
const int CHECK_SIZE = 5;			//kich thuoc moi o vuong tren san

int moveDirection;					//huong di chuyen hien tai
float moveX;						//vi tri di chuyen den theo truc x
float moveZ;						//vi tri di chuyen den theo truc Z
float theta;						//goc xoay cua camera(ting toa do x & z)
float y;							//toa do y cua camera
float dTheta;						//muc tang/giam theta khi dieu khien
float dy;							//muc tang/giam y khi dieu khien
//Khai bao ham
void DrawCube(float xPos, float yPos, float zPos);
void DrawArm(float xPos, float yPos, float zPos);
void DrawHead(float xPos, float yPos, float zPos);
void DrawTorso(float xPos, float yPos, float zPos);
void DrawLeg(float xPos, float yPos, float zPos);
void DrawRobot(float xPos, float yPos, float zPos);
void Prepare();
void Display();
void Init();
void Reshape(int Width, int Height);
void Idle();
void DrawGround();
void SettingCamera(float theta, float y);
void KeyBoard(unsigned char key, int, int);
void Special(int key, int, int);

//--------Ve hinh lap phuong tai vi tri chi dinh----------
void DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();		//Luu trang thai bien doi vao stack
	glBegin(GL_QUADS);
	//ve mat tren
	glNormal3d(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	//ve mat truoc
	glNormal3d(0, 0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	//ve mat ben phai
	glNormal3d(1, 0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	//ve mat ben trai
	glNormal3d(-1, 0, 0);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	//ve mat duoi
	glNormal3d(0, -1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	//ve mat sau
	glNormal3d(0, 0, -10);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();		//quay lai trang da luu
}

//---------ve canh tay (hinh hop cn) tai vi tri chi dinh
void DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);		//ve mau do
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 4.0f, 1.0f);			//kich thuoc 1x4x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//--------ve dau (hinh hop cn) tai vi tri chi dinh
void DrawHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);		//ve mau trang
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 2.0f, 2.0f);			//kich thuoc 2x2x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();

}

//--------ve than (hinh hop cn) tai vi tri chi dinh
void DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);		//ve mau xanh
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 5.0f, 2.0f);			//kich thuoc 3x5x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//-------ve chan (hinh hop cn) tai vi tri chi dinh
void DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);		//ve mau vang
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 5.0f, 1.0f);			//kich thuoc 1x5x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//-------ve toan bo Robot tai vi tri chi dinh
void DrawRobot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);		//toa do ve robot
										//ve dau va than
	DrawHead(1.0f, 2.0f, 0.0f);
	DrawTorso(1.5f, 0.0f, 0.0f);
	//tao hieu ung dang buoc di (di chuyen tay trai va xoay canh tay)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawArm(2.5f, 0.0f, -0.5f);
	glPopMatrix();
	//tao hieu ung dang buoc di (di chuyen tay phai va xoay canh tay)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawArm(-1.5f, 0.0f, -0.5f);
	glPopMatrix();
	//tao hieu ung dang buoc di (di chuyen chan trai va xoay chan)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawLeg(-0.5f, -5.0f, -0.5f);
	glPopMatrix();
	//tao hieu ung dang buoc di (di chuyen chan phai va xoay chan)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawLeg(1.5f, -5.0f, -0.5f);
	glPopMatrix();
	glPopMatrix();
}

//---------tinh toan goc xoay cua tay va chan
void Prepare()
{
	//neu tay chan di chuyen ve phia truoc thi tang goc xoay
	//nguoc lai (di chuyen ve phia sau) thi giam goc xoay
	for (char side = 0; side < 2; side++) {
		//********goc xoay cho tay*********/
		if (armStates[side] == FORWARD_STATE)
			armAngles[side] += 0.1f;
		else
			armAngles[side] -= 0.1f;
		//thay doi trang thai neu goc xoay vuot gia tri cho phep
		if (armAngles[side] >= 15.0f)
			armStates[side] = BACKWARD_STATE;
		else if (armAngles[side] <= -15.0f)
			armStates[side] = FORWARD_STATE;
		/***********************************/

		//********goc xoay cho chan*********/
		if (legStates[side] == FORWARD_STATE)
			legAngles[side] += 0.1f;
		else
			legAngles[side] -= 0.1f;
		//thay doi trang thai neu goc xoay vuot gia tri cho phep
		if (legAngles[side] >= 15.0f)
			legStates[side] = BACKWARD_STATE;
		else if (legAngles[side] <= -15.0f)
			legStates[side] = FORWARD_STATE;
		/***********************************/
	}

	//
	switch (moveDirection) {
		case MOVE_LEFT:
			moveX = moveX - 0.015f;	break;
		case MOVE_RIGHT:
			moveX = moveX + 0.015f;	break;
		case MOVE_UP:
			moveZ = moveZ - 0.015f;	break;
		case MOVE_DOWN:
			moveZ = moveZ + 0.015f;	break;
	}
}

//----------dung hinh
void Display()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();				//reset modelview matrix
	SettingCamera(theta, y);

	glPushMatrix();
		glTranslatef(moveX, 8.0f, moveZ);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		Prepare();
		DrawRobot(0.0f, 0.0f, 1.0f);
	glPopMatrix();

	glPushMatrix();
		DrawGround();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

//-------thiet lap anh sang moi truong va khoi tao mac dinh cho robot
void Init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//mau anh sang ambient
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//mau anh sang diffuse
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//vi tri nguon sang
	GLfloat lightPos[] = { 25.0f, 25.0f, 25.0f, 0.0f };
	//huong chieu sang
	GLfloat spotDir[] = { 0.0, 0.0, 0.0, 0.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//gan gia tri mac dinh ban dau cho robot
	angle = 0.0f;
	armAngles[LEFT] = 0.0;
	armAngles[RIGHT] = 0.0;
	legAngles[LEFT] = 0.0;
	legAngles[RIGHT] = 0.0;
	armStates[LEFT] = FORWARD_STATE;
	armStates[RIGHT] = BACKWARD_STATE;
	legStates[LEFT] = FORWARD_STATE;
	legStates[RIGHT] = BACKWARD_STATE;

	moveDirection = MOVE_LEFT;
	angle = TURN_LEFT;
	moveX = 0.0f;
	moveZ = 0.0f;

	//gan gia tri mac dinh ban dau cho camera
	theta = 0.0f;
	y = 5.0f;
	dTheta = 0.04f;
	dy = 1.0f;
}

//--------thiet lap khung nhin, phep chieu, camera
void Reshape(int Width, int Height)
{
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)Width / (GLfloat)Height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//--------yeu cau ve lai man hinh lien tuc
void Idle()
{
	glutPostRedisplay();
}

//--------ve mat san
void DrawGround()
{
	int x, z;
	int counter = 0;
	for (x = -GROUND_SIZE; x < GROUND_SIZE; x += CHECK_SIZE)
	{
		for (z = -GROUND_SIZE; z < GROUND_SIZE; z += CHECK_SIZE)
		{
			if (counter % 2 == 0)
				glColor3f(0.2, 0.2, 1.0);
			else
				glColor3f(1.0, 1.0, 1.0);

			glBegin(GL_QUADS);
				glNormal3d(0, 1, 0);
				glVertex3f(x, FLOOR_HEIGHT, z);
				glVertex3f(x, FLOOR_HEIGHT, z + CHECK_SIZE);
				glVertex3f(x + CHECK_SIZE, FLOOR_HEIGHT, z + CHECK_SIZE);
				glVertex3f(x + CHECK_SIZE, FLOOR_HEIGHT, z);
			glEnd();
			counter++;
		}
		counter++;
	}
}
//------thiet lap camera
void SettingCamera(float theta, float y)
{
	gluLookAt(50 * sin(theta), y, 50 * cos(theta),
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

//--------xu li ban phim (thay doi goc quay)
void KeyBoard(unsigned char key, int, int)
{
	switch (key)
	{
	case 'a': theta -= dTheta;	break;
	case 'd': theta += dTheta;	break;
	case 'w': y += dy;	break;
	case 's': if (y > dy) y -= dy;	break;
	}
	glutPostRedisplay();
}

//---------xu li ban phim (huong di chuyen)
void Special(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		moveDirection = MOVE_LEFT;
		angle = TURN_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		moveDirection = MOVE_RIGHT;
		angle = TURN_RIGHT;
		break;
	case GLUT_KEY_UP:
		moveDirection = MOVE_UP;
		angle = TURN_UP;
		break;
	case GLUT_KEY_DOWN:
		moveDirection = MOVE_DOWN;
		angle = TURN_DOWN;
		break;
	}
	glutPostRedisplay();
}

//--------Ham Main
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 800);
	glutCreateWindow("ROBOT_02");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutSpecialFunc(Special);
	glutKeyboardFunc(KeyBoard);
	Init();
	glutMainLoop();
	return 0;
}