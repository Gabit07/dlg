#pragma once

#include "CInterfaceDlg.h"
#include "usercontrol/StaticColorCtrl.h"
// CDlgRoiConfig 대화 상자

class CDlgRoiConfig : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgRoiConfig)

public:
	CDlgRoiConfig(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgRoiConfig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROI_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	CStaticColorCtrl m_staticTitle;
	int m_nSelectedIndex;
	

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
	afx_msg void OnBnClickedCheckRoundRect();
	afx_msg void OnBnClickedCheckDoncare();
	afx_msg void OnBnClickedButtonRatioApply();
	afx_msg void OnBnClickedButtonSetaApply();
	afx_msg void OnBnClickedButtonDoncareAdd();
	afx_msg void OnBnClickedButtonDoncareDelete();

public:

	void GetIndexROI(int _idx) { m_nSelectedIndex = _idx; }
	
	
	afx_msg void OnBnClickedCheckUsePolyArea();
	afx_msg void OnBnClickedCheckDrawPoly();
	afx_msg void OnBnClickedCheckDrawDoncare();
};
