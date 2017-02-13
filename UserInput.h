#ifndef HFILE_USERINPUT
#define HFILE_USERINPUT


/*
* Global function prototypes
*/
void HandleKeyboardInput(unsigned char key, int mouseX, int mouseY);
void HandleMouseInput(int button, int action, int mouseX, int mouseY);
void HandlePassiveMouseMove(int mouseX, int mouseY);
void HandleNonpassiveMouseMove(int mouseX, int mouseY);


#endif /*HFILE_USERINPUT*/