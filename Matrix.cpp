#include <iostream>
#include <fstream>
#include "Matrix.h"





/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Matrix
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Matrix::Matrix(unsigned int numRows, unsigned int numColumns)
{
	mNumRows = numRows;
	mNumColumns = numColumns;

	/*Initialize the matrix to be filled with zeros.*/
	for (unsigned int row = 0; row < numRows; row++)
	{
		std::vector<float> tempRow;
		mMatrix.push_back(tempRow);
		for (unsigned int column = 0; column < numColumns; column++)
		{
			mMatrix[row].push_back(0.0f);
		}
	}
}

/*
* Accessors
*/
/*Returns NULL if no inverse exists.*/
Matrix *Matrix::GetInverse() const
{
	/*If this matrix is not a square matrix, return NULL.*/
	if (mNumRows != mNumColumns)
		return NULL;

	/*If the determinate of this matrix is zero, return NULL since no inverse exists.*/
	float determinate = *GetDeterminant();
	if (determinate == 0)
		return NULL;

	/*If this is a 4x4 matrix, get the inverse by using the RREF transformation of the identity matrix.*/
	if (mNumRows == 4)
		return GetInverseOf4x4();

	/*Otherwise, get the inverse of this matrix using the adjoint method.*/
	Matrix *inverse = new Matrix();
	*inverse = Matrix((1 / determinate) * *GetAdjoint());
	return inverse;
}

/*Returns NULL if no inverse exists.*/
Matrix *Matrix::GetInverseOf4x4() const
{
	/*Initialize the identity matrix.*/
	Matrix *inverseMatrix = new Matrix(4, 4);
	inverseMatrix->Insert(0, 0, 1);
	inverseMatrix->Insert(1, 1, 1);
	inverseMatrix->Insert(2, 2, 1);
	inverseMatrix->Insert(3, 3, 1);

	/*Create a copy of this matrix.*/
	Matrix copy = *this;

	/*Get copy into REF, and apply each of the transformations to inverseMatrix.*/
	for (unsigned int diagonalIndex = 0; diagonalIndex < 4; diagonalIndex++)
	{
		/*Make the current number along the diagonal a 1.*/
		float temp = copy.Get(diagonalIndex, diagonalIndex);
		if (temp != 1)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				copy.Insert(diagonalIndex, column, copy.Get(diagonalIndex, column) / temp);
				inverseMatrix->Insert(diagonalIndex, column, inverseMatrix->Get(diagonalIndex, column) / temp);
			}
		}
		int x = 5;

		/*Make the numbers in the column under the current number along the diagonal zeros.*/
		if (diagonalIndex + 1 < 4)
		{
			for (unsigned int row = diagonalIndex + 1; row < 4; row++)
			{
				temp = -1 * copy.Get(row, diagonalIndex);
				if (temp != 0)
				{
					for (unsigned int column = 0; column < 4; column++)
					{
						copy.Insert(row, column, (copy.Get(diagonalIndex, column) * temp) + copy.Get(row, column));
						inverseMatrix->Insert(row, column, (inverseMatrix->Get(diagonalIndex, column) * temp) + inverseMatrix->Get(row, column));
					}
				}
				int y = 5;
			}
		}
	}

	/*Get the copy matrix into RREF, and apply each of the transformations to inverseMatrix.*/
	for (int diagonalIndex = 3; diagonalIndex >= 0; diagonalIndex--)
	{
		/*Make the numbers in the column above the current number along the diagonal zeros.*/
		if (diagonalIndex - 1 >= 0)
		{	
			for (int row = diagonalIndex - 1; row >= 0; row--)
			{
				float temp = -1 * copy.Get((unsigned int)row, (unsigned int)diagonalIndex);
				if (temp != 0)
				{
					for (int column = 3; column >= 0; column--)
					{
						copy.Insert((unsigned int)row, (unsigned int)column, (copy.Get((unsigned int)diagonalIndex, (unsigned int)column) * temp) + copy.Get((unsigned int)row, (unsigned int)column));
						inverseMatrix->Insert((unsigned int)row, (unsigned int)column, (inverseMatrix->Get((unsigned int)diagonalIndex, (unsigned int)column) * temp) + inverseMatrix->Get((unsigned int)row, (unsigned int)column));
					}
				}
			}
		}
	}

	return inverseMatrix;
}

