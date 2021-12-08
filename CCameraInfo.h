#pragma once

#include "CInterfaceOptic.h"


class CCameraInfo : public CInterfaceOptic
{
public:
	CCameraInfo() = default;

private:

	int m_port;
	int m_pcno;
	CString m_strCamName;

public:

	void InitCamera(int _opticno, CString _stropticname, int _port, int _pcno, CString _camname, bool _connect)
	{
		__connected = _connect;
		__OpticNo = _opticno;
		__strOpticName = _stropticname;
		m_port = _port;
		m_strCamName = _camname;
		m_pcno = _pcno;
	}

	bool IsConnected() { return __connected; }
	int GetOpticNo() { return __OpticNo; }
	int GetPort() { return m_port; }
	int GetPcNo() { return m_pcno; }
	CString GetOpticName() { return __strOpticName; }
	CString GetCameraName() { return m_strCamName; }

};

