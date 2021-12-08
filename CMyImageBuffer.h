#pragma once

#include <afxwin.h>
#include "common/CommonDefine.h"
#include "inspectfunction/CInspectFunc.h"
#include "KjwMatrix.h"
#include <vector>




class CMyImageBuffer
{
public:

	CMyImageBuffer() : top(0), bottom(0), left(0), right(0), width(0), height(0) , m_2DMatrix(0,0), m_2DMatrixtemp(0,0)
	{
		
	}
	
	virtual ~CMyImageBuffer() = default;

private:

	int top;
	int bottom;
	int left;
	int right;
	int width;
	int height;

	KjwMatrix< BYTE > m_2DMatrix;
	KjwMatrix< BYTE > m_2DMatrixtemp;

	std::vector< std::vector<BYTE> > v2DImage;
	std::vector< std::vector<BYTE> > v2DTemp;
	

	

	double* dLUT_SIN;
	double* dLUT_COS;

public:

	void setPosition(int _left, int _right, int _top, int _bottom);
	void Input2DvectorImage(LPBYTE pSrc);
	auto get2DVectorptr() const { return v2DImage; }
	void deleteBuffer();
	void output2DvectorInstanceImage(LPBYTE pDst);
	void output2DVectorImage(LPBYTE pDst);

	// filter
	void binary(int th, int mode);
	void dilate();
	void erode();
	void gausianmean();
	void laplacian();
	void medianfilter();
	void sobelfilter(int direction);

	// algorithm
	void createhoughLookuptable();
	void deleteLookuptable();
	void houghlinefit(int _mode, int _dir, double _slope, double& a, double& b);
	bool edgefindhorizontal(std::vector<double>* vdataPtr, int _dir, int _indexY, double _slope, double& objectpixel); // 수평방향
	bool edgefindvertical(std::vector<double>* vdataPtr, int _dir, int _indexY, double _slope, double &objectpixel); // 수직방향
};
