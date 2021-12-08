#pragma once

#include "pch.h"
#include <atlcoll.h>
#include "SingleTone.h"
#include "CCameraInfo.h"

/* 향후 IJP 마킹기가 사라질 경우 사용될 것으로 판단하여 구성*/

class CCameraManager
{
public:
	CCameraManager() = default;

private:

	CAtlMap<int, CCameraInfo*> m_cameramap;

	CCameraInfo m_DirectTransmmision[14];
	CCameraInfo m_UnderRefrection[14];
	CCameraInfo m_EdgeTransmmision[14];

	void _SetCameraData();
	void _RegistDevice();
	void _DeleteManager();

public:

	void BeginManager();
	int  SearchCameraOpticNo(int pcno);
};


class CSingleToneCameraManager : public CSingleTone<CCameraManager> { };

