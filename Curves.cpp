#include <iostream>
#include <vector>
#include "Main.h"
#include "MyMath.h"
#include "Line.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Tree.h"
#include "Matrix.h"
#include "Camera.h"


/*
*Global variables
*/
Curve *curveParentInFocus;
int rowOfCPInFocus;
int colOfCPInFocus;

unsigned int resolution; /*How many points used to draw each NURBS surface.*/
const unsigned int MIN_ORDER = 3;
unsigned int orderK;
unsigned int orderL;
bool controlPointsVisible; /*True if the controlPointCheckbox is checked.*/
bool controlMeshVisible; /*True if the controlMeshCheckbox is checked.*/
bool surfaceMeshVisible; /*True if the surfaceMeshCheckbox is checked.*/
bool gouraudShadingVisible; /*True if the gouraudShadingCheckbox is checked.*/
NURBSSurface *nurbsSurface;





/* 
 * -------------------------------------------------------------------------------------------------------
 * Implementation of class ControlPoints
 * -------------------------------------------------------------------------------------------------------
 */
/*
* Constructor(s)
*/
ControlPoints::ControlPoints(const Color3 &newColor)
{
	mColor.Set(newColor);
}

/*
* Accessors
*/
std::vector<Vector2I*>* ControlPoints::GetControlPointVec()
{
	return &mCPVec;
}

Color3 ControlPoints::GetColor() const
{
	return mColor;
}

MyRectangle ControlPoints::GetMBB() const
{
	if (mCPVec.size() == 0)
		return MyRectangle();

	int leftMostX;
	int rightMostX;
	int bottomMostY;
	int topMostY;


	/*Find the MBB x and y values.*/
	for (unsigned int i = 0; i < mCPVec.size(); i++)
	{
		if (i == 0)
		{
			leftMostX = mCPVec[i]->GetX();
			rightMostX = mCPVec[i]->GetX();
			bottomMostY = mCPVec[i]->GetY();
			topMostY = mCPVec[i]->GetY();
		}
		else
		{
			if (mCPVec[i]->GetX() < leftMostX)
				leftMostX = mCPVec[i]->GetX();
			else if (mCPVec[i]->GetX() > rightMostX)
				rightMostX = mCPVec[i]->GetX();

			if (mCPVec[i]->GetY() < bottomMostY)
				bottomMostY = mCPVec[i]->GetY();
			else if (mCPVec[i]->GetY() > topMostY)
				topMostY = mCPVec[i]->GetY();
		}
	}

	/*Create the MBB, then return it.*/
	MyRectangle *mbb = new MyRectangle(Vector2I(leftMostX, topMostY), Vector2I(rightMostX - leftMostX, topMostY - bottomMostY), mColor);
	return *mbb;
}

void ControlPoints::Draw() const
{
	for (unsigned int controlPoint = 0; controlPoint < mCPVec.size(); controlPoint++)
	{
		for (int relativeY = -5; relativeY <= 5; relativeY++)
			for (int relativeX = -5; relativeX <= 5; relativeX++)
				SetPixel(mCPVec[controlPoint]->GetX() + relativeX, mCPVec[controlPoint]->GetY() + relativeY, mColor);
	}
}

/*Connect all adjacent control points*/
void ControlPoints::DrawControlPolygon() const
{
	for (unsigned int controlPoint = 1; controlPoint < mCPVec.size(); controlPoint++)
	{
		Line *tempLine = new Line(*mCPVec[controlPoint - 1], *mCPVec[controlPoint], Color3(1.0f, 1.0f, 1.0f));
		tempLine->Draw();
	}
}

/*Draw convex hull*/
void ControlPoints::DrawConvexHull() const
{

}

/*Draw Minimum Bounding Box*/
void ControlPoints::DrawMBB() const
{
	GetMBB().DrawOutline();
}

/*Erase all control point pixels from pixel buffer*/
void ControlPoints::Clear() const
{
	for (unsigned int controlPoint = 0; controlPoint < mCPVec.size(); controlPoint++)
	{
		for (int relativeY = -5; relativeY <= 5; relativeY++)
			for (int relativeX = -5; relativeX <= 5; relativeX++)
				SetPixel(mCPVec[controlPoint]->GetX() + relativeX, mCPVec[controlPoint]->GetY() + relativeY, Color3(0.0f, 0.0f, 0.0f));
	}
}

/*Erase the given control point's pixels from pixel buffer.*/
void ControlPoints::ClearControlPoint(int index) const
{
	for (int relativeY = -5; relativeY <= 5; relativeY++)
		for (int relativeX = -5; relativeX <= 5; relativeX++)
			SetPixel(mCPVec[index]->GetX() + relativeX, mCPVec[index]->GetY() + relativeY, Color3(0.0f, 0.0f, 0.0f));
}

/*Erase control polygon pixels from pixel buffer*/
void ControlPoints::ClearControlPolygon() const
{
	for (unsigned int controlPoint = 1; controlPoint < mCPVec.size(); controlPoint++)
	{
		Line *tempLine = new Line(*mCPVec[controlPoint - 1], *mCPVec[controlPoint], Color3(0.0f, 0.0f, 0.0f));
		tempLine->Draw();
	}
}

/*Erase convex hull pixels from pixel buffer*/
void ControlPoints::ClearConvexHull() const
{

}

/*Erase MBB pixels from pixel buffer*/
void ControlPoints::ClearMBB() const
{
	GetMBB().DrawOutline(Color3(0.0f, 0.0f, 0.0f));
}


/*
* Mutators
*/
void ControlPoints::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
}

void ControlPoints::PushBack(const Vector2I &newControlPoint)
{
	Vector2I *cpPtr = new Vector2I(newControlPoint);
	mCPVec.push_back(cpPtr);
}

void ControlPoints::EraseAll()
{
	Clear();
	mCPVec.clear();
}

void ControlPoints::Erase(int index)
{
	if (index < 0 || (unsigned int)index >= mCPVec.size())
		return;

	ClearControlPoint(index);
	mCPVec.erase(mCPVec.begin() + index);
}


/*
* Overloaded operators
*/
Vector2I& ControlPoints::operator[](int index)
{
	if (index < 0 || index >= (int)mCPVec.size())
		return *mCPVec[0]; /*If index is out of bounds, return the first element by default.*/
	return *mCPVec[index];
}
const Vector2I& ControlPoints::operator[](int index) const
{
	if (index < 0 || index >= (int)mCPVec.size())
		return *mCPVec[0]; /*If index is out of bounds, return the first element by default.*/
	return *mCPVec[index];
}






/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Curve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Curve::Curve(const Color3 &newColor)
{
	mColor.Set(newColor);
	mControlPoints.SetColor(mColor);

	mControlPointVisible = false;
	mControlPolygonVisible = false;
	mMBBVisible = false;
}

Curve::Curve(Curve *copy)
{
	/*Give this curve a random color.*/
	mColor.Set(GetRandomCurveColor().GetColor3());
	mControlPoints.SetColor(mColor);
	

	/*Set boolean variables to have the same value as those of the copy's.*/
	SetControlPointVisibility(copy->GetControlPointVisibility());
	SetControlPolygonVisibility(copy->GetControlPolygonVisibility());
	SetMBBVisibility(copy->GetMBBVisibility());
}

/*
* Accessors
*/
Curve::CurveType Curve::GetType() const
{
	return TYPE;
}

std::vector<Vector2I*>* Curve::GetControlPointVec()
{
	return mControlPoints.GetControlPointVec();
}

std::vector<Vector2I*>* Curve::GetPlottedPointsVec()
{
	return &mPlotVec;
}

Color3 Curve::GetColor() const
{
	return mColor;
}

bool Curve::GetControlPointVisibility() const
{
	return mControlPointVisible;
}

bool Curve::GetControlPolygonVisibility() const
{
	return mControlPolygonVisible;
}

bool Curve::GetMBBVisibility() const
{
	return mMBBVisible;
}

MyRectangle Curve::GetMBB()
{
	MyRectangle *returnRect = new MyRectangle();
	returnRect->Set(mControlPoints.GetMBB());
	return *returnRect;
}

void Curve::Draw() const
{
	Draw(mColor);
}

void Curve::Draw(const Color3 &drawColor) const
{
	Line tempLine;
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
	{
		/*Draw a point within the vec holding the points of the curve*/
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), drawColor);

		/*Draw lines connecting two adjecent points in mPlotVec, in case the resolution
		of the parametric range isn't sufficiently large to draw a continuous curve by
		plotting individual points alone*/
		if (i > 0)
		{
			tempLine = Line(*mPlotVec[i - 1], *mPlotVec[i], drawColor);
			tempLine.Draw();
		}

		/*Draw intermediate lines*/
		//if (mGenerationLines.size() > 0)
		//{
		//	if (i > 0)
		//	{
		//		/*Erase previous generation's itermediate lines*/
		//		for (unsigned int gen = 0; gen < mGenerationLines[i - 1].size(); gen++)
		//			mGenerationLines[i - 1][gen]->Draw(Color3(0.0f, 0.0f, 0.0f));
		//	}

		//	/*Draw current generation's intermediate lines*/
		//	for (unsigned int gen = 0; gen < mGenerationLines[i].size(); gen++)
		//		mGenerationLines[i][gen]->Draw();
		//}
	}

	if (mControlPointVisible)
		mControlPoints.Draw();

	if (mControlPolygonVisible)
		mControlPoints.DrawControlPolygon();

	if (mMBBVisible)
		mControlPoints.DrawMBB();
}

void Curve::DrawIntermediateLines() const
{
	Color3 intermediateLineColor = Color3(mColor.GetR() / 2.0f, mColor.GetG() / 2.0f, mColor.GetB() / 2.0f);
	for (unsigned int i = 0; i < mIntermediateLines.size(); i++)
	{
		Line tempLine = Line(mIntermediateLines[i].first, mIntermediateLines[i].second, intermediateLineColor);
		tempLine.Draw();
	}
}

/*
 * Mutators
 */
