#pragma once

#include "CInterfaceDlg.h"
#include "usercontrol/StaticColorCtrl.h"
// CDlgModel 대화 상자

class CDlgModel : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgModel)

public:
	CDlgModel(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgModel();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODEL };
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


	CStaticColorCtrl m_staticTitle;
};
