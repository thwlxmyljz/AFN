#include "YQDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "YQCommon.h"
#include "LogFileu.h"
#include "YQErrCode.h"

TYQDataBase::TYQDataBase(db_type_t dbType)
{
	m_dbType = dbType;
}

TYQDataBase::~TYQDataBase()
{

}
db_type_t TYQDataBase::GetDBType()
{
	return m_dbType;
}

TYQMySQLDabaBase::TYQMySQLDabaBase():TYQDataBase(MYSQL_DB)
{

}
TYQMySQLDabaBase::~TYQMySQLDabaBase()
{

}
int TYQMySQLDabaBase::OpenDatabase(const char *host,const char *user,const char *passwd,const char *dbname)
{
	
	if( mysql_init(&m_mySQL) == NULL ) 
	{ 		
		return -1; 
	} 		

	if (mysql_real_connect(&m_mySQL,host,user,passwd,dbname,0,NULL,0) == NULL) 
	{ 	
		return -1; 
	} 

	if( 0 != SetCharacter())
		return -1;	

	return 0;	

}
MYSQL *TYQMySQLDabaBase::GetMySQLHandle()
{
	return &m_mySQL;
}
int TYQMySQLDabaBase::Close()
{
	mysql_close(&m_mySQL);
	return 0;
}

int TYQMySQLDabaBase::CheckServerStatus()
{
	int nRet = mysql_ping(&m_mySQL);

	return nRet;


}

const char* TYQMySQLDabaBase::GetServerVer()
{
	return mysql_get_server_info(&m_mySQL) ;

}

int TYQMySQLDabaBase::SetCharacter()
{
	MY_CHARSET_INFO cs;
	mysql_get_character_set_info(&m_mySQL, &cs);

	if(strcmp(cs.csname,"utf8") != 0)
	{
		if (mysql_set_character_set(&m_mySQL, "utf8"))
		{
			return -1;
		}
	}
	return 0;
}

TYQDBQuery::TYQDBQuery(TYQDataBase *YQDatabase)
{
	assert(NULL != YQDatabase);
	
	if(YQDatabase->GetDBType() == MYSQL_DB)
	{
		m_Query = new TYQMySQLQuery(YQDatabase);
		
	}
	else if(YQDatabase->GetDBType() == SYBASE_DB)
	{
		m_Query = NULL;
	}
	
	assert(m_Query != NULL);
}
//---------------------------------------------------------------------------
TYQDBQuery::~TYQDBQuery()
{
	if(m_Query != NULL)
	{
		delete m_Query;
		m_Query = NULL;
	}
}


int TYQDBQuery::ExecSQL()
{
	try
	{
		if(m_Query == NULL)
			return -1;			
		return m_Query->ExecSQL();
		
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "ExecSQL failed")); 
	}
}
char *TYQDBQuery::GetSQL()
{
	if(m_Query == NULL)
		return NULL;
	//printf("exec sql =%s\r\n",m_Query->GetSQL());
	return m_Query->GetSQL();
}
void TYQDBQuery::Open()
{
	try
	{
		if(m_Query == NULL)
			return ;
		m_Query->Open();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "Open failed")); 
	}

}
void TYQDBQuery::Close() 
{
	try
	{
		if(m_Query == NULL)
			return ;
		m_Query->Close();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "Close failed")); 
	}
}
void TYQDBQuery::AddSQL(const char *szSQL)
{
	try
	{
		if(m_Query == NULL)
			return ;
		m_Query->AddSQL(szSQL);
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "AddSQL failed")); 
	}
}
void TYQDBQuery::ClearSQL()
{
	try
	{
		if(m_Query == NULL)
			return ;
		m_Query->ClearSQL();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "ClearSQL failed")); 
	}

}
int TYQDBQuery::GetRecordCount() 
{
	try
	{
		if(m_Query == NULL)
			return 0;
		return m_Query->GetRecordCount();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "GetRecordCount failed")); 
	}
}
int TYQDBQuery::GetColCount()
{
	try
	{
		if(m_Query == NULL)
			return 0;
		return m_Query->GetColCount();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "GetColCount failed")); 
	}
}
bool TYQDBQuery::IsEof()
{
	try
	{
		if(m_Query == NULL)
			return true;
		return m_Query->IsEof();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "IsEof failed")); 
	}
}