/*Whenever a new control point is inserted, (re)calculate all the plot points of the curve*/
void Curve::InsertControlPoint(const Vector2I &newControlPoint)
{
	/*First, clear previously drawn curve*/
	Clear();

	/*Also clear any intermediate lines that may be in the pixelBuffer*/
	ClearIntermediateLines();

	/*Make a copy of the newControlPoint*/
	Vector2I *temp = new Vector2I(newControlPoint.GetX(), newControlPoint.GetY());

	/*Insert the new control point and recalculate the curve*/
	mControlPoints.PushBack(*temp);
	CalcPlotVec();
}

void Curve::EraseAllControlPoints()
{
	mControlPoints.EraseAll();
}

void Curve::EraseControlPoint(int index)
{
	mControlPoints.Erase(index);
}

void Curve::SetIntermediateLines(float t)
{
	/*If there are no control points, return*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return;

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2I *generationTable = new Vector2I[(n + 1)*(n + 1)]; /*Each row represents a generation,
															   starting with generation 0 at the top.
															   Each column within a row represents
															   a child of that generation, in
															   increasing order from left to right.*/

	/*Populate the generation table*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX(mControlPoints[i].GetX());
				generationTable[i].SetY(mControlPoints[i].GetY());
			}
			else
			{
				Vector2I temp = Vector2I((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}

			/*Calculate intermediate lines*/
			if (i > 0)
			{
				/*
				* Store line connecting two adjacent children of the same generation
				*/
				/*Calculate which point on the curve this is*/
				//float tStep = 1.0f / curveResolution;
				//int s = (int)(t / tStep); /*Counter of which pixel is being calculated in the curve*/
				/*Store a generation line*/
				std::pair<Vector2I, Vector2I> *tempLine = new std::pair<Vector2I, Vector2I>;
				tempLine->first = generationTable[j * (n + 1) + i - 1];
				tempLine->second = generationTable[j * (n + 1) + i];
			
				mIntermediateLines.push_back(*tempLine);
			}
		}
	}
}

void Curve::Clear()
{
	/*Clear pixels from pixelBuffer*/
	Line tempLine;
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
	{
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), Color3(0.0f, 0.0f, 0.0f));
		if (i > 0)
		{
			tempLine = Line(*mPlotVec[i - 1], *mPlotVec[i], Color3(0.0f, 0.0f, 0.0f));
			tempLine.Draw();
		}
	}

	/*Clear stored pixel locations within mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();
}

void Curve::ClearIntermediateLines()
{
	/*Clear pixels from pixelBuffer*/
	Line tempLine;
	for (unsigned int i = 0; i < mIntermediateLines.size(); i++)
	{
		Line tempLine = Line(mIntermediateLines[i].first, mIntermediateLines[i].second, Color3(0.0f, 0.0f, 0.0f));
		tempLine.Draw();
	}

	/*Clear stored pixel locations within mIntermediateLines*/
	mIntermediateLines.clear();
}

/*Clear the MBB lines from the pixelBuffer.*/
void Curve::ClearMBB()
{
	mControlPoints.ClearMBB();
}

/*Clear curve, intermediate lines, control points, control polygon, MBB*/
void Curve::ClearEverything()
{
	Clear();
	ClearIntermediateLines();
	mControlPoints.Clear();
	mControlPoints.ClearControlPolygon();
	/*mControlPoints.ClearConvexHull();*/
	mControlPoints.ClearMBB();
}

void Curve::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
	mControlPoints.SetColor(newColor);
}

/*Splits this curve at t. This curve becomes the left child and the right child is returned.*/
Curve* Curve::Split(float t)
{
	/*If there are no control points, return*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return NULL;

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2I *generationTable = new Vector2I[(n + 1)*(n + 1)]; /*Each row represents a generation,
															   starting with generation 0 at the top.
															   Each column within a row represents
															   a child of that generation, in
															   increasing order from left to right.*/

	/*Populate the generation table*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX(mControlPoints[i].GetX());
				generationTable[i].SetY(mControlPoints[i].GetY());
			}
			else
			{
				Vector2I temp = Vector2I((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}
		}
	}

	/*Clear this curve's control points, then set its new control points to be those
	  of the left child*/
	EraseAllControlPoints();
	for (unsigned int i = 0; i < n + 1; i++)
		InsertControlPoint(generationTable[i * (n + 1)]);

	/*Create a pointer to a curve with the right child's control points.*/
	Curve *rightChild;
	rightChild = new BernsteinCurve(GetRandomCurveColor().GetColor3()); 
	
	for (unsigned int i = 0; i < n + 1; i++)
		rightChild->InsertControlPoint(generationTable[i * (n + 1) + n - i]);

	/*Handle flags for right child*/
	rightChild->SetControlPointVisibility(mControlPointVisible);
	rightChild->SetControlPolygonVisibility(mControlPolygonVisible);
	rightChild->SetMBBVisibility(mMBBVisible);

	/*return right child curve*/
	return rightChild;
}

/*Increment curve degree by 1.*/
void Curve::RaiseDegree()
{
}

/*Decrement curve degree by 1.*/
void Curve::ReduceDegree()
{
}

void Curve::SetControlPointVisibility(bool newVisibility)
{
	mControlPointVisible = newVisibility;

	/*If control point visibility has just been turned off, clear all control points that may
	  be onscreen.*/
	if (newVisibility == false)
		mControlPoints.Clear();

	/*If control point visibility has just been turned on, draw control points*/
	else
		mControlPoints.Draw();
}

void Curve::SetControlPolygonVisibility(bool newVisibility)
{
	mControlPolygonVisible = newVisibility;

	/*If control polygon visibility has just been turned off, clear all lines from the
	  control polygon that may be onscreen.*/
	if (newVisibility == false)
		mControlPoints.ClearControlPolygon();
	

	/*If control polygon visibility has just been turned on, draw lines that make up
	  the control polygon*/
	else
		mControlPoints.DrawControlPolygon();
}

