#ifndef HFILE_MATRIX
#define HFILE_MATRIX

#include <fstream>
#include <vector>
#include "General.h"


class Matrix
{
public:
	/*
	* Constructor(s)
	*/
	Matrix(unsigned int numRows = 2, unsigned int numColumns = 3);

	/*
	* Accessors
	*/
	Matrix *GetInverse() const; /*Returns NULL if no inverse exists.*/
private:
	Matrix *GetInverseOf4x4() const; /*Returns NULL if no inverse exists.*/
public:
	Matrix GetTranspose() const;
	Matrix *GetMinor(unsigned int rowIndex, unsigned int columnIndex) const;
	Matrix *GetCofactorMatrix() const;
	Matrix *GetAdjoint() const;
	float *GetDeterminant() const;
	static Matrix GetGarbageMatrix(unsigned int numRows, unsigned int numColumns); /*Returns a matrix filled with garbage values.
														    Used as a return value for certain functions that
															fail.*/
	float Get(unsigned int row, unsigned int column) const;
	unsigned int GetNumRows() const;
	unsigned int GetNumColumns() const;
	void Display() const; /*Outputs to console.*/

	/*
	* Mutators
	*/
	void Insert(unsigned int rowIndex, unsigned int columnIndex, float insertNum);

	/*
	* Overloaded operators
	*/
	void operator=(const Matrix &right);
	friend Matrix operator+(const Matrix &left, const Matrix &right);
	friend Matrix operator-(const Matrix &left, const Matrix &right);
	friend Matrix operator*(const Matrix &left, const Matrix &right);
	friend Matrix operator*(const Matrix &left, float multiplier);
	friend Matrix operator*(float multiplier, const Matrix &right);
	friend Matrix operator/(const Matrix &left, float divisor);
	friend bool operator==(const Matrix &left, const Matrix &right);
	friend bool operator!=(const Matrix &left, const Matrix &right);
	friend Matrix operator+=(const Matrix &left, const Matrix &right);
	friend Matrix operator-=(const Matrix &left, const Matrix &right);
	friend Matrix operator*=(const Matrix &left, const Matrix &right);
	friend Matrix operator*=(const Matrix &left, float multiplier);
	friend Matrix operator/=(const Matrix &left, float divisor);
	friend std::ostream& operator<<(std::ostream &out, const Matrix &mat);

private:
	std::vector<std::vector<float>> mMatrix; /*Outter vec: rows, Inner vec: columns*/
	unsigned int mNumRows;
	unsigned int mNumColumns;
};



/*
* Global function prototypes
*/
void TestMatrixClass();
Vector3F CrossProduct(const Vector3F &left, const Vector3F &right);



#endif /*HFILE_MATRIX*/