void TYQDBQuery::First()
{
	try
	{
		if(m_Query == NULL)
			return ;
		m_Query->First();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "First failed")); 
	}
}
void TYQDBQuery::Last()
{
	try
	{
		if(m_Query == NULL)
			return ;
		 m_Query->Last();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "Last failed")); 
	}
}
void TYQDBQuery::Next()
{
	try
	{
		if(m_Query == NULL)
			return ;
		 m_Query->Next();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "Next failed")); 
	}
}
TParams *TYQDBQuery::FieldByName(const char *szFieldName)
{
	try
	{
		if(m_Query == NULL)
			return NULL;
		return m_Query->FieldByName(szFieldName);
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "FieldByName failed")); 
	}
}
TParams *TYQDBQuery::FieldByID(unsigned int unFieldID)
{
	try
	{
		if(m_Query == NULL)
			return NULL;
		return m_Query->FieldByID(unFieldID);
	}	
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "FieldByID failed")); 
	}
}
/*
unsigned int TYQDBQuery::GetLastErrNo()
{
	try
	{
		if(m_Query == NULL)
			return 0;
		return m_Query->GetLastErrNo();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "GetLastErrNo failed")); 
	}
}
 const char *TYQDBQuery::GetLastErrMsg()
{
	try
	{
		if(m_Query == NULL)
			return NULL;
		return m_Query->GetLastErrMsg();
	}
	catch(...)
	{
		throw (new TYQException(__FILE__, __LINE__, "GetLastErrMsg failed")); 
	}
}*/
	 

//---------------------------------------------------------------------------

/*��ѯ����*/
TYQDBBaseQuery::TYQDBBaseQuery(TYQDataBase *YQDatabase)
{
	assert(NULL != YQDatabase);
	m_FSQL            = NULL;  
	nRecordCount    = 0;
	m_FYQDatabase = YQDatabase;	
}

TYQDBBaseQuery::~TYQDBBaseQuery()
{
	ClearSQL();
}


void TYQDBBaseQuery::AddSQL(const char *szSQL)
{
	if(szSQL == NULL)
		return;

	ClearSQL();
	if(szSQL == NULL)                          
	{
		m_FSQL = new char[1];    // ���ܼ� NULL �ж������
		if(m_FSQL != NULL)
			*m_FSQL = '\0'; 
	}  
	else
	{
		m_FSQL = new char[strlen(szSQL) + 1];  // ���ܼ� NULL �ж������  
		if(m_FSQL != NULL)
		{
			memset(m_FSQL,0,strlen(szSQL) + 1);
			strcpy(m_FSQL, szSQL);  
		}
	}
}
void TYQDBBaseQuery::ClearSQL()
{
	if(NULL != m_FSQL)
		delete []m_FSQL;
}
char *TYQDBBaseQuery::GetSQL()
{
	return m_FSQL;
}

//MYSQL�Ĳ�ѯ��*/
TYQMySQLQuery::TYQMySQLQuery(TYQDataBase *YQDatabase):TYQDBBaseQuery(YQDatabase)
{
	m_res = NULL;
	field    = NULL;	
	bEof   = true;
		pParms = new TParams();

}
TYQMySQLQuery::~TYQMySQLQuery()
{
	if(NULL !=	pParms)
	{
		delete 	pParms;
		pParms = NULL;
	}	
}
/************************************************************************
* �� �� ���� Open����
* ��    �ã� MYSQL SELECT ��Ĳ�ѯ
* �����������
* �����������
*************************************************************************/
void TYQMySQLQuery::Open()
{	
    if (m_FSQL == NULL)
    {
    	return;
    }     
	Close();

	int nRet  = 0;
	
	nRet = mysql_query(m_FYQDatabase->GetMySQLHandle(),m_FSQL);
	if(nRet != 0) 
	{ 		
		return;
	} 

	m_res = mysql_store_result(m_FYQDatabase->GetMySQLHandle()); 

	if(m_res == NULL)
		bEof = true;
	else
	{
		if(m_res->row_count == 0)
			bEof = true;
		else
			bEof = false;
	}
		
}

