#ifndef HFILE_GRAPH
#define HFILE_GRAPH


#include <string>
#include "Rectangle.h"


class Axis
{
public:
	/*
	* Constructor(s)
	*/
	Axis();

	/*
	* Accessors
	*/
	MyRectangle GetAxis() const;
	unsigned int GetNumTickMarks() const;
	std::string GetLabelName() const;
	Vector2F GetLabelPosition() const;
	Vector2I GetRange() const;
	Vector2I GetPosition(int value) const;
	void Draw() const;

	/*
	* Mutators
	*/
	void SetAxis(const MyRectangle &newAxis); /*Also sets tick mark positions in mTickVec*/
	void SetNumTickMarks(unsigned int newNumTickMarks);
private:
	void SetTickMarks();
public:
	void SetLabelName(const std::string &newLabelName);
	void SetLabelPosition(const Vector2F &newLabelPos);
	void SetRange(const Vector2I &newRange);

	/*
	* Overloaded operators
	*/
	const Vector2I& operator[](int index) const;

private:
	MyRectangle mAxis;
	std::vector<MyRectangle*> mTickVec;
	unsigned int mNumTickMarks;
	std::string mLabelName;
	Vector2F mLabelPos; /*In raster coordinates*/
	Vector2I mRange; /*First tick mark starts at mRange.GetX() and last tick mark ends at
					   mRange.GetY(), with the rest of the tick marks spanning the range.*/
};


class Legend
{
public:
	/*
	* Constructor(s)
	*/
	Legend();

	/*
	* Accessors
	*/
	unsigned int GetNumItems() const;
	void Draw() const;

	/*
	* Mutators
	*/
	void Insert(const std::string &newItemName, const MyRectangle &newItemRect);

	/*
	* Overloaded operators
	*/
	const std::pair<std::string, MyRectangle>& operator[](int index) const;

private:
	std::vector<std::string> mNameVec;
	std::vector<MyRectangle> mRectVec;
};


class Graph
{
public:
	/*
	* Constructor(s)
	*/
	Graph();

	/*
	* Accessors
	*/
	Axis GetVerticalAxis() const;
	Axis GetHorizontalAxis() const;
	std::string GetGraphName() const;
	Vector2F GetGraphNamePosition() const;
	Legend GetLegend() const;
	void Draw() const;

	/*
	* Mutators
	*/
	void SetVerticalAxis(const MyRectangle &newAxis, unsigned int newNumTickMarks, const std::string &newLabelName, const Vector2F &newLabelPos, const Vector2I &newRange);
	void SetVerticalAxis(const Axis &newAxis);
	void SetHorizontalAxis(const MyRectangle &newAxis, unsigned int newNumTickMarks, const std::string &newLabelName, const Vector2F &newLabelPos, const Vector2I &newRange);
	void SetHorizontalAxis(const Axis &newAxis);
	void SetName(const std::string &newGraphName);
	void SetNamePosition(const Vector2F &newNamePosition);
	void InsertLegendItem(const std::pair<std::string, MyRectangle> &newLegendItem);
	void SetLegend(const Legend &newLegend);
	void InsertData(unsigned int set, int x, int y);

private:
	Axis mVerticalAxis;
	Axis mHorizontalAxis;
	std::string mName;
	Vector2F mNamePos; /*In raster coordinates*/
	Legend mLegend;
	std::vector<std::vector<Vector2I>> mDataSetVec; /*
													 * A vector of vectors of data plot points. This
													 * way mutliple sets of data can be contained in
													 * a graph and the plot points of different
													 * sets of data can be drawn in different colors.
													 * 
													 * NOTE: The number of vectors within the outter
													 *		 vector container should be equal to the
													 *		 number of name-rect pairs in mLegend.
													 */
};




/*
* Global function prototypes
*/


/*
* Global variables
*/



#endif /*HFILE_GRAPH*/