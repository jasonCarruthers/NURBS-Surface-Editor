#include "General.h"
#include "Matrix.h"
#include "Light.h"
#include "Circle.h"
#include "Sphere.h"
#include "Curves.h"
#include "GUI.h"
#include "Camera.h"



/*
* Global variables
*/
const unsigned int MAX_RAY_DEPTH = 15;
std::vector<DirectedLight*> directedLightVec;
const float AIR_INDEX_OF_REFRACTION = 1.000277f;


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class DirectedLight
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
DirectedLight::DirectedLight(const Vector3F &newPosition, const Vector3F &newLookAt, float newBrightness)
{
	mPosition.Set(newPosition);
	LookAt(newLookAt);
	SetBrightness(newBrightness);
}

/*
* Accessors
*/
Vector3F DirectedLight::GetPosition() const
{
	return mPosition;
}

Vector3F DirectedLight::GetN() const
{
	return mN;
}

Vector3F DirectedLight::GetV() const
{
	return mV;
}

Vector3F DirectedLight::GetU() const
{
	return mU;
}

float DirectedLight::GetBrightness() const
{
	return mBrightness;
}


/*
* Mutators
*/
void DirectedLight::SetPosition(const Vector3F &newPosition)
{
	mPosition.Set(newPosition);
}

void DirectedLight::SetBrightness(float newBrightness)
{
	/*Ensure that newBrightness is between 0 and 1, inclusive.*/
	if (newBrightness < 0) newBrightness = 0;
	else if (newBrightness > 1) newBrightness = 1;

	mBrightness = newBrightness;
}

void DirectedLight::LookAt(const Vector3F &newLookAt)
{
	Vector3F delta = mPosition + -1.0f * newLookAt;
	float deltaMag = sqrt(delta.GetX()*delta.GetX() + delta.GetY()*delta.GetY() + delta.GetZ()*delta.GetZ());
	mN = delta * (1.0f / deltaMag);
	mN.Normalize();

	Vector3F tempV = Vector3F(0.0f, 1.0f, 0.0f);
	mU.Set(CrossProduct(tempV, mN));
	mU.Normalize();

	mV.Set(CrossProduct(mN, mU));
	mV.Normalize();
}




