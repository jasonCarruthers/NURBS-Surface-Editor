#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include "Main.h"
#include "Line.h"
#include "Rectangle.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"




/*
* Global variables
*/





/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Axis
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Axis::Axis()
{
	mNumTickMarks = 5;
}

/*
* Accessors
*/
MyRectangle Axis::GetAxis() const
{
	return mAxis;
}

unsigned int Axis::GetNumTickMarks() const
{
	return mNumTickMarks;
}

std::string Axis::GetLabelName() const
{
	return mLabelName;
}

Vector2F Axis::GetLabelPosition() const
{
	return mLabelPos;
}

Vector2I Axis::GetRange() const
{
	return mRange;
}

/*Takes in a value and returns the screen position along this axis of the given value*/
Vector2I Axis::GetPosition(int value) const
{
	/*
	* Bound the value to this axis.
	* NOTE: This may not be the best way to handle a point that doesn't lie along the axis.
	*/
	if (value < mRange.GetX())
		return mTickVec[0]->GetCenter();
	else if (value > mRange.GetY())
		return mTickVec[mTickVec.size() - 1]->GetCenter();

	/*Calculate the position along this axis*/
	float normalizedWeight = MyAbs((float)(value - mRange.GetX()) / mRange.GetY());
	Vector2I retVal = mTickVec[0]->GetCenter();
	retVal.SetX(retVal.GetX() + (int)((mTickVec[mTickVec.size() - 1]->GetCenter().GetX() - mTickVec[0]->GetCenter().GetX()) * normalizedWeight));
	retVal.SetY(retVal.GetY() + (int)((mTickVec[mTickVec.size() - 1]->GetCenter().GetY() - mTickVec[0]->GetCenter().GetY()) * normalizedWeight));

	return retVal;
}

