// CDlgRoiConfig.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgRoiConfig.h"
#include "afxdialogex.h"
#include "common/configure.h"
#include "CRoiManager.h"


// CDlgRoiConfig 대화 상자

IMPLEMENT_DYNAMIC(CDlgRoiConfig, CDialogEx)

CDlgRoiConfig::CDlgRoiConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROI_CONFIG, pParent)
{
	
	m_nSelectedIndex = -1;
}

CDlgRoiConfig::~CDlgRoiConfig()
{
}

void CDlgRoiConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ROI_TITLE, m_staticTitle);
}


BEGIN_MESSAGE_MAP(CDlgRoiConfig, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgRoiConfig::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgRoiConfig::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_CHECK_ROUND_RECT, &CDlgRoiConfig::OnBnClickedCheckRoundRect)
	ON_BN_CLICKED(IDC_CHECK_DONCARE, &CDlgRoiConfig::OnBnClickedCheckDoncare)
	ON_BN_CLICKED(IDC_BUTTON_RATIO_APPLY, &CDlgRoiConfig::OnBnClickedButtonRatioApply)
	ON_BN_CLICKED(IDC_BUTTON_SETA_APPLY, &CDlgRoiConfig::OnBnClickedButtonSetaApply)
	ON_BN_CLICKED(IDC_BUTTON_DONCARE_ADD, &CDlgRoiConfig::OnBnClickedButtonDoncareAdd)
	ON_BN_CLICKED(IDC_BUTTON_DONCARE_DELETE, &CDlgRoiConfig::OnBnClickedButtonDoncareDelete)
	ON_BN_CLICKED(IDC_CHECK_USE_POLY_AREA, &CDlgRoiConfig::OnBnClickedCheckUsePolyArea)
	ON_BN_CLICKED(IDC_CHECK_DRAW_POLY, &CDlgRoiConfig::OnBnClickedCheckDrawPoly)
	ON_BN_CLICKED(IDC_CHECK_DRAW_DONCARE, &CDlgRoiConfig::OnBnClickedCheckDrawDoncare)
END_MESSAGE_MAP()


// CDlgRoiConfig 메시지 처리기


BOOL CDlgRoiConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgRoiConfig::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	m_staticTitle.SetColor(L"ROI CONTROL", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);
	
	SetWindowTheme(GetDlgItem(IDC_CHECK_ROUND_RECT)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_DONCARE)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_USE_POLY_AREA)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_DRAW_POLY)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_DRAW_DONCARE)->m_hWnd, L"", L"");

	CString strText;
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	CButton* pCheckRndRect = (CButton*)GetDlgItem(IDC_CHECK_ROUND_RECT);
	pCheckRndRect->SetCheck(pRoi->m_bUseRoundRect);

	CEdit* pEditRnd = (CEdit*)GetDlgItem(IDC_EDIT_ROUND_RATIO);
	strText.Format(L"%d", pRoi->m_nRndRatio);
	pEditRnd->SetWindowTextW(strText);

	CEdit* pEditSeta = (CEdit*)GetDlgItem(IDC_EDIT_SETA);
	strText.Format(L"%.2f", pRoi->m_tagRect.fSeta);
	pEditSeta->SetWindowTextW(strText);

	CButton* pUsePolyCheckbox = (CButton*)GetDlgItem(IDC_CHECK_USE_POLY_AREA);
	CButton* pDrawPolyCheckbox = (CButton*)GetDlgItem(IDC_CHECK_DRAW_POLY);

	if (pRoi->m_bUsePolyRoi) {
		pCheckRndRect->EnableWindow(false);
		pEditRnd->EnableWindow(false);
		pUsePolyCheckbox->SetCheck(true);
		pDrawPolyCheckbox->EnableWindow(true);
	}
	else
	{
		pCheckRndRect->EnableWindow(true);
		pEditRnd->EnableWindow(true);
		pUsePolyCheckbox->SetCheck(false);
		pDrawPolyCheckbox->EnableWindow(false);
	}


}

HBRUSH CDlgRoiConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT uCtrlID = pWnd->GetDlgCtrlID();

	switch (uCtrlID)
	{
	case IDC_STATIC:
	case IDC_CHECK_ROUND_RECT:
	case IDC_CHECK_DONCARE:
	case IDC_CHECK_DRAW_POLY:
	case IDC_CHECK_DRAW_DONCARE:
	case IDC_CHECK_USE_POLY_AREA:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE2);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CDlgRoiConfig::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}