/*
* Global functions
*/
void PerformRayTrace()
{
	/*Initialize variables.*/
	Matrix cameraToWorldTransform = camera->GetCameraToWorldTransform();
	float rayDirInWorldSpaceX;
	float rayDirInWorldSpaceY;
	float rayDirInWorldSpaceZ;
	Matrix worldToCameraTransform = *(cameraToWorldTransform.GetInverse());
	Color3 pixelColor;
	float canvasX;
	float canvasY;
	Vector3F rayDirection;
	Ray3D primaryRay;
	float distToCanvas = camera->GetDistanceFromCanvas();
	float halfWindowWidth = WINDOW_WIDTH / 2.0f;
	float halfWindowHeight = WINDOW_HEIGHT / 2.0f;
	float halfCanvasWidth = CANVAS_WIDTH / 2.0f;
	float halfCanvasHeight = CANVAS_HEIGHT / 2.0f;
	//Matrix *rayInCameraSpace = new Matrix(4, 4);
	//for (unsigned int row = 0; row < rayInCameraSpace->GetNumRows(); row++)
	//	for (unsigned int column = 0; column < rayInCameraSpace->GetNumColumns(); column++)
	//		rayInCameraSpace->Insert(row, column, 0);
	//rayInCameraSpace->Insert(3, 3, 1);
	//Matrix *rayInWorldSpace = new Matrix(4, 4);

	/*For each ray with origin at the camera eye and direction from its origin to a point on the canvas,
	see if that ray intersects any object in the scene. If so, if it is transparent, recursively shoot
	a reflective ray and a refraction ray; if the object is opaque, shoot a shadow ray from the point
	of intersection of the primary ray and the surface to the light source; if the shadow ray intersects
	an object, then the pixel corresponding to the primary ray's origin is black; otherwise, it is
	illuminated.*/
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
	{
		std::cout << "Y = " << y << " out of " << WINDOW_HEIGHT << "." << "\t";
		for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
		{
			//std::cout << "X = " << x << " out of " << WINDOW_WIDTH << "." << "\n";

			/*Initialze the primary ray originating at the camera's position and shooting through the current
			pixel, designated by the current x and y values.*/
			canvasX = (((float)x - halfWindowWidth) / halfWindowWidth) * halfCanvasWidth;
			canvasY = (((float)y - halfWindowHeight) / halfWindowHeight) * halfCanvasHeight;
			rayDirection = Vector3F(canvasX, canvasY, -1.0f * distToCanvas /*- 10.0f*/);
			//rayDirection.Normalize();

			///*Initialize the matrix for the ray in camera space.*/
			//if (rayDirection.GetX() == 0)
			//	rayInCameraSpace->Insert(0, 0, 1);
			//else
			//	rayInCameraSpace->Insert(0, 0, rayDirection.GetX());

			//if (rayDirection.GetY() == 0)
			//	rayInCameraSpace->Insert(1, 1, 1);
			//else
			//	rayInCameraSpace->Insert(1, 1, rayDirection.GetY());

			//rayInCameraSpace->Insert(2, 2, rayDirection.GetZ());

			
			//*rayInWorldSpace = *rayInCameraSpace * cameraToWorldTransform;
			

			/*If the rayInWorldSpace matrix is not NULL, then set the ray's direction in world space.
			  If the rayInWorldSpace matrix is NULL, then use the current ray's direction computed as is.*/
			//if (rayInWorldSpace != NULL)
			//{
				/*Initialize the ray's direction in world space.*/
				//rayDirection = Vector3F(rayInWorldSpace->Get(0, 0) + rayInWorldSpace->Get(1, 0) + rayInWorldSpace->Get(2, 0),
				//						rayInWorldSpace->Get(0, 1) + rayInWorldSpace->Get(1, 1) + rayInWorldSpace->Get(2, 1),
				//						rayInWorldSpace->Get(0, 2) + rayInWorldSpace->Get(1, 2) + rayInWorldSpace->Get(2, 2));
				/*rayDirection = Vector3F(rayInWorldSpace->Get(0, 0) + rayInWorldSpace->Get(0, 1) + rayInWorldSpace->Get(0, 2),
										rayInWorldSpace->Get(1, 0) + rayInWorldSpace->Get(1, 1) + rayInWorldSpace->Get(1, 2),
										rayInWorldSpace->Get(2, 0) + rayInWorldSpace->Get(2, 1) + rayInWorldSpace->Get(2, 2));*/
				
			//}

			rayDirInWorldSpaceX = rayDirection.GetX() * cameraToWorldTransform.Get(0, 0) +
								rayDirection.GetY() * cameraToWorldTransform.Get(1, 0) +
								rayDirection.GetZ() * 1 * cameraToWorldTransform.Get(2, 0);
			rayDirInWorldSpaceY = rayDirection.GetX() * cameraToWorldTransform.Get(0, 1) +
								rayDirection.GetY() * cameraToWorldTransform.Get(1, 1) +
								rayDirection.GetZ() * 1 * cameraToWorldTransform.Get(2, 1);
			rayDirInWorldSpaceZ = rayDirection.GetX() * cameraToWorldTransform.Get(0, 2) +
								rayDirection.GetY() * cameraToWorldTransform.Get(1, 2) +
								rayDirection.GetZ() * 1 * cameraToWorldTransform.Get(2, 2);

			rayDirection = Vector3F(rayDirInWorldSpaceX, rayDirInWorldSpaceY, /*-1 **/ rayDirInWorldSpaceZ);

			rayDirection.Normalize();

			/*Intitialize the primary ray in world space.*/
			primaryRay = Ray3D(camera->GetPosition(), rayDirection);

			pixelColor = RayTrace(primaryRay, 0, worldToCameraTransform);
			SetPixel(x, y, pixelColor);
		}
	}
}

