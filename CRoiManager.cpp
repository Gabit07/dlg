#include "CRoiManager.h"


CRoiManager::~CRoiManager()
{
	deleteAll();
}

void CRoiManager::addroi(int _type, CString _strname)
{
	CRoiClass* pRoi = new CRoiClass();
	pRoi->CreateRoi();	
	pRoi->SetROI(m_nRoiTypeCount[_type], _type, _strname);

	m_nRoiTypeCount[_type]++;

	push(pRoi);
}

void CRoiManager::push(CRoiClass* r)
{ 
	m_Roibuffer.push(r); 
	
}
void CRoiManager::deleteroi(int _index)
{
	CRoiClass* r = nullptr;
	r = m_Roibuffer.getdata(_index);
	m_nRoiTypeCount[r->GetRoiType()]--;
	m_Roibuffer.erase(r);
}

void CRoiManager::deleteAll()
{ 
	m_Roibuffer.erase_all(); 
	memset(m_nRoiTypeCount, 0, sizeof(int) * 20); 
}