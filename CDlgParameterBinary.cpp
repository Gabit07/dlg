// CDlgParameterBinary.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgParameterBinary.h"
#include "afxdialogex.h"
#include "common/configure.h"
#include "CParameterManager.h"
#include "resource.h"


// CDlgParameterBinary 대화 상자

IMPLEMENT_DYNAMIC(CDlgParameterBinary, CDialogEx)

CDlgParameterBinary::CDlgParameterBinary(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BINARY, pParent)
	, m_nBinaryth(0)
{

}

CDlgParameterBinary::~CDlgParameterBinary()
{
}

void CDlgParameterBinary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	
}


BEGIN_MESSAGE_MAP(CDlgParameterBinary, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgParameterBinary::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgParameterBinary::OnBnClickedButtonCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_FIND_HIGH, IDC_RADIO_FIND_LOW, _RadioSelectWhiteBlack)
END_MESSAGE_MAP()


// CDlgParameterBinary 메시지 처리기

BOOL CDlgParameterBinary::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_GetParameterBinary();

	InitControl();

	return TRUE;  
}

void CDlgParameterBinary::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	m_staticTitle.SetColor(L"BINARY PARAMETER", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);

	SetWindowTheme(GetDlgItem(IDC_RADIO_FIND_LOW)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_FIND_HIGH)->m_hWnd, L"", L"");

	CString strText;
	strText.Format(L"%d", m_nBinaryth);

	CEdit* pEditTh = (CEdit*)GetDlgItem(IDC_EDIT_TH);
	pEditTh->SetWindowTextW(strText);

	if (m_nMode == FIND_LOW)
	{
		CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_FIND_LOW);
		pRadio->SetCheck(true);
	}
	else
	{
		CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_FIND_HIGH);
		pRadio->SetCheck(true);
	}

}

BOOL CDlgParameterBinary::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}


HBRUSH CDlgParameterBinary::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT uId = pWnd->GetDlgCtrlID();

	switch (uId)
	{
	case IDC_STATIC:
	case IDC_RADIO_FIND_HIGH:
	case IDC_RADIO_FIND_LOW:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE2);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	}
	return hbr;
}

void CDlgParameterBinary::_RadioSelectWhiteBlack(UINT id)
{
	switch (id)
	{
	case IDC_RADIO_FIND_HIGH:
	{
		m_nMode = FIND_HIGH;
		break;
	}
	case IDC_RADIO_FIND_LOW:
	{
		m_nMode = FIND_LOW;
		break;
	}
	}
	Invalidate();
}

void CDlgParameterBinary::_GetParameterBinary()
{
	CParameterManager* pParam = CSingleToneParameter::GetInstance();
	ST_BINARY_PARAMETER tagBinaryParam;

	pParam->getParameter(m_nRoiNo, m_nType, (DWORD_PTR)&tagBinaryParam);

	m_nBinaryth = tagBinaryParam._gray;
	m_nMode = tagBinaryParam._mode;
}



void CDlgParameterBinary::OnBnClickedButtonUpdate()
{
	CString strText;
	CEdit* pEditTh = (CEdit*)GetDlgItem(IDC_EDIT_TH);
	pEditTh->GetWindowTextW(strText);
	CParameterManager* pParam = CSingleToneParameter::GetInstance();
	

	ST_BINARY_PARAMETER param;
	param._gray = _ttoi(strText);
	param._mode = m_nMode;

	pParam->updateParameter(m_nRoiNo, m_nType, (DWORD_PTR)&param);

	this->SendMessage(WM_CLOSE);
}


void CDlgParameterBinary::OnBnClickedButtonCancel()
{
	this->SendMessage(WM_CLOSE);
}
