
// VisionInspectDlg.h: 헤더 파일
//

#pragma once

#include <memory>
#include "CInterfaceDlg.h"
#include "CDlgVisionDisplay.h"
#include "CDlgRoiControl.h"
#include "CInspectControl.h"
#include "usercontrol/UserButton.h"


// CVisionInspectDlg 대화 상자
class CVisionInspectDlg : public CDialogEx, public CInterfaceDlg
{
// 생성입니다.
public:
	CVisionInspectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISIONINSPECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	
	

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	virtual void InitControl() override;
	virtual void InitDialog() override;
	virtual void DeleteDialog() override;
	virtual void BeginThread() override {};
	virtual void DeleteThread() override {};

	//std::unique_ptr<CDlgVisionDisplay> m_pDlgDisplay;
	std::unique_ptr<CDlgRoiControl> m_pDlgRoiControl;
	CDlgVisionDisplay* m_pDlgDisplay;
	CInspectControl m_CInspectControl;

	CRect m_rectFrame;

	CUserButton m_staticModelBtn;
	CUserButton m_staticLoadBtn;
	CUserButton m_staticSaveBtn;
	CUserButton m_staticRoisettingBtn;
	CUserButton m_staticInspectBtn;
	CUserButton m_staticParallelBtn;
	CUserButton m_staticExitBtn;
	
	void _UserButtonLoad();
	void _HideDialog();
	void _TestInsepct();
	void _TestParallelInspect();
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangeComboZoom();
	
public:

	CRect GetWndRect() const { return m_rectFrame; }
	
	
	
	
	
};
