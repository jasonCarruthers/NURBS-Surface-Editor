#include "General.h"
#include "MyMath.h"
#include "Circle.h"





/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Circle
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Circle::Circle(const Vector2I &newCenter, int newRadius, const Color3 &newColor)
{
	mColor.Set(newColor);
	mCenter.SetX(newCenter.GetX());
	mCenter.SetY(newCenter.GetY());
	mRadius = newRadius;

	CalculatePlotVec();
}

/*
* Accessors
*/
Vector2I Circle::GetCenter() const
{
	return mCenter;
}

int Circle::GetRadius() const
{
	return mRadius;
}

Color3 Circle::GetColor() const
{
	return mColor;
}

bool Circle::ContainsPoint(const Vector2I &point) const
{
	return ((int)MyAbs((float)(mCenter.GetX() - point.GetX())) <= mRadius && (int)MyAbs((float)(mCenter.GetY() - point.GetY())) <= mRadius);
}

void Circle::Draw() const
{
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), mColor);
}

/*
* Mutators
*/
void Circle::SetCenter(const Vector2I &newCenter)
{
	/*Adjust positions of points in mPlotVec*/
	if (mPlotVec.size() > 0)
	{
		Vector2I deltaPos = Vector2I(newCenter.GetX() - mCenter.GetX(), newCenter.GetY() - mCenter.GetY());

		for (unsigned int i = 0; i < mPlotVec.size(); i++)
		{
			mPlotVec[i]->SetX(mPlotVec[i]->GetX() + deltaPos.GetX());
			mPlotVec[i]->SetY(mPlotVec[i]->GetY() + deltaPos.GetY());
		}
	}

	/*Set new center.*/
	mCenter.SetX(newCenter.GetX());
	mCenter.SetY(newCenter.GetY());
}

void Circle::SetRadius(int newRadius)
{
	mRadius = newRadius;
	CalculatePlotVec();
}

void Circle::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
}

void Circle::CalculatePlotVec()
{
	/*Clear any points that might have been in mPlotVec*/
	mPlotVec.clear();

	/*
	* Calculate all the points in mPlotVec.
	* This is the Midpoint Circle Algorithm from "Computer Graphics with OpenGL, 4th Ed.", pg. 150.
	*/
	/*Step 1: Let the first point be the topmost point on the circle. Also calculate the
			  bottommost point and the leftmost and rightmost points.*/
	/*Topmost point*/
	Vector2I *newPoint = new Vector2I(mCenter.GetX(), mCenter.GetY() + mRadius);
	mPlotVec.push_back(newPoint);
	/*Bottommost point*/
	newPoint = new Vector2I(mCenter.GetX(), mCenter.GetY() - mRadius);
	mPlotVec.push_back(newPoint);
	/*Leftmost point*/
	newPoint = new Vector2I(mCenter.GetX() - mRadius, mCenter.GetY());
	mPlotVec.push_back(newPoint);
	/*Rightmost point*/
	newPoint = new Vector2I(mCenter.GetX() + mRadius, mCenter.GetY());
	mPlotVec.push_back(newPoint);

	/*Step 2: Let the initial value of the decision parameter, P0, be as follows.*/
	float Pk = (5.0f / 4.0f) - mRadius;

	/*Step 3: At each Xk position, starting at k = 0, perform the following test...*/
	Vector2I newCoord = Vector2I(0, mRadius);
	do
	{
		/*If Pk < 0, the next point along the circle centered on (0, 0) is (X(k+1), Yk) and
		P(k+1) = Pk + (2*Xk + 2) + 1.*/
		if (Pk < 0)
		{
			newCoord = Vector2I(newCoord.GetX() + 1, newCoord.GetY());
			Pk = Pk + (2 * newCoord.GetX() + 2) + 1;
		}

		/*Otherwise, the next point along the circle is (Xk + 1, Yk - 1) and
		P(k+1) = Pk + (2*Xk + 2) + 1 - (2*Yk - 2).*/
		else
		{
			newCoord = Vector2I(newCoord.GetX() + 1, newCoord.GetY() - 1);
			Pk = Pk + 2 * newCoord.GetX() + 2 + 1 - (2 * newCoord.GetY() - 2);
		}

		/*Step 4: Determine symmetry points in the other seven octants.
		Step 5: Move each calculated pixel position (X, Y) onto the circular path
		centerd at (Xc, Yc) and plot the coordinate values as follows:
		X = X + Xc, Y = Y + Yc.*/
		/*The following octants assume octant 1 is the "bottom" octant in quadrant 1
		and that the octants increase around the circle in a counter-clockwise fashion.*/
		/*Octant 1*/
		newPoint = new Vector2I(mCenter.GetX() + newCoord.GetX(), mCenter.GetY() + newCoord.GetY());
		mPlotVec.push_back(newPoint);
		/*Octant 2*/
		newPoint = new Vector2I(mCenter.GetX() + newCoord.GetY(), mCenter.GetY() + newCoord.GetX());
		mPlotVec.push_back(newPoint);
		/*Octant 3*/
		newPoint = new Vector2I(mCenter.GetX() - newCoord.GetX(), mCenter.GetY() + newCoord.GetY());
		mPlotVec.push_back(newPoint);
		/*Octant 4*/
		newPoint = new Vector2I(mCenter.GetX() - newCoord.GetY(), mCenter.GetY() + newCoord.GetX());
		mPlotVec.push_back(newPoint);
		/*Octant 5*/
		newPoint = new Vector2I(mCenter.GetX() - newCoord.GetY(), mCenter.GetY() - newCoord.GetX());
		mPlotVec.push_back(newPoint);
		/*Octant 6*/
		newPoint = new Vector2I(mCenter.GetX() - newCoord.GetX(), mCenter.GetY() - newCoord.GetY());
		mPlotVec.push_back(newPoint);
		/*Octant 7*/
		newPoint = new Vector2I(mCenter.GetX() + newCoord.GetX(), mCenter.GetY() - newCoord.GetY());
		mPlotVec.push_back(newPoint);
		/*Octant 8*/
		newPoint = new Vector2I(mCenter.GetX() + newCoord.GetY(), mCenter.GetY() - newCoord.GetX());
		mPlotVec.push_back(newPoint);

		/*Step 6: Repeat steps 3 through 5 until X >= Y.*/
	} while (newCoord.GetX() < newCoord.GetY());
}



/*
* Global functions
*/
void TestCircleDrawing()
{
	Circle testCircle = Circle(Vector2I(800, 300), 250, Color3(0.0f, 1.0f, 0.0f));
	testCircle.Draw();

	const unsigned int NUM_CIRCLES_TO_TEST = 100;
	for (unsigned int i = 0; i < NUM_CIRCLES_TO_TEST; i++)
	{
		testCircle = Circle(Vector2I(600 + (rand() % 401), 200 + (rand() % 201)), 20 + (rand() % 181), GetRandomColor().GetColor3());
		testCircle.Draw();
	}
}