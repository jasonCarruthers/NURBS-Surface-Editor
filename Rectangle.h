#ifndef HFILE_RECTANGLE
#define HFILE_RECTANGLE

#include "General.h"



/*
 * Classes
 */
class MyRectangle
{
public:
	/*
	 * Constructors
	 */
	MyRectangle();
	MyRectangle(int topLeftX, int topLeftY, int width, int height, float r, float g, float b);
	MyRectangle(const Vector2I &newTopLeft, const Vector2I &newDimensions, const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	 * Accessors
	 */
	Vector2I GetTopLeft() const;
	Vector2I GetCenter() const;
	Vector2I GetDimensions() const;
	int GetWidth() const;
	int GetHeight() const;
	Color3 GetColor() const;
	bool ContainsPoint(int x, int y) const; /*Assumes origin at bottom left corner of window*/
	bool ContainsRect(const MyRectangle &other) const; /*Assumes origin at bottom left corner of window*/
	void Draw() const;
	void Draw(const Color3 &colorOverride) const;
	void DrawOutline() const;
	void DrawOutline(const Color3 &colorOverride) const;

	/*
	 * Mutators
	 */
	void Set(const MyRectangle &newRectangle);
	void SetTopLeft(const Vector2I &newTopLeft);
	void SetCenter(const Vector2I &newCenter);
	void SetCenter(int newX, int newY);
	void SetTopLeft(int newTopLeftX, int newTopLeftY);
	void SetDimensions(const Vector2I &newDimensions);
	void SetDimensions(int newWidth, int newHeight);
	void SetColor(const Color3 &newColor);
	void SetColor(float newR, float newG, float newB);

	/*
	 * Overloaded operators
	 */
	friend MyRectangle* operator-(const MyRectangle &left, const MyRectangle &right);

private:
	Vector2I topLeft; /*top left coordinate*/
	Vector2I dimensions; /*width, height*/
	Color3 color;
};







#endif /*HFILE_RECTANGLE*/