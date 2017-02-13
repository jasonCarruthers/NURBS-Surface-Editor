#include <iostream>
#include "Rectangle.h"




/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class MyRectangle
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
MyRectangle::MyRectangle()
{
	topLeft = Vector2I(0, 0);
	dimensions = Vector2I(100, 100);
	color = Color3(0.0f, 0.0f, 1.0f);
}

MyRectangle::MyRectangle(int topLeftX, int topLeftY, int width, int height, float r, float g, float b) :
	MyRectangle::MyRectangle(Vector2I(topLeftX, topLeftY), Vector2I(width, height), Color3(r, g, b))
{
}

MyRectangle::MyRectangle(const Vector2I &newTopLeft, const Vector2I &newDimensions, const Color3 &newColor)
{
	topLeft.SetX(newTopLeft.GetX());
	topLeft.SetY(newTopLeft.GetY());
	dimensions.SetX(newDimensions.GetX());
	dimensions.SetY(newDimensions.GetY());
	color.Set(newColor.GetR(), newColor.GetG(), newColor.GetB());
}

/*
* Accessors
*/
Vector2I MyRectangle::GetTopLeft() const
{
	return topLeft;
}

Vector2I MyRectangle::GetCenter() const
{
	return Vector2I(topLeft.GetX() + dimensions.GetX() / 2, topLeft.GetY() - dimensions.GetY() / 2);
}

Vector2I MyRectangle::GetDimensions() const
{
	return dimensions;
}

int MyRectangle::GetWidth() const
{
	return dimensions.GetX();
}

int MyRectangle::GetHeight() const
{
	return dimensions.GetY();
}

Color3 MyRectangle::GetColor() const
{
	return color;
}

/*Assumes origin at bottom left corner of window*/
bool MyRectangle::ContainsPoint(int x, int y) const
{
	return (topLeft.GetX() <= x && topLeft.GetX() + dimensions.GetX() >= x &&
		topLeft.GetY() - dimensions.GetY() <= y && topLeft.GetY() >= y);
}

/*Assumes origin at bottom left corner of window. Returns true if any points within either rect
  are within the other rect.*/
bool MyRectangle::ContainsRect(const MyRectangle &other) const
{
	for (int dx = 0; dx <= other.dimensions.GetX(); dx++)
		for (int dy = 0; dy <= other.dimensions.GetY(); dy++)
			if (ContainsPoint(other.GetTopLeft().GetX() + dx, other.GetTopLeft().GetY() - dy))
				return true;

	return false;
}

void MyRectangle::Draw() const
{
	for (int y = 0; y < dimensions.GetY(); y++)
	{
		for (int x = 0; x < dimensions.GetX(); x++)
		{
			SetPixel(topLeft.GetX() + x, topLeft.GetY() - y, color);
		}
	}
}

void MyRectangle::Draw(const Color3 &colorOverride) const
{
	for (int y = 0; y < dimensions.GetY(); y++)
	{
		for (int x = 0; x < dimensions.GetX(); x++)
		{
			SetPixel(topLeft.GetX() + x, topLeft.GetY() - y, colorOverride);
		}
	}
}

void MyRectangle::DrawOutline() const
{
	for (int x = 0; x < dimensions.GetX(); x++)
	{
		SetPixel(topLeft.GetX() + x, topLeft.GetY(), color); /*Top edge*/
		SetPixel(topLeft.GetX() + x, topLeft.GetY() - dimensions.GetY(), color); /*Bottom edge*/
	}

	for (int y = 0; y < dimensions.GetY(); y++)
	{
		SetPixel(topLeft.GetX(), topLeft.GetY() - y, color); /*Left edge*/
		SetPixel(topLeft.GetX() + dimensions.GetX(), topLeft.GetY() - y, color); /*Right edge*/
	}
}

void MyRectangle::DrawOutline(const Color3 &colorOverride) const
{
	for (int x = 0; x < dimensions.GetX(); x++)
	{
		SetPixel(topLeft.GetX() + x, topLeft.GetY(), colorOverride); /*Top edge*/
		SetPixel(topLeft.GetX() + x, topLeft.GetY() - dimensions.GetY(), colorOverride); /*Bottom edge*/
	}

	for (int y = 0; y < dimensions.GetY(); y++)
	{
		SetPixel(topLeft.GetX(), topLeft.GetY() - y, colorOverride); /*Left edge*/
		SetPixel(topLeft.GetX() + dimensions.GetX(), topLeft.GetY() - y, colorOverride); /*Right edge*/
	}
}

/*
* Mutators
*/
void MyRectangle::Set(const MyRectangle &newRectangle)
{
	SetTopLeft(newRectangle.topLeft);
	SetDimensions(newRectangle.dimensions);
	SetColor(newRectangle.color);
}

void MyRectangle::SetTopLeft(const Vector2I &newTopLeft)
{
	topLeft.SetX(newTopLeft.GetX());
	topLeft.SetY(newTopLeft.GetY());
}

void MyRectangle::SetCenter(const Vector2I &newCenter)
{
	topLeft.SetX(newCenter.GetX() - dimensions.GetX() / 2);
	topLeft.SetY(newCenter.GetY() + dimensions.GetY() / 2);
}

void MyRectangle::SetCenter(int newX, int newY)
{
	SetCenter(Vector2I(newX, newY));
}

void MyRectangle::SetTopLeft(int newTopLeftX, int newTopLeftY)
{
	topLeft.SetX(newTopLeftX);
	topLeft.SetY(newTopLeftY);
}

void MyRectangle::SetDimensions(const Vector2I &newDimensions)
{
	dimensions.SetX(newDimensions.GetX());
	dimensions.SetY(newDimensions.GetY());
}

void MyRectangle::SetDimensions(int newWidth, int newHeight)
{
	dimensions.SetX(newWidth);
	dimensions.SetY(newHeight);
}

void MyRectangle::SetColor(const Color3 &newColor)
{
	color.Set(newColor.GetR(), newColor.GetG(), newColor.GetB());
}

void MyRectangle::SetColor(float newR, float newG, float newB)
{
	color.Set(newR, newG, newB);
}

/*
 * Overloaded operators
 */
/*Returns the set intersection (as a MyRectangle object) of left and right. If
  left and right don't intersect, return null.*/
MyRectangle* operator-(const MyRectangle &left, const MyRectangle &right)
{
	/*If no intersection, return null*/
	if (!left.ContainsRect(right))
		return NULL;
	else
		return new MyRectangle();
}