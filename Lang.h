#ifndef Lang_h__
#define Lang_h__

#include "SingleTone.h"
#include "common/Ini.h"

class CLang
{
public:
	CLang(void);
	~CLang(void);

	void SetLanguage(const int &nLangCode);
	bool SetLangIniPath();
	
	char *GetStringA(const char *szSection, const char *szKey);
	wchar_t *GetStringW(const char *szSection, const char *szKey);
	
	int GetLang();

private:
	CIni *m_pIni;
	wchar_t m_wszPath[MAX_PATH];
	wchar_t m_wszConfigPath[MAX_PATH];
	int m_nLang;

};

class CSingleToneLang : public CSingleTone<CLang> { };

#endif // Lang_h__