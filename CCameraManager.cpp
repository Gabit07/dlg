#include "CCameraManager.h"
#include "common/configure.h"


void CCameraManager::BeginManager()
{
	m_cameramap.InitHashTable(NEL7_14+1);
	m_cameramap.RemoveAll();
	_SetCameraData();
	_RegistDevice();
}

void CCameraManager::_DeleteManager()
{
	m_cameramap.RemoveAll();
}

void CCameraManager::_SetCameraData()
{
	for (int i = 0; i < 14; i++)
	{
		CString strCamname;
		strCamname.Format(L"NEL-4%02d", i);
		m_DirectTransmmision[i].InitCamera(4, L"NEL-4", 41 + i, 401 + i, strCamname, true);


		strCamname.Format(L"NEL-6%02d", i);
		m_UnderRefrection[i].InitCamera(5, L"NEL-6", 61 + i, 601 + i, strCamname, true);


		strCamname.Format(L"NEL-7%02d", i);
		m_EdgeTransmmision[i].InitCamera(2, L"NEL-7", 71 + i, 701 + i, strCamname, true);
	}
}

void CCameraManager::_RegistDevice()
{
	for (int i = NEL4_1; i <= NEL4_14; i++)
	{
		m_cameramap.SetAt(i, &m_DirectTransmmision[i]); // 정투과 등록
	}

	for (int i = NEL6_1; i <= NEL6_14; i++)
	{
		m_cameramap.SetAt(i, &m_UnderRefrection[i - NEL6_1]); // 하반사 등록
	}

	for (int i = NEL7_1; i <= NEL7_14; i++)
	{
		m_cameramap.SetAt(i, &m_EdgeTransmmision[i - NEL7_1]); // 엣지투과 등록
	}

}


int  CCameraManager::SearchCameraOpticNo(int pcno)
{
	for (int i = NEL4_1; i <= NEL7_14; i++)
	{
		CCameraInfo* p;
		BOOL bRet = m_cameramap.Lookup(i, p);
		
		int n = p->GetPcNo();
		if (pcno == n)
			return p->GetOpticNo();
	}

	return -1;
}