#include "pch.h"
#include "CMyImageBuffer.h"
#include "common/configure.h"
#include "common/CommonUtil.h"
#include <algorithm>


void CMyImageBuffer::setPosition(int _left, int _right, int _top, int _bottom) 
{
	left = _left;
	right = _right;
	top = _top;
	bottom = _bottom;

	width = right - left;
	height = bottom - top;

	m_2DMatrix = KjwMatrix< BYTE >(height, width);
	m_2DMatrixtemp = KjwMatrix< BYTE >(height, width);
}

void CMyImageBuffer::Input2DvectorImage(LPBYTE pSrc) 
{

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			m_2DMatrix(j, i) = *(pSrc + width * j + i);
		}

	}
	
}

void CMyImageBuffer::output2DvectorInstanceImage(LPBYTE pDst)
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			*(pDst + width * j + i) = m_2DMatrix(j, i);
		}
	}
	
}

void CMyImageBuffer::output2DVectorImage(LPBYTE pDst) {

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			*(pDst + IMAGE_BUFFER_SIZE_X * (top + j) + (left + i)) = m_2DMatrix(j,i);
		}
	}

}

void CMyImageBuffer::binary(int th, int mode) 
{
	
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (mode == FIND_LOW)
			{
				if (m_2DMatrix(j,i) < th)
				{
					m_2DMatrix(j, i) = OBJECT_VAL;
				}
				else
				{
					m_2DMatrix(j, i) = BACKGROUND_VAL;
				}
			}
			else
			{
				if (m_2DMatrix(j, i) > th)
				{
					m_2DMatrix(j, i) = OBJECT_VAL;
				}
				else
				{
					m_2DMatrix(j, i) = BACKGROUND_VAL;
				}
			}
		}
	}

	
	/*for (std::size_t j = 0; j < v2DImage.size(); j++)
	{
		for (std::size_t i = 0; i < v2DImage[j].size(); i++)
		{
			if (mode == FIND_LOW)
			{
				if (v2DImage[j][i] < th)
				{
					v2DImage[j][i] = OBJECT_VAL;
				}
				else
				{
					v2DImage[j][i] = BACKGROUND_VAL;
				}
			}
			else
			{
				if (v2DImage[j][i] > th)
				{
					v2DImage[j][i] = OBJECT_VAL;
				}
				else
				{
					v2DImage[j][i] = BACKGROUND_VAL;
				}
			}
			
		}
	}*/
}

void CMyImageBuffer::dilate()
{
	for (int j = 1; j < height-1; j++)
	{
		for (int i = 1; i < width-1; i++)
		{
			if (m_2DMatrix(j-1, i-1) + m_2DMatrix(j-1, i) + m_2DMatrix(j-1, i+1) +
				m_2DMatrix(j, i-1) + m_2DMatrix(j, i) + m_2DMatrix(j, i+1) +
				m_2DMatrix(j-1, i-1) + m_2DMatrix(j-1, i) + m_2DMatrix(j-1, i+1))
			{
				m_2DMatrixtemp(j,i) = OBJECT_VAL;
			}
			else
			{
				m_2DMatrixtemp(j, i) = BACKGROUND_VAL;
			}
		}
	}

	m_2DMatrix <= m_2DMatrixtemp;

	m_2DMatrixtemp.getMatrixPtr().clear();

	/*v2DTemp.assign(height, std::vector<BYTE>(width, 0));

	for (std::size_t j = 1; j < v2DImage.size()-1; j++)
	{
		for (std::size_t i = 1; i < v2DImage[j].size()-1; i++)
		{
			if (v2DImage[j - 1][i - 1] + v2DImage[j - 1][i] + v2DImage[j - 1][i + 1] +
				v2DImage[j][i - 1] + v2DImage[j][i] + v2DImage[j][i + 1] +
				v2DImage[j - 1][i - 1] + v2DImage[j - 1][i] + v2DImage[j - 1][i + 1])
			{
				v2DTemp[j][i] = OBJECT_VAL;
			}	
			else
			{
				v2DTemp[j][i] = BACKGROUND_VAL;
			}
		}
	}

	std::copy(v2DTemp.begin(), v2DTemp.end(), v2DImage.begin());

	for (std::size_t j = 0; j < v2DTemp.size(); j++)
		v2DTemp[j].clear();
	v2DTemp.clear();*/

}

