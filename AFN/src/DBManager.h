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
	//���ݿ�
	TYQDataBase* FDB;		
public:
	TQuery* Query;
	/*
	��SQL��Query,���QueryΪ����Ĭ�ϼӵ�������ѯQuery��
	*/
	void OpenQuery(std::string sql,bool Select = true,TQuery* qy = NULL);
	/*
	���QueryΪ����Ĭ�ϼӵ��ر�Query
	*/
	void CloseQuery(TQuery* qy = NULL);
};

#define qopen(SQL) DBManager::Instance()->OpenQuery(SQL)
#define qclose() DBManager::Instance()->CloseQuery()
#define query() DBManager::Instance()->Query

