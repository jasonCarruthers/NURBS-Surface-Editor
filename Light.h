#ifndef LIGHT_HFILE
#define LIGHT_HFILE


#include "Line.h"


class DirectedLight
{
public:
	/*
	* Constructor(s)
	*/
	DirectedLight(const Vector3F &newPosition = Vector3F(0, 100, 0),
				  const Vector3F &newLookAt = Vector3F(0, 0, 0),
				  float newBrightness = 1.0f);

	/*
	* Accessors
	*/
	Vector3F GetPosition() const;
	Vector3F GetN() const;
	Vector3F GetV() const;
	Vector3F GetU() const;
	float GetBrightness() const;

	/*
	* Mutators
	*/
	void SetPosition(const Vector3F &newPosition);
	void SetBrightness(float newBrightness);
	void LookAt(const Vector3F &newLookAt);

private:
	Vector3F mPosition;
	Vector3F mN; /*"Look-at" vector: the direction for the z-view axis of the directed light.*/
	Vector3F mV; /*"Up" vector: the direction for the y-view axis of the directed light.*/
	Vector3F mU; /*The direction for the x-view axis of the directed light; derived from mN and mV unit vectors.*/
	float mBrightness; /*A value between 0.0f and 1.0f, inclusive.*/
};



/*
* Global function prototypes
*/
void PerformRayTrace();
Color3 RayTrace(const Ray3D &ray, unsigned int currentDepth, const Matrix &worldToCameraTransform);
Vector3F *GetNormalizedDirectionOfReflectionRay(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal);
Vector3F *GetNormalizedDirectionOfRefractionRay(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal, float n1, float n2);
float GetModifiedSchlickApproximationForReflectance(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal, Vector3F *reflectionDirection, Vector3F *refractionDirection, float n1, float n2);
float GetSchlickApproximation(float theta, float n1, float n2);
void DrawLights();


/*
* Global variables
*/
extern const unsigned int MAX_RAY_DEPTH;
extern std::vector<DirectedLight*> directedLightVec;
extern const float AIR_INDEX_OF_REFRACTION;


#endif /*LIGHT_HFILE*/