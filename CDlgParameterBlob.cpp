// CDlgParameterBlob.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgParameterBlob.h"
#include "afxdialogex.h"
#include "common/configure.h"
#include "CParameterManager.h"
#include "inspectfunction/CInspectFunc.h"
#include "CommonData.h"


// CDlgParameterBlob 대화 상자

IMPLEMENT_DYNAMIC(CDlgParameterBlob, CDialogEx)

CDlgParameterBlob::CDlgParameterBlob(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BLOB, pParent)
{

}

CDlgParameterBlob::~CDlgParameterBlob()
{
}

void CDlgParameterBlob::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_SLIDER_TH, m_sliderctrl);
}


BEGIN_MESSAGE_MAP(CDlgParameterBlob, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgParameterBlob::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgParameterBlob::OnBnClickedButtonCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BLOB_LOW, IDC_RADIO_BLOB_HIGH, _RadioSelectBlobFind)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgParameterBlob 메시지 처리기


BOOL CDlgParameterBlob::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_GetParameterBinary();

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgParameterBlob::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	SetWindowTheme(GetDlgItem(IDC_RADIO_BLOB_HIGH)->m_hWnd, L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_BLOB_LOW)->m_hWnd, L"", L"");
	//SetWindowTheme(GetDlgItem(IDC_SLIDER_TH)->m_hWnd, L"", L"");

	m_staticTitle.SetColor(L"BLOB CONTROL", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);

	m_sliderctrl.SetRange(0, 255);
	m_sliderctrl.SetPos(m_nBinaryth);

	CButton* pRadioH = (CButton*)GetDlgItem(IDC_RADIO_BLOB_HIGH);
	CButton* pRadioL = (CButton*)GetDlgItem(IDC_RADIO_BLOB_LOW);

	if (m_nMode == FIND_HIGH)
		pRadioH->SetCheck(true);
	else
		pRadioL->SetCheck(true);

	CString strText;
	CEdit* pEditTh = (CEdit*)GetDlgItem(IDC_EDIT_TH_VALUE);
	strText.Format(L"%d", m_nBinaryth);
	pEditTh->SetWindowTextW(strText);

	CEdit* pSizeMin = (CEdit*)GetDlgItem(IDC_EDIT_SIZE_MIN);
	strText.Format(L"%.3f", m_fminsize);
	pSizeMin->SetWindowTextW(strText);

	CEdit* pSizeMax = (CEdit*)GetDlgItem(IDC_EDIT_SIZE_MAX);
	strText.Format(L"%.3f", m_fmaxsize);
	pSizeMax->SetWindowTextW(strText);
}

void CDlgParameterBlob::_GetParameterBinary()
{
	CParameterManager* pParam = CSingleToneParameter::GetInstance();
	ST_BLOB_PARAMETER tagBlobParam;

	pParam->getParameter(m_nRoiNo, m_nType, (DWORD_PTR)&tagBlobParam);

	m_nBinaryth = tagBlobParam._gray;
	m_nMode = tagBlobParam._mode;
	m_fminsize = tagBlobParam._minsize;
	m_fmaxsize = tagBlobParam._maxsize;
}

void CDlgParameterBlob::_RadioSelectBlobFind(UINT id)
{
	switch (id)
	{
	case IDC_RADIO_BLOB_HIGH:
	{
		m_nMode = FIND_HIGH;
		break;
	}
	case IDC_RADIO_BLOB_LOW:
	{
		m_nMode = FIND_LOW;
		break;
	}
	}
}

HBRUSH CDlgParameterBlob::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT uId = pWnd->GetDlgCtrlID();

	switch (uId)
	{
	case IDC_RADIO_BLOB_HIGH:
	case IDC_RADIO_BLOB_LOW:
	//case IDC_SLIDER_TH:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE2);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	case IDC_SLIDER_TH:
	{
		//pDC->SetBkColor(COLOR_BACKGRND);
		break;
	}
	}

	return hbr;
}


BOOL CDlgParameterBlob::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	//GetDlgItem(IDC_SLIDER_TH)->GetWindowRect(rect);
	//pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}


void CDlgParameterBlob::OnBnClickedButtonUpdate()
{
	CString strText;
	CEdit* pEditTh = (CEdit*)GetDlgItem(IDC_EDIT_TH_VALUE);
	pEditTh->GetWindowTextW(strText);
	m_nBinaryth = _ttoi(strText);

	CEdit* pSizeMin = (CEdit*)GetDlgItem(IDC_EDIT_SIZE_MIN);
	pSizeMin->GetWindowTextW(strText);
	m_fminsize = static_cast<float>(_wtof(strText));

	CEdit* pSizeMax = (CEdit*)GetDlgItem(IDC_EDIT_SIZE_MAX);
	pSizeMax->GetWindowTextW(strText);
	m_fmaxsize = static_cast<float>(_wtof(strText));

	CParameterManager* pParam = CSingleToneParameter::GetInstance();
	
	ST_BLOB_PARAMETER tagParam;
	tagParam._gray = m_nBinaryth;
	tagParam._mode = m_nMode;
	tagParam._minsize = m_fminsize;
	tagParam._maxsize = m_fmaxsize;

	pParam->updateParameter(m_nRoiNo, m_nType, (DWORD_PTR)&tagParam);

	if(m_displayPtr)
	    m_displayPtr->RestoreDispImage();

	this->SendMessage(WM_CLOSE);
}


void CDlgParameterBlob::OnBnClickedButtonCancel()
{
	if (m_displayPtr)
	    m_displayPtr->RestoreDispImage();
	this->SendMessage(WM_CLOSE);
}


void CDlgParameterBlob::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_TH)
	{
		int pos = m_sliderctrl.GetPos();

		CString strText;
		strText.Format(L"%d", pos);

		CEdit* pEditTh = (CEdit*)GetDlgItem(IDC_EDIT_TH_VALUE);
		pEditTh->SetWindowTextW(strText);

		CCommonData* pCommonData = CSingleToneCommonData::GetInstance();
		m_displayPtr = (CDlgVisionDisplay*)pCommonData->getDispPtr();
		//CInspectFunc::Binary(m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), m_pRoiInstance, pos, m_nMode);
		m_displayPtr->Invalidate();

	}
	__super::OnHScroll(nSBCode, nPos, pScrollBar);
}
