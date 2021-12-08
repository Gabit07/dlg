
#include <mutex>
#include <vector>
#include "CInspectControl.h"
#include "inspectfunction/CInspectFunc.h"
#include "common/configure.h"
#include "CParameterManager.h"




int CInspectControl::_ParallelBinaryprocess(LPVOID lparam, int num)
{
	CInspectControl* pThis = (CInspectControl*)lparam;
	CParameterManager* pParamManager = CSingleToneParameter::GetInstance();
	ST_BINARY_PARAMETER Param;

	int _left = 0, _right = 0, _top = 0, _bottom = 0;
	
	CRoiClass* pRoi = &pThis->m_pRoi[num];
	
	pRoi->GetRectArea(_left, _top, _right, _bottom);
	
	pParamManager->getParameter(pRoi->GetRoiID(), FILTER_BINARY, (DWORD_PTR)&Param);
	
	//CInspectFunc::Binary(pThis->m_displayPtr->GetOriginImage(), pThis->m_displayPtr->GetDispImage(), pRoi, Param._gray, Param._mode);
	
	return 1;
}

int CInspectControl::_ParallelBlobprocess(LPVOID lparam, int num)
{
	CInspectControl* pThis = (CInspectControl*)lparam;
	CParameterManager* pParamManager = CSingleToneParameter::GetInstance();

	int _left = 0, _right = 0, _top = 0, _bottom = 0;

	CRoiClass* pRoi = &pThis->m_pRoi[num];

	pRoi->GetRectArea(_left, _top, _right, _bottom);

	ST_BLOB_PARAMETER Param;
	pParamManager->getParameter(pRoi->GetRoiID(), INSP_BLOB, (DWORD_PTR)&Param);

	std::vector<CRect> vFindArea;
	CInspectFunc::FindBlob(pThis->m_displayPtr->GetDispImage(), pRoi, Param._gray, Param._mode, vFindArea);
	for (auto& blob : vFindArea)
	{
		pThis->m_displayPtr->DrawRectangle(NULL, blob, COLOR_YELLOW);
	}

	return 1;
}

int CInspectControl::_ParallelLineFitprocess(LPVOID lparam, int num)
{
	CInspectControl* pThis = (CInspectControl*)lparam;
	CParameterManager* pParamManager = CSingleToneParameter::GetInstance();

	int _left = 0, _right = 0, _top = 0, _bottom = 0;

	CRoiClass* pRoi = &pThis->m_pRoi[num];

	pRoi->GetRectArea(_left, _top, _right, _bottom);

	ST_LINEFIT_PARAMETER Param;
	pParamManager->getParameter(pRoi->GetRoiID(), INSP_HOUGH_LINEFIT, (DWORD_PTR)&Param);

	double FA = 0, FB = 0, FC = 1;
	CInspectFunc::HoughTransLineFitting(pThis->m_displayPtr->GetOriginImage(), pRoi, Param._dirxy, Param._dir, Param._slope, &FA, &FB);

	double dx, dy, dx2, dy2;

	if (Param._dirxy == (int)CEdgeDirection::horizontal)
	{   // horizontal
		CInspectFunc::FindCrossPoint(FC, FA, FB, 1, 0, 0, &dx, &dy);
		CInspectFunc::FindCrossPoint(FC, FA, FB, 1, 0, _bottom - _top, &dx2, &dy2);
	}
	else
	{
		//vertical
		CInspectFunc::FindCrossPoint(FC, FA, FB, 0, 1, 0, &dx, &dy);
		CInspectFunc::FindCrossPoint(FC, FA, FB, 0, 1, -(_right - _left), &dx2, &dy2);
	}

	dx += _left;
	dy += _top;
	dx2 += _left;
	dy2 += _top;

	pThis->m_displayPtr->DrawLine(NULL, CPoint((LONG)dx, (LONG)dy), CPoint((LONG)dx2, (LONG)dy2), RGB(255, 255, 0));

	return 1;
}

void CInspectControl::inspect(CRoiClass* ptr)
{
}

