#include "CJsonControl.h"
#include "common/CommonUtil.h"
#include "common/configure.h"
#include "CLogProc.h"
#include "CParameterManager.h"
#include "CRoiManager.h"


void CJsonControl::SetPath(CString strpath)
{
	m_strPath = strpath;

	CFileFind find;
	BOOL bRet = find.FindFile(m_strPath);
	if (!bRet)
	{
		CommonUtil::MakeDirectory(m_strPath);
	}
}

void CJsonControl::writeRoidata()
{

	USES_CONVERSION;

	SetPath(ROI_PARAMETER);

	Json::FastWriter writer;

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();

	for (int i=0; i<pRoiManager->size(); i++)
	{
		CRoiClass* roi = pRoiManager->getRoidata(i);
		// rect
		int left = 0, right = 0, top = 0, bottom = 0;
		roi->GetRectArea(left, top, right, bottom);
		m_rootWrite["left"][i] = left;
		m_rootWrite["right"][i] = right;
		m_rootWrite["top"][i] = top;
		m_rootWrite["bottom"][i] = bottom;
		//subrect
		PST_RECT tagSubrect = roi->GetSubROI();
		m_rootWrite["seta"][i] = tagSubrect->fSeta;
		m_rootWrite["topleftx"][i] = tagSubrect->pt[0].x;
		m_rootWrite["toplefty"][i] = tagSubrect->pt[0].y;
		m_rootWrite["toprightx"][i] = tagSubrect->pt[1].x;
		m_rootWrite["toprighty"][i] = tagSubrect->pt[1].y;
		m_rootWrite["bottomleftx"][i] = tagSubrect->pt[2].x;
		m_rootWrite["bottomlefty"][i] = tagSubrect->pt[2].y;
		m_rootWrite["bottomrightx"][i] = tagSubrect->pt[3].x;
		m_rootWrite["bottomrighty"][i] = tagSubrect->pt[3].y;
		// 다각형 데이터
		m_rootWrite["use poly"][i] = roi->m_bUsePolyRoi;
		m_rootWrite["polycnt"][i] = roi->m_nPolyCnt;
		for (int polypt = 0; polypt < DONTCARE_POINT; polypt++)
		{
			m_rootWrite["poly pointx"][i][polypt] = roi->m_polyRoi[polypt].x;
			m_rootWrite["poly pointy"][i][polypt] = roi->m_polyRoi[polypt].y;
		}

		// dontcare 
		m_rootWrite["use doncare"][i] = roi->m_bUseDonCare;
		m_rootWrite["use doncarearea"][i] = roi->m_nDonCareAreaCnt;
		for (int doncarecnt = 0; doncarecnt < 3; doncarecnt++)
		{
			m_rootWrite["doncare count"][i][doncarecnt] = roi->m_nDoncarePtCnt[doncarecnt];
			for (int pt = 0; pt < DONTCARE_POINT; pt++)
			{
				m_rootWrite["doncare pointx"][i][doncarecnt][pt] = roi->m_pointDoncare[doncarecnt][pt].x;
				m_rootWrite["doncare pointy"][i][doncarecnt][pt] = roi->m_pointDoncare[doncarecnt][pt].y;
			}
		}
		

		// property
		m_rootWrite["id"][i] = roi->GetRoiID();
		m_rootWrite["type"][i] = roi->GetRoiType();
		std::string strname = CW2AEX<128>(roi->GetRoiName());
		m_rootWrite["name"][i] = strname;

		// round rect
		m_rootWrite["useround"][i] = roi->m_bUseRoundRect;
		m_rootWrite["roundratio"][i] = roi->m_nRndRatio;

	}

	m_rootWrite["roi cnt"] = pRoiManager->size();

	m_rootHeader["roi"] = m_rootWrite;

	std::string str = writer.write(m_rootHeader);

	CommonUtil::WriteToFile(CW2AEX<128>(m_strPath), str.c_str(), (int)str.length());
}

