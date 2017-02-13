#ifndef HFILE_MYMATH
#define HFILE_MYMATH

#include <cmath>
#include <cstdlib>
#include <vector>
#include "General.h"


/*
 * Global function prototypes
 */
void TestTrigThetaSign();
void InitPascalsTriangle(); /*Call this function if the code uses Pascal's Triangle at all.
							  While values of the triangle can be computed at runtime, this
							  function calculates the first n rows, where n is arbitrarily
							  large, at the start of the program to help speed up dynamic
							  graphics calculations later*/

unsigned long long MyFactorial(int num);
float MyPow(float base, int exponent);
float MyAbs(float num);
Vector3F Normalize(const Vector3F &vector);
float MyPI();
float MyRandF(int base, int max);
int MyRound(float num);



/*
* Global variables
*/
extern std::vector<std::vector<unsigned long long>> pascalsTriangle;


#endif /*HFILE_MYMATH*/