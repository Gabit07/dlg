// CDlgVisionDisplay.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "VisionInspectDlg.h"
#include "CDlgVisionDisplay.h"
#include "afxdialogex.h"
#include "common/CommonDefine.h"
#include "CommonData.h"
#include "inspectfunction/CInspectFunc.h"
#include "CRoiManager.h"
#include <vector>


// CDlgVisionDisplay 대화 상자

IMPLEMENT_DYNAMIC(CDlgVisionDisplay, CDialogEx)

CDlgVisionDisplay::CDlgVisionDisplay(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DISPLAY, pParent)
{
	m_pParent = pParent;
	m_nIMGX = 0;
	m_nIMGY = 0;
	m_pDIB = nullptr;
	m_pImage = nullptr;
	m_pProcessImage = nullptr;
	m_hDIB = nullptr;
	m_palDIB = nullptr;
	m_nDisplayWidth = 0; 
	m_nDisplayHeight = 0; 
	m_nDisplayPitch = 0;
	_OriX = 0;
	_OriY = 0;
	m_bClicked = false;
	m_nZoomValue = 0;
	
}

CDlgVisionDisplay::~CDlgVisionDisplay()
{
	if (m_pDIB)
	{
		SAFE_DELETE(m_pDIB);
	}

	if (m_hDIB)
	{
		SAFE_DELETE(m_hDIB);
	}

	if (m_palDIB)
	{
		SAFE_DELETE(m_palDIB);
	}

	if (m_pProcessImage)
	{
		SAFE_DELETE(m_pProcessImage);
	}

	if (m_pImage)
	{
		SAFE_DELETE(m_pImage);
	}
}

void CDlgVisionDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVisionDisplay, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgVisionDisplay 메시지 처리기

BOOL CDlgVisionDisplay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CVisionInspectDlg* pInstanceDlg = (CVisionInspectDlg*)m_pParent;

	CRect rect = pInstanceDlg->GetWndRect();
	GetClientRect(m_rect);

	this->MoveWindow(rect.left, rect.top+35, m_rect.Width(), m_rect.Height());

	m_nIMGX = IMAGE_BUFFER_SIZE_X;
	m_nIMGY = IMAGE_BUFFER_SIZE_Y;

	if (!m_pDIB)
	{
		m_pDIB = new CDib();

	}
	if (!m_hDIB)
	{
		m_hDIB = new CDIBitmap;
		m_hDIB->MakeGrayImage(m_nIMGX, m_nIMGY);
	}

	if (!m_palDIB)
	{
		m_palDIB = new CDIBPalette;
		m_palDIB->CreatePal(m_hDIB);
	}

	if (!m_pImage)
	{
		m_pImage = new BYTE[IMAGE_BUFFER_SIZE_X * IMAGE_BUFFER_SIZE_Y];
		memset(m_pImage, 0, sizeof(BYTE) * IMAGE_BUFFER_SIZE_X * IMAGE_BUFFER_SIZE_Y);
	}
	if (!m_pProcessImage)
	{
		m_pProcessImage = new BYTE[IMAGE_BUFFER_SIZE_X * IMAGE_BUFFER_SIZE_Y];
		memset(m_pProcessImage, 0, sizeof(BYTE) * IMAGE_BUFFER_SIZE_X * IMAGE_BUFFER_SIZE_Y);
	}

	_FitZoom();

	reset();
	

	return TRUE;  
}

void CDlgVisionDisplay::SetZoom(int n)
{
	m_nZoomValue = n;

	switch (n)
	{
	case 0:
		_FitZoom();
		break;
	case 1:
		_dSfx = 0.25;
		_dSfy = 0.25;
		break;
	case 2:
		_dSfx = 0.5;
		_dSfy = 0.5;
		break;
	case 3:
		_dSfx = 1;
		_dSfy = 1;
		break;
	case 4:
		_dSfx = 2;
		_dSfy = 2;
		break;
	case 5:
		_dSfx = 4;
		_dSfy = 4;
		break;
	}

	_SetScrollBar(_dSfx, _dSfy);

	Invalidate();

}

void CDlgVisionDisplay::_FitZoom()
{
	m_nZoomValue = 0;
	_dSfx = (double)DISP_FOV_X / IMAGE_BUFFER_SIZE_X;
	_dSfy = (double)DISP_FOV_Y / IMAGE_BUFFER_SIZE_Y;
	_SetScrollBar(_dSfx, _dSfy);
}

void CDlgVisionDisplay::reset()
{
	//m_bIsNoImage = TRUE;
	memset(m_hDIB->GetBMBitsPtr(), 0, static_cast<int>(m_nIMGX * m_nIMGY));
	Invalidate();
}

void CDlgVisionDisplay::_GetScrollPosToFovX(int nXpos)
{
	_OriX = -nXpos;
	Invalidate();
}
void CDlgVisionDisplay::_GetScrollPosToFovY(int nYpos)
{
	_OriY = -nYpos;
	Invalidate();
}

