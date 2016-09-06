#ifndef __SVR_H__
#define __SVR_H__

#include "YQProcess.h"

//MG����
class TSvr : public TYQProcessMsg
{
protected:
	virtual void BeforeRun();
	virtual void OnRun();
	virtual void AfterRun();
	virtual void OnSignal(int Signal);
};
//---------------------------------------------------------------------------
//MG�������
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