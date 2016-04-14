#ifndef   _HEGANG_CONFIG_H_
#define   _HEGANG_CONFIG_H_
#include "YQDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
typedef struct _ConfigItem
{
	std::string name;
	std::string value;
}ConfigItem;
class TIniFile
{
public:
	TIniFile(LPCSTR filename);
	~TIniFile();
public:
	int ReadInteger(LPCSTR section,LPCSTR key,int defaultVal);
	int ReadInteger(const std::string& section,const std::string& key,int defaultVal);

	bool WriteInteger(LPCSTR section,LPCSTR key,int val);
	bool WriteInteger(const std::string&  section,const std::string&  key,int val);
	
	std::string ReadString(LPCSTR section,LPCSTR key,LPCSTR defaultVal);
	std::string ReadString(const std::string& section,const std::string& key,const std::string& defaultVal);

	bool WriteString(LPCSTR section,LPCSTR key,LPCSTR Val);
	bool WriteString(const std::string& section,const std::string& key,const std::string& Val);

	bool GetSectionItem(LPCSTR section,std::list<ConfigItem>& RetList);
	bool GetSectionItem(const std::string& section,std::list<ConfigItem>& RetList);
private:
	int getconfigstr(const char* section, 
                          const char* keyname,
                          char* keyvalue,
                          unsigned int len,
                          const char* filename,int* LinePos=NULL,int* LineCnt=NULL);	
	int getconfigint(const char* section,
                          const char* keyname,
                          int* keyvalue,
                          const char* filename);

private:
	FILE* stream;
	std::string FileData;
	std::string FileName;
	//char WorkDir[256];
private:
	int isremark(const char* line);
	int isjoint(const char* line);
	char* readline(char* line,int n,FILE *stream);
	int getsection(const char* line,char* section,unsigned int seclen);
	int getkeyname(const char* line,char* keyname,unsigned int keylen);
	int getkeyvalue(const char* line,char* keyvalue,unsigned int valen);
	int getkeyadd(const char* line,char* keyvalue,unsigned int valen);
	void Load();
	void Save();
	void Close();
};
#endif