//실제이미지의 위치를 Display되는 영역에서의 위치로 환산 
void CDlgVisionDisplay::Global2Local(int x, int y, int* x_, int* y_)
{
	*x_ = int(x * _dSfx + _OriX);
	*y_ = int(y * _dSfy + _OriY);
}

//Dialog FOV 영역이 실제 이미지에서 어디인지 구하는 함수 
void CDlgVisionDisplay::Local2Global(int x, int y, int* x_, int* y_)
{
	*x_ = int((x - _OriX) / _dSfx);
	*y_ = int((y - _OriY) / _dSfy);
}

void CDlgVisionDisplay::_GetGUIDisplayImage(CDC* memDC)
{
	int i, j;
	int left, top, right, bottom;
	int left_, top_, right_, bottom_;
	int nImageWidth, nImageHeight;
	int nGUIWidth, nGUIHeight;
	int width, height;          //화면에 Display하려고하는 폭 
	double dMultix, dMultiy;
	RECT rc;

	if (_dSfx == 0) { _dSfx = 1; _dSfy = 1; }

	CSize cz;
	LPBYTE fmT = NULL;

	nImageWidth = int(m_nIMGX * _dSfx);
	nImageHeight = int(m_nIMGY * _dSfy);
	GetClientRect(&rc);
	nGUIWidth = rc.right - rc.left;
	nGUIHeight = rc.bottom - rc.top;

	if (nGUIWidth >= nImageWidth && nGUIHeight >= nImageHeight)
	{
		left = 0; top = 0;
		right = nImageWidth;
		bottom = nImageHeight;

		width = right - left;
		height = bottom - top;
	}
	else
	{
		left = 0; top = 0;
		Local2Global(left, top, &left_, &top_);
		if (left_ < 0) left_ = 0;
		if (top_ < 0) top_ = 0;

		right = nGUIWidth - 1;
		bottom = nGUIHeight - 1;
		Local2Global(right, bottom, &right_, &bottom_);
		if (right_ >= m_nIMGX) right_ = m_nIMGX - 1;
		if (bottom_ >= m_nIMGY) bottom_ = m_nIMGY - 1;

		width = right - left;
		height = bottom - top;

		left = left_;
		top = top_;
		right = right_;
		bottom = bottom_;
	}

	if (left + width / _dSfx > m_nIMGX)	width = int((m_nIMGX - left) * _dSfx);
	if (top + height / _dSfy > m_nIMGY)  height = int((m_nIMGY - top) * _dSfy);

	if (m_pDIB == NULL || m_nDisplayWidth != width || m_nDisplayHeight != height)
	{
		if (width <= 0 || height <= 0) return;

		m_nDisplayWidth = width;
		if (width % 4)
			cz.cx = width + 4 - width % 4;
		else
			cz.cx = width;
		cz.cy = m_nDisplayHeight = height;

		m_nDisplayPitch = ((DWORD)cz.cx * 8) / 32;
		if (((DWORD)cz.cx * 8) % 32) 	m_nDisplayPitch++;
		m_nDisplayPitch *= 4;


		if (m_nDisplayPitch > width)
		{
			m_nDisplayPitch -= 4;
			m_nDisplayWidth = cz.cx = width = m_nDisplayPitch;
		}

		if (m_pDIB) delete m_pDIB;
		m_pDIB = new CDib(cz, 8);
	}

	dMultix = 1 / _dSfx;
	dMultiy = 1 / _dSfy;

	fmT = m_pDIB->m_lpImage;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			*(fmT + m_nDisplayPitch * static_cast<int>(m_nDisplayHeight - 1 - i) + j) = *(m_pProcessImage + m_nIMGX * static_cast<int>(top + i * dMultiy) + static_cast<int>(left + j * dMultix));

	if (m_pDIB)
	{
		m_pDIB->UsePalette(memDC);
		m_pDIB->Draw(memDC, CPoint(0, 0), CSize(m_nDisplayWidth, m_nDisplayHeight));
	}

}

void CDlgVisionDisplay::_DrawPoly(CDC* pDC, CRoiClass* pRoi)
{
	/*CCommonData* pCommonData = CSingleToneCommonData::GetInstance();

	
	CDlgRoiSetting* pRoiCtrl = (CDlgRoiSetting*)pCommonData->GetDlgRoiCtrl();

	if (pDC == NULL) pDC = GetDC();

	if (pRoiCtrl->IsPolyMode())
	{
		for (int i = 0; i < pRoiCtrl->m_nPolyCount; i++)
		{
			DrawCloss(pDC, pRoiCtrl->m_PolyPoint[i], RGB(255, 0, 0));
			if (i >= 1)
				DrawLine(pDC, pRoiCtrl->m_PolyPoint[i - 1], pRoiCtrl->m_PolyPoint[i], RGB(255, 0, 255));
		}
	}
	else
	{
		for (int i = 0; i < pRoi->m_nPolyCnt; i++)
		{
			DrawCloss(pDC, pRoi->m_polyRoi[i], RGB(255, 0, 0));
			if (i >= 1)
			{
				if (pRoi->m_bSelected)
					DrawLine(pDC, pRoi->m_polyRoi[i - 1], pRoi->m_polyRoi[i], RGB(0, 0, 255));
				else
					DrawLine(pDC, pRoi->m_polyRoi[i - 1], pRoi->m_polyRoi[i], RGB(255, 0, 0));
			}

			DrawRoiTextOut(pDC, CRect(pRoi->m_polyRoi[0].x, pRoi->m_polyRoi[0].y, NULL, NULL), pRoi->GetRoiName(), RGB(255, 0, 255));

		}
	}*/
	

}

