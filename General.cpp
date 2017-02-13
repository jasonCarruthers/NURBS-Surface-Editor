#include <GL/glut.h>
#include <ctime>
#include <cstdlib>
#include "General.h"
#include "MyMath.h"
#include "Line.h"
#include "Main.h"


/*
 * Global variables
 *
 * NOTE: const global variables declared with extern in a .h file need to be set in a single .cpp
 *		 file, but will still be visible to other .cpp files. If a global variable declared const
 *		 has the extern keyword modifier, it can't be intialized in the .h file.
 *
 * NOTE: It appears any global variables declared with extern in a .h file need to be set in a single .cpp
 *		 file, but will still be visible to other .cpp files. If any global variable
 *		 has the extern keyword modifier, it can't be intialized in the .h file and must be
 *		 declared in a single .cpp file.
 */
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int SLEEP_DURATION = 1;
const int MAGIC_GARBAGE = -999;
float *pixelBuffer;




/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Vector4F
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Vector4F::Vector4F(float newX, float newY, float newZ, float newW)
{
	mArr[Vector4F::X] = newX;
	mArr[Vector4F::Y] = newY;
	mArr[Vector4F::Z] = newZ;
	mArr[Vector4F::W] = newW;
}

/*
* Accessors
*/
float Vector4F::GetX() const
{
	return mArr[Vector4F::X];
}

float Vector4F::GetY() const
{
	return mArr[Vector4F::Y];
}

float Vector4F::GetZ() const
{
	return mArr[Vector4F::Z];
}

float Vector4F::GetW() const
{
	return mArr[Vector4F::W];
}


/*
* Mutators
*/
void Vector4F::Set(const Vector4F &newCoord)
{
	mArr[Vector4F::X] = newCoord.mArr[Vector4F::X];
	mArr[Vector4F::Y] = newCoord.mArr[Vector4F::Y];
	mArr[Vector4F::Z] = newCoord.mArr[Vector4F::Z];
	mArr[Vector4F::W] = newCoord.mArr[Vector4F::W];
}

void Vector4F::Set(float newX, float newY, float newZ, float newW)
{
	mArr[Vector4F::X] = newX;
	mArr[Vector4F::Y] = newY;
	mArr[Vector4F::Z] = newZ;
	mArr[Vector4F::W] = newW;
}

void Vector4F::SetX(float newX)
{
	mArr[Vector4F::X] = newX;
}

void Vector4F::SetY(float newY)
{
	mArr[Vector4F::Y] = newY;
}

void Vector4F::SetZ(float newZ)
{
	mArr[Vector4F::Z] = newZ;
}

void Vector4F::SetW(float newW)
{
	mArr[Vector4F::W] = newW;
}

/*
* Overloaded operators
*/
float &Vector4F::operator[](int index)
{
	if (index < 0 || index >= (int)Num__Elements)
		return mArr[X]; /*If index is out of bounds, return the first element by default.*/
	return mArr[index];
}
const float &Vector4F::operator[](int index) const
{
	if (index < 0 || index >= (int)Num__Elements)
		return mArr[X]; /*If index is out of bounds, return the first element by default.*/
	return mArr[index];
}

bool operator==(const Vector4F &coord1, const Vector4F &coord2)
{
	return (coord1.GetX() == coord2.GetX() &&
			coord1.GetY() == coord2.GetY() &&
			coord1.GetZ() == coord2.GetZ() &&
			coord1.GetW() == coord2.GetW());
}

Vector4F operator*(const Vector4F &left, const Vector4F &right)
{
	return Vector4F(left[Vector4F::X] * right[Vector4F::X],
					left[Vector4F::Y] * right[Vector4F::Y], 
					left[Vector4F::Z] * right[Vector4F::Z], 
					left[Vector4F::W] * right[Vector4F::W]);
}

Vector4F operator*(const Vector4F &left, float multiplier)
{
	return Vector4F(left[Vector4F::X] * multiplier, 
					left[Vector4F::Y] * multiplier, 
					left[Vector4F::Z] * multiplier, 
					left[Vector4F::W] * multiplier);
}

Vector4F operator*(float multiplier, const Vector4F &right)
{
	return Vector4F(right[Vector4F::X] * multiplier, 
					right[Vector4F::Y] * multiplier, 
					right[Vector4F::Z] * multiplier, 
					right[Vector4F::W] * multiplier);
}

Vector4F operator+(const Vector4F &left, const Vector4F &right)
{
	return Vector4F(left[Vector4F::X] + right[Vector4F::X],
					left[Vector4F::Y] + right[Vector4F::Y],
					left[Vector4F::Z] + right[Vector4F::Z],
					left[Vector4F::W] + right[Vector4F::W]);
}

Vector4F operator+(const Vector4F &left, float deltaSum)
{
	return Vector4F(left[Vector4F::X] + deltaSum,
					left[Vector4F::Y] + deltaSum,
					left[Vector4F::Z] + deltaSum,
					left[Vector4F::W] + deltaSum);
}

void operator+=(Vector4F &left, const Vector4F &right)
{
	left = left + right;
}

