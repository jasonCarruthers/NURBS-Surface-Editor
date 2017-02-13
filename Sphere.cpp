#include "Matrix.h"
#include "Sphere.h"
#include "MyMath.h"


/*
* Global variables
*/
std::vector<Object*> objectVec;



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Material
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Material::Material(const Material::SurfaceType &newSurfaceType, float newAlpha, float newIndexOfRefraction, Color3 *newColorPtr)
{
	mSurfaceType = newSurfaceType;
	mAlpha = newAlpha;
	mIndexOfRefraction = newIndexOfRefraction;
	mColor = newColorPtr;
}

/*
* Destructor
*/
Material::~Material()
{
	if (mColor != NULL)
		delete mColor;
}

/*
* Accessors
*/
Material::SurfaceType Material::GetSurfaceType() const
{
	return mSurfaceType;
}

float Material::GetAlpha() const
{
	return mAlpha;
}

float Material::GetIndexOfRefraction() const
{
	return mIndexOfRefraction;
}

Color3 *Material::GetColor()
{
	return mColor;
}

/*
* Mutators
*/
void Material::SetSurfaceType(const Material::SurfaceType &newSurfaceType)
{
	mSurfaceType = newSurfaceType;
}

void Material::SetAlpha(float newAlpha)
{
	mAlpha = newAlpha;
}

void Material::SetIndexOfRefraction(float newIndexOfRefraction)
{
	mIndexOfRefraction = newIndexOfRefraction;
}

