#include "Main.h"
#include "General.h"
#include "Line.h"
#include "GUI.h"
#include "Camera.h"
#include "Matrix.h"



/*
* Global variables
*/
Camera *camera;
GraphicVector2D *xAxis;
GraphicVector2D *yAxis;
GraphicVector2D *zAxis;
float ZNEAR = 200.0f;
float ZFAR = -200.0f;

/*NOTE: The ratio of CANVAS_WIDTH/CANVAS_HEIGHT should be the same as the ratio of
		WINDOW_WIDTH/WINDOW_HEIGHT.*/
const float CANVAS_WIDTH = 80;
const float CANVAS_HEIGHT = 40;




/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Camera
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Camera::Camera(const Vector3F &newPosition,
			   const Vector3F &newLookAt,
			   float newFieldOfViewAngle)
{
	mPosition.Set(newPosition);
	LookAt(newLookAt);
	mFieldOfViewAngle = newFieldOfViewAngle;

	//mZViewTheta = 45 * MyPI() / 180.0f;
	//mZViewPhi = 45 * MyPI() / 180.0f;

	//Vector3F tempN = Vector3F(sin(mZViewPhi)*cos(mZViewTheta), sin(mZViewPhi)*sin(mZViewTheta), cos(mZViewPhi));
	//Vector3F tempV = Vector3F(0.0f, 1.0f, 0.0f);
	//mN.Set(tempN);
	//mV.Set(tempV);
	//Vector3F tempU = CrossProduct(mV, mN);
	//tempU.Normalize();
	//mU.Set(tempU);
	//tempV = CrossProduct(mN, mU);
	//tempV.Normalize();
	//mV.Set(tempV);
}

/*
* Accessors
*/
Vector3F Camera::GetPosition() const
{
	return mPosition;
}

Vector3F Camera::GetN() const
{
	return mN;
}

Vector3F Camera::GetV() const
{
	return mV;
}

Vector3F Camera::GetU() const
{
	return mU;
}

float Camera::GetFieldOfViewAngle() const
{
	return mFieldOfViewAngle;
}

/*Returns the shortest distance between the camera and the canvas.*/
float Camera::GetDistanceFromCanvas() const
{
	float dist = CANVAS_HEIGHT / (2.0f * tan(camera->GetFieldOfViewAngle() / 2.0f));
	return dist;
}

//float Camera::GetZViewTheta() const
//{
//	return mZViewTheta;
//}
//
//float Camera::GetZViewPhi() const
//{
//	return mZViewPhi;
//}

Matrix Camera::GetCameraToWorldTransform() const
{
	/*Initialize camera-to-world tansform matrix.*/
	Matrix *cameraToWorldTransform = new Matrix(4, 4);
	for (unsigned int row = 0; row < cameraToWorldTransform->GetNumRows(); row++)
		for (unsigned int column = 0; column < cameraToWorldTransform->GetNumColumns(); column++)
			cameraToWorldTransform->Insert(row, column, 0);
	cameraToWorldTransform->Insert(0, 0, mU.GetX());
	cameraToWorldTransform->Insert(0, 1, mU.GetY());
	cameraToWorldTransform->Insert(0, 2, mU.GetZ());
	cameraToWorldTransform->Insert(1, 0, mV.GetX());
	cameraToWorldTransform->Insert(1, 1, mV.GetY());
	cameraToWorldTransform->Insert(1, 2, mV.GetZ());
	cameraToWorldTransform->Insert(2, 0, mN.GetX());
	cameraToWorldTransform->Insert(2, 1, mN.GetY());
	cameraToWorldTransform->Insert(2, 2, mN.GetZ());
	cameraToWorldTransform->Insert(3, 0, mPosition.GetX());
	cameraToWorldTransform->Insert(3, 1, mPosition.GetY());
	cameraToWorldTransform->Insert(3, 2, mPosition.GetZ());
	cameraToWorldTransform->Insert(3, 3, 1);

	//if (cameraToWorldTransform->Get(0, 0) == 0)
	//	cameraToWorldTransform->Insert(0, 0, 1);
	//if (cameraToWorldTransform->Get(1, 1) == 0)
	//	cameraToWorldTransform->Insert(1, 1, 1);
	//if (cameraToWorldTransform->Get(2, 2) == 0)
	//	cameraToWorldTransform->Insert(2, 2, 1);

	return *cameraToWorldTransform;
}

