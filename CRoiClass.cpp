#include "CRoiClass.h"
#include "common/configure.h"
#include "inspectfunction/CInspectFunc.h"


void CRoiClass::CreateRoi()
{

	m_tagRect.rect.top = 10;
	m_tagRect.rect.bottom = 522;
	m_tagRect.rect.left = 10;
	m_tagRect.rect.right = 522;
	SetSubROI();

	_dScaleX = 1;
	_dScaleY = 1;

	m_bUseRoundRect = false;
	m_bDrawDonCare = false;
}

void CRoiClass::SetSubROI()
{
	//lefttop
	m_tagRect.pt[0].x = m_tagRect.rect.left;
	m_tagRect.pt[0].y = m_tagRect.rect.top;
	//righttop
	m_tagRect.pt[1].x = m_tagRect.rect.right;
	m_tagRect.pt[1].y = m_tagRect.rect.top;
	//rigtbottom
	m_tagRect.pt[2].x = m_tagRect.rect.right;
	m_tagRect.pt[2].y = m_tagRect.rect.bottom;
	//leftbottom
	m_tagRect.pt[3].x = m_tagRect.rect.left;
	m_tagRect.pt[3].y = m_tagRect.rect.bottom;
}

void CRoiClass::SetSubROI(PST_RECT pRect)
{
	//lefttop
	m_tagRect.pt[0].x = pRect->pt[0].x;
	m_tagRect.pt[0].y = pRect->pt[0].y;
	//righttop
	m_tagRect.pt[1].x = pRect->pt[1].x;
	m_tagRect.pt[1].y = pRect->pt[1].y;
	//rigtbottom
	m_tagRect.pt[2].x = pRect->pt[2].x;
	m_tagRect.pt[2].y = pRect->pt[2].y;
	//leftbottom
	m_tagRect.pt[3].x = pRect->pt[3].x;
	m_tagRect.pt[3].y = pRect->pt[3].y;
}

void CRoiClass::RectReSize()
{
	if (m_tagRect.rect.left > m_tagRect.rect.right)
	{
		int temp = m_tagRect.rect.left;
		m_tagRect.rect.left = m_tagRect.rect.right;
		m_tagRect.rect.right = temp;
		//SetSubROI();
	}
	if (m_tagRect.rect.top > m_tagRect.rect.bottom)
	{
		int temp = m_tagRect.rect.bottom;
		m_tagRect.rect.bottom = m_tagRect.rect.top;
		m_tagRect.rect.top = temp;
		//SetSubROI();
	}

}

BOOL  CRoiClass::PitInROI(CPoint point)
{
	if (m_bUsePolyRoi)
	{
		if (CInspectFunc::CnCheckArea(m_polyRoi, m_nPolyCnt, point.x, point.y))
		{
			m_bSelected = TRUE;
			m_OldPoint = point;
		}
		else
			m_bSelected = FALSE;
	}
	else
	{
		RectReSize();
		if (m_tagRect.rect.PtInRect(point))
		{
			m_bSelected = TRUE;
			m_OldPoint = point;
		}
		else
		{
			m_bSelected = FALSE;
		}
	}

	return m_bSelected;
}


void  CRoiClass::SetRoiScale(double dSfx, double dSfy)
{
	_dScaleX = dSfx;
	_dScaleY = dSfy;
}
// local -> global
void CRoiClass::SetRectTracker(CRect rect)
{
	int nSizeX = (int)(7 / _dScaleX);
	int nSizeY = (int)(7 / _dScaleY);

	CRect rcLeftTop = CRect(rect.left - nSizeX, rect.top - nSizeY, rect.left + nSizeX, rect.top + nSizeY);
	CRect rcRightTop = CRect(rect.right - nSizeX, rect.top - nSizeY, rect.right + nSizeX, rect.top + nSizeY);
	CRect rcLeftBottom = CRect(rect.left - nSizeX, rect.bottom - nSizeY, rect.left + nSizeX, rect.bottom + nSizeY);
	CRect rcRightBottom = CRect(rect.right - nSizeX, rect.bottom - nSizeY, rect.right + nSizeX, rect.bottom + nSizeY);
	for (int i = 0; i < TRACKER_COUNT; i++) _tracker[i].m_nStyle = CRectTracker::resizeOutside;

	_tracker[CRectTracker::hitTopLeft].m_rect = rcLeftTop;
	_tracker[CRectTracker::hitTopRight].m_rect = rcRightTop;
	_tracker[CRectTracker::hitBottomLeft].m_rect = rcLeftBottom;
	_tracker[CRectTracker::hitBottomRight].m_rect = rcRightBottom;
}