void CDlgVisionDisplay::_DrawPixelInfo(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	if (pDC == NULL) pDC = GetDC();

	int x = 0, y = 0;
	Local2Global(m_NewPoint.x, m_NewPoint.y, &x, &y);

	if (x < 0 || y < 0) return;
	if (x >= IMAGE_BUFFER_SIZE_X || y >= IMAGE_BUFFER_SIZE_Y) return;

	CPen* oldPen = NULL, newPen;

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetBkMode(TRANSPARENT);

	int nVal = *(m_pImage + m_nIMGX * y + x);
	CString str;
	str.Format(_T("X:%d Y:%d Gray:%d"), x, y, nVal);


	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->TextOutW(rect.left + 10, rect.top + 10, str);


	ReleaseDC(pDC);

}


void CDlgVisionDisplay::_DrowROI(CDC* pDC, CRect rectFov)
{
	CCommonData* pCommonData = CSingleToneCommonData::GetInstance();
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	
	if (pDC == NULL) pDC = GetDC();

	pDC->SetBkMode(TRANSPARENT);


	/*if (pRoiCtrl->IsPolyMode())
	{
		DrawPoly(pDC, NULL);
	}*/
	
	for (auto& roi : pRoiManager->getrange())
	{
		
		CPoint point;

		int x, y, x1, y1;

		roi->SetRoiScale(_dSfx, _dSfy);
		roi->GetRectArea(x, y, x1, y1);

		CRect rectROI(x, y, x1, y1);


		if (roi->m_bUsePolyRoi)
		{
			DrawPoly(pDC, roi);
		}
		else if (roi->m_bUseRoundRect)
		{
			if (roi->m_bSelected)
			{
				DrawRoundRect(pDC, rectROI, roi->m_nRndRatio, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, roi);
			}
			else
			{
				DrawRoundRect(pDC, rectROI, roi->m_nRndRatio, RGB(255, 0, 0));
			}
		}
		else if (roi->GetRoiType() == ELIPSE_ROI)
		{
			if (roi->m_bSelected)
			{
				DrawElipse(pDC, rectROI, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, roi);
			}
			else
			{
				DrawElipse(pDC, rectROI, RGB(255, 0, 0));
			}
		}
		else
		{
			if (roi->m_bSelected)
			{
				/* ROI 중복 선택 표시 방지 */
				for (size_t index = 0; index < pRoiManager->getrange().size(); index++)
				{
					if (roi->GetRoiID() != pRoiManager->getRoidata(index)->GetRoiID()) 
					{
						pRoiManager->getRoidata(index)->m_bSelected = false;
					}
					else if (roi->GetRoiID() == pRoiManager->getRoidata(index)->GetRoiID()
						&& roi->GetRoiType() != pRoiManager->getRoidata(index)->GetRoiType())
					{
						pRoiManager->getRoidata(index)->m_bSelected = false;
					}
				}

				DrawRectangle(pDC, rectROI, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, roi);
			}
			else
				DrawRectangle(pDC, rectROI, RGB(255, 0, 0));

			if (roi->m_tagRect.fSeta != 0)
			{
				DrawSubRectangle(pDC, &roi->m_tagRect, RGB(0, 0, 255));
			}

			DrawRoiTextOut(pDC, rectROI, roi->GetRoiName(), RGB(255, 0, 255));
		}

		if(roi->m_bUseDonCare)
		    DrawDonCareArea(pDC, roi);

		
	}

	/*while (_itr != v.end())
	{
		RoiClass* pRoi = (*_itr);

		CPoint point;

		int x, y, x1, y1;

		pRoi->SetRoiScale(_dSfx, _dSfy);
		pRoi->GetRectArea(x, y, x1, y1);

		CRect rectROI(x, y, x1, y1);


		if (pRoi->GetRoiType() == POLY_ROI)
		{
			DrawPoly(pDC, pRoi);
		}
		else if (pRoi->GetRoiType() == ROUND_RECT_ROI)
		{

			if (pRoi->m_bSelected)
			{
				DrawRoundRect(pDC, rectROI, pRoi->m_nRndRatio, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, pRoi);
			}
			else
			{
				DrawRoundRect(pDC, rectROI, pRoi->m_nRndRatio, RGB(255, 0, 0));
			}
		}
		else if (pRoi->GetRoiType() == ELIPSE_ROI)
		{
			if (pRoi->m_bSelected)
			{
				DrawElipse(pDC, rectROI, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, pRoi);
			}
			else
			{
				DrawElipse(pDC, rectROI, RGB(255, 0, 0));
			}
		}
		else
		{
			if (pRoi->m_bSelected)
			{
				DrawRectangle(pDC, rectROI, RGB(0, 0, 255));
				DrawTrackerRect(pDC, rectROI, pRoi);

			}
			else
				DrawRectangle(pDC, rectROI, RGB(255, 0, 0));

			if (pRoi->m_tagRect.fSeta != 0)
			{
				DrawSubRectangle(pDC, &pRoi->m_tagRect, RGB(0, 0, 255));
			}



		}

		DrawDonCareArea(pDC, pRoi);

		DrawRoiTextOut(pDC, rectROI, pRoi->GetRoiName(), RGB(255, 0, 255));

		_itr++;
	}*/

}

void CDlgVisionDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CRect rect;

	this->GetClientRect(&rect);

	CDC memDC;

	CBitmap* pOldBmp;
	CBitmap bitmap;

	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBmp = memDC.SelectObject(&bitmap);

	_GetGUIDisplayImage(&memDC);
	_DrowROI(&memDC, rect);
	_DrawPixelInfo(&memDC);


	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CDlgVisionDisplay::CopyImage(BYTE* src)
{
	if (src)
	{
		//m_bIsNoImage = FALSE;
		memcpy(m_hDIB->GetBMBitsPtr(), src, static_cast<int>(m_nIMGX * m_nIMGY));
	}
	else
	{
		//m_bIsNoImage = TRUE;
	}
	Invalidate();
}

// local -> global
void CDlgVisionDisplay::_GetScrollToOriPos(int& x, int& y)
{
	x = -_OriX;
	y = -_OriY;
}

void CDlgVisionDisplay::MoveImagePosition(int nX, int nY, BOOL bUseScroll)
{
	if (_dSfx == 0 || _dSfy == 0) return;

	if (nX < 0) nX = 0;
	if (nY < 0) nY = 0;

	if (_dSfx > 0.0125 && _dSfy > 0.0125) 
	{
		if (bUseScroll)
		{
			_OriX = int(DISP_FOV_X / 2 - nX * _dSfx);
			if (_OriX > 0) _OriX = 0;
		}

		if (bUseScroll)
		{
			_OriY = int(DISP_FOV_Y / 2 - nY * _dSfy);
			if (_OriY > 0) _OriY = 0;
		}
	}

	Invalidate(true);
}

void CDlgVisionDisplay::_SetScrollOriPos(int x, int y, BOOL bUseScroll)
{
	_OriX = x;
	_OriY = y;

	int nPrevPosX, nPrevPosY;
	Global2Local(m_nDisplayWidth / 2, m_nDisplayHeight / 2, &nPrevPosX, &nPrevPosY);
	//Local2Global(m_nDisplayWidth / 2, m_nDisplayHeight / 2, &nPrevPosX, &nPrevPosY);

	MoveImagePosition(nPrevPosX, nPrevPosY, bUseScroll);
}

void CDlgVisionDisplay::_SetScrollPosXY(BOOL bUseScroll)
{
	_OriX = GetScrollPos(SB_HORZ);
	_OriY = GetScrollPos(SB_VERT);

	_SetScrollOriPos(_OriX, _OriY, bUseScroll);
}

void CDlgVisionDisplay::_SetScrollBar(double dx, double dy)
{
	BOOL bUseScroll = FALSE;
	
	_OriX = GetScrollPos(SB_HORZ);
	_OriY = GetScrollPos(SB_VERT);

	if (IMAGE_BUFFER_SIZE_X * _dSfx - DISP_FOV_X > 0)
	{
		SetScrollRange(SB_HORZ, 0, int(IMAGE_BUFFER_SIZE_X * _dSfx - DISP_FOV_X), TRUE);
		SetScrollPos(SB_HORZ, _OriX);

		bUseScroll = true;
	}
	else
	{
		SetScrollRange(SB_HORZ, 0, 0, FALSE);
		bUseScroll = false;
		_OriX = 0;
	}

	if (IMAGE_BUFFER_SIZE_Y * _dSfy - DISP_FOV_Y > 0)
	{
		//SB_VERT
		SetScrollRange(SB_VERT, 0, int(IMAGE_BUFFER_SIZE_Y * _dSfy - DISP_FOV_Y), TRUE);
		SetScrollPos(SB_VERT, _OriY);

		bUseScroll = true;
	}
	else
	{
		SetScrollRange(SB_VERT, 0, 0, FALSE);
		bUseScroll = false;
		_OriY = 0;
	}

	_SetScrollPosXY(bUseScroll);
}