void operator+=(Vector4F &left, float deltaSum)
{
	left = left + deltaSum;
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Timer
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Timer::Timer(float newDuration, float newElapsedTime, float newStartTime)
{
	mDuration = newDuration;
	mElapsedTime = newElapsedTime;
	mStartTime = newStartTime;
}

/*
* Accessors
*/
float Timer::GetDuration() const
{
	return mDuration;
}

float Timer::GetElapsedTime() const
{
	return mElapsedTime;
}

float Timer::GetStartTime() const
{
	return mStartTime;
}

/*
* Mutators
*/
void Timer::SetDuration(float newDuration)
{
	mDuration = newDuration;
}

void Timer::SetElapsedTime(float newElapsedTime)
{
	mElapsedTime = newElapsedTime;
}

void Timer::SetStartTime(float newStartTime)
{
	mStartTime = newStartTime;
}

/*Reset elapsed time and set start time to current time*/
void Timer::Reset()
{
	mStartTime = (float)clock();
	mElapsedTime = 0.0f;
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class GraphicVector2D
* -------------------------------------------------------------------------------------------------------
*/
/*
* Initialization of static const member variables.
*/
const unsigned int GraphicVector2D::ARROW_HEAD_FIN_MAGNITUDE = 10;
const float GraphicVector2D::ARROW_HEAD_FIN_ROTATION = 45.0f; /*In degrees*/

/*
* Constructor(s)
*/
GraphicVector2D::GraphicVector2D(const Vector2F &newTail, const Vector2F &newHead)
{
	mTail = new Vector2F(newTail.GetX(), newTail.GetY());
	mHead = new Vector2F(newHead.GetX(), newHead.GetY());
}

GraphicVector2D::GraphicVector2D(const Vector2I &newTail, const Vector2I &newHead, const Color3 &newColor)
{
	mTail = new Vector2F((float)newTail.GetX(), (float)newTail.GetY());
	mHead = new Vector2F((float)newHead.GetX(), (float)newHead.GetY());
	mColor.Set(newColor);
}

/*
* Accessors
*/
Vector2F GraphicVector2D::GetHead() const
{
	return *mHead;
}

Vector2F GraphicVector2D::GetTail() const
{
	return *mTail;
}

Color3 GraphicVector2D::GetColor() const
{
	return mColor;
}

float GraphicVector2D::GetMagnitude() const
{
	return sqrtf(MyPow((float)(mHead->GetX() - mTail->GetX()), 2) + MyPow((float)(mHead->GetY() - mTail->GetY()), 2));
}

/*Increasing theta in the counter-clockwise direction.*/
float GraphicVector2D::GetDirectionInDegrees() const
{
	float theta = (GetDirectionInRadians() * 180.0f / MyPI());
	return theta;
}

/*Increasing theta in the counter-clockwise direction.*/
float GraphicVector2D::GetDirectionInRadians() const
{
	if (mHead->GetX() - mTail->GetX() == 0)
		return (mHead->GetY() - mTail->GetY() >= 0) ? MyPI() / 2.0f : -MyPI() / 2.0f;

	float theta = atanf((float)(mHead->GetY() - mTail->GetY()) / (float)(mHead->GetX() - mTail->GetX()));
	theta = (theta == 0 && mHead->GetX() - mTail->GetX() < 0) ? MyPI() : theta;

	/*atanf only returns a value between -pi and pi, so check to see if we should add
	  pi to the return value.*/
	if (theta != MyPI() && mHead->GetX() <= mTail->GetX())
		theta += MyPI();

	return theta;
}

/*Keeps the tail fixed and adjusts the head position so that the magnitude is 1.*/
GraphicVector2D GraphicVector2D::GetNormalized() const
{
	GraphicVector2D *retGraphicVector2D = new GraphicVector2D(Vector2F(0.0f, 0.0f), *mHead);
	retGraphicVector2D->SetHead(Vector2F(MyPow(cos(GetDirectionInRadians()), 1), MyPow(sin(GetDirectionInRadians()), 1)));
	
	return *retGraphicVector2D;
}

void GraphicVector2D::Draw() const
{
	Draw(mColor);
}

void GraphicVector2D::Draw(const Color3 &overrideColor) const
{
	Line mainBody = Line(Vector2I((int)mTail->GetX(), (int)mTail->GetY()), Vector2I((int)mHead->GetX(), (int)mHead->GetY()), mColor);

	float leftFinTheta = GetDirectionInRadians() + MyPI() + ARROW_HEAD_FIN_ROTATION * MyPI() / 180.0f;
	float rightFinTheta = GetDirectionInRadians() + MyPI() - ARROW_HEAD_FIN_ROTATION * MyPI() / 180.0f;

	Line leftFin = Line(Vector2I((int)mHead->GetX(), (int)mHead->GetY()),
						Vector2I((int)(mHead->GetX() + ARROW_HEAD_FIN_MAGNITUDE * cos(leftFinTheta)),
								 (int)(mHead->GetY() + ARROW_HEAD_FIN_MAGNITUDE * sin(leftFinTheta))),
						mColor);
	Line rightFin = Line(Vector2I((int)mHead->GetX(), (int)mHead->GetY()),
						 Vector2I((int)(mHead->GetX() + ARROW_HEAD_FIN_MAGNITUDE * cos(rightFinTheta)),
								  (int)(mHead->GetY() + ARROW_HEAD_FIN_MAGNITUDE * sin(rightFinTheta))),
						 mColor);

	mainBody.Draw(overrideColor);
	leftFin.Draw(overrideColor);
	rightFin.Draw(overrideColor);
}

/*
* Mutators
*/
void GraphicVector2D::SetHead(const Vector2F &newHead)
{
	Vector2F *temp = new Vector2F(newHead.GetX(), newHead.GetY());

	mHead->SetX(temp->GetX());
	mHead->SetY(temp->GetY());
}

void GraphicVector2D::SetTail(const Vector2F &newTail)
{
	Vector2F *temp = new Vector2F(newTail.GetX(), newTail.GetY());

	mTail->SetX(temp->GetX());
	mTail->SetY(temp->GetY());
}

/*Rotates the head about the tail by newRotation degrees. Magnitude is preserved.*/
void GraphicVector2D::SetDirectionInDegrees(float newRotation)
{
	float magnitude = GetMagnitude();
	float thetaInRadians = newRotation * MyPI() / 180.0f;
	float dx = magnitude * cos(thetaInRadians);
	float dy = magnitude * sin(thetaInRadians);
	mHead->SetX(mTail->GetX() + dx);
	mHead->SetY(mTail->GetY() + dy);
}

void GraphicVector2D::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
}

void GraphicVector2D::Normalize()
{
	GraphicVector2D temp = GetNormalized();
	SetHead(temp.GetHead());
}

/*Clear pixels from pixelBuffer.*/
void GraphicVector2D::Clear()
{
	Draw(Color3(0.0f, 0.0f, 0.0f));
}


/*
 * Global functions
 */
void TestGraphicVector2D()
{
	GraphicVector2D test;
	test.SetTail(Vector2F(500, 400));
	const unsigned int MAGNITUDE = 50;

	for (int i = 0; i < 360; i++)
	{
		test.SetHead(Vector2F(test.GetTail().GetX() + MAGNITUDE * cos(i * MyPI() / 180.0f),
							  test.GetTail().GetY() + MAGNITUDE * sin(i * MyPI() / 180.0f)));
		test.Draw();
		Display();
		test.Clear();
	}
}

Color4 GetRandomColor()
{
	float r = (rand() % 256) / 255.0f;
	float g = (rand() % 256) / 255.0f;
	float b = (rand() % 256) / 255.0f;
	float a = (rand() % 256) / 255.0f;
	return Color4(r, g, b, a);
}

/*Returns a non-yellow, non-dark color.*/
Color4 GetRandomCurveColor()
{
	Color4 retColor;
	do
	{
		retColor = GetRandomColor();
	} while ((retColor.GetR() >= 0.8f && retColor.GetG() >= 0.8f) ||
		(retColor.GetR() <= 0.5f && retColor.GetG() <= 0.5f && retColor.GetB() <= 0.5f));

	return retColor;
}

void SetPixel(int x, int y, const Color3 &color)
{
	for (int colorIndex = 0; colorIndex < Color3::Num__RGBParameters; colorIndex++)
	{
		if ((x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex) < 0 ||
			(x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex) >= WINDOW_WIDTH * WINDOW_HEIGHT * 3)
			break;
		pixelBuffer[x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex] = color[colorIndex]; //See pgs. 146-147 to optimize this.
	}
}

unsigned int StringToUInt(const std::string &str)
{
	unsigned int retVal = 0;
	unsigned int weight = 0;

	for (unsigned int i = str.size() - 1; i >= 0; i--)
	{
		retVal += ((int)str[i] - 48) * weight;
		weight += 10;
	}

	return retVal;
}

std::string UIntToString(unsigned int num)
{
	std::string retVal = "";

	do
	{
		retVal.insert(retVal.begin(), (char)((num % 10) + 48));
		num /= 10;
	} while (num > 0);

	return retVal;
}

std::string FloatToString(float num)
{
	std::string retVal = "";

	/*If the float is negative, insert a negative sign into the string retVal. Then
	  make the float positive for the rest of the calculations*/
	if (num < 0)
	{
		retVal = "-";
		num *= -1;
	}

	/*Insert the portion of the float > 0 into the string retVal*/
	unsigned int numAsUInt = (unsigned int)num;
	retVal += UIntToString(numAsUInt);

	/*Remove the portion of the float > 0, leaving only the decimal.
	  Also, insert a decimal point into the string retVal*/
	num -= numAsUInt;
	retVal += ".";

	/*Insert the decimal portion of the float into the string retVal, with a max
	  of 2 decimal places*/
	unsigned int count = 0;
	do
	{
		num *= 10;
		retVal += (char)((int)num + 48);
		num -= (int)num;

		count++;
	} while (count < 2 && num != 0);

	return retVal;
}

void RefreshScreen()
{
	delete[] pixelBuffer;
	pixelBuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT * 3];
}