void CDlgRoiConfig::OnBnClickedCheckRoundRect()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_ROUND_RECT);
	bool check = pCheck->GetCheck();

	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);
	pRoi->m_bUseRoundRect = check;
}

void CDlgRoiConfig::OnBnClickedButtonRatioApply()
{
	CString strText;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ROUND_RATIO);
	pEdit->GetWindowTextW(strText);

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);
	pRoi->m_nRndRatio = _ttoi(strText);

}


void CDlgRoiConfig::OnBnClickedCheckDoncare()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);
	
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_DONCARE);
	bool check = pCheck->GetCheck();
	pRoi->m_bUseDonCare = check;
	Invalidate();
}

void CDlgRoiConfig::OnBnClickedCheckDrawDoncare()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_DRAW_DONCARE);
	bool check = pCheck->GetCheck();
	pRoi->m_bDrawDonCare = check;
	Invalidate();
}

void CDlgRoiConfig::OnBnClickedButtonDoncareAdd()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_DRAW_DONCARE);
	bool check = pCheck->GetCheck();

	if (check)
	{
		pCheck->SetCheck(false);
		pRoi->m_bDrawDonCare = false;
	}
	

	pRoi->CreateDonCareArea(pRoi->m_pointDoncare[pRoi->m_nDonCareAreaCnt], pRoi->m_nDoncarePtCnt[pRoi->m_nDonCareAreaCnt]);
}

void CDlgRoiConfig::OnBnClickedButtonDoncareDelete()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);
	pRoi->DeleteDonCareArea();
}


void CDlgRoiConfig::OnBnClickedButtonSetaApply()
{
	CString strText;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SETA);
	pEdit->GetWindowTextW(strText);

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	if (!pRoi) return;

	pRoi->CreateSetaRect(_wtof(strText));

}



void CDlgRoiConfig::OnBnClickedButtonUpdate()
{
	this->SendMessage(WM_CLOSE);
}


void CDlgRoiConfig::OnBnClickedButtonCancel()
{
	if (AfxMessageBox(L"모든 효과를 취소하시겠습니까?", MB_OKCANCEL) == IDOK)
	{
		CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
		CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

		pRoi->m_nRndRatio = 0;
		pRoi->m_bUseRoundRect = false;
		pRoi->DeleteDonCareArea();
		pRoi->ResetSetaRect();
	}
	
	this->SendMessage(WM_CLOSE);
}

void CDlgRoiConfig::OnBnClickedCheckUsePolyArea()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	CButton* pRatioApplyBtn = (CButton*)GetDlgItem(IDC_BUTTON_RATIO_APPLY);
	CButton* pCheckRndRect = (CButton*)GetDlgItem(IDC_CHECK_ROUND_RECT);
	CEdit* pEditRnd = (CEdit*)GetDlgItem(IDC_EDIT_ROUND_RATIO);

	CButton* pUsePolyCheckbox = (CButton*)GetDlgItem(IDC_CHECK_USE_POLY_AREA);
	CButton* pDrawPolyCheckbox = (CButton*)GetDlgItem(IDC_CHECK_DRAW_POLY);
	
	if (pUsePolyCheckbox->GetCheck())
	{
		pRatioApplyBtn->EnableWindow(false);
		pCheckRndRect->EnableWindow(false);
		pEditRnd->EnableWindow(false);
		pDrawPolyCheckbox->EnableWindow(true);
		pRoi->m_bUsePolyRoi = true;
		//pRoi->SetRectArea(CRect(0, 0, 0, 0));
	}
	else
	{
		pRatioApplyBtn->EnableWindow(true);
		pCheckRndRect->EnableWindow(true);
		pEditRnd->EnableWindow(true);
		pDrawPolyCheckbox->EnableWindow(false);
		pRoi->m_bUsePolyRoi = false;
		pRoi->SetRectArea(CRect(10, 10, 522, 522));
	}
	Invalidate();
}


void CDlgRoiConfig::OnBnClickedCheckDrawPoly()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CRoiClass* pRoi = pRoiManager->getRoidata(m_nSelectedIndex);

	CButton* pDrawPolyCheckbox = (CButton*)GetDlgItem(IDC_CHECK_DRAW_POLY);

	if (pDrawPolyCheckbox->GetCheck())
	{
		pRoi->m_bDrawPoly = true;
	}
	else
	{
		pRoi->m_bDrawPoly = false;

	}
	Invalidate();
}



