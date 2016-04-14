#include "TQuery.h"
#include <stdio.h>
#include "YQComFun.h"
#include "Log.h"

bool TQuery::s_bLogSQL = false;

TQuery::TQuery(TYQDataBase *YQDatabase)
	:TYQDBQuery(YQDatabase),Params(NULL)
{
	Params = new TYQParams();
	Paramcount = 0;
	ColCount = 0;
	RecordCount = 0;
	RowsAffected = 0;
	newsql = "";
}
TQuery::~TQuery()
{
	DeleteParams();
	delete Params;
	Params = NULL;
}
void TQuery::AddSQL(const char * szSQL)
{
	newsql = "";
	TYQDBQuery::AddSQL(szSQL);
	CreateParams();
}
void TQuery::Open()
{		
	RowsAffected = 0;
	ReplaceSql();
	if (s_bLogSQL)
		LOG(LOG_INFORMATION,"TQuery::Open[%s]",GetSQL());
	TYQDBQuery::Open();	
	TYQDBQuery::First();
	ColCount = GetColCount();
	RecordCount = GetRecordCount();
}
int TQuery::ExecSQL()
{	
	RowsAffected = 0;
	ReplaceSql();
	if (s_bLogSQL)
		LOG(LOG_INFORMATION,"TQuery::ExecSQL[%s]",GetSQL());
	RowsAffected = TYQDBQuery::ExecSQL();
	return RowsAffected;
}
void TQuery::ReplaceSql()
{
	if (newsql.size() == 0)
		return;
	/*
	替换算法类似CreateParams
	*/
	std::string real;
	int count = 0;

	const char* start = newsql.c_str();
	std::list<TYQParam*>::iterator it = Params->FParams.begin();	
	while(start && (*start) != '\0')
	{
		if ((*start) == '\'')
		{
			count++;
		}
		if ((*start) == '?' && count%2==0)
		{
			if (it != Params->FParams.end())
			{
				if ((*it)->FValid == VALID_INT)
				{
					int val = (*it)->AsInteger;
					char valbuf[32];
					memset(valbuf,0,32);
					COM::itoa(val,valbuf,10);
					real += valbuf;
					start++;
				}
				if ((*it)->FValid == VALID_STR)
				{
					real += "\'";
					std::string val = (*it)->AsString;
					real += val.c_str();
					real += "\'";
					start++;
				}
				it++;
			}
		}
		real += (*start);
		start++;
	}
	TYQDBQuery::AddSQL(real.c_str());
}

void TQuery::CreateParams()
{
	char* sql = GetSQL();
	if (sql == NULL)
		return;
	DeleteParams();	
	//取":"标志的参数列，分析规则:
	//如果":"之前的"'"符号是单数个，则认为:符号是字符串内容，不做参数
	//否则做为一个参数
	int count=0;/*"'"符号的个数*/
	char* start = sql;
	char* ParamStart = NULL;

	bool add = true;
    Paramcount = 0;
	
	while(start && (*start) != '\0')
	{
		if ((*start) == '\'')
		{
			count++;
		}
		if ((*start) == ':' && count%2==0)
		{
			ParamStart = start+1;
			add = false;
		}
		if ((*start) == ' ' || (*start) == ',' || (*start) == ')')
		{
			if (ParamStart)
			{					
				char name[32];
				memset(name,0,32);
				memcpy(name,ParamStart,start-ParamStart);
				TYQParam *pa = new TYQParam(name);
				Params->FParams.push_back(pa); 
                Paramcount++;
				ParamStart = NULL;
				newsql += '?';
				add = true;
			}
		}
		if (add)
			newsql += (*start);
		start++;
	}
    //最后一个
    if (ParamStart)
    {
    	newsql += '?';        
		char name[32];
		memset(name,0,32);
		memcpy(name,ParamStart,start-ParamStart);
		TYQParam *pa = new TYQParam(name);
		Paramcount++;
		Params->FParams.push_back(pa); 
    }
	if (Paramcount <= 0)
		newsql = ""; //没有参数，则不用替换SQL
}
void TQuery::DeleteParams()
{
	Params->Clear();	
	Paramcount = 0;
}

TYQParam* TQuery::ParamByName(std::string name)
{
	if (Params)
		return Params->ParamByName(name);
	return NULL;
}

TYQParam* TQuery::ParamByName(const char* name)
{
	if (Params)
		return Params->ParamByName(name);
	return NULL;
}