void Axis::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0, 1.0, 1.0); /*Set text color*/

	Vector2F rasterPos;
	std::string str;
	/*Draw axis name text*/
	str = mLabelName;
	glRasterPos2f(mLabelPos.GetX(), mLabelPos.GetY());
	for (unsigned int i = 0; i < mLabelName.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw label text for some tick marks*/
	unsigned int numTickLabels = ((mRange.GetY() - mRange.GetX()) > 40) ? 40 : mRange.GetY() - mRange.GetX();
	for (unsigned int i = 0; i <= numTickLabels; i++)
	{
		str = UIntToString(mRange.GetX() + (unsigned int)(i * (mRange.GetY() - mRange.GetX()) / numTickLabels));
		int offset = -40;
		Vector2I tickLabelPos = (mAxis.GetDimensions().GetY() >= mAxis.GetDimensions().GetX()) ?
								 Vector2I(mTickVec[0]->GetCenter().GetX() + offset, GetPosition(mRange.GetX() + (unsigned int)(i * (mRange.GetY() - mRange.GetX()) / numTickLabels)).GetY()) :
								 Vector2I(GetPosition(mRange.GetX() + (unsigned int)(i * (mRange.GetY() - mRange.GetX()) / numTickLabels)).GetX(), mTickVec[0]->GetCenter().GetY() + offset);
		rasterPos = GetRasterPosition(tickLabelPos, str.size());
		glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
		for (unsigned int i = 0; i < str.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}

	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/


	/*Draw axis*/
	mAxis.Draw();

	/*Draw tick marks*/
	for (unsigned int i = 0; i < mTickVec.size(); i++)
		mTickVec[i]->Draw();
}


/*
* Mutators
*/
/*Also sets tick mark positions in mTickVec*/
void Axis::SetAxis(const MyRectangle &newAxis)
{
	mAxis.Set(newAxis);
	SetTickMarks();
}

void Axis::SetNumTickMarks(unsigned int newNumTickMarks)
{
	mNumTickMarks = newNumTickMarks;
	SetTickMarks();
}

void Axis::SetTickMarks()
{
	/*Clear old tick marks*/
	mTickVec.clear();

	for (unsigned int i = 0; i < mNumTickMarks; i++)
	{
		/*Create a new MyRectangle object*/
		MyRectangle *newTickMark = new MyRectangle(Vector2I(0, 0), Vector2I(0, 0), Color3(1.0f, 1.0f, 1.0f));

		/*If axis is vertical, tick marks are horizontal*/
		if (mAxis.GetDimensions().GetY() >= mAxis.GetDimensions().GetX())
			newTickMark->SetDimensions(11, 1);

		/*Otherwise, tick marks are vertical*/
		else
			newTickMark->SetDimensions(1, 11);

		/*Set the tick mark position*/
		Vector2I pos = (mAxis.GetDimensions().GetY() >= mAxis.GetDimensions().GetX()) ? Vector2I(mAxis.GetTopLeft().GetX(), mAxis.GetTopLeft().GetY() - mAxis.GetDimensions().GetY()) : Vector2I(mAxis.GetTopLeft().GetX(), mAxis.GetTopLeft().GetY() - mAxis.GetDimensions().GetY() / 2);
		if (mAxis.GetDimensions().GetY() >= mAxis.GetDimensions().GetX())
			pos.SetY(pos.GetY() + (int)(((float)mAxis.GetDimensions().GetY() / (int)(mNumTickMarks - 1)) * (int)i));
		else
			pos.SetX(pos.GetX() + (int)(((float)mAxis.GetDimensions().GetX() / (int)(mNumTickMarks - 1)) * (int)i));
		
		newTickMark->SetCenter(pos);

		mTickVec.push_back(newTickMark);
	}
}

void Axis::SetLabelName(const std::string &newLabelName)
{
	mLabelName = newLabelName;
}

void Axis::SetLabelPosition(const Vector2F &newLabelPos)
{
	mLabelPos.SetX(newLabelPos.GetX());
	mLabelPos.SetY(newLabelPos.GetY());
}

void Axis::SetRange(const Vector2I &newRange)
{
	mRange.SetX(newRange.GetX());
	mRange.SetY(newRange.GetY());
}


/*
* Overloaded operators
*/
const Vector2I& Axis::operator[](int index) const
{
	if (index < 0 || index >= (int)mTickVec.size())
		return mTickVec[0]->GetCenter(); /*If index is out of bounds, return the first element by default.*/
	return mTickVec[index]->GetCenter();
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Axis
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Legend::Legend()
{
}

/*
* Accessors
*/
unsigned int Legend::GetNumItems() const
{
	return mNameVec.size();
}

void Legend::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0f, 1.0f, 1.0f); /*Set text color*/
	Vector2F rasterPos;
	std::string str;
	/*Draw legend text*/
	for (unsigned int legendItem = 0; legendItem < GetNumItems(); legendItem++)
	{
		str = mNameVec[legendItem];

		Vector2F rasterPos = GetRasterPosition(Vector2I(mRectVec[legendItem].GetTopLeft().GetX() + mRectVec[legendItem].GetDimensions().GetX() + 5, mRectVec[legendItem].GetTopLeft().GetY() - mRectVec[legendItem].GetDimensions().GetY() / 2), mNameVec[legendItem].size());
		glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
		for (unsigned int i = 0; i < mNameVec[legendItem].size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}
	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/


	/*Draw rects*/
	for (unsigned int legendItem = 0; legendItem < GetNumItems(); legendItem++)
		mRectVec[legendItem].Draw();
}

/*
* Mutators
*/
void Legend::Insert(const std::string &newItemName, const MyRectangle &newItemRect)
{
	mNameVec.push_back(newItemName);
	mRectVec.push_back(newItemRect);
}

/*
* Overloaded operators
*/
const std::pair<std::string, MyRectangle>& Legend::operator[](int index) const
{
	std::pair<std::string, MyRectangle> *retVal = new std::pair<std::string, MyRectangle>;

	if (index < 0 || index >= (int)mNameVec.size())
	{
		retVal->first = mNameVec[0];
		retVal->second = mRectVec[0];
		return *retVal; /*If index is out of bounds, return the first element by default.*/
	}

	retVal->first = mNameVec[index];
	retVal->second = mRectVec[index];
	return *retVal;
}





/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Axis
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Graph::Graph()
{
	mName = "";
}

/*
* Accessors
*/
Axis Graph::GetVerticalAxis() const
{
	return mVerticalAxis;
}

Axis Graph::GetHorizontalAxis() const
{
	return mHorizontalAxis;
}

std::string Graph::GetGraphName() const
{
	return mName;
}

Vector2F Graph::GetGraphNamePosition() const
{
	return mNamePos;
}

Legend Graph::GetLegend() const
{
	return mLegend;
}

void Graph::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0f, 1.0f, 1.0f); /*Set text color*/

	Vector2F rasterPos;
	std::string str;
	/*Draw graph name text*/
	str = mName;
	glRasterPos2f(mNamePos.GetX(), mNamePos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	glutPostRedisplay();
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/

	/*Draw other fixed graph features*/
	mVerticalAxis.Draw();
	mHorizontalAxis.Draw();
	mLegend.Draw();

	/*Draw plot data*/
	for (unsigned int set = 0; set < mDataSetVec.size(); set++)
	{
		for (unsigned int point = 0; point < mDataSetVec[set].size(); point++)
		{
			MyRectangle plotPoint;
			plotPoint.SetDimensions(5, 5);
			plotPoint.SetCenter(mDataSetVec[set][point]);
			plotPoint.SetColor(mLegend[set].second.GetColor());
			plotPoint.Draw();
		}
	}
}