/*
* Mutators
*/
void Camera::SetPosition(const Vector3F &newPosition)
{
	mPosition.Set(newPosition);
}

void Camera::SetN(const Vector3F &newN)
{
	mN.Set(newN);
}

void Camera::SetV(const Vector3F &newV)
{
	mV.Set(newV);
}

void Camera::SetU(const Vector3F &newU)
{
	mU.Set(newU);
}

void Camera::SetFieldOfViewAngle(float newFieldOfViewAngle)
{
	mFieldOfViewAngle = newFieldOfViewAngle;
}

//void Camera::SetZViewTheta(float newZViewTheta)
//{
//	mZViewTheta = newZViewTheta;
//
//	Vector3F tempN = Vector3F(sin(mZViewPhi)*cos(mZViewTheta), sin(mZViewPhi)*sin(mZViewTheta), cos(mZViewPhi));
//	Vector3F tempV = Vector3F(0.0f, 1.0f, 0.0f);
//
//	mN.Set(tempN);
//
//	Vector3F tempU = CrossProduct(mV, mN);
//	tempU.Normalize();
//	mU.Set(tempU);
//
//	tempV = CrossProduct(mN, mU);
//	tempV.Normalize();
//	mV.Set(tempV);
//}
//
//void Camera::SetZViewPhi(float newZViewPhi)
//{
//	mZViewPhi = newZViewPhi;
//
//	Vector3F tempN = Vector3F(sin(mZViewPhi)*cos(mZViewTheta), sin(mZViewPhi)*sin(mZViewTheta), cos(mZViewPhi));
//	Vector3F tempV = Vector3F(0.0f, 1.0f, 0.0f);
//	
//	mN.Set(tempN);
//	
//	Vector3F tempU = CrossProduct(mV, mN);
//	tempU.Normalize();
//	mU.Set(tempU);
//
//	tempV = CrossProduct(mN, mU);
//	tempV.Normalize();
//	mV.Set(tempV);
//}

void Camera::LookAt(const Vector3F &newLookAt)
{
	Vector3F delta = mPosition + -1.0f * newLookAt;
	float deltaMag = sqrt(delta.GetX()*delta.GetX() + delta.GetY()*delta.GetY() + delta.GetZ()*delta.GetZ());
	mN = delta * (1.0f / deltaMag);
	mN.Normalize();

	Vector3F tempV = Vector3F(0.0f, 1.0f, 0.0f);
	if (mN == Vector3F(0, 1, 0)) tempV = Vector3F(0, 0, -1);
	else if (mN == Vector3F(0, -1, 0)) tempV = Vector3F(0, 0, 1);
	Vector3F tempU = CrossProduct(tempV, mN);
	tempU.Normalize();
	mU.Set(tempU);

	tempV = CrossProduct(mN, mU);
	tempV.Normalize();
	mV.Set(tempV);
}



