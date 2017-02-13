#ifndef HFILE_CIRCLE
#define HFILE_CIRCLE


#include "General.h"
#include <vector>


class Circle
{
public:
	/*
	* Constructor(s)
	*/
	Circle(const Vector2I &newCenter = Vector2I(10, 10), int newRadius = 5, const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
	Vector2I GetCenter() const;
	int GetRadius() const;
	Color3 GetColor() const;
	bool ContainsPoint(const Vector2I &point) const;
	void Draw() const;

	/*
	* Mutators
	*/
	void SetCenter(const Vector2I &newCenter);
	void SetRadius(int newRadius);
	void SetColor(const Color3 &newColor);
private:
	void CalculatePlotVec();
public:

private:
	Vector2I mCenter;
	int mRadius;
	Color3 mColor;
	std::vector<Vector2I*> mPlotVec;
};


/*
* Global function prototypes
*/
void TestCircleDrawing();



#endif /*HFILE_CIRCLE*/