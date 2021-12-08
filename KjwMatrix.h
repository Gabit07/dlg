#pragma once

#include <vector>
#include <new>

template<class T>
class KjwMatrix
{
public:

	KjwMatrix(int _row, int _col) : m2DVector(_row * _col, 0)
	{
		mRow = _row;
		mCol = _col;
	}
	
	void operator<=(const KjwMatrix& lhs)
	{
		m2DVector = lhs.m2DVector;
	}

	T& operator() (int _y, int _x)
	{
		const int iIdex = _y * mCol + _x;
		return m2DVector[iIdex];
	}

	int getsize() const { return mRow * mCol; }
	auto getMatrixPtr() const { return m2DVector; }
	
private:
	std::vector<T> m2DVector;
	int mRow;
	int mCol;
};