//Color3 RayTrace(int currentDepth)
Color3 RayTrace(const Ray3D &ray, unsigned int currentDepth, const Matrix &worldToCameraTransform)
{
	/*Initialize variables.*/
	Object *objectPtr = NULL;
	float minDist = 9999999;
	Vector3F nearIntersectionPoint;
	Vector3F nearSurfaceNormal;
	Color3 pixelColor = Color3(0, 0, 0);
	Vector3F tempNearIntersectionPoint;
	Vector3F tempNearSurfaceNormal;
	
	/*Initialize the placeholder sphere. When more diffuse objects are added to the scene that specular objects
	  can reflect light from, get rid of this variable and the block of code a little ways down below that hardcodes
	  the color of specular objects in places that aren't in shadow and that don't successfully reflect light from
	  diffuse objects.*/
	Object *placeholderObject = new Sphere();
	placeholderObject->SetSurfaceType(Material::Diffuse);
	placeholderObject->SetAlpha(1.0f);
	placeholderObject->SetColor(new Color3(1, 1, 1));

	/*Get the object that the current ray intersects.*/
	for (unsigned int objectIndex = 0; objectIndex < objectVec.size(); objectIndex++)
	{
		if (objectVec[objectIndex]->DoesIntersect(ray, tempNearIntersectionPoint, tempNearSurfaceNormal))
		{
			/*Instead of comparing the intersection distances, compare the distance squared of intersections in order
			  to eliminate the need for the sqrt(...), which speeds up the ray tracing algorithm.*/
			float distSquared = MyPow(tempNearIntersectionPoint.GetX() - ray.GetOrigin().GetX(), 2) +
								MyPow(tempNearIntersectionPoint.GetY() - ray.GetOrigin().GetY(), 2) +
								MyPow(tempNearIntersectionPoint.GetZ() - ray.GetOrigin().GetZ(), 2);
			if (distSquared < minDist)
			{
				nearIntersectionPoint = tempNearIntersectionPoint;
				nearSurfaceNormal = tempNearSurfaceNormal;

				minDist = distSquared;
				objectPtr = objectVec[objectIndex];
			}
		}
	}

	if (objectPtr != NULL && objectPtr->GetPosition() == Vector3F(0, 50, 0) /*&& (nearSurfaceNormal.GetY() > 0)*/)
		int x = 5;

	/*
	* If the ray didn't intersect any object, but the currentDepth > 0, then the ray is on the surface
	* of an object facing away from that object. Since the ray didn't intersect any object, then see if the ray
	* intersects a light source.
	*
	* NOTE: This if block is a temporary fix to the problem of illuminating parts of specular and/or translucent objects
	*		that would remain in shadow. Specular objects reflect light from objects around them, so if there are parts
	*		the specular object that don't reflect light from objects, then it appears in shadow currently. As a temporary
	*		fix, just draw these shadow parts as a dim shade of the object's color.
	*/
	if (objectPtr == NULL && currentDepth > 0)
	{
		nearIntersectionPoint = ray.GetOrigin();
		nearSurfaceNormal = ray.GetDirection();
		nearSurfaceNormal.Normalize();
		
		objectPtr = placeholderObject;
	}

	/*If the ray didn't intersect any object, then no light is contributed to the current pixel color from this light ray.*/
	else if (objectPtr == NULL)
	{
		objectPtr = NULL;
		delete placeholderObject;
		return Color3(0, 0, 0);
	}

	/*If the object's material is reflective and opaque, then shoot a reflection ray.*/
	else if (objectPtr->GetMaterial()->GetSurfaceType() == Material::Specular &&
		objectPtr->GetMaterial()->GetAlpha() == 1.0f &&
			 currentDepth < MAX_RAY_DEPTH)
	{
		Vector3F *reflectionDirection = NULL;
		Ray3D reflectionRay;
		Color3 reflectionColor = Color3(0, 0, 0);

		/*Calculate the reflection ray.*/
		reflectionDirection = GetNormalizedDirectionOfReflectionRay(ray.GetDirection(), nearSurfaceNormal);
		reflectionRay = Ray3D(nearIntersectionPoint + 0.2f * *reflectionDirection, *reflectionDirection);
		reflectionColor = RayTrace(reflectionRay, currentDepth + 1, worldToCameraTransform);

		objectPtr = NULL;
		delete placeholderObject;
		delete reflectionDirection;
		return reflectionColor;
	}

	/* 
	 * If the object's material is translucent, shoot a refraction ray. If the object is not
	 * transparent (completely translucent), also shoot a reflection ray.
	 */
	else if (objectPtr->GetMaterial()->GetAlpha() < 1.0f && currentDepth < MAX_RAY_DEPTH)
	{
		Vector3F *reflectionDirection = NULL;
		Ray3D reflectionRay;
		Color3 reflectionColor = Color3(0, 0, 0);
		Vector3F *refractionDirection = NULL;
		Ray3D refractionRay;
		Color3 refractionColor = Color3(0, 0, 0);
		float n1 = /*(!isRefractedRay) ?*/ AIR_INDEX_OF_REFRACTION /*: spherePtr->GetMaterial().GetIndexOfRefraction()*/;
		float n2 = /*(!isRefractedRay) ?*/ objectPtr->GetMaterial()->GetIndexOfRefraction() /*: AIR_INDEX_OF_REFRACTION*/;

		/*Calculate the reflection ray.*/
		if (objectPtr->GetMaterial()->GetSurfaceType() == Material::Specular)
		{
			reflectionDirection = GetNormalizedDirectionOfReflectionRay(ray.GetDirection(), nearSurfaceNormal);
			reflectionRay = Ray3D(nearIntersectionPoint + 0.2f * *reflectionDirection, *reflectionDirection);
			reflectionColor = RayTrace(reflectionRay, currentDepth + 1, worldToCameraTransform);
		}
				
		/*Calculate the refraction ray. If there is total internal reflection, then
			refractionDirection == NULL.*/
		Vector3F testUnitVec = nearIntersectionPoint + -1 * objectPtr->GetPosition();
		testUnitVec.Normalize();
		if (nearSurfaceNormal == testUnitVec)
			refractionDirection = GetNormalizedDirectionOfRefractionRay(ray.GetDirection(), nearSurfaceNormal, n1, n2);
		else
			refractionDirection = GetNormalizedDirectionOfRefractionRay(ray.GetDirection(), nearSurfaceNormal, n2, n1);
		if (refractionDirection != NULL)
		{
			refractionRay = Ray3D(nearIntersectionPoint + 0.2f * *refractionDirection, *refractionDirection);
			refractionColor = RayTrace(refractionRay, currentDepth + 1, worldToCameraTransform);
		}

		/*Calculate schlick's approximation for reflectance.*/
		float reflectance;
		if (objectPtr->GetMaterial()->GetSurfaceType() == Material::Specular && objectPtr->GetMaterial()->GetAlpha() < 1.0f)
		{
			if (nearSurfaceNormal == testUnitVec)
				reflectance = GetModifiedSchlickApproximationForReflectance(ray.GetDirection(), nearSurfaceNormal, reflectionDirection, refractionDirection, n1, n2);
			else
				reflectance = GetModifiedSchlickApproximationForReflectance(ray.GetDirection(), nearSurfaceNormal, reflectionDirection, refractionDirection, n2, n1);
		}
		else /*if (spherePtr->GetMaterial().GetSurfaceType() != Material::Specular)*/
			reflectance = 0.0f;

		/*Calculate the pixelColor based on all of the reflections and refractions that are seen by
			the current pixel.*/
		float sphereAlpha = objectPtr->GetMaterial()->GetAlpha();
		//pixelColor = (reflectionColor * reflectance + refractionColor * (1.0f - reflectance) * sphereAlpha) * *(spherePtr->GetMaterial()->GetColor());
		pixelColor = (reflectionColor * reflectance + refractionColor * (1.0f - reflectance) * (1.0f - sphereAlpha)) + *(objectPtr->GetMaterial()->GetColor()) * sphereAlpha;
		//pixelColor = reflectance * (reflectionColor * (1.0f - sphereAlpha) + *(spherePtr->GetMaterial()->GetColor()) * sphereAlpha) +
		//	(1.0f - reflectance) * (refractionColor * (1.0f - sphereAlpha) + *(spherePtr->GetMaterial()->GetColor()) * sphereAlpha);

		objectPtr = NULL;
		delete placeholderObject;
		delete reflectionDirection;
		delete refractionDirection;
		return pixelColor;
	}


	/*If the object is a diffuse opaque object, shoot a shadow ray to each light source. Sum the
		overlapping light values.*/
	Ray3D shadowRay;
	for (unsigned int directedLightIndex = 0; directedLightIndex < directedLightVec.size(); directedLightIndex++)
	{
		bool lightIsBlocked = false;

		/*Create a shadow ray with origin slightly offset in order to move it off of the surface of the object that
		  is causing this shadow ray.*/
		shadowRay.SetOrigin(nearIntersectionPoint + 0.25f * nearSurfaceNormal);
		shadowRay.SetDirection(-1 * shadowRay.GetOrigin() + directedLightVec[directedLightIndex]->GetPosition());
		shadowRay.NormalizeDirection();
		for (unsigned int objectIndex = 0; objectIndex < objectVec.size(); objectIndex++)
		{
			/*If the shadow ray intersects any object, set the current screen pixel to black.*/
			if (objectVec[objectIndex]->DoesIntersect(shadowRay))
			{
				/*If the intersected object is behind the current light source, disregard this intersection.*/
				Vector3F rayOrigin = shadowRay.GetOrigin();
				Vector3F lightPos = directedLightVec[directedLightIndex]->GetPosition();
				Vector3F objPos = objectVec[objectIndex]->GetPosition();
				float distToLightSquared = MyPow(lightPos.GetX() - rayOrigin.GetX(), 2) + MyPow(lightPos.GetY() - rayOrigin.GetY(), 2) + MyPow(lightPos.GetZ() - rayOrigin.GetZ(), 2);
				float distToObjSquared = MyPow(objPos.GetX() - rayOrigin.GetX(), 2) + MyPow(objPos.GetY() - rayOrigin.GetY(), 2) + MyPow(objPos.GetZ() - rayOrigin.GetZ(), 2);
				if (distToLightSquared < distToObjSquared)
					continue;

				/*Otherwise, the shadow ray is actually blocked by this object, so this light doesn't contribute
				  any light to the current pixel.*/
				lightIsBlocked = true;
				break;
			}
		}

		/*If the shadow ray did not intersect any objects in the scene, then the current pixel is illuminated by
		  the current light source. Set the pixelColor based on the intersected object's color and the light's brightness.*/
		if (!lightIsBlocked)
		{
			//Vector3F normalizedShadowRayDirection = shadowRay.GetDirection();
			//normalizedShadowRayDirection.Normalize();
			float brightnessCoeff = directedLightVec[directedLightIndex]->GetN() * shadowRay.GetDirection();
			if (brightnessCoeff < 0)
				brightnessCoeff = 0.0f;
			pixelColor += *(objectPtr->GetMaterial()->GetColor()) * (brightnessCoeff * directedLightVec[directedLightIndex]->GetBrightness());
		}
	}


	/*If the current pixel is in shadow, instead of drawing black, draw the object's color divided by some constant.*/
	if (pixelColor == Color3(0, 0, 0))
		pixelColor = *(objectPtr->GetMaterial()->GetColor()) / 4.0f;
	
	objectPtr = NULL;
	delete placeholderObject;
	return pixelColor;
}

