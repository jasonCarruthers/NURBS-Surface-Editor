#ifndef HFILE_MAIN
#define HFILE_MAIN


#include <GL/glut.h>


/*
* Global function prototypes
*/
void TestRectangleIntersection();

void InitGlobalVariables();
void InitOpenGLStuff();

void ReshapeWindow(GLint newWidth, GLint newHeight);
void KeyboardInput(unsigned char key, int mouseX, int mouseY);
void MouseInput(int button, int action, int mouseX, int mouseY);
void PassiveMouseMove(int mouseX, int mouseY);
void NonpassiveMouseMove(int mouseX, int mouseY);
void Display();


#endif /*HFILE_MAIN*/