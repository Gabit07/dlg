#pragma once

#include "CInterfaceDlg.h"
#include <memory>
#include "usercontrol/UserButton.h"
#include "usercontrol/StaticColorCtrl.h"

// CDlgRoiControl 대화 상자

class CDlgRoiControl : public CDialogEx, public CInterfaceDlg
{
	DECLARE_DYNAMIC(CDlgRoiControl)

public:
	CDlgRoiControl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgRoiControl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROI_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	CStaticColorCtrl m_staticTitle;

	CUserButton m_staticAddBtn;
	CUserButton m_staticDeleteBtn;
	CUserButton m_statiCParameterManagerBtn;

	CListBox m_listRoi;
	CComboBox m_comboType;


	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLbnDblclkListRoi();

	virtual void InitControl() override;
	virtual void InitDialog() override {};
	virtual void DeleteDialog() override {};
	virtual void BeginThread() override {};
	virtual void DeleteThread() override {};

	void _AddRoiButton();
	void _DeleteRoiButton();
	void _ParameterButton();
	
public:

	void UpdateList();
	
	
};
