/****************************************************************************
1:打开数据库
	//MYSQL数据库
	TYQMySQLDabaBase *m_DB = new TYQMySQLDabaBase();
	if(m_DB == NULL)
		return -1;


	if(m_DB->OpenDatabase("192.168.1.99","root","yq902902","switch") != 0)
	{
		printf("open data faile\r\n");
	}
	else
	{
		printf("open databse OK\r\n");
	}


	//Sybase数据库
	TYQSyBaseDabaBase *m_DB = new TYQSyBaseDabaBase();
	if(m_DB == NULL)
		return -1;

	if(m_DB->OpenDatabase("NSMSSERVER","sa","yq902902","SWITCH") != 0)
	{
		printf("open data faile\r\n");
	}
	else
	{
		printf("open databse OK\r\n");
	}
2:创建查询
	TYQDBQuery *m_query = new TYQDBQuery(m_DB);

	m_query->AddSQL("select * from AD_BOOKTYPE");

	m_query->Open();
	
	printf("record = %d\r\n",m_query->GetRecordCount());

	m_query->First();
	while(!m_query->IsEof())
	{
		printf("%12d",m_query->FieldByName("CORPID")->AsInteger());
		printf("%12d",m_query->FieldByName("BOOKID")->AsInteger());
		printf("%12s\r\n",m_query->FieldByName("NAME")->AsString());
		m_query->Next();
	}
	m_query->Close();
	delete m_query;
	m_DB->Close();
	delete m_DB;
****************************************************************************/


#ifndef YQDBH
#define YQDBH
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "mysql.h"
#include "yqlist.h"
#include <time.h>

typedef enum db_fieldtype_t
{
    YQDB_CHAR          = 0,
    YQDB_VARCHAR       = 1,
    YQDB_INT           = 2,
    YQDB_SMALL_INT     = 3,
    YQDB_FLOAT         = 4,
    YQDB_DATETIME      = 5,
    YQDB_TEXT          = 6,
    YQDB_BLOB          = 7, 
    YQDB_BINARY        = 8,
    YQDB_BIT           = 9,
    YQDB_MONEY         = 10, //     
    YQDB_NUMERIC       = 11, //
    YQDB_DECIMAL       = 12, //
    YQDB_INVAILD       = 0xFF
}db_fieldtype;

typedef enum db_type_t
{
	MYSQL_DB      = 0,
	SYBASE_DB     = 1
}db_type_t;

typedef struct _YQDataField_t
{
	char *name;
	void *value;
    int   fieldwidth;
    int   fieldtype;
    
}YQDataField_t;

typedef struct _YQDataRecord_t
{
	YQList *fieldlist;

}YQDataRecord_t;

typedef struct _YQDataResultSet_t
{
	int nCurrentRow; //当前行
	int nFieldCount; //列数
	YQList *recordlist;
    YQList *recordinfo; //列信息
}
YQDataResultSet_t;


class TParams
{
public:
	TParams();	
	virtual ~TParams();

	int AsInteger();
	double AsFloat();
	char *AsString();
    char AsChar();
	time_t AsDateTime();
    db_fieldtype GetFieldType(){return (db_fieldtype)FieldType;};
    db_fieldtype GetFieldSubType(){return YQDB_INVAILD;};
    int GetFieldWidth(){return FieldWidth;};
    char *GetFieldName(){return FieldName;};

public://外部不要调用这些接口
    void SetValue(void *v);
    void SetFieldWidth(int Width){FieldWidth = Width;};
    void SetFieldType(int type){FieldType = type;};
    void SetFieldName(const char*Name);
private:
	int ParamSybaseDateTime(char *src,tm &ftm);
	int ParamMySQLDateTime(char *src,tm &ftm);
	void ltrim(char *s,char c);
private:	
    int FieldWidth;
    int FieldType;
    char FieldName[32]; //字段名最长32位
	void *Value;
};

