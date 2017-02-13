/*
* Programmer: Jason Carruthers
* Last modified: 9/14/16
*/
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "Main.h"
#include "MyMath.h"
#include "Matrix.h"
#include "General.h"
#include "UserInput.h"
#include "Circle.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Camera.h"
#include "Light.h"







/*
* Functions
*/
void TestRectangleIntersection()
{
	MyRectangle rect1 = MyRectangle(Vector2I(100, 100), Vector2I(100, 100), GetRandomColor().GetColor3());
	MyRectangle rect2 = MyRectangle(Vector2I(150, 200), Vector2I(50, 50), GetRandomColor().GetColor3());

	if (rect1.ContainsRect(rect2))
		int x = 5;
}

int main(int argc, char *argv[])
{
	/*Seed the random number generator*/
	srand(static_cast<int>(time(0)));


	glutInit(&argc, argv);
	InitGlobalVariables();
	InitGUI();
	InitCamera();
	InitOpenGLStuff();
	/*TestCircleDrawing();*/
	/*TestRectangleIntersection();*/
	/*TestGraphicVector2D();*/
	TestMatrixClass();
	/*TestTrigThetaSign();*/
	/*DrawXYZPlaneLines();*/
	//DrawAxes();
	/*TestDrawingIn3D();*/
	//InitNURBSSurface();
	PerformRayTrace();
	DrawLights();

	glutDisplayFunc(Display);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(KeyboardInput);
	glutMouseFunc(MouseInput);
	glutPassiveMotionFunc(PassiveMouseMove);
	glutMotionFunc(NonpassiveMouseMove);

	glutMainLoop();
	return 0;
}

void InitGlobalVariables()
{
	pixelBuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT * 3];
	nurbsSurface = new NURBSSurface();
	rowOfCPInFocus = 0;
	colOfCPInFocus = 0;

	SetResolution(250);
	orderK = 3;
	orderL = 3;
	controlPointsVisible = false;
	controlMeshVisible = false; 
	surfaceMeshVisible = false;
	gouraudShadingVisible = false;

	/*Initialize the lights in the scene.*/
	float lightHeight = 40.0f;
	float horizontalOffset = 50.0f;
	directedLightVec.push_back(new DirectedLight(Vector3F(0, lightHeight, 0), Vector3F(0, 0, 0), 0.65f)); /*Center light.*/
	directedLightVec.push_back(new DirectedLight(Vector3F(0, -lightHeight, 0), Vector3F(0, 0, 0), 0.25f)); /*Floor light.*/
	directedLightVec.push_back(new DirectedLight(Vector3F(horizontalOffset, lightHeight, 0), Vector3F(horizontalOffset, 0, 0), 0.25f)); /*Right light.*/
	directedLightVec.push_back(new DirectedLight(Vector3F(-horizontalOffset, lightHeight, 0), Vector3F(-horizontalOffset, 0, 0), 0.25f)); /*Left light.*/
	directedLightVec.push_back(new DirectedLight(Vector3F(0, lightHeight, horizontalOffset), Vector3F(0, 0, horizontalOffset), 0.25f)); /*Front light.*/
	directedLightVec.push_back(new DirectedLight(Vector3F(0, lightHeight, -horizontalOffset), Vector3F(0, 0, -horizontalOffset), 0.25f)); /*Back light.*/

	/*Initialize the spheres in the scene.*/
	float radius = 50.0f;
	float sphereRadius = 20.0f;
	objectVec.push_back(new Sphere(Vector3F(0, 0, 0), 10, Material::Specular, 1/*0.01f*/, 0.6f, new Color3(1, 0, 0)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(0 * MyPI() / 180.0f), 0, radius * sin(0 * MyPI() / 180.0f)), sphereRadius, Material::Diffuse, 1.0f, 1.5f, new Color3(1, 0, 0)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(60.0f * MyPI() / 180.0f), 0, radius * sin(60.0f * MyPI() / 180.0f)), sphereRadius, Material::Specular, 1.0f, 1.5f, new Color3(0, 1, 0)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(120.0f * MyPI() / 180.0f), 0, radius * sin(120.0f * MyPI() / 180.0f)), sphereRadius, Material::Diffuse, 1.0f, 1.5f, new Color3(0, 0, 1)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(180.0f * MyPI() / 180.0f), 0, radius * sin(180.0f * MyPI() / 180.0f)), sphereRadius, Material::Specular, 1.0f, 1.5f, new Color3(1, 1, 0)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(240.0f * MyPI() / 180.0f), 0, radius * sin(240.0f * MyPI() / 180.0f)), sphereRadius, Material::Diffuse, 1.0f, 1.5f, new Color3(1, 0, 1)));
	objectVec.push_back(new Sphere(Vector3F(radius * cos(300.0f * MyPI() / 180.0f), 0, radius * sin(300.0f * MyPI() / 180.0f)), sphereRadius, Material::Specular, 1.0f, 1.5f, new Color3(0, 1, 0)));


	/*Initialize the planes in the scene.*/
	objectVec.push_back(new Plane(Vector3F(0, -50, 0), 200.0f, 200.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(1, 0, 1))); /*Floor*/
	objectVec.push_back(new Plane(Vector3F(0, 0, -100), 200.0f, 100.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(1, 0, 1))); /*Back wall*/
	objectVec.push_back(new Plane(Vector3F(-100, 0, 0), 100.0f, 200.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(1, 0, 1))); /*Left wall*/
	objectVec.push_back(new Plane(Vector3F(100, 0, 0), 100.0f, 200.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(1, 0, 1))); /*Right wall*/
	objectVec.push_back(new Plane(Vector3F(0, 0, 100), 200.0f, 100.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(1, 0, 1))); /*Front wall*/
	objectVec.push_back(new Plane(Vector3F(0, 50, 0), 200.0f, 200.0f, Vector3F(0, 0, 0), Material::Specular, 1.0f, AIR_INDEX_OF_REFRACTION, new Color3(0, 1, 1))); /*Ceiling*/
}

void InitOpenGLStuff()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	int mainWindow = glutCreateWindow("Project 5");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	/*RefreshScreen();*/

	/*Stuff to display goes here*/
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	//DrawGUI();
	
	/*Draw yellow circle around control point in focus.*/
	//DrawCircleAroundControlPointInFocus();

	/*End stuff to display*/
	
	
	glutPostRedisplay();
	
	glFlush();
	glutSwapBuffers();
}

void ReshapeWindow(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);

	glClear(GL_COLOR_BUFFER_BIT);
}

void KeyboardInput(unsigned char key, int mouseX, int mouseY)
{
	HandleKeyboardInput(key, mouseX, mouseY);

	glutPostRedisplay();
}

void MouseInput(int button, int action, int mouseX, int mouseY)
{
	HandleMouseInput(button, action, mouseX, mouseY);
}

void PassiveMouseMove(int mouseX, int mouseY)
{
	HandlePassiveMouseMove(mouseX, mouseY);
}

void NonpassiveMouseMove(int mouseX, int mouseY)
{
	HandleNonpassiveMouseMove(mouseX, mouseY);
}