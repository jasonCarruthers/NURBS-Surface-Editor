#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "MyMath.h"



/*
* Global variables
*/
std::vector<std::vector<unsigned long long>> pascalsTriangle;


/*
* Global functions
*/
void TestTrigThetaSign()
{
	std::cout << sin(MyPI() / 4.0f) << "\n";
}

void InitPascalsTriangle()
{
	/*Using unsigned long long variables to store a given entry, entries of up to about 19 digits
	  long (base 10) can be stored. Row 40 only has numbers with about 12 digits long.*/

	unsigned long long newEntry = 0;
	unsigned long long nFactorial = 0;
	for (unsigned int n = 0; n < 40; n++)
	{
		std::vector<unsigned long long> temp;
		pascalsTriangle.push_back(temp);

		for (unsigned int i = 0; i < n + 1; i++)
		{
			if (i == 0 || i == n)
				pascalsTriangle[n].push_back(1);
			else
			{
				newEntry = pascalsTriangle[n - 1][i - 1] + pascalsTriangle[n - 1][i];
				pascalsTriangle[n].push_back(newEntry);
			}
		}
	}
}

unsigned long long MyFactorial(int num)
{
	unsigned long long retVal = 1;
	while (num > 1)
	{
		retVal *= num;
		num--;
	}
	return retVal;
}

float MyPow(float base, int exponent)
{
	if (exponent == 0)
		return 1.0f;

	bool expIsNeg = (exponent < 0);
	exponent = (int)MyAbs((float)exponent);
	float retVal = base;
	for (int i = 1; i < exponent; i++)
		retVal *= base;
	if (expIsNeg)
		retVal = 1.0f / retVal;

	return retVal;
}

float MyAbs(float num)
{
	return (num < 0) ? num * -1 : num;
}

Vector3F Normalize(const Vector3F &vector)
{
	float length = sqrt((vector.GetX() * vector.GetX()) + (vector.GetY() * vector.GetY()) + (vector.GetZ() * vector.GetZ()));
	if (length == 0)
		return Vector3F(0.0f, 0.0f, 0.0f);

	Vector3F retVector = Vector3F(vector.GetX() / length, vector.GetY() / length, vector.GetZ() / length);
	return retVector;
}

float MyPI()
{
	return 3.14159f;
}

float MyRandF(int base, int max)
{
	float retVal = (float)(base + (rand() % (max - base))); /*integer component*/
	retVal += (1.0f + (rand() % 1000)) / 1000.0f; /*decimal component*/
	return retVal;
}

int MyRound(float num)
{
	float temp = num - (int)num;
	num += (temp >= 0.5f) ? 1.0f : 0.0f;
	return (int)num;
}