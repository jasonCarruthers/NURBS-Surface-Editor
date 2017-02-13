#ifndef HFILE_LINE
#define HFILE_LINE

#include "General.h"
#include "MyMath.h"
#include <iostream>
#include <vector>


/*
 * Function prototypes
 */
void SetPixel(int x, int y, const Color3 &color);
Color4 GetRandomColor();

/*
 * Classes
 */
/*Rename to LineSegment???*/
class Line
{
public:
	/*
	 * Constructors
	 */
	Line()
	{
		//By default, a new line is blue and spans the diagonal between the bottom left and top right
		//of the window.
		p1.SetX(0);
		p1.SetY(0);
		p2.SetX(WINDOW_WIDTH - 1);
		p2.SetY(WINDOW_HEIGHT - 1);
		color.Set(0.0f, 0.0f, 1.0f);
		SetPixels();
	}
	Line(const Vector2I &coord1, const Vector2I &coord2, const Color3 &newColor)
	{
		p1.SetX(coord1.GetX());
		p1.SetY(coord1.GetY());
		p2.SetX(coord2.GetX());
		p2.SetY(coord2.GetY());
		color.Set(newColor.GetR(), newColor.GetG(), newColor.GetB());
		SetPixels();
	}

	/*
	 * Accessors
	 */
private:
	bool IsLegal(const Vector2I &leftCoord, const Vector2I &rightCoord) const
	{
		return IsLegal(leftCoord.GetX(), leftCoord.GetY(), rightCoord.GetX(), rightCoord.GetY());
	}
	bool IsLegal(int leftX, int leftY, int rightX, int rightY) const
	{
		return !(leftX < 0 || (unsigned int)leftX >= WINDOW_WIDTH ||
			rightX < 0 || (unsigned int)rightX >= WINDOW_WIDTH ||
			leftY < 0 || (unsigned int)leftY >= WINDOW_HEIGHT ||
			rightY < 0 || (unsigned int)rightY >= WINDOW_HEIGHT);
	}
public:
	std::vector<Vector2I> GetPixelVector() const
	{
		return pixelVec;
	}
	Vector2I GetP1() const
	{
		return p1;
	}
	Vector2I GetP2() const
	{
		return p2;
	}
	void Draw() const
	{
		for (unsigned int i = 0; i < pixelVec.size(); i++)
			SetPixel(pixelVec[i].GetX(), pixelVec[i].GetY(), color);
	}
	void Draw(const Color3 &overrideColor) const
	{
		for (unsigned int i = 0; i < pixelVec.size(); i++)
			SetPixel(pixelVec[i].GetX(), pixelVec[i].GetY(), overrideColor);
	}


	/*
	 * Mutators
	 */
	void SetCoordinates(const Vector2I &newP1, const Vector2I &newP2)
	{
		if (!IsLegal(newP1, newP2))
		{
			std::cout << "Error (line.h): Line coord(s) out of bounds. Line coords not set!" << "\n";
			return;
		}

		p1.SetX(newP1.GetX());
		p1.SetY(newP1.GetY());
		p2.SetX(newP2.GetX());
		p2.SetY(newP2.GetY());

		pixelVec.clear();
		SetPixels();
	}
	void SwapCoordinates()
	{
		Vector2I temp = p1;
		p1 = p2;
		p2 = temp;
	}
	void SetColor(const Color3 &newColor)
	{
		color.Set(newColor.GetR(), newColor.GetG(), newColor.GetB());
	}
	//Bresenham line-drawing function
private:
	void SetPixels()
	{
		//If the two points are the same, set one point and exit the function
		if (p1 == p2)
		{
			pixelVec.push_back(p1);
			return;
		}

		//Init variables
		int dx = (int)MyAbs((float)(p2.GetX() - p1.GetX()));
		int dy = (int)MyAbs((float)(p2.GetY() - p1.GetY()));
		int twoDy = 2 * dy;
		int twoDyMinusTwoDx = 2 * (dy - dx);
		int twoDx = 2 * dx;
		int twoDxMinusTwoDy = 2 * (dx - dy);
		int p = 2 * dy - dx;
		int x = p1.GetX();
		int y = p1.GetY();
		bool slopeAbsIsLessThanOne = (dy <= dx);
		bool slopeIsPos = !(((p2.GetX() - p1.GetX()) < 0) ^ ((p2.GetY() - p1.GetY()) < 0)); //^ operator is XOR

		//Determine which endpoint to use as start position
		if (p1.GetX() > p2.GetX())
		{
			x = p2.GetX();
			y = p2.GetY();
		}

		//Plot the first point
		pixelVec.push_back(Vector2I(x, y));

		//Check for vertical or horizontal line, and also check for line with |m| = 1.
		//If any of the three exist, exit function after setting pixels.
		if (dx == 0 && dy > 0) //Vertical line
		{
			y = (p1.GetY() < p2.GetY()) ? p1.GetY() : p2.GetY();
			for (int i = 0; i < dy; i++)
			{
				pixelVec.push_back(Vector2I(x, y));
				y++;
			}
			return;
		}
		else if (dx > 0 && dy == 0) //Horizontal line
		{
			for (int i = 0; i < dx; i++)
			{
				pixelVec.push_back(Vector2I(x, y));
				x++;
			}
			return;
		}
		else if (dx == dy) //Line with |m| = 1
		{
			for (int i = 0; i < dx; i++)
			{
				pixelVec.push_back(Vector2I(x, y));
				x++;
				y += (slopeIsPos) ? 1 : -1;
			}
			return;
		}


		//In all other cases, use the Bresenham algorithm to set pixels.
		if (slopeAbsIsLessThanOne) //Cases where |m| < 1
		{
			for (int i = 0; i < dx; i++)
			{
				x++;
				if (p < 0)
					p += twoDy;
				else
				{
					y += (slopeIsPos) ? 1 : -1;
					p += twoDyMinusTwoDx;
				}
				pixelVec.push_back(Vector2I(x, y));
			}
		}
		else //if( !slopeAbsIsLessThanOne ) //Cases where |m| > 1
		{
			p = 2 * dx - dy;

			for (int i = 0; i < dy; i++)
			{
				y += (slopeIsPos) ? 1 : -1;
				if (p < 0)
					p += twoDx;
				else
				{
					x++;
					p += twoDxMinusTwoDy;
				}
				pixelVec.push_back(Vector2I(x, y));
			}
		}
	}

private:
	Vector2I p1;
	Vector2I p2;
	Color3 color;
	std::vector<Vector2I> pixelVec;
};



class Ray3D
{
public:
	/*
	* Constructor(s)
	*/
	Ray3D(const Vector3F &newOrigin = Vector3F(0, 0, 0), const Vector3F &newDirection = Vector3F(1, 0, 0));

	/*
	* Accessors
	*/
	Vector3F GetOrigin() const;
	Vector3F GetDirection() const;

	/*
	* Mutators
	*/
	void SetOrigin(const Vector3F &newOrigin);
	void SetDirection(const Vector3F &newDirection);
	void NormalizeDirection();

private:
	Vector3F mOrigin;
	Vector3F mDirection;
};




#endif /*HFILE_LINE*/