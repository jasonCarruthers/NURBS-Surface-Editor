#ifndef HFILE_CURVES
#define HFILE_CURVES


#include <vector>
#include "General.h"
#include "Line.h"
#include "Rectangle.h"
#include "Matrix.h"




class ControlPoints
{
public:
	/*
	 * Constructor(s)
	 */
	ControlPoints(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	 * Accessors
	 */
	std::vector<Vector2I*> *GetControlPointVec();
	Color3 GetColor() const;
	MyRectangle GetMBB() const;
	void Draw() const;
	void DrawControlPolygon() const; /*Connect all adjacent control points*/
	void DrawConvexHull() const; /*Draw convex hull*/
	void DrawMBB() const; /*Draw Minimum Bounding Box*/
	void Clear() const; /*Erase all control point pixels from pixel buffer*/
	void ClearControlPoint(int index) const; /*Erase the given control point's pixels from pixel buffer.*/
	void ClearControlPolygon() const; /*Erase control polygon pixels from pixel buffer*/
	void ClearConvexHull() const; /*Erase convex hull pixels from pixel buffer*/
	void ClearMBB() const; /*Erase MBB pixels from pixel buffer*/

	/*
	 * Mutators
	 */
	void SetColor(const Color3 &newColor);
	void PushBack(const Vector2I &newControlPoint);
	void EraseAll();
	void Erase(int index);

	/*
	 * Overloaded operators
	 */
	Vector2I &operator[](int index);
	const Vector2I &operator[](int index) const;
	
private:
	std::vector<Vector2I*> mCPVec; /*Control point vec*/
	Color3 mColor;
};


class Curve
{
public:
	enum CurveType
	{
		DeCasteljau,
		Bernstein,
		Aitken,
		DeBoor,
		C1CubicSpline,
		C1QuadraticSpline,
		NURBS_Surface,

		Num__CurveTypes,
	};
public:
	/*
	 * Constructor(s)
	 */
	Curve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));
	Curve(Curve *copy);

	/*
	 * Accessors
	 */
	CurveType GetType() const;
protected:
	virtual Vector2I GetPlotPoint(float t) = 0;
public:
	std::vector<Vector2I*> *GetControlPointVec();
	std::vector<Vector2I*> *GetPlottedPointsVec();
	Color3 GetColor() const;
	bool GetControlPointVisibility() const;
	bool GetControlPolygonVisibility() const;
	bool GetMBBVisibility() const;
	MyRectangle GetMBB();
	virtual void Draw() const;
	virtual void Draw(const Color3 &drawColor) const;
	void DrawIntermediateLines() const;

	/*
	 * Mutators
	 */
	void InsertControlPoint(const Vector2I &newControlPoint); /* Whenever a new control point is inserted,
															  * (re)calculate all the plot points of the
															  * curve*/
	void EraseAllControlPoints();
	virtual void EraseControlPoint(int index);
	virtual void Clear(); /*Clear the points on the curve from the pixelBuffer*/
	void ClearIntermediateLines(); /*Clear the intermediate lines from the pixelBuffer*/
	void ClearMBB(); /*Clear the MBB lines from the pixelBuffer.*/
	void ClearEverything(); /*Clear curve, intermediate lines, control points, control polygon, MBB*/
	virtual void CalcPlotVec() = 0;
	void SetColor(const Color3 &newColor);
	virtual void SetIntermediateLines(float t);
	Curve* Split(float t); /*Splits this curve at t. This curve becomes the left child
						     and the right child is returned.*/
	virtual void RaiseDegree(); /*Increment curve degree by 1.*/
	virtual void ReduceDegree(); /*Decrement curve degree by 1.*/
	void SetControlPointVisibility(bool newVisibility);
	void SetControlPolygonVisibility(bool newVisibility);
	void SetMBBVisibility(bool newVisibility);

protected:
	CurveType TYPE;
	ControlPoints mControlPoints;
	std::vector<Vector2I*> mPlotVec; /*A vec of plotted points on the curve*/
	Color3 mColor;

	bool mControlPointVisible; /*If true, display control points*/
	bool mControlPolygonVisible; /*If true, display control polygon*/
	bool mMBBVisible; /*If true, display MBB as a line rect*/


	/*std::vector<std::vector<Line*>> mGenerationLines;*/ /*All the intermediate lines (drawn between two adjecent children
														  of the same generation) for all points on the curve. This
														  variable keeps track of such groups of
														  lines.*/
	//std::vector<Line*> mIntermediateLines; /*Lines used in the deCasteljau algorithm to plot a point on the curve*/
	std::vector<std::pair<Vector2I, Vector2I>> mIntermediateLines;
};


