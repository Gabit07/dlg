// CDlgModel.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgModel.h"
#include "afxdialogex.h"
#include "common/configure.h"


// CDlgModel 대화 상자

IMPLEMENT_DYNAMIC(CDlgModel, CDialogEx)

CDlgModel::CDlgModel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODEL, pParent)
{

}

CDlgModel::~CDlgModel()
{
}

void CDlgModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
}


BEGIN_MESSAGE_MAP(CDlgModel, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgModel 메시지 처리기


BOOL CDlgModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgModel::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	m_staticTitle.SetColor(L"BINARY PARAMETER", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);
}

HBRUSH CDlgModel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT uId = pWnd->GetDlgCtrlID();

	switch (uId)
	{
	case IDC_STATIC:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE2);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	}

	return hbr;
}


BOOL CDlgModel::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}
