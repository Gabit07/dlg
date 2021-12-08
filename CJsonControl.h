#pragma once


#include "../VisionInspect/json/json.h"
#include <afxwin.h>



#pragma warning(disable : 4996)

#define JSON_PARAM_LENGTH 128

class CJsonControl
{

public:
	CJsonControl() = default;

private:

	CString m_strPath;
	Json::Value m_rootHeader;

	Json::Value m_rootWrite;
	Json::Value m_rootRead;

public:

	void SetPath(CString strpath);
	
	// roi
	void writeRoidata();
	void readRoidata();

	//line fit param
	void writeHoughLinefitParameter();
	void readHoughLinefitParameter();

	//binary param
	void writeBinaryParameter();
	void readBinaryParameter();

	// blob param
	void writeBlobParameter();
	void readBlobParameter();

};

