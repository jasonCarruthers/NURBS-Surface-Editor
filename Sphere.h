#ifndef SPHERE_HFILE
#define SPHERE_HFILE


#include "General.h"
#include "Line.h"



class Material
{
public:
	enum SurfaceType
	{
		Diffuse, /*Rough at the microscopic level, which means incident light rays reflect at random directions.*/
		Specular, /*Smooth at the microscopic level, which means incident light rays reflect predictably.*/
	};
public:
	/*
	* Constructor(s)
	*/
	Material(const SurfaceType &newSurfaceType = Diffuse, float newAlpha = 1.0f, float newIndexOfRefraction = 1.3f, Color3 *newColorPtr = NULL);

	/*
	* Destructor
	*/
	~Material();

	/*
	* Accessors
	*/
	SurfaceType GetSurfaceType() const;
	float GetAlpha() const;
	float GetIndexOfRefraction() const;
	Color3 *GetColor();

	/*
	* Mutators
	*/
	void SetSurfaceType(const SurfaceType &newSurfaceType);
	void SetAlpha(float newAlpha);
	void SetIndexOfRefraction(float newIndexOfRefraction);
	void SetColor(Color3 *newColorPtr);

private:
	SurfaceType mSurfaceType;
	float mAlpha; /*A number in the interval 0 to 1, inclusive. An alpha value of 0 means the material is transparent,
				    while an alpha value of 1 means the material is opaque. An intermedia alpha value means that some
					light rays will reflect off of the surface of the material while others will refract through the material.*/
	//unsigned int mShininess; /*If the material is specualar, this is how large the blur spot is.*/
	float mIndexOfRefraction;
	Color3 *mColor; /*NULL if the material has no color and uses a texture instead.*/
	//Texture *mTexture; /*NULL if the material has no texture and has a color instead.*/
};


class Object
{
public:
	/*
	* Constructor(s)
	*/
	Object(const Material::SurfaceType &newSurfaceType = Material::SurfaceType::Diffuse,
			float newAlpha = 1.0f,
			float newIndexOfRefraction = 1.0f,
			Color3 *newColorPtr = NULL);

	/*
	* Destructor
	*/
	~Object();

	/*
	* Accessors
	*/
	virtual Vector3F GetPosition() const;
	Material *GetMaterial();
	virtual bool DoesIntersect(const Ray3D &ray) = 0;
	virtual bool DoesIntersect(const Ray3D &ray, Vector3F &nearIntersectionPoint, Vector3F &nearSurfaceNormal) = 0;

	/*
	* Mutators
	*/
	virtual void SetPosition(const Vector3F &newPosition);
	void SetSurfaceType(const Material::SurfaceType &newSurfaceType);
	void SetAlpha(float newAlpha);
	void SetIndexOfRefraction(float newIndexOfRefraction);
	void SetColor(Color3 *newColorPtr);

protected:
	Vector3F mPosition;
	Material *mMaterial;
};


class Plane : public Object
{
public:
	/*
	* Constructor(s)
	*/
	Plane(const Vector3F &newPosition = Vector3F(0, 10, 0), float newLength = 10.0f, float newWidth = 10.0f,
		const Vector3F &newLookAt = Vector3F(0, 0, 0),
		const Material::SurfaceType &newSurfaceType = Material::SurfaceType::Diffuse,
		float newAlpha = 1.0f, float newIndexOfRefraction = 1.0f, Color3 *newColorPtr = NULL);

	/*
	* Accessors
	*/
	float GetLength() const;
	float GetWidth() const;
	Vector3F GetN() const;
	Vector3F GetV() const;
	Vector3F GetU() const;
	bool DoesIntersect(const Ray3D &ray);
	bool DoesIntersect(const Ray3D &ray, Vector3F &nearIntersectionPoint, Vector3F &nearSurfaceNormal);

	/*
	* Mutators
	*/
	void SetLength(float newLength);
	void SetWidth(float newWidth);
	void LookAt(const Vector3F &newLookAt);
	

private:
	float mLength;
	float mWidth;
	Vector3F mN; /*The z-axis. Also the width axis.*/
	Vector3F mV; /*The y-axis. Also the "look at" direction.*/
	Vector3F mU; /*The x-axis. Derived from mN and mV unit vectors. Also the length axis.*/
};



class Sphere : public Object
{
public:
	/*
	* Constructor(s)
	*/
	Sphere(const Vector3F &newPosition = Vector3F(0, 0, 0),
			float newRadius = 5.0f,
			const Material::SurfaceType &newSurfaceType = Material::SurfaceType::Diffuse,
			float newAlpha = 1.0f,
			float newIndexOfRefraction = 1.0f,
			Color3 *newColorPtr = NULL);

	/*
	* Accessors
	*/
	float GetRadius() const;
	bool DoesIntersect(const Ray3D &ray);
	bool DoesIntersect(const Ray3D &ray, Vector3F &nearIntersectionPoint, Vector3F &nearSurfaceNormal);
	//void DrawOutline() const;

	/*
	* Mutators
	*/
	void SetRadius(float newRadius);

private:
	float mRadius;
};


/*
* Global variables
*/
extern std::vector<Object*> objectVec;


#endif /*SPHERE_HFILE*/