Vector3F *GetNormalizedDirectionOfReflectionRay(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal)
{
	Vector3F *reflectionDirection = new Vector3F();
	*reflectionDirection = incidentDirectionNormal + -2.0f * (incidentDirectionNormal * surfaceNormal) * surfaceNormal;
	reflectionDirection->Normalize();
	return reflectionDirection;
}

/*
* Equation for refractionDirection taken from starkeffects.com, under the section, "Derive a Vector Form for Snell's Law."
* n1 = index of refraction of material on incident side.
* n2 = index of refraction of material on refracted side.
*
* NOTE: If this function doesn't return the correct refractionDirection, try using this website instead:
*		graphics.stanford.edu, the article, "Reflections and Refractions in Ray Tracing."
*/
Vector3F *GetNormalizedDirectionOfRefractionRay(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal, float n1, float n2)
{
	/*Calculate helper variables.*/
	float nRatio = n1 / n2;
	float nRatioSquared = nRatio * nRatio;
	Vector3F nHatCrossS1Hat = CrossProduct(surfaceNormal, incidentDirectionNormal);
	Vector3F leftTerm = nRatio * CrossProduct(surfaceNormal, -1.0f * nHatCrossS1Hat);
	float contentsUnderRoot = 1.0f - nRatioSquared * (nHatCrossS1Hat * nHatCrossS1Hat);

	/*If contentsUnderRoot < 0, then there is total internal reflection, which means no refraction.*/
	if (contentsUnderRoot < 0)
		return NULL;

	/*Finish calculating rightTerm.*/
	Vector3F rightTerm = -1.0f * surfaceNormal * sqrt(contentsUnderRoot);

	/*Return the refractionDirection.*/
	Vector3F *refractionDirection = new Vector3F();
	*refractionDirection = leftTerm + rightTerm;
	return refractionDirection;
}