void CJsonControl::readRoidata()
{
	USES_CONVERSION;

	SetPath(ROI_PARAMETER);

	Json::Reader reader;
	char czbuffer[10000];
	ZeroMemory(czbuffer, _countof(czbuffer));

	CommonUtil::ReadFromFile(CW2AEX<128>(m_strPath), czbuffer, _countof(czbuffer));
	std::string strData = czbuffer;

	if (strData.length() == 0)
		return;


	reader.parse(strData, m_rootHeader, false);
	m_rootRead = m_rootHeader["roi"];

	CRoiManager* pRoiManager = CSingleToneROI::GetInstance();
	pRoiManager->deleteAll();
	int roiCnt = m_rootRead["roi cnt"].asInt();

	for (int i = 0; i < roiCnt; i++)
	{
		// property
		int id = m_rootRead["id"][i].asInt();
		int type = m_rootRead["type"][i].asInt();
		std::string strname = m_rootRead["name"][i].asString();
		CString strText(strname.c_str());
		pRoiManager->addroi(type, strText); // id 없으면 어떤문제 있는지?

		CRoiClass* roi = pRoiManager->getRoidata(i);
		// rect
		int left = 0, right = 0, top = 0, bottom = 0;
		left = m_rootRead["left"][i].asInt();
		right = m_rootRead["right"][i].asInt();
		top = m_rootRead["top"][i].asInt();
		bottom = m_rootRead["bottom"][i].asInt();
		roi->SetRectArea(CRect(left, top, right, bottom));

		//subrect
		PST_RECT tagSubrect = roi->GetSubROI();
		tagSubrect->fSeta = m_rootRead["seta"][i].asDouble();
		tagSubrect->pt[0].x = m_rootRead["topleftx"][i].asInt();
		tagSubrect->pt[0].y = m_rootRead["toplefty"][i].asInt();
		tagSubrect->pt[1].x = m_rootRead["toprightx"][i].asInt();
		tagSubrect->pt[1].y = m_rootRead["toprighty"][i].asInt();
		tagSubrect->pt[2].x = m_rootRead["bottomleftx"][i].asInt();
		tagSubrect->pt[2].y = m_rootRead["bottomlefty"][i].asInt();
		tagSubrect->pt[3].x = m_rootRead["bottomrightx"][i].asInt();
		tagSubrect->pt[3].y = m_rootRead["bottomrighty"][i].asInt();
		// 다각형 데이터
		roi->m_bUsePolyRoi = m_rootRead["use poly"][i].asInt();
		roi->m_nPolyCnt = m_rootRead["polycnt"][i].asInt();
		for (int polypt = 0; polypt < DONTCARE_POINT; polypt++)
		{
			roi->m_polyRoi[polypt].x = m_rootRead["poly pointx"][i][polypt].asInt();
			roi->m_polyRoi[polypt].y = m_rootRead["poly pointy"][i][polypt].asInt();
		}

		// dontcare 
		roi->m_bUseDonCare = m_rootRead["use doncare"][i].asBool();
		roi->m_nDonCareAreaCnt = m_rootRead["use doncarearea"][i].asInt();

		for (int doncarecnt = 0; doncarecnt < 3; doncarecnt++)
		{
			roi->m_nDoncarePtCnt[doncarecnt] = m_rootRead["doncare count"][i][doncarecnt].asInt();
			for (int pt = 0; pt < DONTCARE_POINT; pt++)
			{
				roi->m_pointDoncare[doncarecnt][pt].x = m_rootRead["doncare pointx"][i][doncarecnt][pt].asInt();
				roi->m_pointDoncare[doncarecnt][pt].y = m_rootRead["doncare pointy"][i][doncarecnt][pt].asInt();
			}
		}
		

		// round rect
		roi->m_bUseRoundRect = m_rootRead["useround"][i].asBool();
		roi->m_nRndRatio = m_rootRead["roundratio"][i].asInt();

	}

}

void CJsonControl::writeHoughLinefitParameter()
{
	int _selectxy = 0;
	int _dir = 0;
	double _slope = 0;

	USES_CONVERSION;

	SetPath(LINEFIT_PARAMETER);

	Json::FastWriter writer;

	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		ST_LINEFIT_PARAMETER tagparam;
		pParam->getParameter(i, INSP_HOUGH_LINEFIT, (DWORD_PTR)&tagparam);

		m_rootWrite["dirxy"][i] = tagparam._dirxy;
		m_rootWrite["dir"][i] = tagparam._dir;
		m_rootWrite["slope"][i] = tagparam._slope;
	}
	

	m_rootHeader["linefit"] = m_rootWrite;

	std::string str = writer.write(m_rootHeader);

	CommonUtil::WriteToFile(CW2AEX<128>(m_strPath), str.c_str(), (int)str.length());
}