/*
* Mutators
*/
void Graph::SetVerticalAxis(const MyRectangle &newAxis, unsigned int newNumTickMarks, const std::string &newLabelName, const Vector2F &newLabelPos, const Vector2I &newRange)
{
	mVerticalAxis.SetAxis(newAxis);
	mVerticalAxis.SetNumTickMarks(newNumTickMarks);
	mVerticalAxis.SetLabelName(newLabelName);
	mVerticalAxis.SetLabelPosition(newLabelPos);
	mVerticalAxis.SetRange(newRange);
}

void Graph::SetVerticalAxis(const Axis &newAxis)
{
	SetVerticalAxis(newAxis.GetAxis(), newAxis.GetNumTickMarks(), newAxis.GetLabelName(), newAxis.GetLabelPosition(), newAxis.GetRange());
}

void Graph::SetHorizontalAxis(const MyRectangle &newAxis, unsigned int newNumTickMarks, const std::string &newLabelName, const Vector2F &newLabelPos, const Vector2I &newRange)
{
	mHorizontalAxis.SetAxis(newAxis);
	mHorizontalAxis.SetNumTickMarks(newNumTickMarks);
	mHorizontalAxis.SetLabelName(newLabelName);
	mHorizontalAxis.SetLabelPosition(newLabelPos);
	mHorizontalAxis.SetRange(newRange);
}

void Graph::SetHorizontalAxis(const Axis &newAxis)
{
	SetHorizontalAxis(newAxis.GetAxis(), newAxis.GetNumTickMarks(), newAxis.GetLabelName(), newAxis.GetLabelPosition(), newAxis.GetRange());
}

void Graph::SetName(const std::string &newGraphName)
{
	mName = newGraphName;
}

void Graph::SetNamePosition(const Vector2F &newNamePosition)
{
	mNamePos.SetX(newNamePosition.GetX());
	mNamePos.SetY(newNamePosition.GetY());
}

void Graph::InsertLegendItem(const std::pair<std::string, MyRectangle> &newLegendItem)
{
	mLegend.Insert(newLegendItem.first, newLegendItem.second);
}

void Graph::SetLegend(const Legend &newLegend)
{
	for (unsigned int i = 0; i < newLegend.GetNumItems(); i++)
		mLegend.Insert(newLegend[i].first, newLegend[i].second);
}

void Graph::InsertData(unsigned int set, int x, int y)
{
	if (mDataSetVec.size() <= set)
	{
		unsigned int numSetsToInsert = set - mDataSetVec.size() + 1;
		for (unsigned int i = 0; i < numSetsToInsert; i++)
		{
			std::vector<Vector2I> *temp = new std::vector<Vector2I>;
			mDataSetVec.push_back(*temp);
		}
	}

	int plotX = mHorizontalAxis.GetPosition(x).GetX();
	int plotY = mVerticalAxis.GetPosition(y).GetY();
	mDataSetVec[set].push_back(Vector2I(plotX, plotY));
}



/*
* Global functions
*/