void CDlgVisionDisplay::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CCommonData* pCommonData = CSingleToneCommonData::GetInstance();
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	CPoint pt;

	m_bClicked = false;

	for (auto& roi : pRoiManager->getrange())
	{

		if (roi->m_bUsePolyRoi)
		{
			if (roi->m_bDrawPoly)
			{
				if (roi->m_nPolyCnt >= DONTCARE_POINT) return;

				int newpolyx = 0, newpolyy = 0;
				Local2Global(point.x, point.y, &newpolyx, &newpolyy);
				roi->m_polyRoi[roi->m_nPolyCnt] = CPoint(newpolyx, newpolyy);
				roi->m_nPolyCnt++;
			}
		}
		else
		{
			memset(roi->m_polyRoi, 0, sizeof(CPoint) * DONTCARE_POINT);
			roi->m_nPolyCnt = 0;
		}


		if (roi->m_bDrawDonCare)
		{
			if (roi->m_nDoncarePtCnt[roi->m_nDonCareAreaCnt] >= DONTCARE_POINT) return;

			int newx, newy;
			Local2Global(point.x, point.y, &newx, &newy);
			roi->m_pointDoncare[roi->m_nDonCareAreaCnt][roi->m_nDoncarePtCnt[roi->m_nDonCareAreaCnt]] = CPoint(newx, newy);
			roi->m_nDoncarePtCnt[roi->m_nDonCareAreaCnt]++;

		}
		else
		{
			memset(roi->m_pointDoncare[roi->m_nDonCareAreaCnt], 0, sizeof(CPoint)* DONTCARE_POINT);
			roi->m_nDoncarePtCnt[roi->m_nDonCareAreaCnt] = 0;
		}

		int nx, ny;
		Local2Global(point.x, point.y, &nx, &ny);
		pt.x = nx;
		pt.y = ny;

		if (roi->PitInROI(pt))
		{
			m_bClicked = true;

			if (roi->GetRoiType() == POLY_ROI) break;

			for (int i = 0; i < TRACKER_COUNT; i++)
			{
				if (m_rectTracker[i].HitTest(pt) >= 0)
				{
					m_nHitTestType = i;
					m_bTrackerOn = true;
					m_ptOld = pt;
					break;
				}
			}
			break;
		}
		
	}

	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgVisionDisplay::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CCommonData* pCommonData = CSingleToneCommonData::GetInstance();
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	GetLocalPoint(point);

	CRoiClass* pRoi = nullptr;
	for (auto& roi : pRoiManager->getrange())
	{
		if (roi->m_bSelected)
		{
			pRoi = roi;
			break;
		}
	}
	
	if (m_bTrackerOn && m_bClicked)
	{
		int x1, y1, x2, y2;

		int newx, newy;
		pRoi->GetRectArea(x1, y1, x2, y2);

		Local2Global(point.x, point.y, &newx, &newy);

		point.x = newx;
		point.y = newy;

		CRect rc(x1, y1, x2, y2);

		switch (m_nHitTestType)
		{
		case CRectTracker::hitTopLeft:
			rc.top += (point.y - m_ptOld.y);
			rc.left += (point.x - m_ptOld.x);
			break;
		case CRectTracker::hitTopRight:
			rc.top += (point.y - m_ptOld.y);
			rc.right += (point.x - m_ptOld.x);
			break;
		case CRectTracker::hitBottomRight:
			rc.bottom += (point.y - m_ptOld.y);
			rc.right += (point.x - m_ptOld.x);
			break;
		case CRectTracker::hitBottomLeft:
			rc.bottom += (point.y - m_ptOld.y);
			rc.left += (point.x - m_ptOld.x);
			break;
		}

		pRoi->SetRectArea(rc);

		m_ptOld = point;

	}

	if (!m_bTrackerOn && m_bClicked)
	{
		int nx, ny;
		CPoint pt;
		Local2Global(point.x, point.y, &nx, &ny);
		pt.x = nx;
		pt.y = ny;

		pRoi->MoveRectArea(pt);
	}
	

	Invalidate();
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgVisionDisplay::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rectDisp;

	if (m_nZoomValue != 3)
	{
		int nPointX, nPointY;
		Local2Global(point.x, point.y, &nPointX, &nPointY);

		//_GetScrollToOriPos(nPointX, nPointY);

		SetZoom(3);
		SetScrollPos(SB_HORZ, nPointX);
		SetScrollPos(SB_VERT, nPointY);

		_SetScrollOriPos(nPointX, nPointY, true);
	}
	else {
		_FitZoom();
	}
	

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CDlgVisionDisplay::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bClicked = FALSE;
	m_bTrackerOn = FALSE;
	Invalidate();

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CDlgVisionDisplay::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}

void CDlgVisionDisplay::DrawRectangle(CDC* pDC, CRect rect, COLORREF color)
{
	int x1_, y1_, x2_, y2_;

	Global2Local(rect.left, rect.top, &x1_, &y1_);
	Global2Local(rect.right, rect.bottom, &x2_, &y2_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_SOLID, 1, color);


	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y1_);
	pDC->LineTo(x2_, y2_);
	pDC->LineTo(x1_, y2_);
	pDC->LineTo(x1_, y1_);


	pDC->SelectObject(oldPen);

	ReleaseDC(pDC);

}

