#pragma once
#include <list>
#include <map>
#include "YQDB.h"
#include "TQuery.h"
#include "TSingleton.h"
using namespace std;

class DBManager
{
	DEFINE_SINGLETON(DBManager)
		
protected:
	void Init();
	BOOL FOK;
	//数据库
	TYQDataBase* FDB;		
public:
	TQuery* Query;
	/*
	加SQL到Query,如果Query为空则默认加到公共查询Query中
	*/
	void OpenQuery(std::string sql,bool Select = true,TQuery* qy = NULL);
	/*
	如果Query为空则默认加到关闭Query
	*/
	void CloseQuery(TQuery* qy = NULL);
};

#define qopen(SQL) DBManager::Instance()->OpenQuery(SQL)
#define qclose() DBManager::Instance()->CloseQuery()
#define query() DBManager::Instance()->Query

