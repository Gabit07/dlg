// CDlgParameterLinefit.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgParameterLinefit.h"
#include "afxdialogex.h"
#include "common/CommonDefine.h"
#include "common/configure.h"
#include "CParameterManager.h"


// CDlgParameterLinefit 대화 상자

IMPLEMENT_DYNAMIC(CDlgParameterLinefit, CDialogEx)

CDlgParameterLinefit::CDlgParameterLinefit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HOUGH_LINEFIT, pParent)
{
	m_nRoiNo = -1;
	m_nType = -1;
	m_nSelectdirection = 0;
	m_nfindDirection = 0;
	m_dSlope = 0;
}

CDlgParameterLinefit::~CDlgParameterLinefit()
{
}

void CDlgParameterLinefit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
}


BEGIN_MESSAGE_MAP(CDlgParameterLinefit, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_HORIZONTAL, IDC_RADIO_VERTICAL, _RadioSelectVerticalAndHorizontal)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgParameterLinefit::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgParameterLinefit::OnBnClickedButtonCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, &CDlgParameterLinefit::OnCbnSelchangeComboDirection)
END_MESSAGE_MAP()


// CDlgParameterLinefit 메시지 처리기

BOOL CDlgParameterLinefit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	InitControl();

	_GetParameterLinefit();

	return TRUE;  
}

void CDlgParameterLinefit::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	SetWindowTheme(GetDlgItem(IDC_RADIO_VERTICAL)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_HORIZONTAL)->m_hWnd, L"", L"");

	m_staticTitle.SetColor(L"LINE FIT PARAMETER", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);
}

void CDlgParameterLinefit::_GetParameterLinefit()
{
	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	ST_LINEFIT_PARAMETER tagParam;
	pParam->getParameter(m_nRoiNo, m_nType, (DWORD_PTR)&tagParam);
	m_nSelectdirection = tagParam._dirxy;
	m_nfindDirection = tagParam._dir;
	m_dSlope = tagParam._slope;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIRECTION);
	pCombo->ResetContent();

	if (m_nSelectdirection == static_cast<int>(CEdgeDirection::horizontal))
	{
		CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_HORIZONTAL);
		pRadio->SetCheck(true);

		pCombo->InsertString(0, L"TO LEFT");
		pCombo->InsertString(1, L"TO RIGHT");

		if (m_nfindDirection == (int)CEdgeDirection::toLeft)
		{
			pCombo->SetCurSel(0);
		}
		else
		{
			pCombo->SetCurSel(1);
		}

		
	}
	else if (m_nSelectdirection == static_cast<int>(CEdgeDirection::vertical))
	{
		CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_VERTICAL);
		pRadio->SetCheck(true);
		
		pCombo->InsertString(0, L"TO BOTTOM");
		pCombo->InsertString(1, L"TO TOP");

		if (m_nfindDirection == (int)CEdgeDirection::toBottom)
		{
			pCombo->SetCurSel(0);
		}
		else
		{
			pCombo->SetCurSel(1);
		}

	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SLOPE_DIST);
	CString strText;
	strText.Format(L"%.2lf", m_dSlope);
	pEdit->SetWindowTextW(strText);

	
}

HBRUSH CDlgParameterLinefit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT uId = pWnd->GetDlgCtrlID();

	switch (uId)
	{
	case IDC_STATIC:
	case IDC_RADIO_HORIZONTAL:
	case IDC_RADIO_VERTICAL:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE2);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	}
	
	return hbr;
}

void CDlgParameterLinefit::_RadioSelectVerticalAndHorizontal(UINT n)
{
	if (n < 0) {
		AfxMessageBox(L"다시 클릭해주세요.");
		return;
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIRECTION);
	pCombo->ResetContent();

	switch (n)
	{
	case IDC_RADIO_HORIZONTAL:
	{
		m_nSelectdirection = static_cast<int>(CEdgeDirection::horizontal);

		pCombo->InsertString(0, L"TO LEFT");
		pCombo->InsertString(1, L"TO RIGHT");
		
		break;
	}
	case IDC_RADIO_VERTICAL:
	{
		m_nSelectdirection = static_cast<int>(CEdgeDirection::vertical);
		pCombo->InsertString(0, L"TO BOTTOM");
		pCombo->InsertString(1, L"TO TOP");
		
		break;
	}
	}
}

BOOL CDlgParameterLinefit::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}


void CDlgParameterLinefit::OnBnClickedButtonUpdate()
{
	CString strText;
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SLOPE_DIST);
	pEdit->GetWindowTextW(strText);
	double slope = _wtof(strText);
	m_dSlope = slope;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIRECTION);
	
	CParameterManager* pParam = CSingleToneParameter::GetInstance();
	
	ST_LINEFIT_PARAMETER param;
	param._dirxy = m_nSelectdirection;
	param._dir = m_nfindDirection;
	param._slope = m_dSlope;

	pParam->updateParameter(m_nRoiNo, m_nType, (DWORD_PTR)&param);
	
	this->SendMessage(WM_CLOSE);
}


void CDlgParameterLinefit::OnBnClickedButtonCancel()
{
	this->SendMessage(WM_CLOSE);
}


void CDlgParameterLinefit::OnCbnSelchangeComboDirection()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIRECTION);
	int cursel = pCombo->GetCurSel();

	if (cursel < 0) {
		AfxMessageBox(L"콤보박스를 선택해 주세요.");
		return;
	}

	if (m_nSelectdirection == (int)CEdgeDirection::horizontal)
	{
		if (cursel == 0)
		{
			m_nfindDirection = (int)CEdgeDirection::toLeft;
		}
		else
		{
			m_nfindDirection = (int)CEdgeDirection::toRight;
		}

	}
	else
	{
		if (cursel == 0)
		{
			m_nfindDirection = (int)CEdgeDirection::toBottom;
		}
		else
		{
			m_nfindDirection = (int)CEdgeDirection::toTop;
		}
	}
}