void CDlgVisionDisplay::DrawRoundRect(CDC* pDC, CRect rect, int nRatio, COLORREF color)
{
	int x1_, y1_, x2_, y2_;

	Global2Local(rect.left, rect.top, &x1_, &y1_);
	Global2Local(rect.right, rect.bottom, &x2_, &y2_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_DOT, 1, color), Pen(PS_SOLID, 1, color);
	CBrush* pOldBrush;

	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y1_);
	pDC->LineTo(x2_, y2_);
	pDC->LineTo(x1_, y2_);
	pDC->LineTo(x1_, y1_);


	oldPen = pDC->SelectObject(&Pen);


	pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	pDC->RoundRect(CRect(x1_, y1_, x2_, y2_), CPoint(nRatio, nRatio));
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(oldPen);

	ReleaseDC(pDC);

}

void CDlgVisionDisplay::DrawElipse(CDC* pDC, CRect rect, COLORREF color)
{
	int x1_, y1_, x2_, y2_;

	Global2Local(rect.left, rect.top, &x1_, &y1_);
	Global2Local(rect.right, rect.bottom, &x2_, &y2_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_DOT, 1, color), Pen(PS_SOLID, 1, color);
	CBrush* pOldBrush;

	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y1_);
	pDC->LineTo(x2_, y2_);
	pDC->LineTo(x1_, y2_);
	pDC->LineTo(x1_, y1_);


	oldPen = pDC->SelectObject(&Pen);

	CRgn rgn;
	pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	rgn.CreateEllipticRgn(x1_, y1_, x2_, y2_);
	pDC->Ellipse(CRect(x1_, y1_, x2_, y2_));

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(oldPen);
	rgn.DeleteObject();
	ReleaseDC(pDC);

}

void CDlgVisionDisplay::DrawSubRectangle(CDC* pDC, PST_RECT pRect, COLORREF color)
{
	int x1_, y1_, x2_, y2_;
	int x3_, y3_, x4_, y4_;

	Global2Local((int)pRect->pt[0].x, (int)pRect->pt[0].y, &x1_, &y1_);
	Global2Local((int)pRect->pt[1].x, (int)pRect->pt[1].y, &x2_, &y2_);
	Global2Local((int)pRect->pt[2].x, (int)pRect->pt[2].y, &x3_, &y3_);
	Global2Local((int)pRect->pt[3].x, (int)pRect->pt[3].y, &x4_, &y4_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_DOT, 1, color);

	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y2_);
	pDC->LineTo(x3_, y3_);
	pDC->LineTo(x4_, y4_);
	pDC->LineTo(x1_, y1_);

	pDC->SelectObject(oldPen);

	ReleaseDC(pDC);

}

void CDlgVisionDisplay::DrawRoiTextOut(CDC* pDC, CRect rect, CString str, COLORREF color)
{
	int x1_, y1_;

	Global2Local(rect.left, rect.top, &x1_, &y1_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_SOLID, 1, color);

	oldPen = pDC->SelectObject(&newPen);

	pDC->SetTextColor(color);
	pDC->TextOutW(x1_, y1_, str); // ROI 네임

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);
}

void CDlgVisionDisplay::DrawLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF color)
{
	int x1_, y1_, x2_, y2_;

	Global2Local(ptStart.x, ptStart.y, &x1_, &y1_);
	Global2Local(ptEnd.x, ptEnd.y, &x2_, &y2_);

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_SOLID, 1, color);

	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y2_);


	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);
}

void CDlgVisionDisplay::DrawCloss(CDC* pDC, CPoint point, COLORREF color)
{
	int x1_, y1_;

	Global2Local(point.x, point.y, &x1_, &y1_);
	if (y1_ < 0) y1_ = 0;

	if (pDC == NULL) pDC = GetDC();

	CPen* oldPen, newPen(PS_SOLID, 1, color);

	oldPen = pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_ - 3, y1_);
	pDC->LineTo(x1_ + 3, y1_);
	pDC->MoveTo(x1_, y1_ - 3);
	pDC->LineTo(x1_, y1_ + 3);


	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);
}

void CDlgVisionDisplay::DrawTrackerRect(CDC* memDC, CRect rect, CRoiClass* roi)
{
	CBrush* pOldBrush, * pNewBrush = new CBrush(RGB(0, 0, 255));
	pOldBrush = memDC->SelectObject(pNewBrush);

	int left = 0, right = 0, top = 0, bottom = 0;

	CRect rcTracker = rect;
	roi->SetRectTracker(rcTracker); // sfx sfy 반영된 좌표
	memcpy(m_rectTracker, roi->GetTracker(), sizeof(CRectTracker) * 4);

	for (int i = 0; i < TRACKER_COUNT; i++)
	{
		int left_, right_, top_, bottom_;
		Global2Local(m_rectTracker[i].m_rect.left, m_rectTracker[i].m_rect.top, &left_, &top_);
		Global2Local(m_rectTracker[i].m_rect.right, m_rectTracker[i].m_rect.bottom, &right_, &bottom_);
		CRect rc(left_, top_, right_, bottom_);
		memDC->FrameRect(rc, pNewBrush);
		memDC->FillSolidRect(rc, RGB(0, 0, 255));
	}
	memDC->SelectObject(pOldBrush);

	delete pNewBrush;
}

