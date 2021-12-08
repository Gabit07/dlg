#include "pch.h"
#include "Lang.h"
#include "common/CommonUtil.h"
#include "common/CommonDefine.h"

CLang::CLang(void) : m_nLang(LANG_KOREAN)
{
	m_pIni = new CIni;

	ZeroMemory(m_wszConfigPath, sizeof(m_wszConfigPath));
	ZeroMemory(m_wszPath, sizeof(m_wszPath));

	GetModuleFileName(NULL, m_wszPath, _countof(m_wszPath));
	PathRemoveFileSpec(m_wszPath);
	swprintf_s(m_wszConfigPath, _countof(m_wszConfigPath), L"%s/config.ini", m_wszPath);
}

CLang::~CLang(void)
{
	SAFE_DELETE(m_pIni);
}

//------------------------------------------------------------------------
/**
	\brief ��� ����
*///----------------------------------------------------------------------
void CLang::SetLanguage(const int &nLangCode)
{
	wchar_t wszLang[32] = {0,};
	swprintf_s(wszLang, _countof(wszLang), L"%d", nLangCode);
	WritePrivateProfileStringW(L"lang", L"val", wszLang, m_wszConfigPath);
}

//------------------------------------------------------------------------
/**
	\brief ��� ini ��ġ ���� 
*///----------------------------------------------------------------------
bool CLang::SetLangIniPath()
{
	wchar_t wszLang[32] = {0,};
	wchar_t wszLangPath[MAX_PATH] = {0,};
	int nLang;	

	// �ٱ��� ���� ������ ���´�
	nLang = GetPrivateProfileIntW(L"lang", L"val", 0, m_wszConfigPath);

	// ���� ���� �Ⱦ����� ���� OS ���� �����Ѵ�
	if(nLang == 0)
	{
		nLang = PRIMARYLANGID(GetSystemDefaultLangID());
		
		swprintf_s(wszLang, _countof(wszLang), L"%d", nLang);
		WritePrivateProfileStringW(L"lang", L"val", wszLang, m_wszConfigPath);
	}
	
	switch(nLang)
	{
	case LANG_KOREAN:   swprintf_s(wszLangPath, _countof(wszLangPath), L"%s\\lang\\kor.ini", m_wszPath); break;
	case LANG_ENGLISH:  swprintf_s(wszLangPath, _countof(wszLangPath), L"%s\\lang\\eng.ini", m_wszPath); break;
	case LANG_CHINESE:  swprintf_s(wszLangPath, _countof(wszLangPath), L"%s\\lang\\chi.ini", m_wszPath); break;
	case LANG_JAPANESE: swprintf_s(wszLangPath, _countof(wszLangPath), L"%s\\lang\\jap.ini", m_wszPath); break;
	}

	char *szPath = CommonUtil::UNICODEtoANSI(wszLangPath);
	m_pIni->SetIniPath(szPath); 
	SAFE_MEM_DELETE(szPath);

	m_nLang = nLang;
	return true;
}

char *CLang::GetStringA(const char *szSection, const char *szKey)
{
	if(!m_pIni)
		return NULL;

	return m_pIni->GetStringA(szSection, szKey);
}

wchar_t *CLang::GetStringW(const char *szSection, const char *szKey)
{
	if(!m_pIni)
		return NULL;

	return m_pIni->GetStringW(szSection, szKey);
}

//------------------------------------------------------------------------
/**
	\brief ��� ���̵�
*///----------------------------------------------------------------------
int CLang::GetLang()
{
	return m_nLang;
}