void CMyImageBuffer::erode()
{

	for (int j = 1; j < height-1; j++)
	{
		for (int i = 1; i < width-1; i++)
		{
			if (m_2DMatrix(j - 1, i - 1) == 0 || m_2DMatrix(j - 1, i) == 0 || m_2DMatrix(j - 1, i + 1) == 0 ||
				m_2DMatrix(j+1, i-1) == 0 || m_2DMatrix(j+1, i) == 0 || m_2DMatrix(j+1, i+1) == 0)
			{
				m_2DMatrixtemp(j,i) = BACKGROUND_VAL;
			}
			else
			{
				m_2DMatrixtemp(j, i) = OBJECT_VAL;
			}

		}
	}

	m_2DMatrix <= m_2DMatrixtemp;

	m_2DMatrixtemp.getMatrixPtr().clear();

	/*v2DTemp.assign(height, std::vector<BYTE>(width, 0));

	for (std::size_t j = 1; j < v2DImage.size() - 1; j++)
	{
		for (std::size_t i = 1; i < v2DImage[j].size() - 1; i++)
		{
			if (v2DImage[j - 1][i - 1] == 0 || v2DImage[j - 1][i]==0 || v2DImage[j - 1][i + 1]== 0 ||
				v2DImage[j + 1][i - 1] ==0 || v2DImage[j + 1][i]==0 || v2DImage[j + 1][i + 1]==0)
			{
				v2DTemp[j][i] = BACKGROUND_VAL;
			}
			else
			{
				v2DTemp[j][i] = OBJECT_VAL;
			}
		}
	}

	std::copy(v2DTemp.begin(), v2DTemp.end(), v2DImage.begin());

	for (std::size_t j = 0; j < v2DTemp.size(); j++)
		v2DTemp[j].clear();
	v2DTemp.clear();*/
}

const int GausianMeanMask[3][3] = { {1,1,1},{1,1,1},{1,1,1} }; // x1/8
const int GausianWeightMask[3][3] = { {1,2,1},{2,4,2},{1,2,1} }; // x1/16

void CMyImageBuffer::gausianmean()
{
	int gray = 0;

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			gray = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					gray += m_2DMatrix(j + masky, i + maskx) * GausianMeanMask[masky + 1][maskx + 1];
				}
			}
			gray /= 8;
			if (gray > 255) gray = 255;
			m_2DMatrixtemp(j,i) = gray;
		}
	}

	m_2DMatrix <= m_2DMatrixtemp;

	m_2DMatrixtemp.getMatrixPtr().clear();

	/*v2DTemp.assign(height, std::vector<BYTE>(width, 0));

	for (std::size_t j = 1; j < v2DImage.size() - 1; j++)
	{
		for (std::size_t i = 1; i < v2DImage[j].size() - 1; i++)
		{
			gray = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					gray += v2DImage[j + masky][i + maskx] * GausianMeanMask[masky + 1][maskx + 1];
				}
			}
			gray /= 8;
			if (gray > 255) gray = 255;
			v2DTemp[j][i] = gray;
		}
		
	}

	std::copy(v2DTemp.begin(), v2DTemp.end(), v2DImage.begin());

	for (std::size_t j = 0; j < v2DTemp.size(); j++)
		v2DTemp[j].clear();
	v2DTemp.clear();*/
	
}

/************************************************************************/
/* negative 중앙값은 원 영상에 - 함                                                                     */
const int NegativeLaplaMask[3][3] = { {0,1,0},{1,-4,1},{0,1,0} };  // x,y 방향 엣지
const int NegativeLaplaMask2[3][3] = { {1,1,1},{1,-8,1},{1,1,1} }; // x,y,대각선 방향 엣지

/************************************************************************/
/************************************************************************/
/* 중앙값은 원 영상에 + 함                                                                     */
const int LaplaMask[3][3] = { {0,-1,0},{-1,4,-1},{0,-1,0} };  // x,y 방향 엣지
const int LaplaMask2[3][3] = { {-1,-1,-1},{-1,8,-1},{-1,-1,-1} }; // x,y,대각선 방향 엣지
/************************************************************************/
// 라플라시안은 가우시언 필터를 거쳐야 정상적인 값이 나옴

void CMyImageBuffer::laplacian()
{
	int val = 0;

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			val = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					val += m_2DMatrix(j + masky,i + maskx) * LaplaMask2[maskx + 1][masky + 1];
				}
			}

			m_2DMatrixtemp(j,i) = val;
		}
	}

	m_2DMatrix <= m_2DMatrixtemp;

	m_2DMatrixtemp.getMatrixPtr().clear();

	/*v2DTemp.assign(height, std::vector<BYTE>(width, 0));

	for (std::size_t j = 1; j < v2DImage.size()-1; j++)
	{
		for (std::size_t i = 1; i < v2DImage[j].size()-1; i++)
		{
			val = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					val += v2DImage[j + masky][i + maskx] * LaplaMask2[maskx + 1][masky + 1];
				}
			}

			v2DTemp[j][i] = val;

		}
	}

	std::copy(v2DTemp.begin(), v2DTemp.end(), v2DImage.begin());

	for (std::size_t j = 0; j < v2DTemp.size(); j++)
		v2DTemp[j].clear();
	v2DTemp.clear();*/

}