void Material::SetColor(Color3 *newColorPtr)
{
	if (mColor != NULL)
		delete mColor;
	mColor = newColorPtr;
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Object
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Object::Object(const Material::SurfaceType &newSurfaceType,
	float newAlpha, float newIndexOfRefraction, Color3 *newColorPtr)
{
	mMaterial = new Material(newSurfaceType, newAlpha, newIndexOfRefraction, newColorPtr);
}

/*
* Destructor
*/
Object::~Object()
{
	if (mMaterial != NULL)
		delete mMaterial;
}

/*
* Accessors
*/
Vector3F Object::GetPosition() const
{
	return mPosition;
}

Material *Object::GetMaterial()
{
	return mMaterial;
}

/*
* Mutators
*/
void Object::SetPosition(const Vector3F &newPosition)
{
	mPosition.Set(newPosition);
}

void Object::SetSurfaceType(const Material::SurfaceType &newSurfaceType)
{
	mMaterial->SetSurfaceType(newSurfaceType);
}

void Object::SetAlpha(float newAlpha)
{
	mMaterial->SetAlpha(newAlpha);
}

void Object::SetIndexOfRefraction(float newIndexOfRefraction)
{
	mMaterial->SetIndexOfRefraction(newIndexOfRefraction);
}

void Object::SetColor(Color3 *newColorPtr)
{
	mMaterial->SetColor(newColorPtr);
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Plane
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Plane::Plane(const Vector3F &newPosition, float newLength, float newWidth,
	const Vector3F &newLookAt, const Material::SurfaceType &newSurfaceType,
	float newAlpha, float newIndexOfRefraction, Color3 *newColorPtr)
{
	mPosition.Set(newPosition);
	mLength = MyAbs(newLength);
	mWidth = MyAbs(newWidth);
	LookAt(newLookAt);
	mMaterial = new Material(newSurfaceType, newAlpha, newIndexOfRefraction, newColorPtr);
}


/*
* Accessors
*/
float Plane::GetLength() const
{
	return mLength;
}

float Plane::GetWidth() const
{
	return mWidth;
}

Vector3F Plane::GetN() const
{
	return mN;
}

Vector3F Plane::GetV() const
{
	return mV;
}

Vector3F Plane::GetU() const
{
	return mU;
}

bool Plane::DoesIntersect(const Ray3D &ray)
{
	/*
	* If the vector perpendicular to this plane is orthogonal to the ray's directional unit vector, then there
	* is no intersection.
	*/
	float aDotd = mV * ray.GetDirection();
	if (aDotd == 0)
		return false;
	float t = (mV * (mPosition - ray.GetOrigin())) / aDotd;
	return t >= 0;
}

bool Plane::DoesIntersect(const Ray3D &ray, Vector3F &intersectionPoint, Vector3F &surfaceNormal)
{
	/*
	* If the vector perpendicular to this plane is orthogonal to the ray's directional unit vector, then there
	* is no intersection.
	*/
	float aDotd = mV * ray.GetDirection();
	float alignmentTolerance = 0.005f;
	if (MyAbs(aDotd) <= alignmentTolerance)
		return false;

	/*
	* Get the ray's point of intersection with this plane. The point of intersection may
	* still be invalid if the point of intersection doesn't lie within the dimensions of
	* the plane.
	*/
	float t = (mV * (mPosition - ray.GetOrigin())) / aDotd;
	if (t < 0)
		return false;

	Vector3F leftEdge = mPosition - mU * (mLength / 2.0f);
	Vector3F rightEdge = mPosition + mU * (mLength / 2.0f);
	if (leftEdge.GetX() > rightEdge.GetX())
	{
		Vector3F temp = leftEdge;
		leftEdge.Set(rightEdge);
		rightEdge.Set(temp);
	}
	Vector3F backEdge = mPosition - mN * (mWidth / 2.0f);
	Vector3F frontEdge = mPosition + mN * (mWidth / 2.0f);
	if (backEdge.GetZ() > frontEdge.GetZ())
	{
		Vector3F temp = backEdge;
		backEdge.Set(frontEdge);
		frontEdge.Set(temp);
	}
	float minY = leftEdge.GetY();
	if (rightEdge.GetY() < minY) minY = rightEdge.GetY();
	if (backEdge.GetY() < minY) minY = backEdge.GetY();
	if (frontEdge.GetY() < minY) minY = frontEdge.GetY();
	float maxY = leftEdge.GetY();
	if (rightEdge.GetY() > maxY) maxY = rightEdge.GetY();
	if (backEdge.GetY() > maxY) maxY = backEdge.GetY();
	if (frontEdge.GetY() > maxY) maxY = frontEdge.GetY();

	intersectionPoint = ray.GetOrigin() + ray.GetDirection() * t;
	
	/*
	* If the ray's point of intersection with this plane is outside of the specified
	* dimensions of this plane, then there is no intersection. Because the thickness of the plane
	* is so small, there is a small tolerance range for an intersection point out of bounds.
	*/
	float tolerance = 0.0005f;
	if (intersectionPoint.GetX() >= leftEdge.GetX() - tolerance && intersectionPoint.GetX() <= rightEdge.GetX() + tolerance &&
		intersectionPoint.GetY() >= minY - tolerance && intersectionPoint.GetY() <= maxY + tolerance &&
		intersectionPoint.GetZ() >= backEdge.GetZ() - tolerance && intersectionPoint.GetZ() <= frontEdge.GetZ() + tolerance)
	{
		surfaceNormal = CrossProduct(mPosition - frontEdge, mPosition - leftEdge);
		surfaceNormal.Normalize();
		float theta = acos(surfaceNormal * ray.GetDirection());
		if (MyAbs(theta) < MyPI() / 2.0f)
			surfaceNormal = surfaceNormal * -1;
		return true;
	}

	return false;
}


/*
* Mutators
*/
void Plane::SetLength(float newLength)
{
	mLength = newLength;
}

void Plane::SetWidth(float newWidth)
{
	mWidth = newWidth;
}

void Plane::LookAt(const Vector3F &newLookAt)
{
	Vector3F delta = -1.0f * mPosition + newLookAt;
	float deltaMag = sqrt(delta.GetX()*delta.GetX() + delta.GetY()*delta.GetY() + delta.GetZ()*delta.GetZ());
	mV = delta * (1.0f / deltaMag);
	mV.Normalize();

	Vector3F tempN = Vector3F(0.0f, 0.0f, -1.0f);
	if (mV == Vector3F(0, 0, -1)) tempN = Vector3F(0, -1, 0);
	else if (mV == Vector3F(0, 0, 1)) tempN = Vector3F(0, 1, 0);
	Vector3F tempU = CrossProduct(tempN, mV);
	tempU.Normalize();
	mU.Set(tempU);

	tempN = CrossProduct(mV, mU);
	tempN.Normalize();
	mN.Set(tempN);
}




/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Sphere
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Sphere::Sphere(const Vector3F &newPosition,
				float newRadius,
				const Material::SurfaceType &newSurfaceType,
				float newAlpha,
				float newIndexOfRefraction,
				Color3 *newColor)
{
	mPosition.Set(newPosition);
	mRadius = MyAbs(newRadius);
	mMaterial = new Material(newSurfaceType, newAlpha, newIndexOfRefraction, newColor);
}

/*
* Accessors
*/
float Sphere::GetRadius() const
{
	return mRadius;
}

bool Sphere::DoesIntersect(const Ray3D &ray)
{
	/*Ensure that the direction of the ray is normalized.*/
	Vector3F normalizedDirection = ray.GetDirection();
	normalizedDirection.Normalize();

	/*Initialize variables. The variables a, b, and c represent the coefficients of the terms of a quadratic equation.
	The variable t is the scalar parameter used to extend the ray.*/
	float t;
	float a = 1;
	float b = 2 * normalizedDirection * (ray.GetOrigin() + -1 * mPosition);
	float c = (ray.GetOrigin() + -1 * mPosition) * (ray.GetOrigin() + -1 * mPosition) - mRadius * mRadius;
	float discriminate = b * b - 4 * a * c;
	t = (-1 * b + sqrt(discriminate)) / (2 * a);

	/*If the discriminate is < 0, then the ray does not intersect the sphere.
	  If the most positive t-value is negative, then the ray does not intersect the sphere in the direction that the
	  ray is facing.
	  If either case is true, return false.*/
	return !(discriminate < 0 || t < 0);
	
}

bool Sphere::DoesIntersect(const Ray3D &ray, Vector3F &nearIntersectionPoint, Vector3F &nearSurfaceNormal)
{
	/*Ensure that the direction of the ray is normalized.*/
	Vector3F normalizedDirection = ray.GetDirection();
	normalizedDirection.Normalize();

	/*Initialize variables. The variables a, b, and c represent the coefficients of the terms of a quadratic equation.
	  The variable t is the scalar parameter used to extend the ray.*/
	float t;
	float a = 1;
	float b = 2 * normalizedDirection * (ray.GetOrigin() + -1 * mPosition);
	float c = (ray.GetOrigin() + -1 * mPosition) * (ray.GetOrigin() + -1 * mPosition) - mRadius * mRadius;
	float discriminate = b * b - 4 * a * c;

	/*If the discriminate is < 0, then the ray does not intersect the sphere, so return false.*/
	if(discriminate < 0)
		return false;

	/*If the discriminate is 0, then the ray is tangent to the surface of the sphere.*/
	else if (discriminate == 0)
	{
		t = (-1 * b + sqrt(discriminate)) / (2 * a);
		nearIntersectionPoint = ray.GetOrigin() + t * normalizedDirection;
		nearSurfaceNormal = nearIntersectionPoint + -1 * mPosition;
		nearSurfaceNormal.Normalize();
		return true;
	}

	/*If the discriminate is > 0, the ray intersects the sphere twice.*/
	float minusBOverTwoA = -b / (2 * a);
	float posRootOverTwoA = sqrt(discriminate) / (2 * a);
	t = minusBOverTwoA - posRootOverTwoA;

	/*
	* If t is negative, then the half-line behind the given ray intersects the sphere. There are two cases here:
	*	(i) The ray's origin is outside of the sphere but both t values are negative. Treat this case as no
	*			intersections occuring.
	*	(ii) The ray's origin is inside the sphere, so that one t value is negative and one is nonnegative. Treat
	*			this case as a single intersection occuring, and return the information associated with the positive
	*			t-value intersection point. Also, set the surface normal facing towards the inside of the sphere, since
	*			the angle between any incident ray and the surface normal of a sphere is >= 90 degrees.
	*/
	if (minusBOverTwoA - posRootOverTwoA < 0 && minusBOverTwoA + posRootOverTwoA < 0)
		return false;

	else if (minusBOverTwoA - posRootOverTwoA < 0 && minusBOverTwoA + posRootOverTwoA >= 0)
	{
		t = minusBOverTwoA + posRootOverTwoA;
		nearIntersectionPoint = ray.GetOrigin() + t * normalizedDirection;
		nearSurfaceNormal = -1 * (nearIntersectionPoint + -1 * mPosition);
		nearSurfaceNormal.Normalize();
	}

	else
	{
		nearIntersectionPoint = ray.GetOrigin() + t * normalizedDirection;
		nearSurfaceNormal = nearIntersectionPoint + -1 * mPosition;
		nearSurfaceNormal.Normalize();
	}

	//if (MyAbs(t) <= 0.005f)
	//	int x = 5;
	
	//t = (-1 * b + sqrt(discriminate)) / (2 * a);
	//if (MyAbs(t) <= 0.005f)
	//	t = (-1 * b - sqrt(discriminate)) / (2 * a);
	

	//if (t < 0)
	//	return false;

	/*Set the intersection point and the surface normal at the point of intersection.*/
	//intersectionPoint = ray.GetOrigin() + t * normalizedDirection;
	//surfaceNormalAtIntersectionPoint = intersectionPoint + -1 * mPosition;
	//surfaceNormalAtIntersectionPoint.Normalize();

	return true;
}


/*
* Mutators
*/
void Sphere::SetRadius(float newRadius)
{
	mRadius = newRadius;
}