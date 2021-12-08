#pragma once

#include "Queue.h"
#include "CInterfaceDlg.h"
#include <afxcmn.h>
#include <utility>
#include <process.h>



// CLogProc

typedef struct __ST_LOG
{
	__ST_LOG()
	{
		ZeroMemory(this, sizeof(*this));
	}
	int nStatus;
	wchar_t msg[4096];
}ST_LOG, *PST_LOG;

enum{
	LOG_SYS=0,
	LOG_NORMAL,
	LOG_WARNNING,
	LOG_ERROR,
	LOG_SPECIAL,
};
enum{
	MODE_ROLLMAP=0,
	MODE_NORMAL,
	MODE_NETWORK,

};

class CLogProc : public CInterfaceDlg
{
public:

	CLogProc() = default; /* comment kjw 안드로메다급 버그를 만나기 싫다면 생성자, 소멸자 건들지 말것*/
	virtual ~CLogProc();

protected:

	virtual void InitDialog() {};
	virtual void InitControl() {};
	virtual void DeleteDialog() {};
	virtual void BeginThread() override;
	virtual void DeleteThread() override;
	

private:

	UINT m_uiThreadId = 0;
	CString m_strFileName;

	HANDLE m_hThread = NULL;

	CRichEditCtrl *m_thisEdit = NULL;

	CQueue <PST_LOG> m_queue;

	
	BOOL _IsEmpty() { return m_queue.Empty(); }
	PST_LOG _Pop()  { return m_queue.Pop(); }
	void _ProcessRun();
	void _TextLogOut(LPCTSTR strText, COLORREF TextColor);

	static unsigned _stdcall StartThread(LPVOID pParam);

public:

	void SetConnectHandle(CRichEditCtrl* pHandle, CString _strFileName);
	void DeleteHandle(CRichEditCtrl* pHandle) { m_thisEdit = pHandle; }
	void InputLog(int nStatus, LPTSTR strMsg);
	HANDLE IsRun() const { return m_hThread; }
};

//extern CLogProc g_LogClassMarkIn;