void CDlgVisionDisplay::DrawPoly(CDC* pDC, CRoiClass* pRoi)
{
	
	if (pDC == NULL) pDC = GetDC();

	if (pRoi->m_bDrawPoly)
	{
		for (int i = 0; i < pRoi->m_nPolyCnt; i++)
		{
			DrawCloss(pDC, pRoi->m_polyRoi[i], RGB(255, 0, 0));
			if (i >= 1)
				DrawLine(pDC, pRoi->m_polyRoi[i - 1], pRoi->m_polyRoi[i], RGB(255, 0, 255));
		}
	}
	else
	{
		for (int i = 0; i < pRoi->m_nPolyCnt; i++)
		{
			DrawCloss(pDC, pRoi->m_polyRoi[i], RGB(255, 0, 0));
			if (i >= 1)
			{
				if (pRoi->m_bSelected)
					DrawLine(pDC, pRoi->m_polyRoi[i - 1], pRoi->m_polyRoi[i], RGB(0, 0, 255));
				else
					DrawLine(pDC, pRoi->m_polyRoi[i - 1], pRoi->m_polyRoi[i], RGB(255, 0, 0));
			}

			DrawRoiTextOut(pDC, CRect(pRoi->m_polyRoi[0].x, pRoi->m_polyRoi[0].y, NULL, NULL), pRoi->GetRoiName(), RGB(255, 0, 255));

		}
	}

}

void CDlgVisionDisplay::DrawDonCareArea(CDC* pDC, CRoiClass* pRoi)
{
	CCommonData* pCommonData = CSingleToneCommonData::GetInstance();

	if (pRoi->m_bDrawDonCare)
	{
		for (int i = 0; i < pRoi->m_nDoncarePtCnt[pRoi->m_nDonCareAreaCnt]; i++)
		{
			DrawCloss(pDC, pRoi->m_pointDoncare[pRoi->m_nDonCareAreaCnt][i], RGB(255, 0, 0));
			if (i >= 1)
				DrawLine(pDC, pRoi->m_pointDoncare[pRoi->m_nDonCareAreaCnt][i - 1], pRoi->m_pointDoncare[pRoi->m_nDonCareAreaCnt][i], RGB(255, 0, 255));
		}
	}
	if (pRoi->m_nDonCareAreaCnt)
	{
		for (int k = 0; k < pRoi->m_nDonCareAreaCnt; k++)
		{
			for (int i = 0; i < pRoi->m_nDoncarePtCnt[k]; i++)
			{
				DrawCloss(pDC, pRoi->m_pointDoncare[k][i], RGB(255, 0, 0));
				if (i >= 1)
				{
					if (pRoi->m_bSelected)
						DrawLine(pDC, pRoi->m_pointDoncare[k][i - 1], pRoi->m_pointDoncare[k][i], RGB(0, 0, 255));
					else
						DrawLine(pDC, pRoi->m_pointDoncare[k][i - 1], pRoi->m_pointDoncare[k][i], RGB(255, 0, 0));
				}

				DrawDonCareColor(pDC, pRoi->m_tagRect.rect, pRoi->m_pointDoncare[k], pRoi->m_nDoncarePtCnt[k], RGB(0, 255, 255));


			}
		}

	}
}

void CDlgVisionDisplay::DrawDonCareColor(CDC* pDC, CRect rectArea, CPoint ptPoint[], int nPtCnt, COLORREF color)
{

	for (int j = rectArea.top; j < rectArea.bottom; j += 4)
	{
		for (int i = rectArea.left; i < rectArea.right; i += 4)
		{
			if (CInspectFunc::CnCheckArea(ptPoint, nPtCnt, i, j))
			{
				int newx = 0, newy = 0;
				Global2Local(i, j, &newx, &newy);
				pDC->SetPixel(CPoint(newx, newy), color);
			}
		}
	}
}