void CJsonControl::readHoughLinefitParameter()
{
	USES_CONVERSION;

	SetPath(LINEFIT_PARAMETER);

	Json::Reader reader;
	char czbuffer[10000];
	ZeroMemory(czbuffer, _countof(czbuffer));

	CommonUtil::ReadFromFile(CW2AEX<128>(m_strPath), czbuffer, _countof(czbuffer));
	std::string strData = czbuffer;

	if (strData.length() == 0)
		return;

	int _selectxy = 0;
	int _dir = 0;
	double _slope = 0;
	
	reader.parse(strData, m_rootHeader, false);
	m_rootRead = m_rootHeader["linefit"];


	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		_selectxy = m_rootRead["dirxy"][i].asInt();
		_dir = m_rootRead["dir"][i].asInt();
		_slope = m_rootRead["slope"][i].asDouble();

		ST_LINEFIT_PARAMETER tagparam;
		tagparam._dirxy = _selectxy;
		tagparam._dir = _dir;
		tagparam._slope = _slope;
		pParam->updateParameter(i, INSP_HOUGH_LINEFIT, (DWORD_PTR)&tagparam);

	}
	
}

void CJsonControl::writeBinaryParameter()
{
	USES_CONVERSION;

	SetPath(BINARY_PARAMETER);

	Json::FastWriter writer;

	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		ST_BINARY_PARAMETER tagparam;
		pParam->getParameter(i, FILTER_BINARY, (DWORD_PTR)&tagparam);

		m_rootWrite["th"][i] = tagparam._gray;
		m_rootWrite["mode"][i] = tagparam._mode;
	}


	m_rootHeader["binary"] = m_rootWrite;

	std::string str = writer.write(m_rootHeader);

	CommonUtil::WriteToFile(CW2AEX<128>(m_strPath), str.c_str(), (int)str.length());
}

void CJsonControl::readBinaryParameter()
{
	USES_CONVERSION;

	SetPath(BINARY_PARAMETER);

	Json::Reader reader;
	char czbuffer[10000];
	ZeroMemory(czbuffer, _countof(czbuffer));

	CommonUtil::ReadFromFile(CW2AEX<128>(m_strPath), czbuffer, _countof(czbuffer));
	std::string strData = czbuffer;

	if (strData.length() == 0)
		return;

	int th = 0;
	int mode = 0;
	

	reader.parse(strData, m_rootHeader, false);
	m_rootRead = m_rootHeader["binary"];


	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		th = m_rootRead["th"][i].asInt();
		mode = m_rootRead["mode"][i].asInt();
		
		ST_BINARY_PARAMETER tagparam;
		tagparam._gray = th;
		tagparam._mode = mode;
		
		pParam->updateParameter(i, FILTER_BINARY, (DWORD_PTR)&tagparam);

	}
}

void CJsonControl::writeBlobParameter()
{
	USES_CONVERSION;

	SetPath(BLOB_PARAMETER);

	Json::FastWriter writer;

	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		ST_BLOB_PARAMETER tagparam;
		pParam->getParameter(i, INSP_BLOB, (DWORD_PTR)&tagparam);

		m_rootWrite["th"][i] = tagparam._gray;
		m_rootWrite["mode"][i] = tagparam._mode;
	}

	m_rootHeader["blob"] = m_rootWrite;

	std::string str = writer.write(m_rootHeader);

	CommonUtil::WriteToFile(CW2AEX<128>(m_strPath), str.c_str(), (int)str.length());
}

void CJsonControl::readBlobParameter()
{
	USES_CONVERSION;

	SetPath(BLOB_PARAMETER);

	Json::Reader reader;
	char czbuffer[10000];
	ZeroMemory(czbuffer, _countof(czbuffer));

	CommonUtil::ReadFromFile(CW2AEX<128>(m_strPath), czbuffer, _countof(czbuffer));
	std::string strData = czbuffer;

	if (strData.length() == 0)
		return;

	int th = 0;
	int mode = 0;

	reader.parse(strData, m_rootHeader, false);
	m_rootRead = m_rootHeader["blob"];

	CParameterManager* pParam = CSingleToneParameter::GetInstance();

	for (int i = 0; i < MAX_PARAMETER; i++)
	{
		th = m_rootRead["th"][i].asInt();
		mode = m_rootRead["mode"][i].asInt();

		ST_BLOB_PARAMETER tagparam;
		tagparam._gray = th;
		tagparam._mode = mode;

		pParam->updateParameter(i, INSP_BLOB, (DWORD_PTR)&tagparam);

	}
}