/*
* The following formula is taken from graphics.stanford.edu, the article, "Reflections and Refractions in Ray Tracing."
* This equation is a slight modification to Schlick's approximation for reflectance (a number between 0 and 1 indicating
* how much of the incident light is reflected). Schlick's unmodified formula doesn't take into account which material
* has which index of refraction; thus, there are three cases:
*	1) "Unmodified" Schlick's approximation for reflectance, when n1 <= n2.
*	2) Schlick's formula, but replace thetaIncident with thetaTransmitted, when n1 > n2 and there is not total internal reflection.
*	3) Return 1 when n1 > n2 and there is total internal reflection.
* 
* n1 = index of refraction of material on incident side.
* n2 = index of refraction of material on refracted side.
*
* NOTE: Schlick's formula for reflectance is an approximation of Fresnel's equations, of which there is one for
*		reflectance and one for transmittance. However, Fresnel's equations are computationally expensive,
*		so Schlick's equation is preferable for calculating reflectance. Transmittance is then simply one minus
*		the reflectance.
*/
float GetModifiedSchlickApproximationForReflectance(const Vector3F &incidentDirectionNormal, const Vector3F &surfaceNormal, Vector3F *reflectionDirection, Vector3F *refractionDirection, float n1, float n2)
{
	/*Check case 3.*/
	if (n1 > n2 && refractionDirection == NULL)
		return 1.0f;

	/*Check case 1.*/
	else if ( n1 <= n2 )
	{
		/*For case 1, we will use thetaIncident, so calculate it.*/
		float thetaIncident = MyAbs(acos(incidentDirectionNormal * surfaceNormal));
		return GetSchlickApproximation(thetaIncident, n1, n2);
	}

	/*If we've made it this far, use case 2.*/
	else /*if (n1 > n2 && refractionDirection != NULL)*/
	{
		/*For case 2, we will use thetaTransmitted, so calculate it.*/
		float thetaTransmitted = MyAbs(acos(*refractionDirection * surfaceNormal));
		return GetSchlickApproximation(thetaTransmitted, n1, n2);
	}
}