void CInspectControl::inspectAll()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CParameterManager* pParamManager = CSingleToneParameter::GetInstance();

	for (auto& roi : pRoiManager->getrange())
	{
		int _left, _right, _top, _bottom;
		roi->GetRectArea(_left, _top, _right, _bottom);

		switch (roi->GetRoiType())
		{
		case INSP_HOUGH_LINEFIT:
		{
			
			ST_LINEFIT_PARAMETER Param;
			pParamManager->getParameter(roi->GetRoiID(), INSP_HOUGH_LINEFIT, (DWORD_PTR)&Param);

			double FA = 0, FB = 0, FC = 1;
			CInspectFunc::HoughTransLineFitting(m_displayPtr->GetOriginImage(), roi, Param._dirxy, Param._dir, Param._slope, &FA, &FB);

			double dx, dy, dx2, dy2;
		
			if (Param._dirxy == (int)CEdgeDirection::horizontal)
			{   // horizontal
				CInspectFunc::FindCrossPoint(FC, FA, FB, 1, 0, 0, &dx, &dy);
				CInspectFunc::FindCrossPoint(FC, FA, FB, 1, 0, _bottom - _top, &dx2, &dy2);
			}
			else
			{
				//vertical
				CInspectFunc::FindCrossPoint(FC, FA, FB, 0, 1, 0, &dx, &dy);
				CInspectFunc::FindCrossPoint(FC, FA, FB, 0, 1, -(_right - _left), &dx2, &dy2);
			}

			dx += _left;
			dy += _top;
			dx2 += _left;
			dy2 += _top;

			m_displayPtr->DrawLine(NULL, CPoint((LONG)dx, (LONG)dy), CPoint((LONG)dx2, (LONG)dy2), RGB(255, 255, 0));

			break;
		}

		case FILTER_BINARY:
		{
			ST_BINARY_PARAMETER Param;
			pParamManager->getParameter(roi->GetRoiID(), FILTER_BINARY, (DWORD_PTR)&Param);
			int distance = (_bottom - _top) / 4;

			
			auto th1 = std::async(std::launch::async, CInspectFunc::Binary, m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), CRect(_left, _top, _right, _top + distance), Param._gray, Param._mode);

		
			auto th2 = std::async(std::launch::async, CInspectFunc::Binary, m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), CRect(_left, _top + distance, _right, _top + distance * 2), Param._gray, Param._mode);
			
			
			auto th3 = std::async(std::launch::async, CInspectFunc::Binary, m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), CRect(_left, _top + distance * 2, _right, _top + distance * 3), Param._gray, Param._mode);
			
		
			auto th4 = std::async(std::launch::async, CInspectFunc::Binary, m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), CRect(_left, _top + distance * 3, _right, _bottom), Param._gray, Param._mode);

			th1.get();
			th2.get();
			th3.get();
			th4.get();
			//CInspectFunc::Binary(m_displayPtr->GetOriginImage(), m_displayPtr->GetDispImage(), roi, Param._gray, Param._mode);

			break;
		}
		case INSP_BLOB:
		{
			ST_BLOB_PARAMETER Param;
			pParamManager->getParameter(roi->GetRoiID(), INSP_BLOB, (DWORD_PTR)&Param);

			std::vector<CRect> vFindArea;
			CInspectFunc::FindBlob(m_displayPtr->GetDispImage(), roi, Param._gray, Param._mode, vFindArea);
			for (auto& blob : vFindArea)
			{
				m_displayPtr->DrawRectangle(NULL, blob, COLOR_YELLOW);
			}

			break;
		}
		}
		
	}
}


void CInspectControl::inspectParallelAll()
{
	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	CParameterManager* pParamManager = CSingleToneParameter::GetInstance();

	ntasknum = 0;

	for (auto& roi : pRoiManager->getrange())
	{
		switch (roi->GetRoiType())
		{
		case INSP_HOUGH_LINEFIT: // comment kjw 알고리즘쪽에 문제가 있는것 같다. 반복하면 뻗거나, 오히려 병렬처리가 더 느린이유 찾기
		{
			m_pRoi[ntasknum] = *roi;
			taskprocess[ntasknum] = std::async(std::launch::async, _ParallelLineFitprocess, this, ntasknum);
			break;
		}

		case FILTER_BINARY:
		{
			m_pRoi[ntasknum] = *roi;
			taskprocess[ntasknum] = std::async(std::launch::async, _ParallelBinaryprocess, this, ntasknum);
			
			break;
		}
		case INSP_BLOB:
		{
			m_pRoi[ntasknum] = *roi;
			taskprocess[ntasknum] = std::async(std::launch::async, _ParallelBlobprocess, this, ntasknum);

			break;
		}
		}
		ntasknum++;

	}

	int success = 0;
	for (int n = 0; n < ntasknum; n++)
	{
		success = taskprocess[n].get();
	}
	
	
}