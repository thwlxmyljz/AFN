#ifndef __SVR_H__
#define __SVR_H__

#include "YQProcess.h"

//MG进程
class TSvr : public TYQProcessMsg
{
protected:
	virtual void BeforeRun();
	virtual void OnRun();
	virtual void AfterRun();
	virtual void OnSignal(int Signal);
};
//---------------------------------------------------------------------------
//MG管理进程
class TSvrAdm : public TYQService
{
private:
	TSvr* FMyMg;
	TYQPrcSync* FYQPrcSync;
protected:
	virtual void OnSignal(int Signal);
	virtual void BeforeRun();
	virtual void OnRun();
	virtual void AfterRun();
};

#endif