#ifndef HFILE_CAMERA
#define HFILE_CAMERA

#include "General.h"
#include "MyMath.h"
#include "Matrix.h"


//theta =
//-0.7f

/*
* The Camera uses the uvn Viewing-Coordinate Reference Frame (described in the Computer Graphics with OpenGL book).
* Uses the right-hand rule.
*/
class Camera
{
public:
	/*
	* Constructor(s)
	*/
	Camera(const Vector3F &newPosition = Vector3F(0, 30, 80),
		   const Vector3F &newLookAt = Vector3F(0.0f, 0.0f, 0.0f),
		   float newFieldOfViewAngle = MyPI()/2.0f);

	/*
	* Accessors
	*/
	Vector3F GetPosition() const;
	Vector3F GetN() const;
	Vector3F GetV() const;
	Vector3F GetU() const;
	float GetFieldOfViewAngle() const;
	float GetDistanceFromCanvas() const; /*Returns the shortest distance between the camera and the canvas.*/
	//float GetZViewTheta() const;
	//float GetZViewPhi() const;
	Matrix GetCameraToWorldTransform() const;

	/*
	* Mutators
	*/
	void SetPosition(const Vector3F &newPosition);
	void SetN(const Vector3F &newN);
	void SetV(const Vector3F &newV);
	void SetU(const Vector3F &newU);
	void SetFieldOfViewAngle(float newFieldOfViewAngle);
	//void SetZViewTheta(float newZViewTheta);
	//void SetZViewPhi(float newZViewPhi);
	void LookAt(const Vector3F &newLookAt);

private:
	Vector3F mPosition;
	Vector3F mN; /*"Look-at" vector: the direction for the z-view axis of the camera.*/
	Vector3F mV; /*"Up" vector: the direction for the y-view axis of the camera.*/
	Vector3F mU; /*The direction for the x-view axis of the camera; derived from mN and mV unit vectors.*/
	float mFieldOfViewAngle; /*In radians.*/
	//float mZViewTheta;
	//float mZViewPhi;
};



/*
* Global function prototypes
*/
void InitCamera();
void DrawAxes();
Vector2I Get2DProjection(const Vector3F &worldPos, const Matrix &worldToCameraTransform);
void TestDrawingIn3D();
void DrawXYZPlaneLines();

//Vector3F GetCameraTransform(const Vector3F &worldPos); /*Transforms a world (global) position to a position
													     //relative to the camera's eye.*/
//Vector2F Get2DProjection(const Vector3F &cameraTransform); /*Takes a camera transform in R^3 and returns the
														    // 2D projection of that point on the xy-plane.*/
//void TestDrawingIn3D();



/*
* Global variables
*/
extern Camera *camera;
extern GraphicVector2D *xAxis;
extern GraphicVector2D *yAxis;
extern GraphicVector2D *zAxis;
extern float ZNEAR;
extern float ZFAR;
extern const float CANVAS_WIDTH;
extern const float CANVAS_HEIGHT;



#endif /*HFILE_CAMERA*/