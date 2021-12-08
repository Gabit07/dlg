#pragma once

#include "CInterfaceDlg.h"
#include "usercontrol/StaticColorCtrl.h"
#include "CDlgVisionDisplay.h"
// CDlgParameterBlob 대화 상자

class CDlgParameterBlob : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgParameterBlob)

public:
	CDlgParameterBlob(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgParameterBlob();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BLOB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	virtual void InitControl() override;
	virtual void InitDialog() override {};
	virtual void DeleteDialog() override {};
	virtual void BeginThread() override {};
	virtual void DeleteThread() override {};

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void _GetParameterBinary();
	void _RadioSelectBlobFind(UINT id);

	CDlgVisionDisplay* m_displayPtr;
	CSliderCtrl m_sliderctrl;
	CStaticColorCtrl m_staticTitle;

	int m_nRoiNo;
	int m_nType;

	int m_nBinaryth;
	int m_nMode;

	float m_fminsize;
	float m_fmaxsize;

	CRoiClass* m_pRoiInstance;

public:

	void setId(int _roiId) { m_nRoiNo = _roiId; }
	void setType(int _type) { m_nType = _type; }
	void getroi(CRoiClass* r) { m_pRoiInstance = r; }
	
	
	
};