void CMyImageBuffer::medianfilter()
{
	int cnt = 0;
	int ngray = 0;
	int compare[9];
	ZeroMemory(compare, _countof(compare));

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			cnt = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					ngray = m_2DMatrix(j + masky,i + maskx);
					compare[cnt] = ngray;
					cnt++;
				}
			}
			std::qsort(compare, sizeof(compare) / sizeof(int), sizeof(int), CommonUtil::Compare);
			m_2DMatrixtemp(j,i) = compare[cnt / 2];
		}
	}

	m_2DMatrix <= m_2DMatrixtemp;

	m_2DMatrixtemp.getMatrixPtr().clear();

	
	/*v2DTemp.assign(height, std::vector<BYTE>(width, 0));

	for (std::size_t j = 1; j < v2DImage.size() - 1; j++)
	{
		for (std::size_t i = 1; i < v2DImage[j].size() - 1; i++)
		{
			cnt = 0;
			for (int masky = -1; masky <= 1; masky++)
			{
				for (int maskx = -1; maskx <= 1; maskx++)
				{
					ngray = v2DImage[j + masky][i + maskx];
					compare[cnt] = ngray;
					cnt++;
				}
			}
			std::qsort(compare, sizeof(compare) / sizeof(int), sizeof(int), CommonUtil::Compare);
			v2DTemp[j][i] = compare[cnt/2];

		}
	}

	std::copy(v2DTemp.begin(), v2DTemp.end(), v2DImage.begin());

	for (std::size_t j = 0; j < v2DTemp.size(); j++)
		v2DTemp[j].clear();
	v2DTemp.clear();*/
}