Matrix Matrix::GetTranspose() const
{
	Matrix *ret = new Matrix(mNumColumns, mNumRows);

	for (unsigned int rowOfOriginalMatrix = 0; rowOfOriginalMatrix < mNumRows; rowOfOriginalMatrix++)
		for (unsigned int columnOfOriginalMatrix = 0; columnOfOriginalMatrix < mNumColumns; columnOfOriginalMatrix++)
			ret->mMatrix[columnOfOriginalMatrix][rowOfOriginalMatrix] = mMatrix[rowOfOriginalMatrix][columnOfOriginalMatrix];

	return *ret;
}

Matrix *Matrix::GetMinor(unsigned int rowIndex, unsigned int columnIndex) const
{
	/*If this matrix is not a square matrix, return NULL.*/
	if (mNumRows != mNumColumns)
		return NULL;

	/*If this matrix is a 0x0 or a 1x1 matrix, return NULL.*/
	if (mNumRows == 0 || mNumRows == 1)
		return  NULL;

	/*If either of the given indeces are out of bounds, return NULL.*/
	if (rowIndex >= mNumRows || columnIndex >= mNumColumns)
		return NULL;

	Matrix *ret = new Matrix(mNumRows - 1, mNumColumns - 1);
	for (unsigned int row = 0; row < mNumRows - 1; row++)
	{
		for (unsigned int column = 0; column < mNumColumns - 1; column++)
		{
			unsigned int extractRowIndex = (row >= rowIndex) ? row + 1 : row;
			unsigned int extractColumnIndex = (column >= columnIndex) ? column + 1 : column;
			ret->mMatrix[row][column] = mMatrix[extractRowIndex][extractColumnIndex];
		}
	}

	return ret;
}

Matrix *Matrix::GetCofactorMatrix() const
{
	/*If this matrix is not a square matrix, return NULL.*/
	if (mNumRows != mNumColumns)
		return NULL;

	Matrix *ret = new Matrix(mNumRows, mNumColumns);

	for (unsigned int row = 0; row < mNumRows; row++)
	{
		for (unsigned int column = 0; column < mNumColumns; column++)
		{
			int sign = ((row + column) % 2 == 0) ? 1 : -1;
			ret->mMatrix[row][column] = sign * *(GetMinor(row, column)->GetDeterminant());
		}
	}

	return ret;
}

Matrix *Matrix::GetAdjoint() const
{
	/*If this matrix is not a square matrix, return NULL.*/
	if (mNumRows != mNumColumns)
		return NULL;

	Matrix *ret = new Matrix();
	*ret = GetCofactorMatrix()->GetTranspose();

	return ret;
}

float *Matrix::GetDeterminant() const
{
	/*If this matrix is not a square matrix, return NULL.*/
	if (mNumRows != mNumColumns)
		return NULL;

	float *retVal = new float;
	*retVal = 0.0f;

	/*Base case: A 2x2 matrix.*/
	if (mNumRows == 2)
	{
		*retVal = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
		return retVal;
	}
	
	/*Recursive case: If N > 2, where this is an NxN matrix.*/
	for (unsigned int column = 0; column < mNumColumns; column++)
	{
		Matrix minor = *GetMinor(0, column);
		int sign = (column % 2 == 0) ? 1 : -1;
		*retVal += sign * mMatrix[0][column] * *(minor.GetDeterminant());
	}

	return retVal;
}

/*Returns a matrix filled with garbage values. Used as a return value for certain functions that fail.*/
Matrix Matrix::GetGarbageMatrix(unsigned int numRows, unsigned int numColumns)
{
	if (numRows < 0) numRows = 2;
	if (numColumns < 0) numColumns = 3;

	Matrix *retMat = new Matrix(numRows, numColumns);
	for (unsigned int row = 0; row < numRows; row++)
		for (unsigned int column = 0; column < numColumns; column++)
			retMat->mMatrix[row][column] = -999.0f;

	return *retMat;
}

float Matrix::Get(unsigned int row, unsigned int column) const
{
	if (row >= mNumRows) row = mNumRows - 1;
	if (column >= mNumColumns) column = mNumColumns - 1;

	return mMatrix[row][column];
}

unsigned int Matrix::GetNumRows() const
{
	return mNumRows;
}

unsigned int Matrix::GetNumColumns() const
{
	return mNumColumns;
}

void Matrix::Display() const
{
	for (unsigned int row = 0; row < mNumRows; row++)
	{
		for (unsigned int column = 0; column < mNumColumns; column++)
		{
			std::cout << mMatrix[row][column] << " ";
		}
		std::cout << "\n";
	}
}

