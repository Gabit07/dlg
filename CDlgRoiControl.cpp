// CDlgRoiControl.cpp: 구현 파일
//

#include "pch.h"
#include "VisionInspect.h"
#include "CDlgRoiControl.h"
#include "afxdialogex.h"
#include "common/configure.h"
#include "CRoiManager.h"
#include "CDlgRoiConfig.h"
#include "CDlgParameterLinefit.h"
#include "CDlgParameterBinary.h"
#include "CDlgParameterBlob.h"



// CDlgRoiControl 대화 상자

IMPLEMENT_DYNAMIC(CDlgRoiControl, CDialogEx)

CDlgRoiControl::CDlgRoiControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROI_SETTING, pParent)
{
	
}

CDlgRoiControl::~CDlgRoiControl()
{
	
}

void CDlgRoiControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ADD_BTN, m_staticAddBtn);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_LIST_ROI, m_listRoi);
	DDX_Control(pDX, IDC_STATIC_DEL_BTN, m_staticDeleteBtn);
	DDX_Control(pDX, IDC_STATIC_PARAMETER, m_statiCParameterManagerBtn);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
}


BEGIN_MESSAGE_MAP(CDlgRoiControl, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_LBN_DBLCLK(IDC_LIST_ROI, &CDlgRoiControl::OnLbnDblclkListRoi)
END_MESSAGE_MAP()


// CDlgRoiControl 메시지 처리기


BOOL CDlgRoiControl::OnInitDialog()
{
	__super::OnInitDialog();

	InitControl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgRoiControl::InitControl()
{
	CRect rect;
	GetClientRect(rect);
	this->MoveWindow(1400, 200, rect.Width(), rect.Height());

	m_staticTitle.SetColor(L"INSPECT CONTROL", FONT_11BOLD, COLOR_BACKGRND, COLOR_WHITE2);

	m_comboType.InsertString(INSP_HOUGH_LINEFIT, L"LINE FIT");
	m_comboType.InsertString(FILTER_BINARY, L"BINARY");
	m_comboType.InsertString(INSP_BLOB, L"BLOB");
	m_comboType.InsertString(3, L"BINARY FILTER");

}

void CDlgRoiControl::_AddRoiButton()
{
	CString strName;
	CString strNum;

	int cursel = m_comboType.GetCurSel();

	ASSERT(cursel >= 0);

	m_comboType.GetLBText(cursel, strName);
	
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	pRoiManager->addroi(cursel, strName);

	strNum.Format(L"%d", pRoiManager->getRoitypeCount(cursel));
	strName.Append(strNum);

	

	m_listRoi.InsertString(pRoiManager->size()-1, strName);
}

void CDlgRoiControl::_DeleteRoiButton()
{
	CString strName;
	int cursel = m_listRoi.GetCurSel();
	if (cursel < 0) {
		AfxMessageBox(L"리스트를 선택해 주세요.");
		return;
	}

	m_listRoi.GetText(cursel, strName);
	m_listRoi.DeleteString(cursel);

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	if (pRoiManager->size() <= 0) 
		return;
	pRoiManager->deleteroi(cursel);
	
}

void CDlgRoiControl::_ParameterButton()
{
	int cursel = m_listRoi.GetCurSel();
	if (cursel < 0) {
		AfxMessageBox(L"리스트를 선택해 주세요.");
		return;
	}

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	int roiNo = pRoiManager->getRoidata(cursel)->GetRoiID();
	int type = pRoiManager->getRoidata(cursel)->GetRoiType();

	switch (type)
	{
	case INSP_HOUGH_LINEFIT:
	{
		CDlgParameterLinefit m_dlgLinefitParameter;
		m_dlgLinefitParameter.setId(roiNo);
		m_dlgLinefitParameter.setType(type);
		m_dlgLinefitParameter.DoModal();

		break;
	}
	case FILTER_BINARY:
	{
		CDlgParameterBinary m_dlgBinaryParameter;
		m_dlgBinaryParameter.setId(roiNo);
		m_dlgBinaryParameter.setType(type);
		m_dlgBinaryParameter.DoModal();

		break;
	}
	case INSP_BLOB:
	{
		CDlgParameterBlob m_dlgBlobParameter;
		m_dlgBlobParameter.setId(roiNo);
		m_dlgBlobParameter.setType(type);
		m_dlgBlobParameter.getroi(pRoiManager->getRoidata(cursel));
		m_dlgBlobParameter.DoModal();

		break;
	}
	}

}

HBRUSH CDlgRoiControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CDlgRoiControl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, COLOR_BACKGRND);

	return TRUE;
}


void CDlgRoiControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;

	m_staticAddBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		_AddRoiButton();
	}

	m_staticDeleteBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		_DeleteRoiButton();
	}

	m_statiCParameterManagerBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		_ParameterButton();
	}

	__super::OnLButtonDown(nFlags, point);
}


void CDlgRoiControl::OnMouseMove(UINT nFlags, CPoint point)
{
	m_staticAddBtn.SetOn(false);
	m_staticDeleteBtn.SetOn(false);
	m_statiCParameterManagerBtn.SetOn(false);

	CRect rect;
	m_staticAddBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		m_staticAddBtn.SetOn(true);
	}

	m_staticDeleteBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		m_staticDeleteBtn.SetOn(true);
	}

	m_statiCParameterManagerBtn.GetWindowRect(rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{
		m_statiCParameterManagerBtn.SetOn(true);
	}

	__super::OnMouseMove(nFlags, point);
}


void CDlgRoiControl::OnLbnDblclkListRoi()
{
	int cursel = m_listRoi.GetCurSel();
	if (cursel < 0)
	{
		AfxMessageBox(L"리스트를 선택해 주세요.");
		return;
	}

	CDlgRoiConfig DlgRoiConfig;
	DlgRoiConfig.GetIndexROI(cursel);
	DlgRoiConfig.DoModal();
	
}

void CDlgRoiControl::UpdateList()
{
	m_listRoi.ResetContent();

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	for (size_t index = 0; index < pRoiManager->size(); index++)
	{
		CRoiClass* pRoi = pRoiManager->getRoidata(index);

		m_listRoi.AddString(pRoi->GetRoiName());
	}
}