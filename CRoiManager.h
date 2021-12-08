#pragma once

#include "SingleTone.h"
#include "CVector.h"
#include "CRoiClass.h"

#define ROI_TYPE 20

class CRoiManager
{
public:
	CRoiManager() { memset(m_nRoiTypeCount, 0, sizeof(int) * 20); }
	virtual ~CRoiManager();

private:

	CVector<CRoiClass*> m_Roibuffer;

	int m_nRoiTypeCount[ROI_TYPE];

public:

	void push(CRoiClass* r);
	int getRoitypeCount(int _type) const { return m_nRoiTypeCount[_type]; }
	auto getRoidata(int _no) { return m_Roibuffer.getdata(_no); }
	void deleteAll();
	auto getrange() { return m_Roibuffer.getbufferptr(); }
	int size() { return m_Roibuffer.getsize(); }
	void addroi(int _type, CString _strname);
	void deleteroi(int _index);
	

};

class CSingleToneROI : public CSingleTone<CRoiManager> {};