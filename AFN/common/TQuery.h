#ifndef YQPARAMSQUERY_H
#define YQPARAMSQUERY_H
#include "YQDB.h"
#include "YQValue.h"
#include <string.h>
#include <string>
#include <list>
#include <stdio.h>

//TQuery�Ĳ���
class TYQParam
{
private:
	void SetStrType()
	{
		//fprintf(stderr,"Param set string type\n");
		FValid = VALID_STR;
	}
	void SetIntType()
	{
		//fprintf(stderr,"Param set integer type\n");
		FValid = VALID_INT;
	}
public:
	TYQParam(const char* name)
		:AsString((*this),&TYQParam::SetStrType),AsInteger((*this),&TYQParam::SetIntType)
	{
		FName = name;		
		FValid = VALID_NONE;
	}
	~TYQParam()
	{
	}
	bool operator==(const TYQParam& pa1)
	{
		return pa1.FName == FName;
	}
	void SetAsString(std::string val)
	{
		FValid = VALID_STR; 
		AsString=val;
	}
	void SetAsInteger(int val)
	{
		FValid = VALID_INT; 
		AsInteger=val;
	}
public:
	std::string FName;
	TYQValue<TYQParam,std::string> AsString;
	TYQValue<TYQParam,int> AsInteger;
	EMValueType FValid;
};
//TQuery�Ĳ�������
class TYQParams
{
	friend class TQuery;
public:
	TYQParams()
	{
	}
	~TYQParams()
	{
		Clear();
	}
	TYQParam* ParamByName(std::string& name)
	{
		for (std::list<TYQParam*>::iterator it = FParams.begin();
			it != FParams.end(); it++)
		{
			if ((*it)->FName == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	TYQParam* ParamByName(const char* name)
	{		
		for (std::list<TYQParam*>::iterator it = FParams.begin();
			it != FParams.end(); it++)
		{
			if (strcmp((*it)->FName.c_str(), name) ==0)
			{
				return (*it);
			}
		}
		return NULL;
	}
	TYQParam* operator[](int index)
	{
		if (index >= (int)FParams.size() || index < 0)
			return NULL;
		std::list<TYQParam*>::iterator it = FParams.begin();
		while((index--)>0)
			it++;
		return (*it);
	}
	void Clear()
	{
		for (std::list<TYQParam*>::iterator it = FParams.begin();
			it != FParams.end(); it++)
		{
			delete (*it);
		}
		FParams.clear();
	}
private:
	std::list<TYQParam*> FParams;
};
//TQuery�ؼ�
class TQuery : public TYQDBQuery
{
public:
	TQuery(TYQDataBase *YQDatabase);
	virtual ~TQuery();
	static bool s_bLogSQL;
public:
	virtual void AddSQL(const char * szSQL);
	virtual void Open();
	virtual int ExecSQL();	
protected:
	//�滻�ϵ�SQL
	void ReplaceSql();
	//���������б�
	void CreateParams();
	//ɾ�������б�(���)
	void DeleteParams();
public:
	//��ȡĳ������
	TYQParam* ParamByName(std::string name);	
	//��ȡĳ������
	TYQParam* ParamByName(const char* name);	
	//�����б�
	TYQParams* Params;
	//��������
	int Paramcount;
	//����
	int ColCount;
	//��������
	int RecordCount;
	//��Ӱ��ļ�¼����
	int RowsAffected;	
	//��ȡ��ǰ��SQL����Ҫ���ڵ���
	const char* SQL()
	{
		return GetSQL();
	}
private:
	//SQL����
	std::string newsql;
};
#endif

