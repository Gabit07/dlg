#pragma once

#include "../VisionInspect/usercontrol/dib.h"
#include "../VisionInspect/usercontrol/dibpal.h"
#include "Cdib.h"
#include "CRoiClass.h"


// CDlgVisionDisplay 대화 상자

class CDlgVisionDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVisionDisplay)

public:
	CDlgVisionDisplay(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgVisionDisplay();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DISPLAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	CWnd* m_pParent;

	CRect m_rect;
	CRectTracker m_rectTracker[4];

	CString m_strPath;
	CDIBPalette* m_palDIB;
	CDIBitmap* m_hDIB;
	CDib* m_pDIB;

	int m_nIMGX;
	int m_nIMGY;

	/*alignas(64) */LPBYTE m_pImage;
	/*alignas(64) */LPBYTE m_pProcessImage;

	int   m_nDisplayWidth, m_nDisplayHeight, m_nDisplayPitch;

	int _OriX; // 원점 x
	int _OriY; // 원점 y
	double _dSfx;
	double _dSfy;

	//Tracker
	int m_nHitTestType;
	BOOL m_bTrackerOn;
	POINT m_ptOld;

	CPoint m_NewPoint;
	void GetLocalPoint(CPoint point) { m_NewPoint = point; }

	bool m_bClicked;

	int m_nZoomValue;

private:

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void _FitZoom();
	
	void _GetScrollPosToFovX(int nXpos);
	void _GetScrollPosToFovY(int nYpos);
	void _GetScrollToOriPos(int& x, int& y);
	void _SetScrollOriPos(int x, int y, BOOL bUseScroll);
	void _SetScrollPosXY(BOOL bUseScroll);
	void _SetScrollBar(double dx, double dy);
	void _GetGUIDisplayImage(CDC* memDC);
	void _DrowROI(CDC* pDC, CRect rectFov);
	void _DrawPoly(CDC* pDC, CRoiClass* pRoi);
	void _DrawPixelInfo(CDC* pDC);
	

public:

	void reset();
	void Global2Local(int x, int y, int* x_, int* y_);
	void Local2Global(int x, int y, int* x_, int* y_);
	//void GetImagePath(CString strSrcImgPath);
	LPBYTE GetBitmapPtr() const { return m_hDIB->GetBMBitsPtr(); }
	LPBYTE GetOriginImage() const { return m_pImage; }
	LPBYTE GetDispImage() const { return m_pProcessImage; }
	void CopyImage(BYTE* src);
	
	void MoveImagePosition(int nX, int nY, BOOL bUseScroll);
	int LoadBMP(CString filename);
	int SaveBMP(CString filename);
	void SetZoom(int n);

public:

	// 그리기 함수

	void DrawRectangle(CDC* pDC, CRect rect, COLORREF color);
	void DrawSubRectangle(CDC* pDC, PST_RECT pRect, COLORREF color);
	void DrawLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF color);
	void DrawCloss(CDC* pDC, CPoint point, COLORREF color);
	void DrawPoly(CDC* pDC, CRoiClass* pRoi);
	void DrawDonCareArea(CDC* pDC, CRoiClass* pRoi);
	void DrawRoundRect(CDC* pDC, CRect rect, int nRatio, COLORREF color);
	void DrawElipse(CDC* pDC, CRect rect, COLORREF color);
	void DrawDonCareColor(CDC* pDC, CRect rectArea, CPoint ptPoint[], int nPtCnt, COLORREF color);
	void DrawRoiTextOut(CDC* pDC, CRect rect, CString str, COLORREF color);
	void DrawTrackerRect(CDC* memDC, CRect rect, CRoiClass* roi);

	// 원본이미지로 디스플레이 회복
	void RestoreDispImage();
	
};
