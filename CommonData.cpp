#include "pch.h"
#include "CommonData.h"
#include "Lang.h"
#include "common/CommonUtil.h"
#include "common/CommonDefine.h"
#include "common/configure.h"

CCommonData::CCommonData(void) 
{
	for(int i=0; i<FONT_COUNT; ++i)
		m_pFont[i] = NULL;
	
	CreateFont();
   
	m_nMarkInCount = 0;
	m_nMarkOutCount = 0;
	m_bProgramStart = FALSE;
	m_bVirtualStart = FALSE;
	m_nLanguageMode = 0;
	m_bUseProgramReport = false;
	m_bIsSearchLog = false;

}

CCommonData::~CCommonData(void)
{
	DestroyFont();
}

//------------------------------------------------------------------------
/**
	\brief 공통 폰트 생성
*///----------------------------------------------------------------------
void CCommonData::CreateFont()
{
	wchar_t wszFont[32] = {0,};

	CLang *pLang = CSingleToneLang::GetInstance();
	if(!pLang)
		wcscat_s(wszFont, _countof(wszFont), L"맑은 고딕");

	switch(pLang->GetLang())
	{
	case LANG_KOREAN: wcscat_s(wszFont, _countof(wszFont), L"맑은 고딕"); break;  // MS Shell Dlg
	case LANG_ENGLISH: wcscat_s(wszFont, _countof(wszFont), L"Arial"); break;
	}

	int nFontSize = 0;
	int nFontWeight = 0;
	HDC pDC = GetDC(NULL);
	for(int i=0; i<FONT_COUNT; ++i)
	{
		m_pFont[i] = new CFont;
	
		switch(i)
		{
		case FONT_9NORMAL: nFontSize = 9; nFontWeight = FW_NORMAL; break;
		case FONT_9BOLD: nFontSize = 9; nFontWeight = FW_BOLD; break;
		case FONT_10NORMAL: nFontSize = 10; nFontWeight = FW_NORMAL; break;
		case FONT_10BOLD: nFontSize = 10; nFontWeight = FW_BOLD; break;
		case FONT_11NORMAL: nFontSize = 11; nFontWeight = FW_NORMAL; break;
		case FONT_11BOLD: nFontSize = 11; nFontWeight = FW_BOLD; break;
		case FONT_12NORMAL: nFontSize = 12; nFontWeight = FW_NORMAL; break;
		case FONT_12BOLD: nFontSize = 12; nFontWeight = FW_BOLD; break;
		case FONT_14NORMAL: nFontSize = 14; nFontWeight = FW_NORMAL; break;
		case FONT_14BOLD: nFontSize = 14; nFontWeight = FW_BOLD; break;
		case FONT_16NORMAL: nFontSize = 16; nFontWeight = FW_NORMAL; break;
		case FONT_16BOLD: nFontSize = 16; nFontWeight = FW_BOLD; break;
		case FONT_18NORMAL: nFontSize = 18; nFontWeight = FW_NORMAL; break;
		case FONT_18BOLD: nFontSize = 18; nFontWeight = FW_BOLD; break;
		case FONT_20NORMAL: nFontSize = 20; nFontWeight = FW_NORMAL; break;
		case FONT_20BOLD: nFontSize = 20; nFontWeight = FW_BOLD; break;
		case FONT_22NORMAL: nFontSize = 22; nFontWeight = FW_NORMAL; break;
		case FONT_22BOLD: nFontSize = 22; nFontWeight = FW_BOLD; break;
		}

		m_pFont[i]->CreateFont(-MulDiv(nFontSize, GetDeviceCaps(pDC, LOGPIXELSY), 72), 0, 0, 0, nFontWeight, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, wszFont);
	}
	ReleaseDC(NULL, pDC);
}

//------------------------------------------------------------------------
/**
	\brief 공통 폰트 삭제
*///----------------------------------------------------------------------
void CCommonData::DestroyFont()
{
	for(int i=0; i<FONT_COUNT; ++i)
	{
		if(m_pFont[i])
		{
			m_pFont[i]->DeleteObject();
			SAFE_DELETE(m_pFont[i]);
		}
	}
}

//------------------------------------------------------------------------
/**
	\brief 폰트 얻기
*///----------------------------------------------------------------------
CFont *CCommonData::GetFont(const int &nIndex)
{
	if(nIndex >= FONT_COUNT)
		return NULL;
	return m_pFont[nIndex];
}