void Curve::SetMBBVisibility(bool newVisibility)
{
	mMBBVisible = newVisibility;

	/*If MBB visibility has just been turned off, clear all lines from the
	MBB that may be onscreen.*/
	if (newVisibility == false)
		mControlPoints.ClearMBB();


	/*If control polygon visibility has just been turned on, draw lines that make up
	the control polygon*/
	else
		mControlPoints.DrawMBB();
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class BezierBase
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
BezierBase::BezierBase(const Color3 &newColor) : Curve(newColor)
{
}

BezierBase::BezierBase(Curve *copy) : Curve(copy)
{
}

/*
* Mutators
*/
/*Increment curve degree by 1.*/
void BezierBase::RaiseDegree()
{
	/*Get this curve's control point vec.*/
	std::vector<Vector2I*> oldCPVec = *(mControlPoints.GetControlPointVec());

	/*Return if this curve has fewer than two control points.*/
	if (oldCPVec.size() < 2)
		return;

	/*Create a new curve, set it's first control point to be the same as this curve's first CP.*/
	Curve *tempBernstein = new BernsteinCurve(mColor);
	tempBernstein->InsertControlPoint(*(oldCPVec[0]));

	/*Calculate the positions of the interior set of points for the new set of control points.*/
	Vector2I tempDelta;
	float ratio = 0.0f;
	for (unsigned int i = 1; i < oldCPVec.size(); i++)
	{
		/*Get the x and y distances between the i+1(st) and the i(th) control points.*/
		tempDelta = Vector2I(oldCPVec[i]->GetX() - oldCPVec[i - 1]->GetX(), oldCPVec[i]->GetY() - oldCPVec[i - 1]->GetY());

		/*Calculate geometric ratio.*/
		ratio = ((float)(oldCPVec.size() - i)) / oldCPVec.size();

		/*Insert a new control point into the temporary curve.*/
		tempBernstein->InsertControlPoint(Vector2I(oldCPVec[i - 1]->GetX() + (int)(ratio * tempDelta.GetX()), oldCPVec[i - 1]->GetY() + (int)(ratio * tempDelta.GetY())));
	}

	/*Set the temp curve's last control point to be the same as this curve's last CP.*/
	tempBernstein->InsertControlPoint(*(oldCPVec[oldCPVec.size() - 1]));

	/*Remove this curve's control points.*/
	ClearEverything();
	EraseAllControlPoints();

	/*Get the temp curve's control point vec.*/
	std::vector<Vector2I*> tempCurveCPVec = *(tempBernstein->GetControlPointVec());

	/*Insert all control points of the newly calculated set.*/
	for (unsigned int i = 0; i < tempCurveCPVec.size(); i++)
		InsertControlPoint(*(tempCurveCPVec[i]));
}

/*Decrement curve degree by 1.*/
void BezierBase::ReduceDegree()
{
	/*Get this curve's control point vec.*/
	std::vector<Vector2I*> oldCPVec = *(mControlPoints.GetControlPointVec());

	/*Return if this curve has fewer than two control points.*/
	if (oldCPVec.size() < 4)
		return;

	/*Create a new curve, set it's first control point to be the same as this curve's first CP.*/
	Curve *tempBernstein = new BernsteinCurve(mColor);
	tempBernstein->InsertControlPoint(*(oldCPVec[0]));

	/*
	* Calculate the positions of the interior set of points for the new set of control points.
	*/
	Vector2I tempDelta;
	float ratio = 0.0f;
	/*Calculate the geometric degree reduction from the first control point.*/
	for (unsigned int i = 1; i < oldCPVec.size() - 2; i++)
	{
		/*Get the x and y distances between the i+1(st) and the i(th) control points.*/
		if (i == 1)
			tempDelta = Vector2I(oldCPVec[1]->GetX() - oldCPVec[0]->GetX(), oldCPVec[1]->GetY() - oldCPVec[0]->GetY());
		else
			tempDelta = Vector2I(oldCPVec[i]->GetX() - (*(tempBernstein->GetControlPointVec()))[i - 1]->GetX(), oldCPVec[i]->GetY() - (*(tempBernstein->GetControlPointVec()))[i - 1]->GetY());

		/*Calculate geometric ratio.*/
		ratio = (oldCPVec.size() - 1) / ((float)(oldCPVec.size() - 1 - i));

		/*Insert a new control point into the temporary curve.*/
		if (i == 1)
			tempBernstein->InsertControlPoint(Vector2I(oldCPVec[0]->GetX() + (int)(ratio * tempDelta.GetX()), oldCPVec[0]->GetY() + (int)(ratio * tempDelta.GetY())));
		else
			tempBernstein->InsertControlPoint(Vector2I((*(tempBernstein->GetControlPointVec()))[i - 1]->GetX() + (int)(ratio * tempDelta.GetX()), (*(tempBernstein->GetControlPointVec()))[i - 1]->GetY() + (int)(ratio * tempDelta.GetY())));
	}
	/*Set the temp curve's last control point to be the same as this curve's last CP.*/
	tempBernstein->InsertControlPoint(*(oldCPVec[oldCPVec.size() - 1]));
	/*Calculate the geometric degree reduction from the last control point, and average
	  each of these computed control point locations with those obtained when computing
	  from the first control point.*/
	for (unsigned int i = 1; i < (*(tempBernstein->GetControlPointVec())).size() - 1; i++)
	{
		/*Get the x and y distances between the i-1(st) and the i(th) control points.*/
		if (i == 1)
			tempDelta = Vector2I(oldCPVec[oldCPVec.size() - 2]->GetX() - oldCPVec[oldCPVec.size() - 1]->GetX(), oldCPVec[oldCPVec.size() - 2]->GetY() - oldCPVec[oldCPVec.size() - 1]->GetY());
		else
			tempDelta = Vector2I(oldCPVec[oldCPVec.size() - i - 1]->GetX() - (*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->GetX(), oldCPVec[oldCPVec.size() - i - 1]->GetY() - (*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->GetY());

		/*Calculate geometric ratio.*/
		ratio = (oldCPVec.size() - 1) / ((float)(oldCPVec.size() - 1 - i));

		/*Insert a new control point into the temporary curve.*/
		Vector2I leftMostPositionCalculation = *((*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]);
		if (i == 1)
		{
			Vector2I rightMostPositionCalculation = Vector2I(oldCPVec[oldCPVec.size() - i]->GetX() + (int)(ratio * tempDelta.GetX()), oldCPVec[oldCPVec.size() - i]->GetY() + (int)(ratio * tempDelta.GetY()));
			(*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->SetX((rightMostPositionCalculation.GetX() + leftMostPositionCalculation.GetX()) / 2);
			(*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->SetY((rightMostPositionCalculation.GetY() + leftMostPositionCalculation.GetY()) / 2);
		}
		else
		{
			(*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->SetX(((*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->GetX() + (int)(ratio * tempDelta.GetX()) + leftMostPositionCalculation.GetX()) / 2);
			(*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->SetY(((*(tempBernstein->GetControlPointVec()))[oldCPVec.size() - i - 2]->GetY() + (int)(ratio * tempDelta.GetY()) + leftMostPositionCalculation.GetY()) / 2);
		}
	}

	/*Remove this curve's control points.*/
	ClearEverything();
	EraseAllControlPoints();

	/*Get the temp curve's control point vec.*/
	std::vector<Vector2I*> tempCurveCPVec = *(tempBernstein->GetControlPointVec());

	/*Insert all control points of the newly calculated set.*/
	for (unsigned int i = 0; i < tempCurveCPVec.size(); i++)
		InsertControlPoint(*(tempCurveCPVec[i]));
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class DeCasteljauCurve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
DeCasteljauCurve::DeCasteljauCurve(const Color3 &newColor) : BezierBase(newColor)
{
	TYPE = Curve::DeCasteljau;
}

DeCasteljauCurve::DeCasteljauCurve(Curve *copy) : BezierBase(copy)
{
	/*Make a deep copy of control points.*/
	std::vector<Vector2I*> copyCPVec = *(copy->GetControlPointVec());
	for (unsigned int i = 0; i < copyCPVec.size(); i++)
	{
		Vector2I *newCPPos = new Vector2I(copyCPVec[i]->GetX(), copyCPVec[i]->GetY());
		InsertControlPoint(*newCPPos);
	}
}

/*
* Accessors
*/
Vector2I DeCasteljauCurve::GetPlotPoint(float t)
{
	/*If there are no control points, return some arbitrary coordinate*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return Vector2I(0, 0);

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n-1*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2F *generationTable = new Vector2F[(n + 1)*(n + 1)]; /*Each row represents a generation,
																starting with generation 0 at the top.
																Each column within a row represents
																a child of that generation, in
																increasing order from left to right.*/

	/*Populate the generation table, then return the single child of the last generation*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX((float)mControlPoints[i].GetX());
				generationTable[i].SetY((float)mControlPoints[i].GetY());
			}
			else
			{
				Vector2F temp = Vector2F((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}

			/*Calculate intermediate lines*/
			//if (i > 0)
			//{
			//	/*
			//	* Store line connecting two adjacent children of the same generation
			//	*/
			//	/*Calculate which point on the curve this is*/
			//	float tStep = 1.0f / curveResolution;
			//	int s = (int)(t / tStep); /*Counter of which pixel is being calculated in the curve*/
			//	/*Store a generation line*/
			//	Line* tempLine = new Line(generationTable[j * (n + 1) + i - 1],
			//		generationTable[j * (n + 1) + i], mColor);
			//	std::vector<Line*> tempGenLine;
			//	tempGenLine.push_back(tempLine);
			//	if (j == 0)
			//		mGenerationLines.push_back(tempGenLine);
			//	else
			//		mGenerationLines[s].push_back(tempLine);
			//}
		}
	}

	Vector2I roundedPlotPoint = Vector2I(MyRound(generationTable[n * (n + 1)].GetX()), MyRound(generationTable[n * (n + 1)].GetY()));
	return roundedPlotPoint; /*Return the first (and only) child of the last generation.*/
}

/*
* Mutators
*/
void DeCasteljauCurve::CalcPlotVec()
{
	/*Since this function will calculate every point on the curve, we can clear any previous
	contents in the mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();

	/*Populate the mPlotVec (and, implicitly, mGenerationLines)*/
	Vector2I *temp;
	float tStep = 1.0f / resolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class BernsteinCurve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
BernsteinCurve::BernsteinCurve(const Color3 &newColor) : BezierBase(newColor)
{
	TYPE = Curve::Bernstein;
}

BernsteinCurve::BernsteinCurve(Curve *copy) : BezierBase(copy)
{
	/*Make a deep copy of control points.*/
	std::vector<Vector2I*> copyCPVec = *(copy->GetControlPointVec());
	for (unsigned int i = 0; i < copyCPVec.size(); i++)
	{
		Vector2I *newCPPos = new Vector2I(copyCPVec[i]->GetX(), copyCPVec[i]->GetY());
		InsertControlPoint(*newCPPos);
	}
}

/*
* Accessors
*/
/*
* This function has odd behavior when the number of control points is greater than
* or equal to 22, i.e. when nPlusOne >= 22
*/
Vector2I BernsteinCurve::GetPlotPoint(float t)
{
	/*If there are no control points, return some arbitrary coordinate*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return Vector2I(0, 0);

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Use the Bernstein algorithm to get the plot point determined by t*/
	float x = 0;
	float y = 0;
	float coeff = 0;
	unsigned int nPlusOne = mControlPoints.GetControlPointVec()->size();
	for (unsigned int i = 0; i < nPlusOne; i++)
	{
		coeff = (float)(pascalsTriangle[nPlusOne - 1][i] * MyPow(1.0f - t, nPlusOne - 1 - i) * MyPow(t, i));
		x += coeff * (*mControlPoints.GetControlPointVec())[i]->GetX();
		y += coeff * (*mControlPoints.GetControlPointVec())[i]->GetY();
	}

	Vector2I roundedPlotPoint = Vector2I(MyRound(x), MyRound(y));
	return roundedPlotPoint; /*Return the first (and only) child of the last generation.*/
}

/*
* Mutators
*/
void BernsteinCurve::CalcPlotVec()
{
	/*Since this function will calculate every point on the curve, we can clear any previous
	contents in the mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();

	/*Populate the mPlotVec (and, implicitly, mGenerationLines)*/
	Vector2I *temp;
	float tStep = 1.0f / resolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class AitkenCurve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
AitkenCurve::AitkenCurve(const Color3 &newColor) : Curve(newColor)
{
	TYPE = Curve::Aitken;
}

/*
* Accessors
*/
Vector2I AitkenCurve::GetPlotPoint(float t)
{
	SetIntermediateLines(t);

	/*The last intermediate "line" of the Aitken algorithm is a single point, which
	  is a point on the curve.*/
	return mIntermediateLines[mIntermediateLines.size() - 1].second;



	

	/*Vector2F tempPlotPoint = generationTable[N * (N - 1)];
	Vector2I *plotPoint = new Vector2I(MyRound(tempPlotPoint.GetX()), MyRound(tempPlotPoint.GetY()));
	return *plotPoint;*/
}

/*
* Mutators
*/
void AitkenCurve::CalcPlotVec()
{
	/*Ensure that there are at least two control points.*/
	if (mControlPoints.GetControlPointVec()->size() < 2)
		return;

	/*
	* Since this function will calculate every point on the curve, we can clear any previous
	* contents in the mPlotVec as well as mParametrizedUpperBound.
	* 
	* NOTE: It would be better to calculate the values of mParametrizedUpperBound every
	*		time a new control point is inserted into the curve.
	*/
	mPlotVec.clear();
	mParametrizedUpperBound.clear();

	/*
	* Calculate all the parametrized chord lengths.
	*/
	float sum = 0.0f;
	std::vector<Vector2I*> CPVec = *(mControlPoints.GetControlPointVec());
	/*First, set all non-normalized chord lenghts. Since the sum of the chord lengths
	  isn't known, it will have to be calculated in the first loop.*/
	for (unsigned int i = 1; i < CPVec.size(); i++)
	{
		float length = sqrt(MyPow((float)(CPVec[i]->GetX() - CPVec[i - 1]->GetX()), 2) + MyPow((float)(CPVec[i]->GetY() - CPVec[i - 1]->GetY()), 2));
		sum += length;
		mParametrizedUpperBound.push_back(sum);
	}
	/*Then go back through each chord length value and normalize it by dividing each by
	  the total chord length.*/
	for (unsigned int i = 0; i < mParametrizedUpperBound.size(); i++)
		mParametrizedUpperBound[i] /= sum;

	/*Populate the mPlotVec.*/
	Vector2I *temp;
	float tStep = 1.0f / resolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		mIntermediateLines.clear();
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn.*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}



void AitkenCurve::SetIntermediateLines(float t)
{
	/*Initialize variables.*/
	const unsigned int N = mParametrizedUpperBound.size();
	const unsigned int SPACE_TO_ALLOCATE = N * N;
	Vector2F *generationTable = new Vector2F[SPACE_TO_ALLOCATE];
	std::vector<Vector2I*> CPVec = *(mControlPoints.GetControlPointVec());

	/*Get the segment that t lies within of the parametrized chords.*/
	unsigned int tRange = 0;
	for (; tRange < mParametrizedUpperBound.size(); tRange++)
	{
		if (mParametrizedUpperBound[tRange] >= t)
			break;
	}

	/*Create the generation table and set the intermediate lines used to find the point
	  on the curve.*/
	for (unsigned int generation = 0; generation < N; generation++)
	{
		for (unsigned int segment = 0; segment < N - generation; segment++)
		{
			Vector2F leftParent = (generation == 0) ? Vector2F((float)CPVec[segment]->GetX(), (float)CPVec[segment]->GetY()) : generationTable[(generation - 1)*N + segment];
			Vector2F rightParent = (generation == 0) ? Vector2F((float)CPVec[segment + 1]->GetX(), (float)CPVec[segment + 1]->GetY()) : generationTable[(generation - 1)*N + segment + 1];

			float tLowerBound = (segment == 0) ? 0.0f : mParametrizedUpperBound[segment - 1];
			float tUpperBound = mParametrizedUpperBound[segment + generation];
			float deltaBounds = tUpperBound - tLowerBound;
			float leftParentCoeff = ((tUpperBound - t) / deltaBounds);
			float rightParentCoeff = ((t - tLowerBound) / deltaBounds);

			Vector2F leftTerm = leftParentCoeff * leftParent;
			Vector2F rightTerm = rightParentCoeff * rightParent;
			Vector2F sum = Vector2F(leftTerm + rightTerm);

			generationTable[generation * N + segment].SetX(sum.GetX());
			generationTable[generation * N + segment].SetY(sum.GetY());

			/*Set the intermediate lines.*/
			std::pair<Vector2I, Vector2I> *newIntermediateLine;

			/*if (generation == 0)
			{
				newIntermediateLine = new std::pair<Vector2I, Vector2I>;
				newIntermediateLine->first = *(CPVec[segment]);
				newIntermediateLine->second = *(CPVec[segment + 1]);
				mIntermediateLines.push_back(*newIntermediateLine);
			}*/
			//if (generation == 1)
			//{
			//	/*Draw the generation 0 intermediate lines.*/
			//	
			//	newIntermediateLine = new std::pair<Vector2I, Vector2I>;

			//	if (segment < tRange)
			//	{
			//		newIntermediateLine->first = *(CPVec[segment]);
			//		newIntermediateLine->second = Vector2I((int)generationTable[segment].GetX(), (int)generationTable[segment].GetY());
			//	}
			//	else if (segment == tRange)
			//	{
			//		newIntermediateLine->first = *(CPVec[segment]);
			//		newIntermediateLine->second = *(CPVec[segment + 1]);
			//	}
			//	else /*if(segment > tRange)*/
			//	{
			//		newIntermediateLine->first = Vector2I((int)generationTable[segment + 1].GetX(), (int)generationTable[segment + 1].GetY());
			//		newIntermediateLine->second = *(CPVec[segment + 1]);
			//	}

			//	mIntermediateLines.push_back(*newIntermediateLine);
			//}
			if (generation > 0)
			{
				newIntermediateLine = new std::pair<Vector2I, Vector2I>;
				newIntermediateLine->first = Vector2I((int)generationTable[(generation - 1) * N + segment].GetX(), (int)generationTable[(generation - 1) * N + segment].GetY());
				newIntermediateLine->second = Vector2I((int)generationTable[(generation - 1) * N + segment + 1].GetX(), (int)generationTable[(generation - 1) * N + segment + 1].GetY());
				mIntermediateLines.push_back(*newIntermediateLine);
			}
			if (generation == N - 1)
			{
				newIntermediateLine = new std::pair<Vector2I, Vector2I>;
				newIntermediateLine->first = Vector2I((int)generationTable[generation * N + segment].GetX(), (int)generationTable[generation * N + segment].GetY());
				newIntermediateLine->second = Vector2I((int)generationTable[generation * N + segment].GetX(), (int)generationTable[generation * N + segment].GetY());
				mIntermediateLines.push_back(*newIntermediateLine);
			}
			//if (segment > 0)
			//{
			//	newIntermediateLine->first = Vector2I((int)generationTable[generation * N + segment - 1].GetX(), (int)generationTable[generation * N + segment - 1].GetY());
			//	newIntermediateLine->second = Vector2I((int)generationTable[generation * N + segment].GetX(), (int)generationTable[generation * N + segment].GetY());
			//	mIntermediateLines.push_back(*newIntermediateLine);
			//}

			/*newIntermediateLine = new std::pair<Vector2I, Vector2I>;
			newIntermediateLine->first = Vector2I((int)leftParent.GetX(), (int)leftParent.GetY());
			newIntermediateLine->second = Vector2I((int)rightParent.GetX(), (int)rightParent.GetY());
			mIntermediateLines.push_back(*newIntermediateLine);*/
		}
	}
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class SplineBase
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
SplineBase::SplineBase(const Color3 &newColor) : Curve(newColor)
{
}

/*
* Accessors
*/
/*Left empty intentionally. For more information, see the note in Curves.h in class SplineBase
  about this function.*/
Vector2I SplineBase::GetPlotPoint(float t)
{
	return Vector2I(MAGIC_GARBAGE, MAGIC_GARBAGE);
}

void SplineBase::Draw() const
{
	Draw(mColor);
}

void SplineBase::Draw(const Color3 &drawColor) const
{
	/*Draw cubic bezier curves.*/
	for (unsigned int bernsteinCurve = 0; bernsteinCurve < mBernsteinCurveVec.size(); bernsteinCurve++)
		mBernsteinCurveVec[bernsteinCurve].Draw(drawColor);

	/*Draw plot vec.*/
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), drawColor);

	if (mControlPointVisible)
		mControlPoints.Draw();

	if (mControlPolygonVisible)
		mControlPoints.DrawControlPolygon();

	if (mMBBVisible)
		mControlPoints.DrawMBB();
}

/*
* Mutators
*/
void SplineBase::EraseControlPoint(int index)
{
	mControlPoints.Erase(index);
}

/*Clear the points on the curve from the pixelBuffer.*/
void SplineBase::Clear()
{
	/*Erase this curve's pixels from the pixelBuffer.*/
	for (unsigned int bernsteinCurve = 0; bernsteinCurve < mBernsteinCurveVec.size(); bernsteinCurve++)
		mBernsteinCurveVec[bernsteinCurve].ClearEverything();
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), Color3(0.0f, 0.0f, 0.0f));


	/*Clear the mBernsteinCurveVec.*/
	mBernsteinCurveVec.clear();

	/*Clear the mPlotVec.*/
	mPlotVec.clear();

	ClearIntermediateLines();
	mControlPoints.ClearControlPolygon();
	/*mControlPoints.ClearConvexHull();*/
	mControlPoints.ClearMBB();
	mControlPoints.Clear();
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class VectorSplineBase
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
VectorSplineBase::VectorSplineBase(const Color3 &newColor) : SplineBase(newColor)
{
}

/*
* Accessors
*/
/*If the head position of any vector in mGraphicVector2D matches matchHead within a certain
  rectangular tolerance, return the corresponding index. Otherwise, return MAGIC_GARBAGE.*/
int VectorSplineBase::GetMatchingVectorIndex(const Vector2I &matchHead) const
{
	for (unsigned int i = 0; i < mGraphicVector2DVec.size(); i++)
	{
		Vector2I vecHead = Vector2I(MyRound(mGraphicVector2DVec[i].GetHead().GetX()), MyRound(mGraphicVector2DVec[i].GetHead().GetY()));
		MyRectangle tempRect = MyRectangle(Vector2I(vecHead.GetX() - 5, vecHead.GetY() + 5), Vector2I(11, 11));
		if (tempRect.ContainsPoint(matchHead.GetX(), matchHead.GetY()))
			return i;
	}
	return MAGIC_GARBAGE;
}

GraphicVector2D &VectorSplineBase::GetMatchingVector(int index)
{
	/*If index is within bounds, return the corresponding GraphicVector2D.*/
	if (index >= 0 && (unsigned int)index < mGraphicVector2DVec.size())
		return mGraphicVector2DVec[index];

	/*Otherwise, return some arbitrary GraphicVector2D object.*/
	GraphicVector2D *temp = new GraphicVector2D();
	return *temp;
}

void VectorSplineBase::HideVectors() const
{
	for (unsigned int i = 0; i < mGraphicVector2DVec.size(); i++)
		mGraphicVector2DVec[i].Draw(Color3(0.0f, 0.0f, 0.0f));
}

void VectorSplineBase::Draw() const
{
	Draw(mColor);
}

void VectorSplineBase::Draw(const Color3 &drawColor) const
{
	/*Draw cubic bezier curves.*/
	for (unsigned int bernsteinCurve = 0; bernsteinCurve < mBernsteinCurveVec.size(); bernsteinCurve++)
		mBernsteinCurveVec[bernsteinCurve].Draw(drawColor);

	/*Draw graphic vectors at each control point.*/
	for (unsigned int graphicVector = 0; graphicVector < mGraphicVector2DVec.size(); graphicVector++)
		mGraphicVector2DVec[graphicVector].Draw();

	if (mControlPointVisible)
		mControlPoints.Draw();

	if (mControlPolygonVisible)
		mControlPoints.DrawControlPolygon();

	if (mMBBVisible)
		mControlPoints.DrawMBB();
}

/*
* Mutators
*/
void VectorSplineBase::EraseGraphicVector2D(int index)
{
	if (index < 0 || (unsigned int)index >= mGraphicVector2DVec.size())
		return;

	mGraphicVector2DVec.erase(mGraphicVector2DVec.begin() + index);
}

void VectorSplineBase::InsertVector(const GraphicVector2D &newVector)
{
	GraphicVector2D *temp = new GraphicVector2D(newVector.GetTail(), newVector.GetHead());
	mGraphicVector2DVec.push_back(*temp);
}

void VectorSplineBase::MoveVector(int index, const GraphicVector2D &changedVector)
{
	mGraphicVector2DVec[index].SetHead(changedVector.GetHead());
	mGraphicVector2DVec[index].SetTail(changedVector.GetTail());
}

/*Clear the points on the curve from the pixelBuffer.*/
void VectorSplineBase::Clear()
{
	/*Erase this curve's pixels from the pixelBuffer.*/
	for (unsigned int bernsteinCurve = 0; bernsteinCurve < mBernsteinCurveVec.size(); bernsteinCurve++)
		mBernsteinCurveVec[bernsteinCurve].ClearEverything();

	/*Clear the mBernsteinCurveVec.*/
	mBernsteinCurveVec.clear();
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class DeBoorSpline
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
DeBoorSpline::DeBoorSpline(const Color3 &newColor) : SplineBase(newColor)
{
	TYPE = Curve::DeBoor;
	mOrder = 3;
}

/*
* Accessors
*/
Vector2I DeBoorSpline::GetPlotPoint(float t)
{
	/*Initialize the generationTable.*/
	unsigned int nPlusOne = GetControlPointVec()->size();
	unsigned int tableDimensions = mOrder;
	Vector2F *generationTable = new Vector2F[tableDimensions * tableDimensions];
	
	/*Map t to uBar.*/
	float uBar = t;
	
	/*Get knot that uBar lies within.*/
	unsigned int segment = 0;
	for (unsigned int i = mOrder; i <= nPlusOne; i++)
	{
		if (uBar <= mKnotVec[i])
			break;
	
		segment++;
	}
	
	for (unsigned int j = 0; j < mOrder; j++)
	{
		for (unsigned int i = segment; i <= (mOrder - 1) + segment - j; i++)
		{
			if (j == 0)
			{
				generationTable[i - segment].SetX((float)mControlPoints[i].GetX());
				generationTable[i - segment].SetY((float)mControlPoints[i].GetY());
			}
			else
			{
				float leftTermNumerator = mKnotVec[i + mOrder] - uBar;
				float rightTermNumerator = uBar - mKnotVec[i + j];

				float denominator = mKnotVec[i + mOrder] - mKnotVec[i + j];

				Vector2F rightParent = generationTable[tableDimensions*(j - 1) + i + 1 - segment];
				Vector2F rightTerm = (rightTermNumerator / denominator) * rightParent;

				Vector2F leftParent = generationTable[(tableDimensions*(j - 1)) + i - segment];
				Vector2F leftTerm = (leftTermNumerator / denominator) * leftParent;
	
				Vector2F newChild = leftTerm + rightTerm;
	
				generationTable[(tableDimensions*j) + i - segment].SetX(newChild.GetX());
				generationTable[(tableDimensions*j) + i - segment].SetY(newChild.GetY());
			}
		}
	}
	
	Vector2I roundedPlotPoint = Vector2I(MyRound(generationTable[tableDimensions*(tableDimensions - 1)].GetX()), MyRound(generationTable[tableDimensions*(tableDimensions - 1)].GetY()));
	return roundedPlotPoint; /*Return the first (and only) child of the last generation.*/
}

/*
* Mutators
*/
void DeBoorSpline::SetOrder(unsigned int newOrder)
{
	if (newOrder < MIN_ORDER)
		newOrder = MIN_ORDER;
	/*else if (newOrder > MAX_ORDER)
		newOrder = MAX_ORDER;*/

	mOrder = newOrder;
}

void DeBoorSpline::UpdateKnotVec()
{
	/*Clear old contents of mKnotVec.*/
	mKnotVec.clear();

	/*Recalculate new knots, taking into account the new control point position.*/
	unsigned int n = GetControlPointVec()->size() - 1;
	unsigned int k = mOrder;
	for (unsigned int i = 0; i <= n + k; i++)
	{
		if (i > k - 1 && i <= n + 1)
		{
			mKnotVec.push_back(sqrt(MyPow((float)mControlPoints[i - (k - 1) + 1].GetX() - (float)mControlPoints[i - (k - 1)].GetX(), 2) + MyPow((float)mControlPoints[i - (k - 1) + 1].GetY() - (float)mControlPoints[i - (k - 1)].GetY(), 2)));
			//mKnotVec[i] += 10;
			if (i > k-1)
				mKnotVec[i] += mKnotVec[i - 1];
		}
		else if (i <= k - 1)
			mKnotVec.push_back(0.0f);
		else /*if (i >= n + 1)*/
			mKnotVec.push_back(mKnotVec[n + 1]);
	}

	/*Normalize all knots.*/
	float normalizingDenominator = mKnotVec[n + 1];
	for (unsigned int i = 0; i <= n + k; i++)
		mKnotVec[i] /= normalizingDenominator;
}

void DeBoorSpline::CalcPlotVec()
{
	/*Ensure that there are at least two control points.*/
	if (mControlPoints.GetControlPointVec()->size() < 2)
		return;

	/*Return if the order is greater than the number of placed control points.*/
	if (mOrder > GetControlPointVec()->size())
		return;

	/*
	* Since this function will calculate every point on the curve, we can clear any previous
	* contents in the mPlotVec.
	*/
	ClearEverything();
	mPlotVec.clear();

	/*Update mKnotVec.*/
	UpdateKnotVec();

	/*Populate the mPlotVec.*/
	Vector2I *temp;
	float tStep = 1.0f / resolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn.*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class C1CubicSpline
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
C1CubicSpline::C1CubicSpline(const Color3 &newColor) : VectorSplineBase(newColor)
{
	TYPE = Curve::C1CubicSpline;
}

/*
* Accessors
*/

/*
* Mutators
*/
void C1CubicSpline::EraseControlPoint(int index)
{
	if (index < 0 || (unsigned int)index >= GetControlPointVec()->size())
		return;

	mControlPoints.Erase(index);
	mGraphicVector2DVec.erase(mGraphicVector2DVec.begin() + index);
}

void C1CubicSpline::CalcPlotVec()
{
	/*Ensure there are at least two user-plotted control points.*/
	if (mControlPoints.GetControlPointVec()->size() < 2)
		return;

	/*Clear screen contents and old pixel location info.*/
	Clear();

	/*Calculate parametric distances between user-plotted control points.*/
	/*Get chord length between adjacent control points.*/
	std::vector<float> parametricDistancesVec;
	float sum = 0.0f;
	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
	{
		float chordLength = sqrt(MyPow((float)(mControlPoints[i + 1].GetX() - mControlPoints[i].GetX()), 2) + MyPow((float)(mControlPoints[i + 1].GetY() - mControlPoints[i].GetY()), 2));
		sum += chordLength;
		parametricDistancesVec.push_back(sum);
	}
	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
		parametricDistancesVec[i] /= sum;

	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
	{
		/*For every pair of user-created control points, add two more virtual control points for a total
		  of four control points for a cubic Bezier curve.*/
		BernsteinCurve *newBernsteinCurve = new BernsteinCurve(mColor);

		/*Get normalized derivative vectors at the current pair of user-plotted control points.*/
		GraphicVector2D v0Normalized = mGraphicVector2DVec[i].GetNormalized();
		GraphicVector2D v1Normalized = mGraphicVector2DVec[i + 1].GetNormalized();
		float v0Mag = mGraphicVector2DVec[i].GetMagnitude();
		float v1Mag = mGraphicVector2DVec[i + 1].GetMagnitude();

		/*b0*/newBernsteinCurve->InsertControlPoint(mControlPoints[i]);
		/*b1*/newBernsteinCurve->InsertControlPoint(Vector2I(mControlPoints[i].GetX() + (int)((parametricDistancesVec[i] / 3.0f) * v0Mag * v0Normalized.GetHead().GetX()), mControlPoints[i].GetY() + (int)((parametricDistancesVec[i] / 3.0f) * v0Mag * v0Normalized.GetHead().GetY())));
		/*b2*/newBernsteinCurve->InsertControlPoint(Vector2I(mControlPoints[i + 1].GetX() - (int)((parametricDistancesVec[i] / 3.0f) * v1Mag * v1Normalized.GetHead().GetX()), mControlPoints[i + 1].GetY() - (int)((parametricDistancesVec[i] / 3.0f) * v1Mag * v1Normalized.GetHead().GetY())));
		/*b3*/newBernsteinCurve->InsertControlPoint(mControlPoints[i + 1]);

		/*Insert new bernstein curve into mBernsteinCurveVec.*/
		mBernsteinCurveVec.push_back(*newBernsteinCurve);
	}
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class C1QuadraticSpline
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
C1QuadraticSpline::C1QuadraticSpline(const Color3 &newColor) : VectorSplineBase(newColor)
{
	TYPE = Curve::C1QuadraticSpline;
}

/*
* Accessors
*/

/*
* Mutators
*/
void C1QuadraticSpline::EraseControlPoint(int index)
{
	if (index < 0 || (unsigned int)index >= GetControlPointVec()->size())
		return;

	int numControlPoints = GetControlPointVec()->size();
	/*If there is only 1 control point.*/
	if (numControlPoints == 1)
		mGraphicVector2DVec.erase(mGraphicVector2DVec.begin());

	/*If erasing first control point, and there are 2 control points.*/
	else if (index == 0 && numControlPoints == 2)
	{
		mGraphicVector2DVec[0] = mGraphicVector2DVec[1];
		mGraphicVector2DVec.erase(mGraphicVector2DVec.begin() + 1);
	}

	/*If erasing first control point, and there are > 2 control points.*/
	else if (index == 0 && numControlPoints > 2)
	{
		Vector2F newVectorPos = Vector2F((float)(*(curveParentInFocus->GetControlPointVec()))[1]->GetX(), (float)(*(curveParentInFocus->GetControlPointVec()))[1]->GetY());
		mGraphicVector2DVec[0].SetHead(mGraphicVector2DVec[0].GetHead() + -1.0f * mGraphicVector2DVec[0].GetTail() + newVectorPos);
		mGraphicVector2DVec[0].SetTail(newVectorPos);
	}

	/*If erasing second control point, and there are only 2 control points.*/
	else if (index == 1 && numControlPoints == 2)
		mGraphicVector2DVec.erase(mGraphicVector2DVec.begin() + 1);

	/*If erasing second control point, and there are > 2 control points.*/
	else if (index == 1 && numControlPoints > 2)
	{
		Vector2F newVectorPos = Vector2F((float)(*(curveParentInFocus->GetControlPointVec()))[numControlPoints - 2]->GetX(), (float)(*(curveParentInFocus->GetControlPointVec()))[numControlPoints - 2]->GetY());
		mGraphicVector2DVec[1].SetHead(mGraphicVector2DVec[1].GetHead() + -1.0f * mGraphicVector2DVec[1].GetTail() + newVectorPos);
		mGraphicVector2DVec[1].SetTail(newVectorPos);
	}

	mControlPoints.Erase(index);
}

void C1QuadraticSpline::CalcPlotVec()
{
	/*Ensure there are at least two user-plotted control points.*/
	unsigned int numControlPoints = mControlPoints.GetControlPointVec()->size();
	if (numControlPoints < 2)
		return;

	/*Clear this curve from the screen and old stored pixel location info.*/
	Clear();

	/*Calculate parametric distances between user-plotted control points.*/
	/*Get chord length between adjacent control points.*/
	std::vector<float> parametricDistancesVec;
	float sum = 0.0f;
	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
	{
		float chordLength = sqrt(MyPow((float)(mControlPoints[i + 1].GetX() - mControlPoints[i].GetX()), 2) + MyPow((float)(mControlPoints[i + 1].GetY() - mControlPoints[i].GetY()), 2));
		sum += chordLength;
		parametricDistancesVec.push_back(sum);
	}
	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
		parametricDistancesVec[i] /= sum;

	/*Calculate relative averaged vector (same magnitude as the vector obtained by averaging
	  the graphic vectors at the first and last control points, but the direction is obtained by
	  taking the difference of the direction of the averaged graphic vector and the direction
	  of the vector from the first control point to the second control point.*/
	GraphicVector2D basisFirstGraphicVector2D = GraphicVector2D(Vector2F((float)mControlPoints[0].GetX(), (float)mControlPoints[0].GetY()), Vector2F((float)mControlPoints[1].GetX(), (float)mControlPoints[1].GetY()));
	GraphicVector2D basisSecondGraphicVector2D = GraphicVector2D(Vector2F((float)mControlPoints[numControlPoints - 2].GetX(), (float)mControlPoints[numControlPoints - 2].GetY()), Vector2F((float)mControlPoints[numControlPoints - 1].GetX(), (float)mControlPoints[numControlPoints - 1].GetY()));
	GraphicVector2D relativeFirstGraphicVector2D = GraphicVector2D(Vector2F(0.0f, 0.0f), Vector2F(mGraphicVector2DVec[0].GetHead().GetX() - mGraphicVector2DVec[0].GetTail().GetX(), mGraphicVector2DVec[0].GetHead().GetY() - mGraphicVector2DVec[0].GetTail().GetY()));
	GraphicVector2D relativeSecondGraphicVector2D = GraphicVector2D(Vector2F(0.0f, 0.0f), Vector2F(mGraphicVector2DVec[1].GetHead().GetX() - mGraphicVector2DVec[1].GetTail().GetX(), mGraphicVector2DVec[1].GetHead().GetY() - mGraphicVector2DVec[1].GetTail().GetY()));
	relativeFirstGraphicVector2D.SetDirectionInDegrees(relativeFirstGraphicVector2D.GetDirectionInDegrees() - basisFirstGraphicVector2D.GetDirectionInDegrees());
	relativeSecondGraphicVector2D.SetDirectionInDegrees(relativeSecondGraphicVector2D.GetDirectionInDegrees() - basisSecondGraphicVector2D.GetDirectionInDegrees());
	GraphicVector2D relativeAverageGraphicVector2D = GraphicVector2D(Vector2F(0.0f, 0.0f), (relativeFirstGraphicVector2D.GetHead() + relativeSecondGraphicVector2D.GetHead()) * 0.5f);

	for (unsigned int i = 0; i < mControlPoints.GetControlPointVec()->size() - 1; i++)
	{
		/*For every pair of user-created control points, add one more virtual control point for a total
		of three control points per segment for a quadratic Bezier curve.*/
		BernsteinCurve *newBernsteinCurve = new BernsteinCurve(mColor);

		/*b0*/newBernsteinCurve->InsertControlPoint(mControlPoints[i]);
		
		if (i == 0)
			/*b1*/newBernsteinCurve->InsertControlPoint(mControlPoints[0] + (parametricDistancesVec[0] / 2.0f) * Vector2I(MyRound(relativeAverageGraphicVector2D.GetHead().GetX()), MyRound(relativeAverageGraphicVector2D.GetHead().GetY())));
		else
		{
			std::vector<Vector2I*> *tempCPVec = mBernsteinCurveVec[i - 1].GetControlPointVec();
			Vector2I tempBPointPos = *(((*(tempCPVec)))[1]);
			/*b1*/newBernsteinCurve->InsertControlPoint(((parametricDistancesVec[i - 1] + parametricDistancesVec[i]) / parametricDistancesVec[i - 1]) * mControlPoints[i] - (parametricDistancesVec[i] / parametricDistancesVec[i - 1]) * tempBPointPos);
		}

		/*b2*/newBernsteinCurve->InsertControlPoint(mControlPoints[i + 1]);


		/*Insert new bernstein curve into mBernsteinCurveVec.*/
		mBernsteinCurveVec.push_back(*newBernsteinCurve);
	}
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class NURBSSurface
* -------------------------------------------------------------------------------------------------------
*/
/*
* Initialization of static variables
*/
const Curve::CurveType NURBSSurface::TYPE = Curve::NURBS_Surface;
const unsigned int NURBSSurface::CP_DIMENSIONS = 3;

/*
* Constructor(s)
*/
NURBSSurface::NURBSSurface(const Color3 &newColor)
{
	mOrderK = 0;
	mOrderL = 0;

	mColor.Set(newColor);

	mControlPointsAreVisible = false;
	mControlMeshIsVisible = false;
	mSurfaceMeshIsVisible = false;
	mGouraudShadingIsVisible = false;
}

/*
* Accessors
*/
Vector2I NURBSSurface::GetPlotPoint(float u, float v, const Matrix &worldToCameraTransform) const
{
	/*
	* General note about variables:
	* m, u, k along the "x-axis"
	* n, v, l along the "y-axis"
	*/

	/*If there are no de Boor control points, return.*/
	if (mProjectedControlPointsVec.size() == 0)
		return Vector2I(0, 0);

	/*Get worldToCameraTransform.*/
	//Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());

	/*Set n and m. n represents the size of a column, and m represents the size of a row.*/
	unsigned int n = mProjectedControlPointsVec.size();
	unsigned int m = mProjectedControlPointsVec[0].size();

	/*Get a point on the curve in R^4.*/
	Vector4F sStar = Vector4F(0, 0, 0, 0);
	for (unsigned int j = 0; j < n; j++) /*y*/
	{
		for (unsigned int i = 0; i < m; i++) /*x*/
		{
			/*Get the de Boor control point in R^3.*/
			Vector3F deBoorCP = Vector3F(mControlPointsVec[j][i].GetX(), mControlPointsVec[j][i].GetY(), mControlPointsVec[j][i].GetZ());

			/*Homogenize each de Boor control point to R^4.*/
			Vector4F deBoorCPStar = Vector4F(deBoorCP.GetX() / mWeightVec[j][i], deBoorCP.GetY() / mWeightVec[j][i], deBoorCP.GetZ() / mWeightVec[j][i], mWeightVec[j][i]);

			/*Calculate Nik.*/
			float Nik = GetNik(mOrderK, (signed int)i, u);
			
			/*Calculate Njl.*/
			float Njl = GetNjl(mOrderL, (signed int)j, v);

			/*Calculate a point on the curve, s*(u, v), in R^4.*/
			sStar = sStar + deBoorCPStar * (Nik * Njl);
		}
	}

	/*Inhomegenize s*(u, v) to s(u, v).*/
	Vector3F s = Vector3F(sStar.GetX() / sStar.GetW(), sStar.GetY() / sStar.GetW(), sStar.GetZ() / sStar.GetW());

	/*Get projection of s onto the screen.*/
	Vector2I sProjection = Get2DProjection(s, worldToCameraTransform);

	/*Return s(u, v).*/
	return sProjection;
}

float NURBSSurface::GetNik(unsigned int orderK, unsigned int i, float u) const
{
	if (orderK == 1)
		return (mUKnotVec[i] <= u && u <= mUKnotVec[i + 1]) ? 1.0f : 0.0f;

	float preBasis = GetNik(orderK - 1, i, u);
	float postBasis = GetNik(orderK - 1, i + 1, u);

	//unsigned int deltaOrder = mOrderK - orderK;

	if (mUKnotVec[i + orderK - 1] - mUKnotVec[i] == 0 ||
		mUKnotVec[i + orderK] - mUKnotVec[i + 1] == 0)
		return 0.0f;

	float Nik = ((u - mUKnotVec[i]) / (mUKnotVec[i + orderK - 1] - mUKnotVec[i])) * preBasis +
				((mUKnotVec[i + orderK] - u) / (mUKnotVec[i + orderK] - mUKnotVec[i + 1])) * postBasis;

	return Nik;
}

float NURBSSurface::GetNjl(unsigned int orderL, unsigned int j, float v) const
{
	if (orderL == 1)
		return (mVKnotVec[j] <= v && v <= mVKnotVec[j + 1]) ? 1.0f : 0.0f;

	float preBasis = GetNjl(orderL - 1, j, v);
	float postBasis = GetNjl(orderL - 1, j + 1, v);

	//unsigned int deltaOrder = mOrderL - orderL;

	if (mVKnotVec[j + orderL - 1] - mVKnotVec[j] == 0 ||
		mVKnotVec[j + orderL] - mVKnotVec[j + 1] == 0)
		return 0.0f;

	float Njl = ((v - mVKnotVec[j]) / (mVKnotVec[j + orderL - 1] - mVKnotVec[j])) * preBasis +
		((mVKnotVec[j + orderL] - v) / (mVKnotVec[j + orderL] - mVKnotVec[j + 1])) * postBasis;

	return Njl;
}

unsigned int NURBSSurface::GetOrderK() const
{
	return mOrderK;
}

unsigned int NURBSSurface::GetOrderL() const
{
	return mOrderL;
}

unsigned int NURBSSurface::GetNumRows() const
{
	return mControlPointsVec.size();
}

unsigned int  NURBSSurface::GetNumColumns() const
{
	return (mControlPointsVec.size() == 0) ? 0 : mControlPointsVec[0].size();
}

void NURBSSurface::ClearFromPixelBuffer() const
{
	/*Clear projected control points and points on the curve from the pixelBuffer.*/
	Draw(Color3(0.0f, 0.0f, 0.0f));
}

void NURBSSurface::Draw() const
{
	Draw(mColor);
}

void NURBSSurface::Draw(const Color3 &drawColor) const
{
	Color3 cpDrawColor = (drawColor == Color3(0, 0, 0)) ? drawColor : Color3(1, 1, 1);

	/*Draw projected plot points.*/
	for (unsigned int row = 0; row < mPlotVec.size(); row++)
		for (unsigned int column = 0; column < mPlotVec[row].size(); column++)
			SetPixel(mPlotVec[row][column].GetX(), mPlotVec[row][column].GetY(), drawColor);

	/*Draw projected control points*/
	if (drawColor == Color3(0, 0, 0) || mControlPointsAreVisible)
	{
		for (unsigned int i = 0; i < mProjectedControlPointsVec.size(); i++)
		{
			for (unsigned int j = 0; j < mProjectedControlPointsVec[i].size(); j++)
			{
				Vector2I tempCP = mProjectedControlPointsVec[i][j];

				for (int deltaX = -((int)NURBSSurface::CP_DIMENSIONS / 2); deltaX < (int)NURBSSurface::CP_DIMENSIONS; deltaX++)
					for (int deltaY = -((int)NURBSSurface::CP_DIMENSIONS / 2); deltaY < (int)NURBSSurface::CP_DIMENSIONS; deltaY++)
						SetPixel((int)tempCP.GetX() + deltaX, (int)tempCP.GetY() + deltaY, cpDrawColor);
			}
		}
	}

	/*Draw control mesh.*/
	if (drawColor == Color3(0, 0, 0) || mControlMeshIsVisible)
	{
		for (unsigned int i = 0; i < mControlMeshVec.size(); i++)
		{
			if (drawColor == Color3(0, 0, 0))
				mControlMeshVec[i].Draw(drawColor);
			else
				mControlMeshVec[i].Draw();
		}
	}

	/*Draw surface mesh.*/
	if (drawColor == Color3(0, 0, 0) || mSurfaceMeshIsVisible)
	{
		for (unsigned int i = 0; i < mSurfaceMeshVec.size(); i++)
		{
			if (drawColor == Color3(0, 0, 0))
				mSurfaceMeshVec[i].Draw(drawColor);
			else
				mSurfaceMeshVec[i].Draw();
		}
	}

	/*Draw Gouraud shading.*/
}


/*
* Mutators
*/
void NURBSSurface::CalcPlotVec()
{
	/*Clear old contents of mPlotVec.*/
	mPlotVec.clear();

	/*If there are no control points, return.*/
	if (mProjectedControlPointsVec.size() == 0)
		return;

	/*Get the worldToCameraTransform.*/
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());

	unsigned int n = mProjectedControlPointsVec.size();
	unsigned int m = (n == 0) ? 0 : mProjectedControlPointsVec[0].size();
	
	float deltaU = (mUKnotVec[m + 1] - mUKnotVec[mOrderK - 2]) / sqrt((float)resolution);
	float deltaV = (mVKnotVec[n + 1] - mVKnotVec[mOrderL - 2]) / sqrt((float)resolution);

	for (float v = mVKnotVec[mOrderL - 2]; v <= mVKnotVec[n + 1]; v += deltaV)
	{
		std::vector<Vector2I> tempPlotRow;
		mPlotVec.push_back(tempPlotRow);
		for (float u = mUKnotVec[mOrderK - 2]; u <= mUKnotVec[m + 1]; u += deltaU)
		{
			mPlotVec[mPlotVec.size() - 1].push_back(GetPlotPoint(u, v, worldToCameraTransform));
		}
	}

	/*Ensure that the last point is plotted.*/
	mPlotVec[mPlotVec.size() - 1].push_back(GetPlotPoint(mUKnotVec[m + 1], mVKnotVec[n + 1], worldToCameraTransform));
}

void NURBSSurface::InsertSegment(SegmentType segType, unsigned int index)
{
	if (segType == NURBSSurface::Row)
	{
		unsigned int numRows = mControlPointsVec.size();
		unsigned int numColumns = (numRows == 0) ? 0 : mControlPointsVec[0].size();

		/*Ensure that index is at most indicating the slot at the end of the last row of the surface.*/
		if (index > numRows) index = numRows;

		/*Insert a new row at the index position.*/
		std::vector<Vector3F> *newRow = new std::vector<Vector3F>;
		mControlPointsVec.insert(mControlPointsVec.begin() + index, *newRow);
		
		/*Populate the new row with default values.*/
		for (unsigned int i = 0; i < numColumns; i++)
		{
			Vector3F p1;
			Vector3F p2;
			Vector3F tempCPPos;

			if (numRows < 2)
			{
				tempCPPos = Vector3F(20, 20, 20 + (float)(i + 1) * 10);
			}
			else if (index == 0)
			{
				p1 = mControlPointsVec[1][i];
				p2 = mControlPointsVec[2][i];
				tempCPPos = p1 + (p1 + -1.0f * p2);
			}
			else
			{
				p1 = mControlPointsVec[index - 2][i];
				p2 = mControlPointsVec[index - 1][i];
				tempCPPos = p2 + (p2 + -1.0f * p1);
			}

			mControlPointsVec[index].push_back(tempCPPos);
		}
	}
	else if (segType == NURBSSurface::Column)
	{
		unsigned int numRows = mControlPointsVec.size();
		unsigned int numColumns = (numRows == 0) ? 0 : mControlPointsVec[0].size();

		/*Ensure that index is at most indicating the slot at the end of the last column of the surface.*/
		if (index > numColumns) index = numColumns;

		/*Populate the new row with default values.*/
		Vector3F tempCPPos = Vector3F(100, 100, 0);
		for (unsigned int i = 0; i < numRows; i++)
		{
			Vector3F p1;
			Vector3F p2;
			Vector3F tempCPPos;

			if (numColumns < 2)
			{
				tempCPPos = Vector3F(20 + (float)(i + 1) * 10, 20, 20);
			}
			else if (index == 0)
			{
				p1 = mControlPointsVec[i][0];
				p2 = mControlPointsVec[i][1];
				tempCPPos = p1 + (p1 + -1.0f * p2);
			}
			else
			{
				p1 = mControlPointsVec[i][index - 2];
				p2 = mControlPointsVec[i][index - 1];
				tempCPPos = p2 + (p2 + -1.0f * p1);
			}

			mControlPointsVec[i].insert(mControlPointsVec[i].begin() + index, tempCPPos);
		}
	}
}

void NURBSSurface::DeleteSegment(SegmentType segType, unsigned int index)
{
	if (segType == NURBSSurface::Row)
	{
		/*A NURBSSurface should never have fewer than two rows (and columns), so if there are only two rows, return.*/
		if (mControlPointsVec.size() <= 2)
			return;

		unsigned int numRows = mControlPointsVec.size();

		if (index >= numRows) index = numRows - 1;

		mControlPointsVec.erase(mControlPointsVec.begin() + index);

		/*If the control point in focus was in the row that was just deleted, change the control point
		  in focus to be in an adjacent row.*/
		if (rowOfCPInFocus != MAGIC_GARBAGE)
		{
			if (rowOfCPInFocus == index && index == 0)
				rowOfCPInFocus = mControlPointsVec.size() - 1;
			else if (rowOfCPInFocus == index && index == mControlPointsVec.size())
				rowOfCPInFocus = mControlPointsVec.size() - 1;
			else if (rowOfCPInFocus == index)
				rowOfCPInFocus--;
		}
	}
	else if (segType == NURBSSurface::Column)
	{
		/*A NURBSSurface should never have fewer than two columns (and rows), so if there are only two columns, return.*/
		if (mControlPointsVec.size() < 0 || mControlPointsVec[0].size() <= 2)
			return;

		unsigned int numColumns = mControlPointsVec[0].size();

		if (index >= numColumns) index = numColumns - 1;

		for (unsigned int row = 0; row < mControlPointsVec.size(); row++)
			mControlPointsVec[row].erase(mControlPointsVec[row].begin() + index);

		/*If the control point in focus was in the column that was just deleted, change the control point
		in focus to be in an adjacent column.*/
		if (colOfCPInFocus != MAGIC_GARBAGE)
		{
			if (colOfCPInFocus == index && index == 0)
				colOfCPInFocus = mControlPointsVec[0].size() - 1;
			else if (colOfCPInFocus == index && index == mControlPointsVec[0].size())
				colOfCPInFocus = mControlPointsVec[0].size() - 1;
			else if (colOfCPInFocus == index)
				colOfCPInFocus--;
		}
	}
}

std::vector<std::vector<Vector3F>> &NURBSSurface::GetControlPointVec()
{
	return mControlPointsVec;
}

std::vector<std::vector<Vector2I>> &NURBSSurface::GetProjectedControlPointVec()
{
	return mProjectedControlPointsVec;
}

std::vector<std::vector<float>> &NURBSSurface::GetWeightVec()
{
	return mWeightVec;
}

void NURBSSurface::SetOrderK(unsigned int newOrderK)
{
	mOrderK = newOrderK;
}

void NURBSSurface::SetOrderL(unsigned int newOrderL)
{
	mOrderL = newOrderL;
}

void NURBSSurface::CalcUKnots()
{
	/*Clear old contents of mUKnotVec.*/
	mUKnotVec.clear();

	unsigned int m = (mProjectedControlPointsVec.size() == 0) ? 0 : mProjectedControlPointsVec[0].size() - 1;
	unsigned int k = mOrderK;

	for (unsigned int i = 0; i <= m + k; i++)
		mUKnotVec.push_back((float)(i + 1));
}

void NURBSSurface::CalcVKnots()
{
	/*Clear old contents of mUKnotVec.*/
	mVKnotVec.clear();

	unsigned int n = mProjectedControlPointsVec.size() - 1;
	unsigned int l = mOrderL;
	
	for (unsigned int i = 0; i <= n + l; i++)
		mVKnotVec.push_back((float)(i + 1));
}

void NURBSSurface::SetControlPointVisibility(bool newVisibility)
{
	mControlPointsAreVisible = newVisibility;
}

void NURBSSurface::SetControlMeshVisibility(bool newVisibility)
{
	mControlMeshIsVisible = newVisibility;
}

void NURBSSurface::SetSurfaceMeshVisibility(bool newVisibility)
{
	mSurfaceMeshIsVisible = newVisibility;
}

void NURBSSurface::SetGouraudShadingVisibility(bool newVisibility)
{
	mGouraudShadingIsVisible = newVisibility;
}

void NURBSSurface::SetProjectedControlPoints()
{
	/*Get worldToCameraTransform.*/
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());

	/*Clear the old mProjectedControlPointsVec contents.*/
	mProjectedControlPointsVec.clear();

	/*Set projected control points.*/
	for (unsigned int row = 0; row < mControlPointsVec.size(); row++)
	{
		std::vector<Vector2I> *tempProjectedCPRow = new std::vector<Vector2I>;
		mProjectedControlPointsVec.push_back(*tempProjectedCPRow);
		for (unsigned int column = 0; column < mControlPointsVec[row].size(); column++)
		{
			mProjectedControlPointsVec[row].push_back(Get2DProjection(mControlPointsVec[row][column], worldToCameraTransform));
		}
	}
}

void NURBSSurface::SetControlMesh()
{
	Color3 controlMeshColor = Color3(0, 1, 1);
	mControlMeshVec.clear();

	for (unsigned int row = 0; row < mProjectedControlPointsVec.size(); row++)
	{
		for (unsigned int column = 0; column < mProjectedControlPointsVec[row].size(); column++)
		{
			Line *tempLine;
			if (column + 1 < mProjectedControlPointsVec[row].size())
			{
				tempLine = new Line(mProjectedControlPointsVec[row][column], mProjectedControlPointsVec[row][column + 1], controlMeshColor);
				mControlMeshVec.push_back(*tempLine);
			}
			if (row + 1 < mProjectedControlPointsVec.size())
			{
				tempLine = new Line(mProjectedControlPointsVec[row][column], mProjectedControlPointsVec[row + 1][column], controlMeshColor);
				mControlMeshVec.push_back(*tempLine);
			}
		}
	}
}

void NURBSSurface::SetSurfaceMesh()
{
	Color3 surfaceMeshColor = Color3(1, 0, 1);
	mSurfaceMeshVec.clear();

	for (unsigned int row = 0; row < mPlotVec.size(); row++)
	{
		for (unsigned int column = 0; column < mPlotVec[row].size(); column++)
		{
			Line *tempLine;
			if (column + 1 < mPlotVec[row].size())
			{
				tempLine = new Line(mPlotVec[row][column], mPlotVec[row][column + 1], surfaceMeshColor);
				mSurfaceMeshVec.push_back(*tempLine);
			}
			if (row + 1 < mPlotVec.size())
			{
				tempLine = new Line(mPlotVec[row][column], mPlotVec[row + 1][column], surfaceMeshColor);
				mSurfaceMeshVec.push_back(*tempLine);
			}
		}
	}
}

void NURBSSurface::EraseEverything()
{
	ClearFromPixelBuffer();

	mControlPointsVec.clear();
	mProjectedControlPointsVec.clear();
	mWeightVec.clear();
	mPlotVec.clear();
	mControlMeshVec.clear();
	mSurfaceMeshVec.clear();
}

void NURBSSurface::ClearCalcDraw()
{
	ClearFromPixelBuffer();
	CalcPlotVec();
	Draw();
}

/*
* Global functions
*/
/*Takes a float param because all functions linked to a slider must have the form:
*			void funcName(float paramName)
*/
void SetResolution(float newResolution)
{
	/*Set new resolution*/
	resolution = (unsigned int)newResolution;

	/*Clear the old NURBSSurface from the pixelBuffer, then redraw with the new resolution.*/
	nurbsSurface->ClearFromPixelBuffer();
	nurbsSurface->CalcPlotVec();
	nurbsSurface->Draw();
}

void SetOrderK(float t)
{
	orderK = (unsigned int)MyRound(t);

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Clear the old NURBSSurface from the pixelBuffer, set the new orderK, then redraw.*/
	nurbsSurface->ClearFromPixelBuffer();
	nurbsSurface->SetOrderK(orderK);
	//nurbsSurface->CalcUKnots();
	nurbsSurface->CalcPlotVec();
	nurbsSurface->Draw();
}

void SetOrderL(float t)
{
	orderL = (unsigned int)MyRound(t);

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Clear the old NURBSSurface from the pixelBuffer, set the new orderL, then redraw.*/
	nurbsSurface->ClearFromPixelBuffer();
	nurbsSurface->SetOrderL(orderL);
	//nurbsSurface->CalcVKnots();
	nurbsSurface->CalcPlotVec();
	nurbsSurface->Draw();
}

void SetWeight(float newWeight)
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	std::vector<std::vector<float>> *weightVec = &(nurbsSurface->GetWeightVec());
	(*weightVec)[rowOfCPInFocus][colOfCPInFocus] = newWeight;
}

void InitNURBSSurface()
{
	const unsigned int NUM_ROWS = 4;
	const unsigned int NUM_COLUMNS = 3;

	/*Clear the old points from the nurbsSurface from the pixelBuffer,
	  and erase all of the data stored in the vectors.*/
	//nurbsSurface->EraseEverything();

	/*Set visibility for control points, the control mesh, and the surface mesh.*/
	nurbsSurface->SetControlPointVisibility(controlPointsVisible);
	nurbsSurface->SetControlMeshVisibility(controlMeshVisible);
	nurbsSurface->SetSurfaceMeshVisibility(surfaceMeshVisible);
	nurbsSurface->SetGouraudShadingVisibility(gouraudShadingVisible);

	/*Insert NUM_ROWS number of rows into the NURBSSurface.*/
	for (unsigned int row = 0; row < NUM_ROWS; row++)
		nurbsSurface->InsertSegment(NURBSSurface::Row, row);

	/*Insert NUM_COLUMNS number of columns into the NURBSSurface.*/
	for (unsigned int column = 0; column < NUM_COLUMNS; column++)
		nurbsSurface->InsertSegment(NURBSSurface::Column, column);

	/*Initialize the control points of the NURBSSurface.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[0][0] = Vector3F(-60, 80, 60);
	(*cpVec)[0][1] = Vector3F(0, 60, 80);
	(*cpVec)[0][2] = Vector3F(60, 80, 60);
	(*cpVec)[1][0] = Vector3F(-40, 40, 40);
	(*cpVec)[1][1] = Vector3F(0, 40, 30);
	(*cpVec)[1][2] = Vector3F(40, 40, 20);
	(*cpVec)[2][0] = Vector3F(-40, 20, -5);
	(*cpVec)[2][1] = Vector3F(0, 40, -10);
	(*cpVec)[2][2] = Vector3F(40, 20, -10);
	(*cpVec)[3][0] = Vector3F(-40, 40, -80);
	(*cpVec)[3][1] = Vector3F(0, 60, -100);
	(*cpVec)[3][2] = Vector3F(40, 80, -90);

	/*Initialize the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Initialize the weights of the control points of the NURBSSurface.*/
	std::vector<std::vector<float>> *weightVec = &(nurbsSurface->GetWeightVec());
	for (unsigned int row = 0; row < NUM_ROWS; row++)
	{
		std::vector<float> tempWeightRow;
		(*weightVec).push_back(tempWeightRow);
		for (unsigned int column = 0; column < NUM_COLUMNS; column++)
		{
			(*weightVec)[row].push_back(1.0f);
		}
	}

	/*Set the order K and the order L of the NURBSSurface.*/
	nurbsSurface->SetOrderK(orderK);
	nurbsSurface->SetOrderL(orderL);

	/*Set the u and v knots of the NURBSSurface.*/
	nurbsSurface->CalcUKnots();
	nurbsSurface->CalcVKnots();

	/*Calculate the points on the surface, the lines of the control mesh, and the lines of the surface mesh,
	  then update the pixel buffer.*/
	nurbsSurface->CalcPlotVec();
	nurbsSurface->SetControlMesh();
	nurbsSurface->SetSurfaceMesh();
	nurbsSurface->Draw();

	//Display();
}