/************************************************************************
* �� �� ���� ExecSQL����
* ��    �ã� MYSQL INSERT,UPDATE,DELETE ��Ĳ�ѯ
* �����������
* ���������-1 ʧ�ܣ�>=0 ����Ӱ������
*************************************************************************/
int TYQMySQLQuery::ExecSQL()
{
	int nRet  = 0;
	if (m_FSQL == NULL)
	{
		return -1;
	}     
	Close();


	
	nRet = mysql_query(m_FYQDatabase->GetMySQLHandle(),m_FSQL);
	if(nRet != 0) 
	{ 		
		return -1;
	} 

	m_res = mysql_store_result(m_FYQDatabase->GetMySQLHandle()); 


	if(m_res == NULL)
	{
		if(mysql_field_count(m_FYQDatabase->GetMySQLHandle()) == 0) // query does not return data            // (it was not a SELECT)
		{           
			return mysql_affected_rows(m_FYQDatabase->GetMySQLHandle()); 

		}   
		else // mysql_store_result() should have returned data        
		{    
			return -1;
		}
	}	
	bEof = true;
	return -1;
}
/************************************************************************
* �� �� ���� Close����
* ��    �ã� �ͷŲ�ѯ��Դ
* �����������
* �����������
*************************************************************************/
void TYQMySQLQuery::Close()
{
	if(m_res != NULL)
	{
		mysql_free_result(m_res); 
		m_res = NULL;
	}
	
	bEof   = true;
}
/************************************************************************
* �� �� ���� GetRecordCount����
* ��    �ã� ����SELECT ���ѯ�������Ŀ
* �����������
* �����������
*************************************************************************/
int TYQMySQLQuery::GetRecordCount()
{
	if(m_res == NULL)
		return 0;
	else 
		return (int)mysql_num_rows(m_res);
}

bool TYQMySQLQuery::IsEof()
{
	return bEof;

}

void TYQMySQLQuery::First()
{
	MYSQL_ROW m_row; 
	if(m_res == NULL)
		return;
	mysql_data_seek(m_res, 0);
	m_row = mysql_fetch_row(m_res);
	if(m_row != NULL)
		bEof = false;
	else
		bEof = true;

}
void TYQMySQLQuery::Last()
{
	if(m_res == NULL)
		return;

	mysql_data_seek(m_res, 	mysql_num_rows(m_res) -1);
}

void TYQMySQLQuery::Next()
{
	MYSQL_ROW m_row; 
	if(m_res == NULL)
		return;
	
	m_row = mysql_fetch_row(m_res);
	if(m_row != NULL)
		bEof = false;
	else
		bEof = true;
}
/************************************************************************
* �� �� ���� FieldByName����
* ��    �ã� ���ؽ�������ֶ�����
* �����������
* �����������
*************************************************************************/
TParams*TYQMySQLQuery::FieldByName(const char *szFieldName)
{
	MYSQL_FIELD *field;

	int i=0;
	if(m_res == NULL)
		return NULL;
	mysql_field_seek(m_res,0);

	while((field = mysql_fetch_field(m_res))
		&& (strcmp(field->name,szFieldName) != 0))
	{
		i++;
	}

	if(field != NULL)
	{
	pParms->SetValue(m_res->current_row[i]);
		
		
		//return (TParams*)m_res->current_row[i];
	}
	else		
		
		pParms->SetValue(NULL);
	return pParms;

}
/************************************************************************
* �� �� ���� FieldByID����
* ��    �ã� ���ؽ�������ֶ�����ID
* �����������
* �����������
*************************************************************************/
TParams *TYQMySQLQuery::FieldByID(unsigned int unFieldID)
{
	if(unFieldID > mysql_field_count(m_FYQDatabase->GetMySQLHandle()))
//		return NULL;
	pParms->SetValue(NULL);
	else

		pParms->SetValue(m_res->current_row[unFieldID]);	


	
	return pParms;
}/*
unsigned int TYQMySQLQuery::GetLastErrNo()
{
	return   mysql_errno(m_FYQDatabase->GetMySQLHandle()) ;
}
const char *TYQMySQLQuery::GetLastErrMsg()
{
	return   mysql_error(m_FYQDatabase->GetMySQLHandle()) ;
}*/