// 数据库基类
class TYQDataBase
{
private:
	db_type_t m_dbType;
	
public:
	TYQDataBase(db_type_t dbType);
	virtual ~TYQDataBase();
	db_type_t GetDBType();
	virtual MYSQL *GetMySQLHandle() = 0;
	virtual int OpenDatabase(const char *host,const char *user,const char *passwd,const char *dbname) = 0;
	virtual int Close() = 0;	

	//检查与服务器的连接状态 0-正常 其它连接断开
	virtual int CheckServerStatus() = 0;
	virtual const char* GetServerVer() = 0;
};


//MYSQL数据库
class TYQMySQLDabaBase:public TYQDataBase
{
private:
	MYSQL    m_mySQL;
public:
	TYQMySQLDabaBase();
	virtual ~TYQMySQLDabaBase();
public:
	int OpenDatabase(const char *host,const char *user,const char *passwd,const char *dbname);
	int Close();
	MYSQL *GetMySQLHandle();
	int CheckServerStatus();
	const char* GetServerVer();
private:
	int SetCharacter();
};

/*查询基类*/
class TYQDBBaseQuery
{
protected:
	char *m_FSQL;      //SQL语句
	TYQDataBase *m_FYQDatabase; //数据库
private:	
	int nRecordCount;
public:
	TYQDBBaseQuery(TYQDataBase *YQDatabase);
	virtual ~TYQDBBaseQuery();
	virtual void AddSQL(const char *szSQL);
	virtual void ClearSQL();
    virtual char* GetSQL();
public:
	virtual int ExecSQL()=0;
	virtual void Open()=0;
	virtual void Close()=0 ;	
	virtual int GetRecordCount()=0;
    virtual int GetColCount() = 0;
	virtual bool IsEof() =0;
	virtual TParams *FieldByName(const char *szFieldName) = 0;
	virtual TParams *FieldByID(unsigned int unFieldID) = 0;
	virtual void First() = 0;
	virtual void Last() = 0;
	virtual void Next() = 0;
	//virtual unsigned int GetLastErrNo() = 0;
	//virtual const char *GetLastErrMsg() = 0;
};



/*MYSQL的查询类*/
class TYQMySQLQuery:public TYQDBBaseQuery
{
private:
	MYSQL_RES *m_res;
	MYSQL_ROW m_row; 
	MYSQL_FIELD *field;
	TParams *pParms;
	bool bEof;
public:
	TYQMySQLQuery(TYQDataBase *YQDatabase);
	virtual ~TYQMySQLQuery();
public:
	virtual int ExecSQL();
	virtual void Open();
	virtual void Close();
	virtual int GetRecordCount();
    virtual int GetColCount(){return 0;};
	virtual bool IsEof();
	virtual TParams *FieldByName(const char *szFieldName);
	virtual TParams *FieldByID(unsigned int unFieldID);
	virtual void First();
	virtual void Last();
	virtual void Next();
	//virtual unsigned int GetLastErrNo();
	//virtual const char *GetLastErrMsg();

};

//对外接口调用类
class TYQDBQuery
{
private:	
	TYQDBBaseQuery *m_Query;	
public:
	TYQDBQuery(TYQDataBase *YQDatabase);
	virtual ~TYQDBQuery();
protected:
     char *GetSQL();
public:
    
	/**
	** >=0表示受影响的行数，<0 失败
	**  结果在Mysql上是实际受影响行数，在SyBase上则不准确
	**/	

	virtual int ExecSQL();
	virtual void Open();
	virtual void AddSQL(const char *szSQL);
    
	void Close() ;

	void ClearSQL();
	int GetRecordCount();
    int GetColCount();
	bool IsEof();
	TParams *FieldByName(const char *szFieldName);
	TParams *FieldByID(unsigned int unFieldID);
     void First();
     void Last();
     void Next();
	// unsigned int GetLastErrNo();
	// const char *GetLastErrMsg();

};
//---------------------------------------------------------------------------
#endif