const int SobelMask_X[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
const int SobelMask_Y[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

void CMyImageBuffer::sobelfilter(int direction)
{
	KjwMatrix< BYTE > matrixSobel(height, width);
	KjwMatrix< BYTE > matrixSobelX(height, width);
	KjwMatrix< BYTE > matrixSobelY(height, width);

	if (direction == DirX || direction == DirXY)
	{
		for (int j = 1; j < height - 1; j++)
		{
			for (int i = 1; i < width - 1; i++)
			{
				int nDx = 0;

				for (int masky = -1; masky <= 1; masky++)
				{
					for (int maskx = -1; maskx <= 1; maskx++)
					{
						nDx += m_2DMatrix(j + masky, i + maskx) * SobelMask_X[maskx + 1][masky + 1];
					}
				}
				matrixSobelX(j,i) = nDx;
			}
		}
	}

	if (direction == DirY || direction == DirXY)
	{
		for (int j = 1; j < height - 1; j++)
		{
			for (int i = 1; i < width - 1; i++)
			{
				int nDy = 0;

				for (int masky = -1; masky <= 1; masky++)
				{
					for (int maskx = -1; maskx <= 1; maskx++)
					{
						nDy += m_2DMatrix(j + masky, i + maskx) * SobelMask_Y[maskx + 1][masky + 1];
					}
				}
				if (nDy < 0) nDy *= -1;
				matrixSobelY(j,i) = nDy;
			}
		}
	}
	
	if (direction == DirY)
	{
		matrixSobel <= matrixSobelY;
	}
	else if (direction == DirX)
	{
		matrixSobel <= matrixSobelX;
	}
	else
	{
		for (std::size_t j = 0; j < v2DImage.size(); j++)
		{
			for (std::size_t i = 0; i < v2DImage[j].size(); i++)
			{
				matrixSobel(j, i) = matrixSobelX(j, i) + matrixSobelY(j, i);
			}
		}
	}

	// 원영상 차
	/*for (std::size_t j = 0; j < v2DImage.size(); j++)
	{
		for (std::size_t i = 0; i < v2DImage[j].size(); i++)
		{
			if (v2DImage[j][i] - v2Sobel[j][i] > 0)
				v2DImage[j][i] -= v2Sobel[j][i];
			else
				v2DImage[j][i] = 0;
		}
	}*/


	m_2DMatrix <= matrixSobel;

	matrixSobelX.getMatrixPtr().clear();
	matrixSobelY.getMatrixPtr().clear();
	matrixSobel.getMatrixPtr().clear();

}




void CMyImageBuffer::createhoughLookuptable()
{
	float fRadian = float(PI / 180);

	dLUT_SIN = new double[_THETA];
	dLUT_COS = new double[_THETA];

	memset(dLUT_SIN, 0, sizeof(double) * _THETA);
	memset(dLUT_COS, 0, sizeof(double) * _THETA);

	for (int i = 0; i < _THETA; i++)
	{
		dLUT_SIN[i] = sin(i * fRadian);
		dLUT_COS[i] = cos(i * fRadian);
	}

}

void CMyImageBuffer::deleteLookuptable()
{
	if (dLUT_SIN)
		SAFE_DELETE(dLUT_SIN);
	if (dLUT_COS)
		SAFE_DELETE(dLUT_COS);
}

bool CMyImageBuffer::edgefindvertical(std::vector<double>* vdataPtr, int _dir, int _indexY, double _slope, double& objectpixel)
{
	int edgecnt = 0;
	int nMaxPos = 0;
	double _dslope = 0, dMaxSlope = 0;

	if (_dir == (int)CEdgeDirection::toBottom)
	{
		for (int y = 0; y < height; y++)
		{
			vdataPtr->emplace_back((double)m_2DMatrix(y, _indexY));
			edgecnt++;
		}

		for (size_t n = 2; n < edgecnt - 1; n++)
		{
			_dslope = vdataPtr->at(n+1) - vdataPtr->at(n-1);
			if (fabs(_dslope) >= _slope)
			{
				if (nMaxPos < n)
				{
					nMaxPos = n;
					dMaxSlope = _dslope;
					break;
				}
			}
		}
	}
	else if(_dir == (int)CEdgeDirection::toTop)
	{
		edgecnt = height;

		for (int y = height - 2; y > 1; y--)
		{
			vdataPtr->emplace_back((double)m_2DMatrix(y, _indexY));
		}

		std::reverse(vdataPtr->begin(), vdataPtr->end());
		
		for (size_t n = vdataPtr->size() -2; n > 1; n--)
		{
			_dslope = vdataPtr->at(n+1) - vdataPtr->at(n-1);
			if (fabs(_dslope) >= _slope)
			{
				if (nMaxPos < n)
				{
					nMaxPos = n;
					dMaxSlope = _dslope;
					break;
				}
			}
		}
	}
	else {
		ASSERT(0);
	}

	if (dMaxSlope == 0)
	{
		return false;
	}

	if (nMaxPos <= 2 || nMaxPos >= edgecnt - 2)
	{
		objectpixel = nMaxPos;
	}
	else
	{
		/*if (fabs(vdataPtr->at(nMaxPos+2) - vdataPtr->at(nMaxPos)) < fabs(vdataPtr->at(nMaxPos) - vdataPtr->at(nMaxPos-2)))
		{
			nMaxPos -= 1;
		}
		else if (vdataPtr->size() <= nMaxPos + 3)
		{
			objectpixel = (int)nMaxPos;
		}
		else
		{
			double Temp_A = (double)fabs(vdataPtr->at(nMaxPos + 2) + (vdataPtr->at(nMaxPos - 2)) - 2 * (vdataPtr->at(nMaxPos)));
			double Temp_B = (double)fabs(vdataPtr->at(nMaxPos + 3) + (vdataPtr->at(nMaxPos - 1)) - 2 * (vdataPtr->at(nMaxPos + 1)));

			if ((Temp_A + Temp_B) != 0)
				objectpixel = (int)(nMaxPos + Temp_A / (Temp_A + Temp_B));
			else*/
				objectpixel = (int)nMaxPos;
		//}
	}

	return true;
}

bool CMyImageBuffer::edgefindhorizontal(std::vector<double>* vdataPtr, int _dir, int _indexY, double _slope, double &objectpixel)
{
	int edgecnt = 0;
	int nMaxPos = 0;
	double _dslope = 0, dMaxSlope = 0;

	if (_dir == (int)CEdgeDirection::toRight)
	{
		for (int x = 0; x < width; x++)
		{
			vdataPtr->emplace_back((double)m_2DMatrix(_indexY, x));
			edgecnt++;
		}

		for (size_t n = 2; n < edgecnt - 1; n++)
		{
			_dslope = vdataPtr->at(n+1) - vdataPtr->at(n-1);
			if (fabs(_dslope) >= _slope)
			{
				if (nMaxPos < n)
				{
					nMaxPos = n;
					dMaxSlope = _dslope;
					break;

				}
			}
		}
	}
	else if(_dir == (int)CEdgeDirection::toLeft)
	{
		edgecnt = width;

		for (int x = width - 2; x > 0; x--)
		{
			vdataPtr->emplace_back(m_2DMatrix(_indexY, x));
			edgecnt--;
		}

		std::reverse(vdataPtr->begin(), vdataPtr->end());

		for (size_t n = vdataPtr->size() -2; n > 1; n--)
		{
			_dslope = vdataPtr->at(n + 1) - vdataPtr->at(n - 1);
			if (fabs(_dslope) >= _slope)
			{
				if (nMaxPos < n)
				{
					nMaxPos = n;
					dMaxSlope = _dslope;
					break;
				}
			}
		}
	}
	else {
		ASSERT(0);
	}

	if (dMaxSlope == 0)
	{
		return false;
	}


	if (nMaxPos <= 2 || nMaxPos >= edgecnt - 2)
	{
		objectpixel = nMaxPos;
	}
	else
	{
		/*if (fabs(vdataPtr->at(nMaxPos+2) - vdataPtr->at(nMaxPos)) < fabs(vdataPtr->at(nMaxPos) - vdataPtr->at(nMaxPos-2)))
		{
			nMaxPos -= 1;
		}
		else if (vdataPtr->size() <= nMaxPos + 3)
		{
			objectpixel = (int)nMaxPos;
		}
		else
		{
			double Temp_A = (double)fabs(vdataPtr->at(nMaxPos + 2) + (vdataPtr->at(nMaxPos - 2)) - 2 * (vdataPtr->at(nMaxPos)));
			double Temp_B = (double)fabs(vdataPtr->at(nMaxPos + 3) + (vdataPtr->at(nMaxPos - 1)) - 2 * (vdataPtr->at(nMaxPos + 1)));

			if ((Temp_A + Temp_B) != 0)
				objectpixel = (int)(nMaxPos + Temp_A / (Temp_A + Temp_B));
			else*/
				objectpixel = (int)nMaxPos;
		//}
		
	}

	return true;
}

void CMyImageBuffer::houghlinefit(int _mode, int _dir, double _slope, double &a, double &b)
{
	float fRadian = float(PI / 180);

	double dRho = sqrt(static_cast<double>((width * width) + (height * height)));
	const int nRho = (int)dRho;

	double edgepos = 0;
	double d = 0;

	KjwMatrix< int > matrixHouphWeight(nRho, _THETA);
	

	if (_mode == (int)CEdgeDirection::horizontal)
	{
		for (int y = 0; y < height; y++)
		{
			std::vector<double> vdata;
			bool find = edgefindhorizontal(&vdata, _dir, y, _slope, edgepos);
			if (find)
			{
				for (int t = 0; t < _THETA; t++)
				{
					d = (int)(edgepos * dLUT_COS[t] + y * dLUT_SIN[t]);
					if (d >= 1 && d <= nRho)
					{
						matrixHouphWeight((int)d, t)++;
					}
				}
			}
			vdata.clear();
		}
		
	}
	else if(_mode == (int)CEdgeDirection::vertical)
	{
		for (int x = 0; x < width; x++)
		{
			std::vector<double> vdata;
			bool find = edgefindvertical(&vdata, _dir, x, _slope, edgepos);
			if (find)
			{
				for (int t = 0; t < _THETA; t++)
				{
					d = (int)(x * dLUT_COS[t] + edgepos * dLUT_SIN[t]);
					if (d >= 1 && d <= nRho)
					{
						matrixHouphWeight((int)d, t)++;
					}
				}
			}
			vdata.clear();
		}
		
	}
	

	

	int nlimitweight = 60;
	int D = 0;
	int nMaxTheta = 0;

	for (int t = 1; t < _THETA; t++)
	{
		for (int i = 1; i < nRho; i++)
		{
			if (nlimitweight < matrixHouphWeight(i, t))
			{
				nlimitweight = matrixHouphWeight(i,t);
				D = i;
				nMaxTheta = t;
			}
		}
	}

	if (nMaxTheta == 0 || D == 0)
	{
		a = 0;
		b = 0;
		matrixHouphWeight.getMatrixPtr().clear();
		return;
	}

	a = -dLUT_COS[nMaxTheta] / dLUT_SIN[nMaxTheta];
	b = D / dLUT_SIN[nMaxTheta];
	
	matrixHouphWeight.getMatrixPtr().clear();
	
	deleteLookuptable();

}

void CMyImageBuffer::deleteBuffer()
{
	m_2DMatrix.getMatrixPtr().clear();
}