//TParams��ʵ�ֲ���
TParams::TParams()
{
	Value = NULL;
}
TParams::~TParams()
{
	
}
int TParams::AsInteger()
{
	int ret = 0;
	try
	{	
		if(Value == NULL)
			return 0;
		ret=atoi((char*)Value);
		return ret;
	}
	catch(...)
	{
		LogFile->WriteLog("TParams AsInteger exception", LOG_MAJOR);
		return 0;
	}

};
char *TParams::AsString()
{

	if(Value == NULL)
		return NULL;
	return (char*)Value;
}
char TParams::AsChar()
{
    if(Value == NULL)
    	    return ' ';
    return ((char*)Value)[0];
}


double TParams::AsFloat()
{
	double ret = 0;
	try
	{	
		if(Value == NULL)
			return 0;
		ret=atof((char*)Value);
		return ret;
	}
	catch(...)
	{
		LogFile->WriteLog("TParams AsFloat exception", LOG_MAJOR);
		return 0;
	}
}
void TParams::SetValue(void *v)
{
	Value = v;
}

void TParams::SetFieldName(const char*Name)
{
	memset(FieldName,0,32);
	if(NULL != Name)
		strcpy(FieldName,Name);
}
const char mons[12][4]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

time_t TParams::AsDateTime()
{
	int     ret         = 0;
	char   *p         = NULL;
	tm gettm;
	time_t rettmp;

	try
	{	
		if(Value == NULL)
			return rettmp;

		p = strchr((char*)Value,'-');
		if(p == NULL)
			ret = ParamSybaseDateTime((char*)Value,gettm);
		else
			ret = ParamMySQLDateTime((char*)Value,gettm);

		if(ret != 0)
		{
			LogFile->WriteLog("TParams AsDateTime exception", LOG_MAJOR);
			return rettmp;
		}
		
		rettmp =mktime(&gettm);
		return rettmp;
	}
	catch(...)
	{	
		LogFile->WriteLog("TParams AsFloat exception", LOG_MAJOR);
		throw (new TYQException(__FILE__, __LINE__, "AsDateTime failed")); 

	}
}

