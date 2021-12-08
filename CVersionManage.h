#pragma once

#include "pch.h"
#include "common/CommonUtil.h"

#define __VERSION_MANAGE_PATH__ L"C:\\NEXTEYE\\BCR_MARK_PROGRAM\\BCR_MARK_DATA\\REPORT\\"
#define __VERSION_MANAGE_FILE__ L"ProgramVersion.txt"
#define __VERSION_MANAGE__ __VERSION_MANAGE_PATH__ __VERSION_MANAGE_FILE__

/* 비교적 중요한 클래스나 모듈에는 해당 클래스를 가능한 삽입한다.*/

class CVersionManage
{
private:

	CString date;
	CString Time;
	CString Reference;

public:
	CVersionManage(CString _date, CString _time, CString _reference) : date(_date), Time(_time), Reference(_reference) {}
	virtual ~CVersionManage() = default;
	
	void write() {
		CString strDate, strTime, strReference;
		strDate.Format(L"Compile Date: %s", date);
		strTime.Format(L"Compile Time: %s", Time);
		strReference.Format(L"<< File Reference: %s >>", Reference);

		CommonUtil::WriteLog(__VERSION_MANAGE__, strReference);
		CommonUtil::WriteLog(__VERSION_MANAGE__, strDate);
		CommonUtil::WriteLog(__VERSION_MANAGE__, strTime);
	}
};