/*
* Global functions
*/
void InitCamera()
{
	camera = new Camera();

	const float length = 100;
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());
	xAxis = new GraphicVector2D(Get2DProjection(Vector3F(-length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Color3(1.0f, 0.0f, 0.0f));
	yAxis = new GraphicVector2D(Get2DProjection(Vector3F(0.0f, -length / 2.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, length / 2.0f, 0.0f), worldToCameraTransform), Color3(0.0f, 1.0f, 0.0f));
	zAxis = new GraphicVector2D(Get2DProjection(Vector3F(0.0f, 0.0f, -length / 2.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, 0.0f, length / 2.0f), worldToCameraTransform), Color3(0.0f, 0.0f, 1.0f));
}

void DrawAxes()
{
	const float length = 100;
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());

	/*Clear old axes from the pixelBuffer.*/
	xAxis->Draw(Color3(0, 0, 0));
	yAxis->Draw(Color3(0, 0, 0));
	zAxis->Draw(Color3(0, 0, 0));

	/*Initialize axes.*/
	xAxis = new GraphicVector2D(Get2DProjection(Vector3F(-length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Color3(1.0f, 0.0f, 0.0f));
	yAxis = new GraphicVector2D(Get2DProjection(Vector3F(0.0f, -length / 2.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, length / 2.0f, 0.0f), worldToCameraTransform), Color3(0.0f, 1.0f, 0.0f));
	zAxis = new GraphicVector2D(Get2DProjection(Vector3F(0.0f, 0.0f, -length / 2.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, 0.0f, length / 2.0f), worldToCameraTransform), Color3(0.0f, 0.0f, 1.0f));

	/*Draw axes.*/
	xAxis->Draw();
	yAxis->Draw();
	zAxis->Draw();
}



//Vector2I Get2DProjection(const Vector3F &worldPos)
//{
//	/*In the textbook, Computer Graphics with OpenGL, see page 315 and in general chapter 10.*/
//
//	/*Convert worldPos into a column matrix.*/
//	Matrix worldPosMatrix = Matrix(4, 1);
//	worldPosMatrix.Insert(0, 0, worldPos.GetX());
//	worldPosMatrix.Insert(1, 0, worldPos.GetY());
//	worldPosMatrix.Insert(2, 0, worldPos.GetZ());
//	worldPosMatrix.Insert(3, 0, 1);
//
//	/*Initialize matrix T, which is the matrix for translating the viewing origin to the world origin.*/
//	Matrix T = Matrix(4, 4);
//	for (unsigned int row = 0; row < T.GetNumRows(); row++)
//		for (unsigned int column = 0; column < T.GetNumColumns(); column++)
//			T.Insert(row, column, 0.0f);
//	T.Insert(0, 0, 1);
//	T.Insert(0, 3, -camera->GetPosition().GetX());
//	T.Insert(1, 1, 1);
//	T.Insert(1, 3, -camera->GetPosition().GetY());
//	T.Insert(2, 2, 1);
//	T.Insert(2, 3, -(camera->GetPosition().GetZ()/* + WINDOW_HEIGHT / (2.0f * tan(camera->GetFieldOfViewAngle() / 2.0f))*/));
//	T.Insert(3, 3, 1);
//
//	/*Initialize matrix R, which is the matrix for super-imposing the viewing axes onto the world frame.*/
//	Matrix R = Matrix(4, 4);
//	for (unsigned int row = 0; row < R.GetNumRows(); row++)
//		for (unsigned int column = 0; column < R.GetNumColumns(); column++)
//			R.Insert(row, column, 0.0f);
//	R.Insert(0, 0, camera->GetU().GetX());
//	R.Insert(0, 1, camera->GetU().GetY());
//	R.Insert(0, 2, camera->GetU().GetZ());
//	R.Insert(1, 0, camera->GetV().GetX());
//	R.Insert(1, 1, camera->GetV().GetY());
//	R.Insert(1, 2, camera->GetV().GetZ());
//	R.Insert(2, 0, camera->GetN().GetX());
//	R.Insert(2, 1, camera->GetN().GetY());
//	R.Insert(2, 2, camera->GetN().GetZ());
//	R.Insert(3, 3, 1);
//
//	/*Define aspect, which is width/height.*/
//	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
//
//	/*Initialize the normalized symmetric perspective matrix.*/
//	Matrix normSymmPers = Matrix(4, 4);
//	for (unsigned int row = 0; row < normSymmPers.GetNumRows(); row++)
//		for (unsigned int column = 0; column < normSymmPers.GetNumColumns(); column++)
//			normSymmPers.Insert(row, column, 0.0f);
//	normSymmPers.Insert(0, 0, 1.0f / (aspect * tan(camera->GetFieldOfViewAngle() / 2.0f)));
//	normSymmPers.Insert(1, 1, 1.0f / tan(camera->GetFieldOfViewAngle() / 2.0f));
//	normSymmPers.Insert(2, 2, (ZNEAR + ZFAR) / (ZNEAR - ZFAR));
//	normSymmPers.Insert(2, 3, -2.0f * ZNEAR * ZFAR / (ZNEAR - ZFAR));
//	normSymmPers.Insert(3, 2, -1);
//
//	/*Define the homogenous parameter h.*/
//	//float h = camera->GetProjectionReferencePoint();
//	float h = WINDOW_HEIGHT / (2.0f * tan(camera->GetFieldOfViewAngle() / 2.0f));
//
//	/*Get the normalized perspective-projection coordinates in matrix form of the given world coordinate.*/
//	Matrix normPersProj = Matrix(4, 4);
//	for (unsigned int row = 0; row < normPersProj.GetNumRows(); row++)
//		for (unsigned int column = 0; column < normPersProj.GetNumColumns(); column++)
//			normPersProj.Insert(row, column, 0.0f);
//	normPersProj = normSymmPers * (R * (T * worldPosMatrix));
//
//	/*Intialize the transformation matrix from normalized view volume to 3D screen coordinates.*/
//	Matrix normViewVolTo3DScreenCoord = Matrix(4, 4);
//	for (unsigned int row = 0; row < normViewVolTo3DScreenCoord.GetNumRows(); row++)
//		for (unsigned int column = 0; column < normViewVolTo3DScreenCoord.GetNumColumns(); column++)
//			normViewVolTo3DScreenCoord.Insert(row, column, 0.0f);
//	normViewVolTo3DScreenCoord.Insert(0, 0, ((float)WINDOW_WIDTH / 2.0f - (-(float)WINDOW_WIDTH / 2.0f)) / 2.0f);
//	normViewVolTo3DScreenCoord.Insert(0, 3, ((float)WINDOW_WIDTH / 2.0f + (-(float)WINDOW_WIDTH / 2.0f)) / 2.0f);
//	normViewVolTo3DScreenCoord.Insert(1, 1, ((float)WINDOW_HEIGHT / 2.0f - (-(float)WINDOW_HEIGHT / 2.0f)) / 2.0f);
//	normViewVolTo3DScreenCoord.Insert(1, 3, ((float)WINDOW_HEIGHT / 2.0f + (-(float)WINDOW_HEIGHT / 2.0f)) / 2.0f);
//	normViewVolTo3DScreenCoord.Insert(2, 2, 0.5f);
//	normViewVolTo3DScreenCoord.Insert(2, 3, 0.5f);
//	normViewVolTo3DScreenCoord.Insert(3, 3, 1.0f);
//
//	/*Initialize the transformation to the viewport matrix. (Scale matrix)*/
//	//Matrix scale = Matrix(4, 4);
//	//for (unsigned int row = 0; row < scale.GetNumRows(); row++)
//	//	for (unsigned int column = 0; column < scale.GetNumColumns(); column++)
//	//		scale.Insert(row, column, 0.0f);
//	//scale.Insert(0, 0, (float)WINDOW_WIDTH);
//	//scale.Insert(1, 1, (float)WINDOW_HEIGHT);
//	//scale.Insert(2, 2, 1);
//	//scale.Insert(3, 3, 1);
//
//	/*Initialize the viewport matrix.*/
//	Matrix viewport = Matrix(4, 4);
//	//viewport = scale * normPersProj;
//	viewport = normViewVolTo3DScreenCoord * normPersProj;
//
//	/*Push the viewport's z-value into the appropriate z-buffer xy-index.*/
//	/*But for now, just return the xy components of viewport.*/
//	Vector2I *ret = new Vector2I(MyRound(viewport.Get(0, 0) / h), MyRound(viewport.Get(1, 0) / h));
//	return *ret;
//}

Vector2I Get2DProjection(const Vector3F &worldPos, const Matrix &worldToCameraTransform)
{
	/*Convert worldPos into a column matrix.*/
	Matrix pointInWorldSpace = Matrix(4, 1);
	pointInWorldSpace.Insert(0, 0, worldPos.GetX());
	pointInWorldSpace.Insert(1, 0, worldPos.GetY());
	pointInWorldSpace.Insert(2, 0, worldPos.GetZ());
	pointInWorldSpace.Insert(3, 0, 1);

	/*Get the world-to-camera transform matrix.*/
	//Matrix worldToCameraTransform = *(cameraToWorldTransform.GetInverse());

	/*Compute the location of the given point in camera space, in matrix form.*/
	Matrix pointInCameraSpace = (pointInWorldSpace.GetTranspose() * worldToCameraTransform).GetTranspose();
	for (unsigned int row = 0; row < 3; row++)
		pointInCameraSpace.Insert(row, 0, pointInCameraSpace.Get(row, 0) / pointInCameraSpace.Get(3, 0));

	/*Here, clipping could occur. This function could return a Vector2I* instead of a Vector2I,
	  and if the pointInCameraSpace is not visible to the camera, then return NULL.*/

	/*Get distance from the eye of the camera to the canvas. This distance is implied by the
	  angle of view of the camera.*/
	float distFromCameraToCanvas = CANVAS_HEIGHT / (2.0f * tan(camera->GetFieldOfViewAngle() / 2.0f));

	/*Compute the position of the point in canvas space, in Vector3F form.
		NOTE: The z-component is incorrect; see Graphics textbook for correct z-transformation.*/
	Vector3F pointInCanvasSpace = Vector3F(pointInCameraSpace.Get(0, 0) * distFromCameraToCanvas / (-1.0f * pointInCameraSpace.Get(2, 0)),
											pointInCameraSpace.Get(1, 0) * distFromCameraToCanvas / (-1.0f * pointInCameraSpace.Get(2, 0)),
											pointInCameraSpace.Get(2, 0));

	/*Compute the position of the point in normalized device coordinates (NDC).*/
	Vector2F pointInNDC = Vector2F((pointInCanvasSpace.GetX() + CANVAS_WIDTH / 2.0f) / CANVAS_WIDTH, (pointInCanvasSpace.GetY() + CANVAS_HEIGHT / 2.0f) / CANVAS_HEIGHT);

	/*Compute the position of the point in raster space (on the actual computer screen).*/
	//Vector2I pointInRasterSpace = Vector2I(MyRound(pointInNDC.GetX() * WINDOW_WIDTH), MyRound(pointInNDC.GetY() * WINDOW_HEIGHT));
	Vector2I deltaDimensions = Vector2I(WINDOW_WIDTH - curveSpace->GetDimensions().GetX(), WINDOW_HEIGHT - curveSpace->GetDimensions().GetY());
	Vector2I pointInRasterSpace = Vector2I(deltaDimensions.GetX() + MyRound(pointInNDC.GetX() * curveSpace->GetDimensions().GetX()),
										   deltaDimensions.GetY() + MyRound(pointInNDC.GetY() * curveSpace->GetDimensions().GetY()));

	return pointInRasterSpace;
}




void TestDrawingIn3D()
{
	/*Create a small rectangle in a 3D coordinate system.*/
	std::vector<Vector3F> makeshift3DRect;
	Vector3F topLeft = Vector3F(100.0f, 500.0f, -0.0f);
	Vector2F dimensions = Vector2F(20.0f, 20.0f);
	for (unsigned int row = (unsigned int)(topLeft.GetY() - dimensions.GetY()); row <= (unsigned int)topLeft.GetY(); row++)
		for (unsigned int column = (unsigned int)topLeft.GetX(); column <= (unsigned int)(topLeft.GetX() + dimensions.GetX()); column++)
			makeshift3DRect.push_back(Vector3F((float)column, (float)row, topLeft.GetZ()));

	/*For each point of the 3D rectangle, perform the camera transform on that point and then display
	  the 2D projection of that point to the screen.*/
	for (unsigned int i = 0; i < makeshift3DRect.size(); i++)
	{
		Vector2I projection = Get2DProjection(makeshift3DRect[i], *(camera->GetCameraToWorldTransform().GetInverse()));
		SetPixel((int)projection.GetX(), (int)projection.GetY(), Color3(1.0f, 0.0f, 0.0f));
	}
	Display();
}

/*Draw 11x11x11 lines (33 total), each set of lines parallel to either the x axis, y axis, or z axis.*/
void DrawXYZPlaneLines()
{
	/*Initialize variables.*/
	const float delta = 10.0f; /*Distance between lines.*/
	const float length = delta * 11;
	std::vector<GraphicVector2D> gvVec;
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());

	///*Initialize lines parallel to the x-axis.*/
	//std::vector<Line> parallelToXAxisLineVec;
	//for (unsigned int i = 0; i < 11; i++)
	//{
	//	Line newLine = Line(Get2DProjection(Vector3F(-length / 2.0f, 0.0f, 5.0f*delta - i*delta), worldToCameraTransform), Get2DProjection(Vector3F(length / 2.0f, 0.0f, 5.0f*delta - i*delta), worldToCameraTransform), Color3(8.0f, 0.0f, 0.0f));
	//	GraphicVector2D gv = GraphicVector2D(Get2DProjection(Vector3F(-length / 2.0f, 0.0f, 5.0f*delta - i*delta), worldToCameraTransform), Get2DProjection(Vector3F(length / 2.0f, 0.0f, 5.0f*delta - i*delta), worldToCameraTransform), Color3(1.0f, 0.0f, 0.0f));
	//	if (i != 5) parallelToXAxisLineVec.push_back(newLine);
	//	else gvVec.push_back(gv);
	//}

	///*Initialize lines parallel to the y-axis.*/
	//std::vector<Line> parallelToYAxisLineVec;
	//for (unsigned int i = 0; i < 11; i++)
	//{
	//	Line newLine = Line(Get2DProjection(Vector3F(0.0f, -length / 2.0f, -5.0f*delta + i*delta), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, length / 2.0f, -5.0f*delta + i*delta), worldToCameraTransform), Color3(0.0f, 8.0f, 0.0f));
	//	GraphicVector2D gv = GraphicVector2D(Get2DProjection(Vector3F(0.0f, -length / 2.0f, -5.0f*delta + i*delta), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, length / 2.0f, -5.0f*delta + i*delta), worldToCameraTransform), Color3(0.0f, 1.0f, 0.0f));
	//	if (i == 5) newLine.SetColor(Color3(1.0f, 1.0f, 1.0f));
	//	if (i != 5) parallelToYAxisLineVec.push_back(newLine);
	//	else gvVec.push_back(gv);
	//}

	///*Intialize lines parallel to the z-axis.*/
	//std::vector<Line> parallelToZAxisLineVec;
	//for (unsigned int i = 0; i < 11; i++)
	//{
	//	Line newLine = Line(Get2DProjection(Vector3F(-5.0f*delta + i*delta, 0.0f, -length / 2.0f), worldToCameraTransform), Get2DProjection(Vector3F(-5.0f*delta + i*delta, 0.0f, length / 2.0f), worldToCameraTransform), Color3(0.0f, 0.0f, 8.0f));
	//	GraphicVector2D gv = GraphicVector2D(Get2DProjection(Vector3F(-5.0f*delta + i*delta, 0.0f, -length / 2.0f), worldToCameraTransform), Get2DProjection(Vector3F(-5.0f*delta + i*delta, 0.0f, length / 2.0f), worldToCameraTransform), Color3(0.0f, 0.0f, 1.0f));
	//	if (i == 5) newLine.SetColor(Color3(1.0f, 1.0f, 1.0f));
	//	if (i != 5) parallelToZAxisLineVec.push_back(newLine);
	//	else gvVec.push_back(gv);
	//}

	///*Draw lines.*/
	//for (unsigned int i = 0; i < parallelToXAxisLineVec.size(); i++)
	//{
	//	parallelToXAxisLineVec[i].Draw();
	//	parallelToZAxisLineVec[i].Draw();
	//	parallelToYAxisLineVec[i].Draw();
	//}

	/*Initialize a GraphicVector2D object along each of the x, y, and z axes.*/
	gvVec.push_back(GraphicVector2D(Get2DProjection(Vector3F(-length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(length / 2.0f, 0.0f, 0.0f), worldToCameraTransform), Color3(1.0f, 0.0f, 0.0f)));
	gvVec.push_back(GraphicVector2D(Get2DProjection(Vector3F(0.0f, -length / 2.0f, 0.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, length / 2.0f, 0.0f), worldToCameraTransform), Color3(0.0f, 1.0f, 0.0f)));
	gvVec.push_back(GraphicVector2D(Get2DProjection(Vector3F(0.0f, 0.0f, -length / 2.0f), worldToCameraTransform), Get2DProjection(Vector3F(0.0f, 0.0f, length / 2.0f), worldToCameraTransform), Color3(0.0f, 0.0f, 1.0f)));

	/*Draw GraphicVector2D objects.*/
	for (unsigned int i = 0; i < gvVec.size(); i++)
		gvVec[i].Draw();

	/*Draw a white dot at the world origin.*/
	Vector2I worldOriginProjection = Get2DProjection(Vector3F(0, 0, 0), worldToCameraTransform);
	for (int deltaX = -5; deltaX <= 5; deltaX++)
		for (int deltaY = -5; deltaY <= 5; deltaY++)
			SetPixel(worldOriginProjection.GetX() + deltaX, worldOriginProjection.GetY() + deltaY, Color3(1, 1, 1));
}








///*
//* Transforms a world (global) position to a position relative to the camera's eye.
//*
//* From the wikipedia page on 3D projection:
//*		a<x,y,z> is the 3D position of a point A that is to be projected.
//*		c<x,y,z> is the 3D position of a point C representing the camera.
//*		theta<x,y,z> is the orientation of the camera.
//*		e<x,y,z> is the viewer's position relative to the display surface which goes through
//*				 point C representing the camera.
//* "we first define a vector d<x,y,z> as
//* the position of point A with respect to a coordinate system defined by the camera,
//* with origin in C and rotated by theta with respect to the initial coordinate system.
//* This is achieved by subtracting C from A and then applying a rotation -theta to the
//* result. This transformation is often called a camera transform...[the following]
//* calculations assume that the axes are ordered as a left-handed system of axes (which
//* would have positive z going into the screen with standard orientation of the x and y
//* axes)."
//*/
//Vector3F GetCameraTransform(const Vector3F &worldPos)
//{
//	/*Initialze variables as described on the wikipedia page for 3D projection.*/
//	Vector3F a = worldPos;
//	Vector3F c = camera->GetPosition();
//	Vector3F theta = camera->GetRotation();
//
//	/*Initialize the x-transform matrix.*/
//	Matrix xTransform = Matrix(3, 3);
//	for (unsigned row = 0; row < xTransform.GetNumRows(); row++)
//		for (unsigned column = 0; column < xTransform.GetNumColumns(); column++)
//			xTransform.Insert(row, column, 0);
//	xTransform.Insert(0, 0, 1);
//	xTransform.Insert(1, 1, cos(-theta.GetX()));
//	xTransform.Insert(1, 2, -sin(-theta.GetX()));
//	xTransform.Insert(2, 1, sin(-theta.GetX()));
//	xTransform.Insert(2, 2, cos(-theta.GetX()));
//
//	/*Initialize the y-transform matrix.*/
//	Matrix yTransform = Matrix(3, 3);
//	for (unsigned row = 0; row < yTransform.GetNumRows(); row++)
//		for (unsigned column = 0; column < yTransform.GetNumColumns(); column++)
//			yTransform.Insert(row, column, 0);
//	yTransform.Insert(0, 0, cos(-theta.GetY()));
//	yTransform.Insert(1, 1, 1);
//	yTransform.Insert(0, 2, sin(-theta.GetY()));
//	yTransform.Insert(2, 0, -sin(-theta.GetY()));
//	yTransform.Insert(2, 2, cos(-theta.GetY()));
//
//	/*Initialize the z-transform matrix.*/
//	Matrix zTransform = Matrix(3, 3);
//	for (unsigned row = 0; row < zTransform.GetNumRows(); row++)
//		for (unsigned column = 0; column < zTransform.GetNumColumns(); column++)
//			zTransform.Insert(row, column, 0);
//	zTransform.Insert(0, 0, cos(-theta.GetZ()));
//	zTransform.Insert(0, 1, -sin(-theta.GetZ()));
//	zTransform.Insert(1, 0, sin(-theta.GetZ()));
//	zTransform.Insert(1, 1, cos(-theta.GetZ()));
//	zTransform.Insert(2, 2, 1);
//
//	/*Initialize the translation matrix.*/
//	Matrix translation = Matrix(3, 1);
//	translation.Insert(0, 0, a.GetX() - c.GetX());
//	translation.Insert(1, 0, a.GetY() - c.GetY());
//	translation.Insert(2, 0, a.GetZ() - c.GetZ());
//
//	/*Calculate the camera transform in matrix form.*/
//	Matrix d = xTransform * (yTransform * (zTransform * translation));
//
//	/*Return the camera transform in Vector3F form.*/
//	Vector3F *dRet = new Vector3F(d.Get(0, 0), d.Get(1, 0), d.Get(2, 0));
//	return *dRet;
//}
//
///*
//* Takes a camera transform in R^3 and returns the 2D projection of that point on the xy-plane.
//* From the wikipedia page on 3D projection:
//*		a<x,y,z> is the 3D position of a point A that is to be projected.
//*		c<x,y,z> is the 3D position of a point C representing the camera.
//*		theta<x,y,z> is the orientation of the camera.
//*		e<x,y,z> is the viewer's position relative to the display surface which goes through
//*				 point C representing the camera.
//*/
//Vector2F Get2DProjection(const Vector3F &cameraTransform)
//{
//	/*Initialze variables as described on the wikipedia page for 3D projection.*/
//	Vector3F d = cameraTransform;
//	Vector3F e = camera->GetRelativeViewerPosition();
//
//	/*Calculate the 2D projection of the given camera-transformed point.*/
//	if (d.GetZ() == 0)
//		d.SetZ(1.0f);
//	Vector2F *b = new Vector2F((e.GetZ() / d.GetZ()) * d.GetX() - e.GetX(), (e.GetZ() / d.GetZ()) * d.GetY() - e.GetY());
//	return *b;
//}
//
//void TestDrawingIn3D()
//{
//	/*Create a small rectangle in a 3D coordinate system.*/
//	std::vector<Vector3F> makeshift3DRect;
//	Vector3F topLeft = Vector3F(500.0f, 300.0f, 0.0f);
//	Vector2F dimensions = Vector2F(50.0f, 50.0f);
//	for (unsigned int row = (unsigned int)(topLeft.GetY() - dimensions.GetY()); row <= (unsigned int)topLeft.GetY(); row++)
//		for (unsigned int column = (unsigned int)topLeft.GetX(); column <= (unsigned int)(topLeft.GetX() + dimensions.GetX()); column++)
//			makeshift3DRect.push_back(Vector3F((float)column, (float)row, topLeft.GetZ()));
//
//	/*For each point of the 3D rectangle, perform the camera transform on that point and then display
//	  the 2D projection of that point to the screen.*/
//	for (unsigned int i = 0; i < makeshift3DRect.size(); i++)
//	{
//		Vector3F cameraTransform = GetCameraTransform(makeshift3DRect[i]);
//		Vector2F projection = Get2DProjection(cameraTransform);
//		SetPixel((int)projection.GetX(), (int)projection.GetY(), Color3(1.0f, 0.0f, 0.0f));
//	}
//	Display();
//}