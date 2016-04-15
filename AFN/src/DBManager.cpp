#include "DBManager.h"
#include "LogFileu.h"
#include "YQConfig.h"
#include "Dir.h"
#include "Log.h"

IMPLEMENT_SINGLETON(DBManager)

void DBManager::Init()
{
	int result = 0;
	FOK = FALSE;
	TIniFile* IniFile = new TIniFile(INIFILE);
    if (!IniFile)
    {
        LOG("Read ini error!",LOG_CRITICAL);
    	return;
    }
    
    std::string DbIP         = IniFile->ReadString("DB", "DBServer", "127.0.0.1");
	std::string DbName       = IniFile->ReadString("DB", "DBName", "powersys");
	std::string DbUser         = IniFile->ReadString("DB", "DBUser", "root");
	std::string DbPwd       = IniFile->ReadString("DB", "DBPwd", "123456");

	delete IniFile;
	IniFile = NULL;

	LOG(LOG_INFORMATION,"db(%s,%s,%s,%s)",DbIP.c_str(),DbName.c_str(),DbUser.c_str(),DbPwd.c_str());
	FDB = new TYQMySQLDabaBase();
	if(FDB == NULL)
	{
		throw (new TYQException(__FILE__, __LINE__, "new database exception")); 
	}
	if((result=FDB->OpenDatabase(DbIP.c_str(),DbUser.c_str(),DbPwd.c_str(),DbName.c_str())) != 0)
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
		YQLogInfo(sql.c_str());
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