class BezierBase : public Curve
{
public:
	/*
	* Constructor(s)
	*/
	BezierBase(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));
	BezierBase(Curve *copy);

	/*
	* Accessors
	*/
	/*
	* Mutators
	*/
	void RaiseDegree(); /*Increment curve degree by 1.*/
	void ReduceDegree(); /*Decrement curve degree by 1.*/
protected:
};


/*Bezier curve implemented with the deCasteljau algorithm*/
class DeCasteljauCurve : public BezierBase
{
public:
	/*
	 * Constructor(s)
	 */
	DeCasteljauCurve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));
	DeCasteljauCurve(Curve *copy);

	/*
	 * Accessors
	 */
private:
	Vector2I GetPlotPoint(float t);
public:
	/*
	 * Mutators
	 */
	void CalcPlotVec();
};


/*Bezier curve implemented with the Bernstein algorithm*/
class BernsteinCurve : public BezierBase
{
public:
	/*
	* Constructor(s)
	*/
	BernsteinCurve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));
	BernsteinCurve(Curve *copy);

	/*
	* Accessors
	*/
private:
	Vector2I GetPlotPoint(float t);
public:
	/*
	* Mutators
	*/
	void CalcPlotVec();
};


/*Curve implemented with Aitken's algorithm, where a chord length parametization is used.*/
class AitkenCurve : public Curve
{
public:
	/*
	* Constructor(s)
	*/
	AitkenCurve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
private:
	Vector2I GetPlotPoint(float t);
public:

	/*
	* Mutators
	*/
	void CalcPlotVec();
	void SetIntermediateLines(float t);

private:
	std::vector<float> mParametrizedUpperBound; /*Stores the parametrized upper bound of each
											     paremtrized range, so that stored values
												 increase over the range of 0.0f to 1.0f,
												 inclusive.
												 Example: Values of 0.3333f, 0.6666f, 1.0f
														  would mean there are four control points,
														  three chords, and each chord is a third
														  of the total chord length.*/
};

/*Move all of the GraphicVector2D-related functions and member variables to the Cubic and Quadratic spline
  classes; not all splines need GraphicVector2Ds, but all splines do have a set of user-input control
  points (stored in the the base class Curve) and a set of Bezier points derived from those control
  points and maybe other information; segments of a spline are then constructed from these Bezier points.*/
class SplineBase : public Curve
{
public:
	/*
	* Constructor(s)
	*/
	SplineBase(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));
	
	/*
	* Accessors
	*/
private: /*private ensures the inheriting classes won't have access to this function,
		   so even though it is written as an empty function, it will never be called.
		   Also, since this class is pure abstract, an object of this type can never
		   be created to call GetPlotPoint. In class Curve, GetPlotPoint is pure virtual
		   and so must be overloaded in inheriting classes. While I don't like having the
		   solution be to write an empty function, it's better than any other solution
		   I could think of.*/
	Vector2I GetPlotPoint(float t);
public:
	virtual void Draw() const;
	virtual void Draw(const Color3 &drawColor) const;

	/*
	* Mutators
	*/
	virtual void EraseControlPoint(int index);
	virtual void CalcPlotVec() = 0;
	virtual void Clear(); /*Clear the points on the curve from the pixelBuffer.*/

protected:
	std::vector<BernsteinCurve> mBernsteinCurveVec;
};

/*The same as class SplineBase, but has member variables and functions related to
  GraphicVector2D objects. Curves that inherit from this class should be splines
  that require at least one GraphicVector2D.*/
class VectorSplineBase : public SplineBase
{
public:
	/*
	* Constructor(s)
	*/
	VectorSplineBase(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
	int GetMatchingVectorIndex(const Vector2I &matchHead) const; /*If the head position of any vector in
														   mGraphicVector2D matches matchHead within
														   a certain rectangular tolerance,
														   return the corresponding index. Otherwise,
														   return MAGIC_GARBAGE.*/
	GraphicVector2D &GetMatchingVector(int index);
	void HideVectors() const;
	virtual void Draw() const;
	virtual void Draw(const Color3 &drawColor) const;

	/*
	* Mutators
	*/
	virtual void EraseControlPoint(int index) = 0;
	void EraseGraphicVector2D(int index);
	virtual void CalcPlotVec() = 0;
	void InsertVector(const GraphicVector2D &newVector);
	void MoveVector(int index, const GraphicVector2D &changedVector);
	virtual void Clear(); /*Clear the points on the curve from the pixelBuffer.*/

protected:
	std::vector<GraphicVector2D> mGraphicVector2DVec;
};

class DeBoorSpline : public SplineBase
{
public:
	/*
	* Constructor(s)
	*/
	DeBoorSpline(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
	Vector2I GetPlotPoint(float t);

	/*
	* Mutators
	*/
	void SetOrder(unsigned int newOrder);
	void UpdateKnotVec();
	void CalcPlotVec();
private:
	unsigned int mOrder; /*Has a value of any natural number, though 1 may be excluded in this project.*/
	std::vector<float> mKnotVec;
};

/*Rename to HermiteSpline???*/
class C1CubicSpline : public VectorSplineBase
{
public:
	/*
	* Constructor(s)
	*/
	C1CubicSpline(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/

	/*
	* Mutators
	*/
	void EraseControlPoint(int index);
	void CalcPlotVec();

private:
};

/*The curve is calculated by means of C1-continuous quadratic B-spline curve interpolation.*/
class C1QuadraticSpline : public VectorSplineBase
{
public:
	/*
	* Constructor(s)
	*/
	C1QuadraticSpline(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/

	/*
	* Mutators
	*/
	void EraseControlPoint(int index);
	void CalcPlotVec();
};

class NURBSSurface
{
public:
	enum SegmentType
	{
		Row,
		Column,
		Num__SegmentTypes
	};

