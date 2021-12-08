#pragma once


#include <afxext.h>
#include "common/CommonDefine.h"

#define DONTCARE_POINT 100

class CRoiClass
{
public:
	CRoiClass() = default;

private:

	CRectTracker _tracker[4];

	//CRect m_tagRect;

	CString m_strRoiName;
	CPoint m_OldPoint;

	int m_nRoiId;
	int m_nRoiType;

	double _dScaleX;
	double _dScaleY;

	void RectReSize();
	void SetSubROI();


public:

	ST_RECT m_tagRect;
	bool m_bShow;
	BOOL m_bSelected;

	// 다각형 roi 관련
	bool   m_bUsePolyRoi;
	bool   m_bDrawPoly;
	CPoint m_polyRoi[DONTCARE_POINT];
	int    m_nPolyCnt;

	bool   m_bUseRoundRect;
	int    m_nRndRatio;

	// doncare 영역 관련
	BOOL   m_bUseDonCare;
	bool   m_bDrawDonCare;
	int    m_nDonCareAreaCnt;

	CPoint m_pointDoncare[5][DONTCARE_POINT];
	int    m_nDoncarePtCnt[5];

	void CreateRoi();

	BOOL  PitInROI(CPoint point);
	void  SetRoiScale(double dSfx, double dSfy);
	//void  SetRoiOrg(int _orix, int _oriy);

	void MoveRectArea(CPoint point);

	void SetROI(int _id, int nType, CString strName);
	void SetOffSetParam(int _roundratio);
	int  GetRoiID() const { return m_nRoiId; }
	int  GetRoiType() const { return m_nRoiType; }
	CString GetRoiName() const { return m_strRoiName; }

	void SetRectArea(CRect rect);
	void GetRectArea(int& nLeft, int& nTop, int& nRight, int& nBottom);

	void SetRectTracker(CRect rect);
	CRectTracker* GetTracker() { return _tracker; }

	void SetSubROI(PST_RECT pRect);
	void SetSubROI(PST_RECT pRect, int nMoveX, int nMoveY);
	PST_RECT GetSubROI() { return &m_tagRect; }

	void CreatePolyRoi(CPoint point[], int nCnt);
	void CreateDonCareArea(CPoint point[], int nCnt);
	void DeleteDonCareArea();

	void CreateSetaRect(float _seta);
	void ResetSetaRect();
};

