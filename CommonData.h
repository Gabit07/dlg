#ifndef CommonData_h__
#define CommonData_h__

#include "SingleTone.h"
#include "common/configure.h"
#include "common/Lock.h"
#include <atlcoll.h>




class CCommonData
{
public:
	CCommonData(void);
	~CCommonData(void);

	
	CFont *GetFont(const int &nIndex);
	

private:

    // SystemData 구조체 만들까?
	BOOL m_bAutoRun;
	BOOL m_bSimulMode;
	BOOL m_bMarkMachineType;
	BOOL m_bUseDotMkLog;
	int  m_nLanguageMode;
	bool m_bUseProgramReport;
    int  m_nProgramMode;
	void CreateFont();
	void DestroyFont();

	CLock m_lockIn;
	CLock m_lockOut;

private:
	
	CFont *m_pFont[FONT_COUNT];

	int m_nMarkInCount;
	int m_nMarkOutCount;

	DWORD_PTR m_pdispPtr;

public:

	void SetUseReportPopUp(bool _b) { m_bUseProgramReport = _b; }
	bool GetUseReportPopUp() const { return m_bUseProgramReport; }

	void SetLanguageMode(int n) { m_nLanguageMode = n; }
	int CheckLanguageMode() const { return m_nLanguageMode; }

	BOOL m_bProgramStart;
	BOOL m_bVirtualStart;
	BOOL m_bIsSearchLog;

	void setDispPtr(DWORD_PTR p) { m_pdispPtr = p; }
	DWORD_PTR getDispPtr() const { return m_pdispPtr; }



};

class CSingleToneCommonData : public CSingleTone<CCommonData> { };

#endif // CommonData_h__