/************************************************************************
* �� �� ���� ParamMySQLDateTime����
* ��    �ã� ���ʹ�MySQL�ж�������ʱ�������ַ���
* ��������ssrc-sybase��ʱ�䴮��
* ���������tm��ʽʱ��0-�ɹ�-1ʧ��
//��ʽ2008-12-12 12:00:34
*************************************************************************/
int TParams::ParamMySQLDateTime(char *src,tm &ftm)
{
	char sztmpdata[10];	
	char *p1;
	char *p2;
	char *tmppos = NULL;

	if(src == NULL)
		return -1;	
	
	try
	{	
		tmppos = new char[strlen(src)];
		if(tmppos == NULL)
			return -1;

		memset(tmppos,0,strlen(src));
		strcpy(tmppos,src);
		p1 = tmppos;

		//��
		p2 = strchr (p1, '-');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2 - p1 > 4) //��
		{
			delete tmppos;
			return -1;
		}

		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);		
		ftm.tm_year= atoi(sztmpdata) - 1900;
		

		//��ȡ�·�
		ltrim(p2,'-');
		p1=p2;
		
	  	p2 = strchr (p2, '-');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2 - p1 > 2)
		{
			delete tmppos;
			return -1;
		}

		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);
		ftm.tm_mon=atoi(sztmpdata);

		//��ȡ����
		ltrim(p2,'-');
		p1=p2;
	  	p2 = strchr (p2, ' ');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_mday= atoi(sztmpdata);



		//ʱ		
		ltrim(p2,' ');
		p1=p2;

	  	p2 = strchr (p2, ':');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_hour= atoi(sztmpdata);



		//��
		ltrim(p2,':');
		p1=p2;

	  	p2 = strchr (p2, ':');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_min= atoi(sztmpdata);

		//��
		ltrim(p2,':');
		p1=p2;

	  	
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,strlen(p1));

		ftm.tm_sec= atoi(sztmpdata);		


		ftm.tm_isdst = 0;
		delete tmppos;		
		return 0;

	}
	catch(...)
	{
		delete tmppos;
		LogFile->WriteLog("TParams AsInteger exception", LOG_MAJOR);
		return -1;
	}
}
/************************************************************************
* �� �� ���� ParamSybaseDateTime����
* ��    �ã� ���ʹ�Sybase�ж�������ʱ�������ַ���
* ��������ssrc-sybase��ʱ�䴮��
* ���������tm��ʽʱ��0-�ɹ�-1ʧ��
//��ʽ2008 27 2008  4:34:20:000PM
*************************************************************************/
int TParams::ParamSybaseDateTime(char *src,tm &ftm)
{

	char sztmpdata[10];	
	 char *p1;
	 char *p2;
	  char *tmppos = NULL;
	//tm ftm;	
	int i = 0;

	if(src == NULL)
		return -1;	
	
	try
	{	
		tmppos = new char[strlen(src)];
		if(tmppos == NULL)
			return -1;

		memset(tmppos,0,strlen(src));
		strcpy(tmppos,src);
		p1 = tmppos;
		
		p2 = strchr (p1, ' ');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2 - p1 > 3) //�·�ֻ�������ַ�
		{
			delete tmppos;
			return -1;
		}

		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);


		for(i=0;i<12;i++)
		{
#ifdef _WIN32
			if(stricmp(mons[i],sztmpdata)== 0)
#else
			if(strcasecmp(mons[i],sztmpdata)== 0)
#endif			
			{
				break;
			}
		}
		if(i <=12)			
			ftm.tm_mon= i+1;
		else
		{
			delete tmppos;
			return -1;
		}
		

		//��ȡ����
		ltrim(p2,' ');
		p1=p2;


	  	p2 = strchr (p2, ' ');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2 - p1 > 2)
		{
			delete tmppos;
			return -1;
		}

		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);
		ftm.tm_mday=atoi(sztmpdata);

		//��ȡ���
		ltrim(p2,' ');
		p1=p2;
	  	p2 = strchr (p2, ' ');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 4)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_year= atoi(sztmpdata) - 1900;



		//ʱ
		ltrim(p2,' ');
		p1=p2;

	  	p2 = strchr (p2, ':');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_hour= atoi(sztmpdata);



		//��
		ltrim(p2,':');
		p1=p2;

	  	p2 = strchr (p2, ':');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_min= atoi(sztmpdata);

		//��
		ltrim(p2,':');
		p1=p2;

	  	p2 = strchr (p2, ':');
		if (p2 == NULL)
		{
			delete tmppos;
			return -1;
		}
		if(p2-p1 > 2)
		{
			delete tmppos;
			return -1;
		}
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,p2-p1);

		ftm.tm_sec= atoi(sztmpdata);		


		p2=p2+4;
		p1=p2;
	  	
		memset(sztmpdata,0,10);
		strncpy(sztmpdata,p1,strlen(p2));

#ifdef _WIN32
			if(stricmp(sztmpdata,"PM")== 0)
#else
			if(strcasecmp(sztmpdata,"PM")== 0)
#endif	
			ftm.tm_hour+=12;
		ftm.tm_isdst = 0;

		delete tmppos;
		return 0;

	}
	catch(...)
	{
		delete tmppos;
		LogFile->WriteLog("TParams AsInteger exception", LOG_MAJOR);
		return -1;
	}
}

/************************************************************************
* �� �� ���� ltrim����
* ��    �ã� ȥ���ַ���Sǰ��ĳ���ض����ַ�c
* ���������s-Ҫȥ�����ַ�����c-ָ���ַ���
* �����������
*************************************************************************/
void TParams::ltrim(char *s,char c)
{
	// ȥ��ǰ����
	int l=0,p=0,k=0;
	l = strlen(s);
	if( l == 0 )
		return;
	p = 0;
	while( s[p] == c || s[p] == '\t' )  
		p++;
	if( p == 0 )
		return;
	while( s[k] != '\0') 
		s[k++] = s[p++];
	return;
}