/*
* Mutators
*/
void Matrix::Insert(unsigned int rowIndex, unsigned int columnIndex, float insertNum)
{
	if (rowIndex < 0) rowIndex = 0;
	else if (rowIndex >= mNumRows) rowIndex = mNumRows - 1;

	if (columnIndex < 0) columnIndex = 0;
	else if (columnIndex >= mNumColumns) columnIndex = mNumColumns - 1;

	mMatrix[rowIndex][columnIndex] = insertNum;
}

/*
* Overloaded operators
*/
void Matrix::operator=(const Matrix &right)
{
	mMatrix.clear();
	
	mNumRows = right.mNumRows;
	mNumColumns = right.mNumColumns;

	

	for (unsigned int row = 0; row < right.mNumRows; row++)
	{
		std::vector<float> tempRow;
		mMatrix.push_back(tempRow);
		for (unsigned int column = 0; column < right.mNumColumns; column++)
		{
			//float *tempPtr = new float;
			mMatrix[row].push_back(right.mMatrix[row][column]);
			//Insert(row, column, right.mMatrix[row][column]);
		}
	}
}

Matrix operator+(const Matrix &left, const Matrix &right)
{
	/*If the dimensions of both matrices don't match, return a garbage matrix.*/
	if (left.mNumRows != right.mNumRows || left.mNumColumns != right.mNumColumns)
		return Matrix::GetGarbageMatrix(2, 3);

	Matrix *retMat = new Matrix();
	*retMat = left;
	for (unsigned int row = 0; row < right.mNumRows; row++)
		for (unsigned int column = 0; column < right.mNumColumns; column++)
			retMat->mMatrix[row][column] += right.mMatrix[row][column];

	return *retMat;
}

Matrix operator-(const Matrix &left, const Matrix &right)
{
	/*If the dimensions of both matrices don't match, return a garbage matrix.*/
	if (left.mNumRows != right.mNumRows || left.mNumColumns != right.mNumColumns)
		return Matrix::GetGarbageMatrix(2, 3);

	Matrix *retMat = new Matrix();
	*retMat = left;
	for (unsigned int row = 0; row < right.mNumRows; row++)
		for (unsigned int column = 0; column < right.mNumColumns; column++)
			retMat->mMatrix[row][column] -= right.mMatrix[row][column];

	return *retMat;
}

Matrix operator*(const Matrix &left, const Matrix &right)
{
	/*If the dimensions of both matrices don't match, return a garbage matrix.*/
	if (left.mNumColumns != right.mNumRows)
		return Matrix::GetGarbageMatrix(2, 3);

	Matrix *retMat = new Matrix(left.mNumRows, right.mNumColumns);
	for (unsigned int matrix1Row = 0; matrix1Row < left.mNumRows; matrix1Row++)
	{
		for (unsigned int matrix2Column = 0; matrix2Column < right.mNumColumns; matrix2Column++)
		{
			float sum = 0;
			for (unsigned int index = 0; index < left.mNumColumns; index++)
			{
				sum += left.mMatrix[matrix1Row][index] * right.mMatrix[index][matrix2Column];
			}
			retMat->mMatrix[matrix1Row][matrix2Column] = sum;
		}
	}

	return *retMat;
}

Matrix operator*(const Matrix &left, float multiplier)
{
	Matrix *retMat = new Matrix();
	*retMat = left;
	for (unsigned int row = 0; row < left.mNumRows; row++)
		for (unsigned int column = 0; column < left.mNumColumns; column++)
			retMat->mMatrix[row][column] *= multiplier;

	return *retMat;
}

Matrix operator*(float multiplier, const Matrix &right)
{
	Matrix *retMat = new Matrix();
	*retMat = right;
	for (unsigned int row = 0; row < right.mNumRows; row++)
		for (unsigned int column = 0; column < right.mNumColumns; column++)
			retMat->mMatrix[row][column] *= multiplier;

	return *retMat;
}

Matrix operator/(const Matrix &left, float divisor)
{
	Matrix *retMat = new Matrix();
	*retMat = left;
	for (unsigned int row = 0; row < left.mNumRows; row++)
		for (unsigned int column = 0; column < left.mNumColumns; column++)
			retMat->mMatrix[row][column] /= divisor;

	return *retMat;
}

bool operator==(const Matrix &left, const Matrix &right)
{
	/*If the dimensions of both matrices don't match, return false.*/
	if (left.mNumRows != right.mNumRows || left.mNumColumns != right.mNumColumns)
		return false;

	for (unsigned int row = 0; row < right.mNumRows; row++)
		for (unsigned int column = 0; column < right.mNumColumns; column++)
			if (left.mMatrix[row][column] != right.mMatrix[row][column])
				return false;

	return true;
}