void CRoiClass::MoveRectArea(CPoint point)
{
	if (m_bSelected)
	{
		int nMoveX = point.x - m_OldPoint.x;
		int nMoveY = point.y - m_OldPoint.y;

		if (m_bUsePolyRoi)
		{
			for (int i = 0; i < m_nPolyCnt; i++)
			{

				if (nMoveX < 0)
					m_polyRoi[i].x -= abs(nMoveX);
				else
					m_polyRoi[i].x += abs(nMoveX);

				if (nMoveY < 0)
					m_polyRoi[i].y -= abs(nMoveY);
				else
					m_polyRoi[i].y += abs(nMoveY);


			}
		}

		if (nMoveX < 0)
		{
			m_tagRect.rect.left -= abs(nMoveX);
			m_tagRect.rect.right -= abs(nMoveX);
		}
		else
		{
			m_tagRect.rect.left += abs(nMoveX);
			m_tagRect.rect.right += abs(nMoveX);
		}

		if (nMoveY < 0)
		{
			m_tagRect.rect.top -= abs(nMoveY);
			m_tagRect.rect.bottom -= abs(nMoveY);
		}
		else
		{
			m_tagRect.rect.top += abs(nMoveY);
			m_tagRect.rect.bottom += abs(nMoveY);
		}


		SetSubROI(&m_tagRect, nMoveX, nMoveY);


		if (m_bUseDonCare)
		{
			for (int k = 0; k < m_nDonCareAreaCnt; k++)
			{
				for (int i = 0; i < m_nDoncarePtCnt[k]; i++)
				{
					if (nMoveX < 0)
						m_pointDoncare[k][i].x -= abs(nMoveX);
					else
						m_pointDoncare[k][i].x += abs(nMoveX);

					if (nMoveY < 0)
						m_pointDoncare[k][i].y -= abs(nMoveY);
					else
						m_pointDoncare[k][i].y += abs(nMoveY);

				}
			}
		}
		

		m_OldPoint = point;
	}

}

void CRoiClass::SetSubROI(PST_RECT pRect, int nMoveX, int nMoveY)
{

	if (nMoveX < 0)
	{
		m_tagRect.pt[0].x -= abs(nMoveX);
		m_tagRect.pt[1].x -= abs(nMoveX);
		m_tagRect.pt[2].x -= abs(nMoveX);
		m_tagRect.pt[3].x -= abs(nMoveX);

	}
	else
	{
		m_tagRect.pt[0].x += abs(nMoveX);
		m_tagRect.pt[1].x += abs(nMoveX);
		m_tagRect.pt[2].x += abs(nMoveX);
		m_tagRect.pt[3].x += abs(nMoveX);
	}

	if (nMoveY < 0)
	{
		m_tagRect.pt[0].y -= abs(nMoveY);
		m_tagRect.pt[1].y -= abs(nMoveY);
		m_tagRect.pt[2].y -= abs(nMoveY);
		m_tagRect.pt[3].y -= abs(nMoveY);
	}
	else
	{
		m_tagRect.pt[0].y += abs(nMoveY);
		m_tagRect.pt[1].y += abs(nMoveY);
		m_tagRect.pt[2].y += abs(nMoveY);
		m_tagRect.pt[3].y += abs(nMoveY);
	}
}

void CRoiClass::SetOffSetParam(int _roundratio)
{
	m_nRndRatio = _roundratio;
}
void CRoiClass::SetRectArea(CRect rect)
{
	m_tagRect.rect.left = rect.left;
	m_tagRect.rect.top = rect.top;
	m_tagRect.rect.right = rect.right;
	m_tagRect.rect.bottom = rect.bottom;
	SetSubROI();
}

void CRoiClass::GetRectArea(int& nLeft, int& nTop, int& nRight, int& nBottom)
{
	nLeft = m_tagRect.rect.left;
	nRight = m_tagRect.rect.right;
	nTop = m_tagRect.rect.top;
	nBottom = m_tagRect.rect.bottom;

}


void CRoiClass::SetROI(int _id, int nType, CString strName)
{
	m_nRoiId = _id;
	m_nRoiType = nType;
	m_strRoiName = strName;
}

void CRoiClass::CreatePolyRoi(CPoint point[], int nCnt)
{
	memcpy(m_polyRoi, point, sizeof(CPoint) * nCnt);
	m_nPolyCnt = nCnt;
}

void CRoiClass::CreateDonCareArea(CPoint point[], int nCnt)
{
	memcpy(m_pointDoncare[m_nDonCareAreaCnt], point, sizeof(CPoint) * nCnt);
	m_nDoncarePtCnt[m_nDonCareAreaCnt] = nCnt;
	m_nDonCareAreaCnt++;
}

void CRoiClass::DeleteDonCareArea()
{
	ZeroMemory(m_pointDoncare, sizeof(CPoint) * DONTCARE_POINT * 5);
	m_nDonCareAreaCnt = 0;
}

void CRoiClass::CreateSetaRect(float _seta)
{
	int _x1, _y1, _x2, _y2;
	int centerx, centery;

	GetRectArea(_x1, _y1, _x2, _y2);
	centerx = (_x1 + _x2) / 2;
	centery = (_y1 + _y2) / 2;

	PST_RECT ptagRect;
	ST_RECT tagTemp;
	ptagRect = GetSubROI();

	m_tagRect.fSeta = _seta;

	for (int i = 0; i < 4; i++)
	{
		tagTemp.pt[i].x = (ptagRect->pt[i].x - centerx) * cos(ptagRect->fSeta) - (ptagRect->pt[i].y - centery) * sin(ptagRect->fSeta) + centerx;
		tagTemp.pt[i].y = (ptagRect->pt[i].x - centerx) * sin(ptagRect->fSeta) + (ptagRect->pt[i].y - centery) * cos(ptagRect->fSeta) + centery;
	}

	SetSubROI(&tagTemp);

}

void CRoiClass::ResetSetaRect()
{
	SetSubROI();
}