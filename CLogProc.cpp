// LogProc.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "CLogProc.h"
#include <ctime>
#include "common/configure.h"
#include "common/CommonUtil.h"



//CLogProc g_LogClassMarkIn;


// CLogProc


CLogProc::~CLogProc()
{
	m_thisEdit = NULL;
	DeleteThread();
	ATLTRACE(_T("extern class delete 타이밍 체크..\n"));
}

// CLogProc 메시지 처리기입니다.

void CLogProc::SetConnectHandle(CRichEditCtrl* pHandle, CString _strFileName)
{
	m_thisEdit = pHandle; 
	m_strFileName = _strFileName;
	BeginThread();
}

unsigned _stdcall CLogProc::StartThread(LPVOID pParam)
{
	CLogProc* pThis = (CLogProc*)(pParam);
	_ASSERTE(pThis != NULL);

	pThis->_ProcessRun();

	return 1L;
}

void CLogProc::BeginThread()
{
	HANDLE hThread;
	m_uiThreadId = 0;

	if (m_hThread)
	{
		DeleteThread();
	}

	hThread = (HANDLE)_beginthreadex(NULL, 0, CLogProc::StartThread, this, NULL, &m_uiThreadId);

	if (NULL != hThread)
	{
		ResumeThread(hThread);
		m_hThread = hThread;
		return;
	}
}

void CLogProc::DeleteThread()
{
	if (m_hThread == NULL)
	{
		return;
	}

	if (WaitForSingleObject(m_hThread, 50L) == WAIT_TIMEOUT)
	{
		//_endthreadex(m_uiThreadId);
		TerminateThread(m_hThread, 1L);
	}

	SAFE_CLOSE_HANDLE(m_hThread);
	

	m_hThread = NULL;
}

void CLogProc::_ProcessRun()
{
	while (IsRun())
	{
		if (!_IsEmpty())
		{
			PST_LOG pData = NULL;

			pData = _Pop();

			if (pData)
			{
				COLORREF color = COLOR_WHITE;
				if (pData->nStatus == LOG_NORMAL)
					color = COLOR_WHITE;
				else if (pData->nStatus == LOG_SYS)
					color = COLOR_GREEN;
				else if (pData->nStatus == LOG_WARNNING)
					color = COLOR_ORANGE;
				else if (pData->nStatus == LOG_ERROR)
					color = COLOR_RED;
				else if (pData->nStatus == LOG_SPECIAL)
					color = COLOR_YELLOW;

				_TextLogOut(pData->msg, color);
				
				SAFE_DELETE(pData);
			}

		}
		Sleep(5);
	}
	
}

void CLogProc::_TextLogOut(LPCTSTR strText, COLORREF TextColor)
{

	if (m_thisEdit == NULL)
	{
		return;
	}
	// 문자출력
	int nSize = 9;
	wchar_t* wszFont = L"Arial";
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_FACE | CFM_SIZE;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = TextColor;
	cf.yHeight = nSize * 20;
	ZeroMemory(cf.szFaceName, sizeof(cf.szFaceName));
	wcscat_s(cf.szFaceName, _countof(cf.szFaceName), wszFont);

	if (m_thisEdit)
	{
		int nLineCnt = m_thisEdit->GetLineCount();
		int first_pos = m_thisEdit->LineIndex(nLineCnt);
		m_thisEdit->SetSel(first_pos, first_pos);
		CPoint point;
		point = m_thisEdit->PosFromChar(first_pos);
		m_thisEdit->SetCaretPos(point);
		m_thisEdit->SetFocus();

		m_thisEdit->SetSelectionCharFormat(cf);
		m_thisEdit->ReplaceSel(strText);
		m_thisEdit->ReplaceSel((LPCTSTR)L"\n");

		CTime time = CTime::GetCurrentTime();
		CString strFilePath;
		strFilePath.Format(L"C:\\NEXTEYE\\BCR_MARK_PROGRAM\\BCR_MARK_LOG\\%s\\%s", time.Format("%Y%m%d"), m_strFileName);
		CommonUtil::WriteLog(strFilePath, strText);

		/* 안비우니 메모리를 차지 해서, 비움 */
		if (m_thisEdit->GetLineCount() > 1000) 
		{
			m_thisEdit->SetSel(0, m_thisEdit->GetLineCount());
			m_thisEdit->SetWindowTextW(L"");
		}
		
		
	}
	
	
}


void CLogProc::InputLog(int nStatus, LPTSTR strMsg)
{
	CTime tm = CTime::GetCurrentTime();
	CString text = tm.Format(L"[%Y-%m-%d %H:%M:%S]");
	//text.Format(L"[%d-%d-%d %d:%d:%d] %s", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(), strMsg);
	text += strMsg;
	PST_LOG ptagLog = new __ST_LOG;
	ptagLog->nStatus = nStatus;
	wcscpy(ptagLog->msg, text);

	m_queue.Push(ptagLog);

}





