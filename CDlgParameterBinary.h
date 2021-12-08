#pragma once

#include "CInterfaceDlg.h"
#include "usercontrol/StaticColorCtrl.h"
// CDlgParameterBinary 대화 상자

class CDlgParameterBinary : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgParameterBinary)

public:
	CDlgParameterBinary(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgParameterBinary();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BINARY };
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

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonCancel();
	virtual BOOL OnInitDialog();

	void _RadioSelectWhiteBlack(UINT id);
	void _GetParameterBinary();

	CStaticColorCtrl m_staticTitle;
	int m_nRoiNo;
	int m_nType;

	int m_nBinaryth;
	int m_nMode;


public:

	void setId(int _roiId) { m_nRoiNo = _roiId; }
	void setType(int _type) { m_nType = _type; }
	
	
};