void CDlgVisionDisplay::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nSBCode == SB_ENDSCROLL) return;


	switch (nSBCode)
	{
	case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우+
	{
		SetScrollPos(SB_VERT, (int)(GetScrollPos(SB_VERT) - 1));
		break;
	}

	case SB_LINERIGHT: // 오른쪽 끝을 누른경우
	{
		SetScrollPos(SB_VERT, (int)(GetScrollPos(SB_VERT) + 1));

		break;
	}

	case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
	{
		//scriInfo.nPos -= scriInfo.nPage;
		break;
	}

	case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
	{
		//scriInfo.nPos += scriInfo.nPage;
		break;
	}

	case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
	{
		SetScrollPos(SB_VERT, nPos);

		break;
	}
	}

	_OriY -= nPos;

	_GetScrollPosToFovY(nPos);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CDlgVisionDisplay::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nSBCode == SB_ENDSCROLL) return;


	switch (nSBCode)
	{
	case SB_LINEDOWN: // 왼쪽 끝의 버튼을 누른경우
	{
		nPos = SetScrollPos(SB_HORZ, (int)(GetScrollPos(SB_HORZ) + 1));

		break;
	}

	case SB_LINEUP: // 오른쪽 끝을 누른경우
	{
		nPos = SetScrollPos(SB_HORZ, (int)(GetScrollPos(SB_HORZ) - 1));

		break;
	}

	case SB_PAGEDOWN: // 왼쪽 몸통부분을 누른경우
	{
		//scriInfo.nPos -= scriInfo.nPage;
		break;
	}

	case SB_PAGEUP: // 오른쪽 몸통부분을 누른경우
	{
		//scriInfo.nPos += scriInfo.nPage;
		break;
	}

	case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
	{
		SetScrollPos(SB_HORZ, nPos);
	
		break;
	}
	}

	_OriX -= nPos;
	_GetScrollPosToFovX(nPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CDlgVisionDisplay::SaveBMP(CString filename)
{
	int ret;
	CFile f;
	CDib* pdib;
	int nByte;
	int pitch = IMAGE_BUFFER_SIZE_X;


	CSize cz(IMAGE_BUFFER_SIZE_X, IMAGE_BUFFER_SIZE_Y);
	pdib = new CDib(cz, 8);

	pdib->ComputeMetrics();
	if (pdib->m_lpImage) delete pdib->m_lpImage;
	pdib->m_lpImage = new BYTE[pdib->GetSizeImage()];



	nByte = pdib->m_lpBMIH->biBitCount / 8;
	pitch = ((DWORD)pdib->m_lpBMIH->biWidth * nByte * 8) / 32;
	if (((DWORD)pdib->m_lpBMIH->biWidth * nByte * 8) % 32) 	pitch++;
	pitch *= 4;



	for (int i = 0; i < IMAGE_BUFFER_SIZE_Y; i++)
	{
		memcpy((pdib->m_lpImage + pitch * i), (m_pImage + IMAGE_BUFFER_SIZE_X * (IMAGE_BUFFER_SIZE_Y - 1 - i)), IMAGE_BUFFER_SIZE_X);
	}



	ret = f.Open(filename, CFile::modeCreate | CFile::modeWrite);
	if (ret)
	{
		pdib->Write(&f);
		f.Close();
	}

	delete pdib;

	return !ret;
}

void CDlgVisionDisplay::RestoreDispImage()
{
	memcpy(m_pProcessImage, m_pImage, m_nIMGX * m_nIMGY);
}

int CDlgVisionDisplay::LoadBMP(CString filename)
{
	int i, j, ret = 1;
	int nByte, pitchBMP;
	CFile file;
	CDib* pdib = NULL;
	//LPBYTE fmS, fmD;
	CString str;

	if (!file.Open(filename, CFile::modeRead)) goto done;

	pdib = new CDib();

	if (pdib->Read(&file) == TRUE)
	{
		file.Close();

		ret = 0;

		if (pdib->m_lpBMIH->biHeight > m_nIMGY || pdib->m_lpBMIH->biWidth > m_nIMGX)
		{
			str.Format(_T("Image Size must be X<%d Y<%d (X=%d, Y=%d)"), m_nIMGX, m_nIMGY, pdib->m_lpBMIH->biWidth, pdib->m_lpBMIH->biHeight);
			AfxMessageBox(str);
			goto done;
		}

		if (pdib->m_lpBMIH->biHeight <= m_nIMGY && pdib->m_lpBMIH->biWidth <= m_nIMGX)
		{
			nByte = pdib->m_lpBMIH->biBitCount / 8;

			pitchBMP = ((DWORD)pdib->m_lpBMIH->biWidth * nByte * 8) / 32;
			if (((DWORD)pdib->m_lpBMIH->biWidth * nByte * 8) % 32) 	pitchBMP++;
			pitchBMP *= 4;

			memset(m_pImage, 0, m_nIMGX * m_nIMGY);

			for (i = 0; i < pdib->m_lpBMIH->biHeight; i++)
				for (j = 0; j < pdib->m_lpBMIH->biWidth; j++)
					*(m_pImage + m_nIMGX * i + j) = *(pdib->m_lpImage + pitchBMP * (pdib->m_lpBMIH->biHeight - 1 - i) + j);

			memcpy(m_pProcessImage, m_pImage, m_nIMGX * m_nIMGY);
		}
	}
	else
	{
		ret = 1;
		file.Close();
		goto done;
	}

done:
	if (pdib) delete pdib;

	Invalidate();

	return ret;
}