/*Return a a number between 0 and 1 indicating how much of the incident light is reflected.*/
float GetSchlickApproximation(float theta, float n1, float n2)
{
	float RKnot = ((n1 - n2) / (n1 + n2)) * ((n1 - n2) / (n1 + n2));
	
	/*
	* NOTE: I had to modify the formula in the following line to account for a negative cos(...) value; with
	*		a negative cos(...) value, this function could return a number outside of the interval 0-1.
	*		To account for this bug in the formula, I've added a MyAbs(...) around the cos(...) function.
	*/
	float schlickApprox = RKnot + (1.0f - RKnot) * MyPow(1.0f - MyAbs(cos(theta)), 5);
	return schlickApprox;
}

void DrawLights()
{
	/*Initialize variables.*/
	Matrix worldToCameraTransform = *(camera->GetCameraToWorldTransform().GetInverse());
	Vector2F rectDimensions = Vector2F(20.0f, 20.0f);
	Vector2I corners[4];
	Line edges[4];
	Color3 centerColor = Color3(1, 1, 1);
	Color3 rectColor = Color3(1, 1, 1);
	Color3 arrowColor = Color3(0, 1, 1);
	GraphicVector2D arrow;
	float arrowLength = 30.0f;

	/*Draw rectangles to represent the directed lights, with arrows indicating their direction and normal
	  to the rectangular face.*/
	for (unsigned int directedLightIndex = 0; directedLightIndex < directedLightVec.size(); directedLightIndex++)
	{
		/*Represent the position and orientation of the directed light with a small
		  rectangle in a 3D coordinate system. Set the corners of the current rect here,
		  calculated in R^3.*/
		Vector3F topLeft = directedLightVec[directedLightIndex]->GetPosition() + (-1 * rectDimensions.GetX() / 2.0f) * directedLightVec[directedLightIndex]->GetU() + (1 * rectDimensions.GetY() / 2.0f) * directedLightVec[directedLightIndex]->GetV();
		Vector3F topRight = directedLightVec[directedLightIndex]->GetPosition() + (1 * rectDimensions.GetX() / 2.0f) * directedLightVec[directedLightIndex]->GetU() + (1 * rectDimensions.GetY() / 2.0f) * directedLightVec[directedLightIndex]->GetV();
		Vector3F bottomLeft = directedLightVec[directedLightIndex]->GetPosition() + (-1 * rectDimensions.GetX() / 2.0f) * directedLightVec[directedLightIndex]->GetU() + (-1 * rectDimensions.GetY() / 2.0f) * directedLightVec[directedLightIndex]->GetV();
		Vector3F bottomRight = directedLightVec[directedLightIndex]->GetPosition() + (1 * rectDimensions.GetX() / 2.0f) * directedLightVec[directedLightIndex]->GetU() + (-1 * rectDimensions.GetY() / 2.0f) * directedLightVec[directedLightIndex]->GetV();

		/*Get 2D projection of corners and center.*/
		Vector2I center = Get2DProjection(directedLightVec[directedLightIndex]->GetPosition(), worldToCameraTransform);
		corners[0] = Get2DProjection(topLeft, worldToCameraTransform);
		corners[1] = Get2DProjection(topRight, worldToCameraTransform);
		corners[2] = Get2DProjection(bottomLeft, worldToCameraTransform);
		corners[3] = Get2DProjection(bottomRight, worldToCameraTransform);

		/*Connect the corners with lines.*/
		edges[0] = Line(corners[0], corners[1], rectColor);
		edges[1] = Line(corners[1], corners[3], rectColor);
		edges[2] = Line(corners[3], corners[2], rectColor);
		edges[3] = Line(corners[2], corners[0], rectColor);

		/*Set the arrow for the current directed light, which are GraphicVector2D objects in R^3 that start at
		  the directed light source position and shoot in the direction of the directed light source.*/
		arrow = GraphicVector2D(Get2DProjection(directedLightVec[directedLightIndex]->GetPosition(), worldToCameraTransform),
			Get2DProjection(directedLightVec[directedLightIndex]->GetPosition() + -1 * arrowLength * directedLightVec[directedLightIndex]->GetN(), worldToCameraTransform),
									arrowColor);

		/*Draw the center, the corners, the edges, and the arrow.*/
		for (int deltaX = -2; deltaX < 2; deltaX++)
			for (int deltaY = -2; deltaY < 2; deltaY++)
				SetPixel(center.GetX() + deltaX, center.GetY() + deltaY, centerColor);
		for (unsigned int cornerIndex = 0; cornerIndex < 4; cornerIndex++)
			for (int deltaX = -2; deltaX < 2; deltaX++)
				for (int deltaY = -2; deltaY < 2; deltaY++)
					SetPixel(corners[cornerIndex].GetX() + deltaX, corners[cornerIndex].GetY() + deltaY, rectColor);
		for (unsigned int edgeIndex = 0; edgeIndex < 4; edgeIndex++)
			edges[edgeIndex].Draw();
		arrow.Draw();
	}
}