	static const Curve::CurveType TYPE;
	static const unsigned int CP_DIMENSIONS;
public:
	/*
	* Constructor(s)
	*/
	NURBSSurface(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
	Vector2I GetPlotPoint(float u, float v, const Matrix &worldToCameraTransform) const;
	float GetNik(unsigned int orderK, unsigned int i, float u) const;
	float GetNjl(unsigned int orderL, unsigned int j, float v) const;
	unsigned int GetOrderK() const;
	unsigned int GetOrderL() const;
	unsigned int GetNumRows() const;
	unsigned int GetNumColumns() const;
	void ClearFromPixelBuffer() const;
	void Draw() const;
	void Draw(const Color3 &drawColor) const;
	
	/*
	* Mutators
	*/
	void CalcPlotVec();
	void InsertSegment(SegmentType segType, unsigned int index);
	void DeleteSegment(SegmentType segType, unsigned int index);
	void SetOrderK(unsigned int newOrderK);
	void SetOrderL(unsigned int newOrderL);
	void CalcUKnots();
	void CalcVKnots();
	std::vector<std::vector<Vector3F>> &GetControlPointVec();
	std::vector<std::vector<Vector2I>> &GetProjectedControlPointVec();
	std::vector<std::vector<float>> &GetWeightVec();
	void SetControlPointVisibility(bool newVisibility);
	void SetControlMeshVisibility(bool newVisibility);
	void SetSurfaceMeshVisibility(bool newVisibility);
	void SetGouraudShadingVisibility(bool newVisibility);
	void SetProjectedControlPoints();
	void SetControlMesh();
	void SetSurfaceMesh();
	void EraseEverything();
	void ClearCalcDraw();

private:
	std::vector<std::vector<Vector3F>> mControlPointsVec; /*Row-Column order, just like matrices.*/
	std::vector<std::vector<Vector2I>> mProjectedControlPointsVec; /*Row-Column order, just like matrices.*/
	std::vector<std::vector<float>> mWeightVec;
	std::vector<std::vector<Vector2I>> mPlotVec; /*Row-Column order, just like matrices.*/
	unsigned int mOrderK;
	unsigned int mOrderL;
	std::vector<float> mUKnotVec;
	std::vector<float> mVKnotVec;
	std::vector<Line> mControlMeshVec;
	std::vector<Line> mSurfaceMeshVec;
	Color3 mColor;
	bool mControlPointsAreVisible;
	bool mControlMeshIsVisible;
	bool mSurfaceMeshIsVisible;
	bool mGouraudShadingIsVisible;
};



/*
* Global function prototypes
*/
void SetResolution(float newResolution);
void SetOrderK(float t);
void SetOrderL(float t);
void SetWeight(float newWeight);
void InitNURBSSurface();

/*
* Global variables
*/
extern Curve *curveParentInFocus;
extern int rowOfCPInFocus;
extern int colOfCPInFocus;

extern unsigned int resolution; /*How many points used to draw each NURBS surface.*/
extern const unsigned int MIN_ORDER;
extern unsigned int orderK;
extern unsigned int orderL;
extern bool controlPointsVisible; /*True if the controlPointCheckbox is checked.*/
extern bool controlMeshVisible; /*True if the controlMeshCheckbox is checked.*/
extern bool surfaceMeshVisible; /*True if the surfaceMeshCheckbox is checked.*/
extern bool gouraudShadingVisible; /*True if the gouraudShadingCheckbox is checked.*/
extern NURBSSurface *nurbsSurface;

#endif /*HFILE_CURVES*/