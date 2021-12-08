// cdib.h
// new version for WIN32
#ifndef CDIB_H
#define CDIB_H

#include "pch.h"

class CDib : public CObject
{
    enum Alloc {noAlloc, crtAlloc,
                heapAlloc}; // applies to BITMAPINFOHEADER
    DECLARE_SERIAL(CDib)
public:
    LPVOID  m_lpvColorTable;
    HBITMAP m_hBitmap;
    LPBYTE  m_lpImage;  // starting address of DIB bits
    LPBITMAPINFOHEADER m_lpBMIH; // buffer containing the
                                 //  BITMAPINFOHEADER
private:
    HGLOBAL m_hGlobal; // for external windows we need to free
                       // could be allocated by this class or
                       //  allocated externally
    Alloc m_nBmihAlloc;
    Alloc m_nImageAlloc;
    DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER
                         //  or BITMAPFILEHEADER
    int m_nColorTableEntries;

    HANDLE m_hFile;
    HANDLE m_hMap;
    LPVOID m_lpvFile;
    HPALETTE m_hPalette;
public:
    CDib();
    CDib(CSize size, int nBitCount);  // builds BITMAPINFOHEADER
    ~CDib();
    int GetSizeImage() {return m_dwSizeImage;}
    int GetSizeHeader()
        {return sizeof(BITMAPINFOHEADER) +
                sizeof(RGBQUAD) * m_nColorTableEntries;}
    CSize GetDimensions();
    BOOL AttachMapFile(const TCHAR* strPathname, BOOL bShare = FALSE);
    BOOL CopyToMapFile(const TCHAR* strPathname);
    BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE,
        HGLOBAL hGlobal = NULL);
    BOOL Draw(CDC* pDC, CPoint origin,
        CSize size); // until we implement CreateDibSection
    HBITMAP CreateSection(CDC* pDC = NULL);
    UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
    BOOL MakePalette();
    BOOL SetSystemPalette(CDC* pDC);
    BOOL Compress(CDC* pDC,
        BOOL bCompress = TRUE); // FALSE means decompress
    HBITMAP CreateBitmap(CDC* pDC);
    BOOL Read(CFile* pFile);
    BOOL Read8BitRaw(CFile* pFile, int nWidth, int nHeight);
    BOOL Write(CFile* pFile);

	//Made By Y.W KO
    BOOL Write8BitRaw(CFile* pFile);
    BOOL Write8BitGrayBMP(CFile* pFile, LPBYTE fm, int width, int height);
   void Serialize(CArchive& ar);
    void Empty();

	BOOL ReadPattern(CFile* pFile);

    void ComputeMetrics();	//@080403 private에서 Pullic로 이동함. 
private:
    void DetachMapFile();
    void ComputePaletteSize(int nBitCount);
    int MakeBMPFileHead(LPBITMAPFILEHEADER bmfh,int nWidth, int nHeight);
	BOOL MakeBMPRead(LPBITMAPINFOHEADER bmih, int nWidth, int nHeight);
    BOOL MakePalette(BOOL flag);
 
};
#endif