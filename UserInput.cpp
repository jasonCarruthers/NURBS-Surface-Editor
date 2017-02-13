#include "Main.h"
#include "General.h"
#include "UserInput.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Camera.h"


float cameraXZRotationAboutWorldOrigin = 45.0f; /*In degrees.*/
float xzDistFromWorldOrigin;
const unsigned int deltaTheta = 10;


/*
* Global functions
*/
void HandleKeyboardInput(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	//case 'N': /*Move camera in the -x direction*/
	//case 'n':
	//	camera->SetPosition(camera->GetPosition() + Vector3F(-200.0f, 0.0f, 0.0f));
	//	DrawXYZPlaneLines();
	//	TestDrawingIn3D();
	//	Display();
	//	break;
	//case 'M': /*Move camera in the +x direction*/
	//case 'm':
	//	camera->SetPosition(camera->GetPosition() + Vector3F(200.0f, 0.0f, 0.0f));
	//	DrawXYZPlaneLines();
	//	TestDrawingIn3D();
	//	Display();
	//	break;
	//case 'Z': /*Move camera in the -z direction*/
	//case 'z':
	//	camera->SetPosition(camera->GetPosition() + Vector3F(0.0f, 0.0f, -10.0f));
	//	DrawXYZPlaneLines();
	//	TestDrawingIn3D();
	//	Display();
	//	break;
	//case 'X': /*Move camera in the +z direction*/
	//case 'x':
	//	camera->SetPosition(camera->GetPosition() + Vector3F(0.0f, 0.0f, +10.0f));
	//	DrawXYZPlaneLines();
	//	TestDrawingIn3D();
	//	Display();
	//	break;
	case 'W': /*Increase the row of the control point in focus.*/
	case 'w':
		/*if (rowOfCPInFocus == MAGIC_GARBAGE)
			rowOfCPInFocus = 0;
		else if (rowOfCPInFocus != nurbsSurface->GetControlPointVec().size() - 1)
			rowOfCPInFocus++;
		else
			rowOfCPInFocus = MAGIC_GARBAGE;
		break;*/
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));
		rowOfCPInFocus = (rowOfCPInFocus + 1) % nurbsSurface->GetControlPointVec().size();
		break;
	case 'S': /*Decrease the row of the control point in focus.*/
	case 's':
		/*if (rowOfCPInFocus == MAGIC_GARBAGE)
			rowOfCPInFocus = nurbsSurface->GetControlPointVec().size() - 1;
		else if (rowOfCPInFocus != 0)
			rowOfCPInFocus--;
		else
			rowOfCPInFocus = MAGIC_GARBAGE;
		break;*/
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));
		rowOfCPInFocus = (rowOfCPInFocus == 0) ? nurbsSurface->GetControlPointVec().size() - 1 : rowOfCPInFocus - 1;
		break;
	case 'A': /*Increase the column of the control point in focus.*/
	case 'a':
		/*if (colOfCPInFocus == MAGIC_GARBAGE)
			colOfCPInFocus = 0;
		else if (colOfCPInFocus != nurbsSurface->GetControlPointVec()[0].size() - 1)
			colOfCPInFocus++;
		else
			colOfCPInFocus = MAGIC_GARBAGE;
		break;*/
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));
		colOfCPInFocus = (colOfCPInFocus + 1) % nurbsSurface->GetControlPointVec()[0].size();
		break;
	case 'D': /*Decrease the column of the control point in focus.*/
	case 'd':
		/*if (colOfCPInFocus == MAGIC_GARBAGE)
			colOfCPInFocus = nurbsSurface->GetControlPointVec()[0].size() - 1;
		else if (colOfCPInFocus != 0)
			colOfCPInFocus--;
		else
			colOfCPInFocus = MAGIC_GARBAGE;*/
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));
		colOfCPInFocus = (colOfCPInFocus == 0) ? nurbsSurface->GetControlPointVec()[0].size() - 1 : colOfCPInFocus - 1;
		break;
	case 'E': /*Rotate the camera right about the world origin, cause the scene to
			    rotate left about the origin.*/
	case 'e':
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

		xzDistFromWorldOrigin = sqrt(camera->GetPosition().GetX()*camera->GetPosition().GetX() +
			camera->GetPosition().GetZ()*camera->GetPosition().GetZ());
		cameraXZRotationAboutWorldOrigin += 5.0f;
		camera->SetPosition(Vector3F(xzDistFromWorldOrigin*sin((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f),
			camera->GetPosition().GetY(),
			xzDistFromWorldOrigin*cos((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f)));
		camera->LookAt(Vector3F(0.0f, 0.0f, 0.0f));
		DrawAxes();
		nurbsSurface->ClearFromPixelBuffer();
		nurbsSurface->SetProjectedControlPoints();
		nurbsSurface->CalcPlotVec();
		if(controlMeshVisible) nurbsSurface->SetControlMesh();
		if (surfaceMeshVisible) nurbsSurface->SetSurfaceMesh();
		//if (gouraudShadingVisible) nurbsSurface->SetGouraudShading();
		nurbsSurface->Draw();
		Display();
		break;
	case 'R': /*Rotate the camera left about the world origin, causing the scene to
			    rotate right about the origin.*/
	case 'r':
		DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

		xzDistFromWorldOrigin = sqrt(camera->GetPosition().GetX()*camera->GetPosition().GetX() +
										 camera->GetPosition().GetZ()*camera->GetPosition().GetZ());
		cameraXZRotationAboutWorldOrigin -= 5.0f;
		camera->SetPosition(Vector3F(xzDistFromWorldOrigin*sin((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f),
									camera->GetPosition().GetY(),
									xzDistFromWorldOrigin*cos((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f)));
		camera->LookAt(Vector3F(0.0f, 0.0f, 0.0f));
		DrawAxes();
		nurbsSurface->ClearFromPixelBuffer();
		nurbsSurface->SetProjectedControlPoints();
		nurbsSurface->CalcPlotVec();
		if (controlMeshVisible) nurbsSurface->SetControlMesh();
		if (surfaceMeshVisible) nurbsSurface->SetSurfaceMesh();
		//if (gouraudShadingVisible) nurbsSurface->SetGouraudShading();
		nurbsSurface->Draw();
		Display();
		break;
	//case 'T': /*Rotate the camera a full 360 about the origin, causing an "animating" effect.*/
	//case 't':
	//	for (unsigned int i = 0; i < 360 / deltaTheta; i++)
	//	{
	//		xzDistFromWorldOrigin = sqrt(camera->GetPosition().GetX()*camera->GetPosition().GetX() +
	//			camera->GetPosition().GetZ()*camera->GetPosition().GetZ());
	//		cameraXZRotationAboutWorldOrigin = (float)((int)(cameraXZRotationAboutWorldOrigin + deltaTheta) % 360);
	//		camera->SetPosition(Vector3F(xzDistFromWorldOrigin*sin((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f),
	//			camera->GetPosition().GetY(),
	//			xzDistFromWorldOrigin*cos((cameraXZRotationAboutWorldOrigin)* MyPI() / 180.0f)));
	//		camera->LookAt(Vector3F(0.0f, 0.0f, 0.0f));
	//		
	//		DrawXYZPlaneLines();
	//		Display();
	//	}
	default:
		break;
	}
}

void HandleMouseInput(int button, int action, int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes.*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, button, action);

	/*Pass along mouse info for updating sliders.*/
	sliderHandler->MonitorSliders(mouseX, mouseY, button, action);

	/*Pass along mouse info for updating buttons.*/
	buttonHandler->MonitorButtons(mouseX, mouseY, button, action);
}

void HandlePassiveMouseMove(int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);

	/*Pass along mouse info for updating buttons*/
	buttonHandler->MonitorButtons(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);
}

void HandleNonpassiveMouseMove(int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes.*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);

	/*Pass along mouse info for updating sliders.*/
	sliderHandler->MonitorSliders(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);

	/*Pass along mouse info for updating buttons.*/
	buttonHandler->MonitorButtons(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);
}