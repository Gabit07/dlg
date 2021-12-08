#pragma once

//#include "pch.h"
#include "SingleTone.h"

#define MAX_PARAMETER 20

typedef struct __ST_LINEFIT_PARAMETER {
	__ST_LINEFIT_PARAMETER() {
		ZeroMemory(this, sizeof(*this));
	}

	int _dirxy;
	int _dir;
	double _slope;

}ST_LINEFIT_PARAMETER, *PST_LINEFIT_PARAMETER;

typedef struct __ST_BINARY_PARAMETER {
	__ST_BINARY_PARAMETER() {
		ZeroMemory(this, sizeof(*this));
	}

	int _gray;
	int _mode;

}ST_BINARY_PARAMETER, *PST_BINARY_PARAMETER;

typedef struct __ST_BLOB_PARAMETER {
	__ST_BLOB_PARAMETER() {
		ZeroMemory(this, sizeof(*this));
	}

	int _gray;
	int _mode;
	float _minsize;
	float _maxsize;

}ST_BLOB_PARAMETER, * PST_BLOB_PARAMETER;

class CParameterManager
{
public:
	CParameterManager() = default;
	virtual ~CParameterManager() = default;

private:

	ST_LINEFIT_PARAMETER m_paramLinefit[MAX_PARAMETER];
	ST_BINARY_PARAMETER  m_paramBinary[MAX_PARAMETER];
	ST_BLOB_PARAMETER    m_paramBlob[MAX_PARAMETER];

public:

	void updateParameter(int _index, int _type, DWORD_PTR p);
	void getParameter(int _index, int _type, DWORD_PTR p);
};

class CSingleToneParameter : public CSingleTone<CParameterManager> {};