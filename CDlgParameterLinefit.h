#pragma once


#include "CInterfaceDlg.h"
#include "usercontrol/StaticColorCtrl.h"

// CDlgParameterLinefit 대화 상자

class CDlgParameterLinefit : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgParameterLinefit)

public:
	CDlgParameterLinefit(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgParameterLinefit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HOUGH_LINEFIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	virtual void InitControl() override;
	virtual void InitDialog() override {};
	virtual void DeleteDialog() override  {};
	virtual void BeginThread() override {};
	virtual void DeleteThread() override {};


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnCbnSelchangeComboDirection();

	virtual BOOL OnInitDialog();
	void _RadioSelectVerticalAndHorizontal(UINT n);
	void _GetParameterLinefit();

	CStaticColorCtrl m_staticTitle;

	int m_nRoiNo;
	int m_nType;
	int m_nSelectdirection;
	int m_nfindDirection;
	double m_dSlope;

public:

	void setId(int _roiId) { m_nRoiNo = _roiId; }
	void setType(int _type) { m_nType = _type; }
	
};
