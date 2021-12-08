#pragma once

#include "pch.h"
#include "CDlgVisionDisplay.h"
#include "CRoiManager.h"


class CInspectControl
{
public:
	CInspectControl() : m_displayPtr(nullptr), ntasknum(0) {};
	virtual ~CInspectControl() {};

private:

	CDlgVisionDisplay* m_displayPtr;

	static int _ParallelBinaryprocess(LPVOID lparam, int num);
	static int _ParallelBlobprocess(LPVOID lparam, int num);
	static int _ParallelLineFitprocess(LPVOID lparam, int num);

	CRoiClass m_pRoi[50];
	std::future<int> taskprocess[50];
	int ntasknum;


public:

	void getdisplayPtr(DWORD_PTR ptr) { m_displayPtr = (CDlgVisionDisplay*)ptr; }
	void inspect(CRoiClass* ptr);
	void inspectAll();
	void inspectParallelAll();
};

