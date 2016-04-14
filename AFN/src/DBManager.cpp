#include "DBManager.h"
#include "LogFileu.h"

IMPLEMENT_SINGLETON(DBManager)

void DBManager::Init()
{
	int result = 0;
	FOK = FALSE;
	FDB = new TYQMySQLDabaBase();
	if(FDB == NULL)
	{
		throw (new TYQException(__FILE__, __LINE__, "new database exception")); 
	}
	if((result=FDB->OpenDatabase("127.0.0.1","root","123456","powersys")) != 0)
	{
		YQLogMaj("open database error");
		delete FDB;
		FDB = NULL;
		throw (new TYQException(__FILE__, __LINE__, "open database exception")); 
	}	
	YQLogInfo("open database ok");
	Query = new TQuery(FDB);
	FOK = TRUE;
}
void DBManager::OpenQuery(std::string sql,bool Select,TQuery* qy)
{
	try{
		qy = qy?qy:Query;
		qy->AddSQL(sql.c_str());
		if (Select)
		{
			qy->Open();
			qy->First();
		}	
		else
			qy->ExecSQL();
	}
	catch(...){
		YQLogMaj("OpenQuery exception");
	}
}
void DBManager::CloseQuery(TQuery* qy)
{
	try{
		qy = qy?qy:Query;
		qy->Close();
	}
	catch(...){
		YQLogMaj("CloseQuery exception");
	}
}