bool operator!=(const Matrix &left, const Matrix &right)
{
	return !(left == right);
}

void operator+=(Matrix &left, const Matrix &right)
{
	left = left + right;
}

void operator-=(Matrix &left, const Matrix &right)
{
	left = left - right;
}

void operator*=(Matrix &left, const Matrix &right)
{
	left = left * right;
}

void operator*=(Matrix &left, float multiplier)
{
	left = left * multiplier;
}

void operator/=(Matrix &left, float divisor)
{
	left = left / divisor;
}

std::ostream& operator<<(std::ostream &out, const Matrix &mat)
{
	for (unsigned int row = 0; row < mat.mNumRows; row++)
	{
		for (unsigned int column = 0; column < mat.mNumColumns; column++)
		{
			out << mat.mMatrix[row][column] << " ";
		}
		out << "\n";
	}

	return out;
}



/*
* Global functions
*/
void TestMatrixClass()
{
	///*Test constructor.*/
	//std::cout << "Testing constructor..." << "\n";
	//Matrix testMatrix = Matrix(2, 3);
	//std::cout << testMatrix;
	
	///*Test GetNumRows().*/
	//std::cout << "Testing GetNumRows()..." << "\n";
	//std::cout << testMatrix.GetNumRows() << "\n";

	///*Test GetNumColumns().*/
	//std::cout << "Testing GetNumColumns()..." << "\n";
	//std::cout << testMatrix.GetNumColumns() << "\n";

	Matrix squareMatrix = Matrix(4, 4);
	/*squareMatrix.Insert(0, 0, 5);
	squareMatrix.Insert(0, 1, 3);
	squareMatrix.Insert(0, 2, -2);
	squareMatrix.Insert(0, 3, -1);
	squareMatrix.Insert(1, 0, 1);
	squareMatrix.Insert(1, 1, 0);
	squareMatrix.Insert(1, 2, 3);
	squareMatrix.Insert(1, 3, -2);
	squareMatrix.Insert(2, 0, -3);
	squareMatrix.Insert(2, 1, 2);
	squareMatrix.Insert(2, 2, 3);
	squareMatrix.Insert(2, 3, -1);
	squareMatrix.Insert(3, 0, 0);
	squareMatrix.Insert(3, 1, 2);
	squareMatrix.Insert(3, 2, 4);
	squareMatrix.Insert(3, 3, 5);*/
	squareMatrix.Insert(0, 0, 1);
	squareMatrix.Insert(0, 1, 2);
	squareMatrix.Insert(0, 2, 3);
	squareMatrix.Insert(0, 3, 4);
	squareMatrix.Insert(1, 0, 0);
	squareMatrix.Insert(1, 1, 4);
	squareMatrix.Insert(1, 2, 5);
	squareMatrix.Insert(1, 3, 5);
	squareMatrix.Insert(2, 0, 1);
	squareMatrix.Insert(2, 1, 0);
	squareMatrix.Insert(2, 2, 6);
	squareMatrix.Insert(2, 3, 6);
	squareMatrix.Insert(3, 0, -3);
	squareMatrix.Insert(3, 1, -2);
	squareMatrix.Insert(3, 2, -1);
	squareMatrix.Insert(3, 3, 7);
	

	/*Test GetTranspose().*/
	std::cout << "Transpose is" << "\n" << squareMatrix.GetTranspose() << "\n";

	/*Test GetMinor(., .).*/
	std::cout << "Minor is" << "\n" << *(squareMatrix.GetMinor(2, 1)) << "\n";

	/*Test GetCofactorMatrix().*/
	std::cout << "Cofactor is" << "\n" << *(squareMatrix.GetCofactorMatrix()) << "\n";

	/*Test GetAdjoint().*/
	std::cout << "Adjoint is" << "\n" << *(squareMatrix.GetAdjoint()) << "\n";

	/*Test GetDeterminant().*/
	std::cout << "Determinant is" << "\n" << *(squareMatrix.GetDeterminant()) << "\n";

	/*Test GetInverse().*/
	std::cout << "Inverse is " << "\n" << *(squareMatrix.GetInverse()) << "\n";

	///*Test GetGarbageMatrix(., .).*/
	//std::cout << "Testing GetGarbageMatrix(., .)..." << "\n";
	//Matrix::GetGarbageMatrix(1, 6).Display();

	///*Test Insert(.).*/
	//std::cout << "Testing Insert(.)..." << "\n";
	//for (unsigned int row = 0; row < testMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < testMatrix.GetNumColumns(); column++)
	//		testMatrix.Insert(row, column, (float)(row*column + column));
	//std::cout << testMatrix << "\n";

	///*Test overloaded =operator.*/
	//std::cout << "Testing =operator..." << "\n";
	//Matrix anotherTestMatrix;
	//anotherTestMatrix = testMatrix;
	//std::cout << anotherTestMatrix << "\n";

	///*Test overloaded +operator.*/
	//std::cout << "Testing +operator..." << "\n";
	//for (unsigned int row = 0; row < testMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < testMatrix.GetNumColumns(); column++)
	//		anotherTestMatrix.Insert(row, column, 4);
	//testMatrix = testMatrix + anotherTestMatrix;
	//std::cout << testMatrix << "\n";

	///*Test overloaded -operator.*/
	//std::cout << "Testing -operator..." << "\n";
	//testMatrix = testMatrix - anotherTestMatrix;
	//std::cout << testMatrix << "\n";

	///*Test overloaded *operator.*/
	//std::cout << "Testing *operator..." << "\n";
	//Matrix yetAnotherTestMatrix = Matrix(testMatrix.GetNumColumns(), 10);
	//for (unsigned int row = 0; row < yetAnotherTestMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < yetAnotherTestMatrix.GetNumColumns(); column++)
	//		yetAnotherTestMatrix.Insert(row, column, 7);
	//yetAnotherTestMatrix = testMatrix * yetAnotherTestMatrix;
	//std::cout << yetAnotherTestMatrix << "\n";
	//testMatrix = testMatrix * 2;
	//std::cout << testMatrix << "\n";
	//testMatrix = 2 * testMatrix;
	//std::cout << testMatrix << "\n";

	///*Test overloaded /operator.*/
	//std::cout << "Testing /operator..." << "\n";
	//testMatrix = testMatrix / 4;
	//std::cout << testMatrix << "\n";

	///*Test overloaded ==operator.*/
	//std::cout << "Testing ==operator..." << "\n";
	//testMatrix = anotherTestMatrix;
	//std::cout << (testMatrix == anotherTestMatrix) << "\n";

	///*Test overloaded !=operator.*/
	//std::cout << "Testing !=operator..." << "\n";
	//std::cout << (testMatrix != anotherTestMatrix) << "\n";

	///*Test overloaded +=operator.*/
	//std::cout << "Testing +=operator..." << "\n";
	//testMatrix += testMatrix;
	//std::cout << testMatrix << "\n";

	///*Test overloaded -=operator.*/
	//std::cout << "Testing -=operator..." << "\n";
	//testMatrix -= testMatrix;
	//std::cout << testMatrix << "\n";

	///*Test overloaded *=operator.*/
	//std::cout << "Testing *=operator..." << "\n";
	//for (unsigned int row = 0; row < testMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < testMatrix.GetNumColumns(); column++)
	//		testMatrix.Insert(row, column, (float)(row*column + column));
	//yetAnotherTestMatrix = Matrix(testMatrix.GetNumColumns(), 10);
	//for (unsigned int row = 0; row < yetAnotherTestMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < yetAnotherTestMatrix.GetNumColumns(); column++)
	//		yetAnotherTestMatrix.Insert(row, column, 7);
	//testMatrix *= yetAnotherTestMatrix;
	//std::cout << testMatrix << "\n";
	//testMatrix *= 2;
	//std::cout << testMatrix << "\n";

	///*Test overloaded /=operator.*/
	//std::cout << "Testing /=operator..." << "\n";
	//for (unsigned int row = 0; row < testMatrix.GetNumRows(); row++)
	//	for (unsigned int column = 0; column < testMatrix.GetNumColumns(); column++)
	//		testMatrix.Insert(row, column, (float)(rand() % 11));
	//std::cout << testMatrix << "\n";
	//testMatrix /= 3;
	//std::cout << testMatrix << "\n";
}

Vector3F CrossProduct(const Vector3F &left, const Vector3F &right)
{
	Vector3F *retVal = new Vector3F();
	retVal->SetX(left.GetY()*right.GetZ() - left.GetZ()*right.GetY());
	retVal->SetY(left.GetZ()*right.GetX() - left.GetX()*right.GetZ());
	retVal->SetZ(left.GetX()*right.GetY() - left.GetY()*right.GetX());
	return *retVal;
}
