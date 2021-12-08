#pragma once

class CInterfaceDlg
{
	virtual void InitControl() = 0;
	virtual void InitDialog() = 0;
	virtual void DeleteDialog() = 0;
	virtual void BeginThread() = 0;
	virtual void DeleteThread() = 0;
};

