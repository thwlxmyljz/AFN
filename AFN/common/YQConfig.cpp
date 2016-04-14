#include   <ctype.h>   
#include   <string.h>   
#include "YQConfig.h"

#define   LINE_LEN   128
#define   VALUE_LEN  512

#define   ERR_NONE   0        /*   read   configuration   file   successfully   */   
#define   ERR_NOFILE   2      /*   not   find   or   open   configuration   file   */   
#define   ERR_READFILE   3    /*   error   occur   in   reading   configuration   file   */   
#define   ERR_FORMAT   4      /*   invalid   format   in   configuration   file   */   
#define   ERR_NOTHING   5     /*   not   find   section   or   key   name   in   configuration   file   */   

char* TIniFile::readline(char* line,int n,FILE *stream)
{
	static char* rs;   

	rs = fgets(line,n,stream);   
	/*if(strlen(line) >= 1)   
	{   
		line[strlen(line)-1] = line[strlen(line)];   
	}   
	*/    
	return rs;   
}   

int TIniFile::isjoint(const char* line)   
{   
	unsigned int i;   
	unsigned int sign=0;   

	for(i = 0;i < strlen(line);i++)
	{
		if(line[i]==' '||line[i]=='\t'||line[i]=='\\')
		{
			if(line[i] == ' ' || line[i] == '\t')
			{
			 	if(sign   <   1   )   
			  		sign++;   
		  	}
			else
			{
		    	if(sign   >   0   )
			  		sign++;
		      	else
			  		return 0;
		  	}
		}
		else
		{
			return 0;
		}
	}   

	if (sign   ==   2   )
	  return 1;
	else
	  return 0;

}   

int TIniFile::isremark(const   char*   line)   
{   
	unsigned   int   i;

	for (i   =   0;   i   <   strlen(line);   i++)   {
	  if (isgraph(line[i]))   {
	      if (line[i]   ==   '#')
	          return   1;
	      else
	          return   0;
	  	}
	}
	return   1;
}   


int TIniFile::getsection(const   char*   line,   char*   section,   unsigned   int   seclen)   
{   
	unsigned   int   start,   end;   

	for   (start   =   0;   start   <   strlen(line);   start++)   {   
	  if   (   isgraph(line[start])   )   {   
	      if   (   line[start]   !=   '['   )   
	          return   0;   
	      else     
	          break;   
	  }   
	}   

	if   (   start   >=   strlen(line)   )   
	  return   0;   


	for   (end   =   strlen(line);   end   >   1;   end   --)   {   
	  if   (   isgraph(line[end])   )   {   
	      if   (line[end]   !=   ']'   )     
	          return   0;   
	      else     
	          break;   
	  }   
	}   

	if   (   end   <=   1   )   
	  return   0;   

	if   (   end   -   start   <   2   )   
	  return   0;   

	/*
	for   (mid   =   start   +   1;   mid   <   end;   mid++)   {   
	  if   (   !isalnum(line[mid])   )     
	      return   0;   
	}   
	*/
	if   (   seclen   >   end   -   start   -   1   )   {   
	  memcpy(section,   &line[start   +   1],   end   -   start   -   1);   
	  section[end   -   start   -   1]   =   '\0';   
	  return   (end   -   start   -   1);   
	}     
	else   {   
	  memcpy(section,   &line[start   +   1],   seclen   -   1);   
	          section[seclen   -   1]   =   '\0';   
	  return   (seclen   -1);   
	}   

}   


int TIniFile::getkeyname(const   char*   line,   char*   keyname,   unsigned   int   keylen)   
{   
	#if 1
	unsigned   int    start,count;
	for   (start   =   0,count = 0;   start   <   strlen(line) && count < keylen-1;   start++)   {   
	  if   (   isgraph(line[start])  )   {   
	  		if (line[start] != '=')
		  		keyname[count++] = line[start];
			else
				break;
	  	}
	}
	if (start == strlen(line)-1 && line[start] != '=' && count >= 1)
		memset(keyname,0,count);
	keyname[count] = '\0';
	return count;
	#else
	unsigned   int   start,   mid,   end;   

	for   (start   =   0;   start   <   strlen(line);   start++)   {   
	  if   (   isgraph(line[start])   )   {   
	      if   (   !isalnum(line[start])   )   
	          return   0;   
	      else     
	          break;   	      
	  }   
	}   

	if   (   start   >=   strlen(line)   )   
	  return   0;   

	for   (end   =   start;   line[end]   !=   '='   &&   end   <   strlen(line);   end++)   {   
	  if   (   !isalnum(line[end])   &&   line[end]   !=   ' '   &&   line[end]   !=   '\t'   )   
	      return   0;   
	}   

	if   (   end   >=   strlen(line)   )   
	  return   0;   

	for   (end   =   end   -   1;   !isalnum(line[end]);   end--)   {   
	}   

	for   (mid   =   start;   mid   <=   end;   mid++)   {   
	  if   (   line[mid]   ==   ' '   ||   line[mid]   ==   '\t'   )     
	      return   0;   
	}   


	if   (   keylen   >   end   -   start   +   1   )   {   
	  memcpy(keyname,   &line[start],   end   -   start   +   1);   
	  keyname[end   -   start   +   1]   =   '\0';   
	  return   (end   -   start   +   1);   
	}     
	else   {   
	  memcpy(keyname,   &line[start],   keylen   -   1);   
	          keyname[keylen   -   1]   =   '\0';   
	  return   (keylen   -1);   
	}   
	#endif
}   


int TIniFile::getkeyvalue(const   char*   line,   char*   keyvalue,   unsigned   int   valen)   
{   
	#if 1
	unsigned int start,count;
	for (start = 0; start <= strlen(line)-1; start++)
		if (line[start] == '=')
			break;
	if (start == strlen(line)-1)
	{
		keyvalue[0] = '\0';
		return 1;
	}
	for (start+=1,count=0; start <= strlen(line)-1; start++)
		if   (   isgraph(line[start]) && count < valen-1)			
			keyvalue[count++] = line[start];
	
	keyvalue[count] = '\0';
	if (count == 0)
		count = 1;
	return count;
	#else
	unsigned   int   start,   end;   
	int   len;   

	for   (start   =   0;   line[start]   !=   '='   &&   start   <   strlen(line);   start++)   {   
	}   

	for   (start   =   start   +   1;   !isgraph(line[start])   &&   start   <   strlen(line);   start++)   {   
	}   

	if   (   start   >=   strlen(line)   )   
	  return   0;   

	for   (end   =   start;   line[end]   !=   ' '   &&   line[end]   !=   '\t'   &&   end   <   strlen(line);   end++)   {   
	}   

	if   (   valen   >   end   -   start   )   {   
	  memcpy(keyvalue,   &line[start],   end   -   start);   
	  keyvalue[end   -   start]   =   '\0';   
	  len   =   end   -   start;   
	}     
	else   {   
	  memcpy(keyvalue,   &line[start],   valen   -   1);   
	          keyvalue[valen   -   1]   =   '\0';   
	  len   =     valen   -1;   
	}   

	if   (   end   >=   strlen(line)   )     
	  return   len;   
	else     
	{   
	  if   (   isjoint(&line[end])   )   
	      return   -len;   
	  return   len;   
	}   
	#endif
}   


int TIniFile::getkeyadd(const   char*   line,   char*   keyvalue,   unsigned   int   valen)   
{   
	unsigned   int   start,   end;   
	int   len;   

	for   (start   =   0;   !isgraph(line[start])   &&   start   <   strlen(line);   start++)   {   
	}   

	if   (   start   >=   strlen(line)   )   
	  return   0;   

	for   (end   =   start;   line[end]   !=   ' '   &&   line[end]   !=   '\t'   &&   end   <   strlen(line);   end++)   {   
	}   

	if   (   valen   >   end   -   start   )   {   
	  memcpy(keyvalue,   &line[start],   end   -   start);   
	  keyvalue[end   -   start]   =   '\0';   
	  len   =   end   -   start;   
	}     
	else   {   
	  memcpy(keyvalue,   &line[start],   valen   -   1);   
	          keyvalue[valen   -   1]   =   '\0';   
	  len   =     valen   -1;   
	}   

	if   (   end   >=   strlen(line)   )     
	  return   len;   
	else     
	{   
	  if   (   isjoint(&line[end])   )   
	      return   -len;   
	  return   len;   
	}   

}   



int TIniFile::getconfigstr(const   char*   section,   
                      const   char*   keyname,   
                      char*   keyvalue,   
                      unsigned   int   len,   
                      const   char*   filename,int* LinePos,int* LineCnt)   
{   
	int   step   =   0;   
	int   rs   =   0;   
	int   ra   =   0;   

	char   line[LINE_LEN];   
	char   sec[LINE_LEN];   
	char   ken[LINE_LEN];   
	char   kev[LINE_LEN];   

	if (filename)  	
		if(   (stream     =   fopen(filename,   "r")   )   ==   NULL   )   
		return   0x2;   
	if (!stream)
		return ERR_NOFILE;
	fseek(stream,0,SEEK_SET);
	while   (   !feof(stream)   )   {   
	  if   (   readline(line,   100,   stream)   ==   NULL   )   {   
	      if (filename)
		  	fclose(stream);   
	      return   0x3;   
	  }           

	  if   (   !isremark(line)   )   {   
	      if   (   step   ==   0   )   {   
	          if   (   getsection(line,   sec,   LINE_LEN)   )   {   
	              if   (   strcmp(sec,   section)   ==   0   )   
	                  step   =   1;   
	          }   
	      }   else   if   (step   ==   1)   {   
	     	  if (getsection(line,   sec,   LINE_LEN))
	     	  {
			  		if (filename)
						fclose(stream);
					return 0x5;
	     	  }
	          if   (   getkeyname(line,   ken,   LINE_LEN)   )   {   
	              if   (   strcmp(ken,   keyname)   ==   0   )   {   
	                  rs   =   getkeyvalue(line,   kev,   LINE_LEN);   
	                  if   (   rs   >   0   )   {   
	                      strncpy(keyvalue,   kev,   len);   
	                      if (LinePos)
						  	(*LinePos) = ftell(stream)-strlen(line);
						  if (LineCnt)
						  	(*LineCnt) = strlen(line);
	                      if (filename)
						  	fclose(stream);   
	                      return   0x0;   
	                  }   else   if   (   rs   <   0   )   {   
	                      step   =   2;   
	                      rs   =   abs(rs);   
	                  }   else   {   
	                      if (filename)
						  	fclose(stream);   
	                      return   0x4;   
	                  }   

	              }   
	          }   
	        
	      }   else   {   
	          ra   =   getkeyadd(line,   kev   +   rs,   LINE_LEN   -   rs);   
	          if   (   ra   >   0   )   {   
	                      strncpy(keyvalue,   kev,   len);   
	                      if (filename)
						  	fclose(stream);   
	                      return   0x0;   
	          }   else   if   (   ra   <   0   )   {   
	                      rs   +=   abs(ra);   
	          }   else   {   
	                      if (filename)
						  	fclose(stream);   
	                      return   0x4;   
	          }   


	      }   


	  }   

	}   
	if (filename)
	  fclose(stream);       
	return   0x5;   

}   

int TIniFile::getconfigint(const   char*   section,   
                      const   char*   keyname,   
                      int*   keyvalue,   
                      const   char*   filename)   
{   
	int   rs;   
	char   kev[32];   

	memset(kev,   0,   32);   
	rs     =   getconfigstr(section,   keyname,   kev,   32,   filename);   
	if   (   rs   ==   0   )   
	  *keyvalue   =   atoi(kev);   
	else
		rs = -rs;
	return   rs;   
}   

TIniFile::TIniFile(LPCSTR filename)
{
	//memset(WorkDir,0,256);
	//getcwd(WorkDir,256);
	FileName = filename;
	FileData = "";
	Load();
}

TIniFile::~TIniFile()	
{
	Close();
	//chdir(WorkDir);
}
void TIniFile::Load()
{
	#ifdef _WIN32
	stream = fopen(FileName.c_str(),"rb");
    #else
    stream = fopen(FileName.c_str(),"r");
    #endif
	if (stream != NULL)
	{		
		fseek(stream,0,SEEK_SET);
		if (FileData.empty())
		{
			//全部读取到FileData中,只有在第一次时写入
			char lines[100];
			while (fgets(lines, 100, stream)) {
				FileData+=lines;
			}
			//LOG(LOG_INFORMATION,"read ini:%s",FileData.c_str());
			//printf("read ini:\n%s",FileData.c_str());
			fseek(stream,0,SEEK_SET);
		}
	}
}
void TIniFile::Save()
{
	Close();
	#ifdef _WIN32
	stream = fopen(FileName.c_str(),"wb+");
	#else
    stream = fopen(FileName.c_str(),"w+");
    #endif
	if (stream != NULL)
	{
		fseek(stream,0,SEEK_SET);
		fwrite(FileData.c_str(),sizeof(char),FileData.size(),stream);
		fclose(stream);		
	}
	Load();
}
void TIniFile::Close()
{
	if (stream)
		fclose(stream);
	stream = 0;
}
int TIniFile::ReadInteger(const std::string& section,const std::string& key,int defaultVal)
{
	int getval;
	int result = getconfigint(section.c_str(),key.c_str(),&getval,NULL);
	if (result < 0)
		return defaultVal;
	else 
		return getval;
}
int TIniFile::ReadInteger(LPCSTR section,LPCSTR key,int defaultVal)
{
	int getval;
	int result = getconfigint(section,key,&getval,NULL);
	if (result < 0)
		return defaultVal;
	else 
		return getval;
}
std::string TIniFile::ReadString(const std::string& section,const std::string& key,const std::string& defaultVal)
{
	char tmp[VALUE_LEN];
	memset(tmp,0,VALUE_LEN);
	int result = getconfigstr(section.c_str(),key.c_str(),tmp,VALUE_LEN,NULL);
	if (result == 0)
		return tmp;
	else
		return defaultVal;
}
std::string TIniFile::ReadString(LPCSTR section,LPCSTR key,LPCSTR defaultVal)
{
	char tmp[VALUE_LEN];
	memset(tmp,0,VALUE_LEN);	
	int result = getconfigstr(section,key,tmp,VALUE_LEN,NULL);
	if (result == 0)
		return tmp;
	else
		return defaultVal;
}
bool TIniFile::WriteInteger(LPCSTR section,LPCSTR key,int val)
{
	char   kev[32];   
	memset(kev,   0,   32);  
	int LinePos = 0;
	int LineCnt = 0;
	int rs     =   getconfigstr(section,   key,   kev,   32,   NULL,&LinePos,&LineCnt);   
	if   (   rs   ==   0   )   
	{
		char nline[VALUE_LEN];
		memset(nline,0,VALUE_LEN);
		sprintf(nline,"%s=%d\r\n",key,val);
		FileData.replace(LinePos,LineCnt,nline);
		Save();
		return true;
	}
	return false;
}
bool TIniFile::WriteInteger(const std::string&  section,const std::string&  key,int val)
{
	char   kev[32];   
	memset(kev,   0,   32);  
	int LinePos = 0;
	int LineCnt = 0;
	int rs     =   getconfigstr(section.c_str(),   key.c_str(),   kev,   32,   NULL,&LinePos,&LineCnt);   
	if   (   rs   ==   0   )   
	{
		char nline[VALUE_LEN];
		memset(nline,0,VALUE_LEN);
		sprintf(nline,"%s=%d\r\n",key.c_str(),val);
		FileData.replace(LinePos,LineCnt,nline);
		Save();
		return true;
	}
	return false;
}
bool TIniFile::WriteString(LPCSTR section,LPCSTR key,LPCSTR Val)
{
	char tmp[VALUE_LEN];
	memset(tmp,0,VALUE_LEN);	
	int LinePos = 0;
	int LineCnt = 0;
	int result = getconfigstr(section,key,tmp,VALUE_LEN,NULL,&LinePos,&LineCnt);
	if (result == 0)	
	{
		char nline[VALUE_LEN];
		memset(nline,0,VALUE_LEN);
		sprintf(nline,"%s=%s\r\n",key,Val);
		FileData.replace(LinePos,LineCnt,nline);
		Save();
		return true;
	}
	return false;
}
bool TIniFile::WriteString(const std::string& section,const std::string& key,const std::string& Val)
{
	char tmp[VALUE_LEN];
	memset(tmp,0,VALUE_LEN);	
	int LinePos = 0;
	int LineCnt = 0;
	int result = getconfigstr(section.c_str(),key.c_str(),tmp,VALUE_LEN,NULL,&LinePos,&LineCnt);
	if (result == 0)	
	{
		char nline[VALUE_LEN];
		memset(nline,0,VALUE_LEN);
		sprintf(nline,"%s=%s\r\n",key.c_str(),Val.c_str());
		FileData.replace(LinePos,LineCnt,nline);
		Save();
		return true;
	}
	return false;
}
bool TIniFile::GetSectionItem(LPCSTR section,std::list<ConfigItem>& RetList)
{
	int   step   =   0;   
	int   rs   =   0;   


	char   line[LINE_LEN];   
	char   sec[LINE_LEN];   
	char   ken[LINE_LEN];   
	char   kev[LINE_LEN];   

	if (!stream)
		return false;
	fseek(stream,0,SEEK_SET);
	while   (   !feof(stream)   )   
	{   
	 	if   (   readline(line,   100,   stream)   ==   NULL   )   
		{   	      
	     	return true;   	 	
	  	}           

	    if   (   !isremark(line)   )   
		{   
	      	if   (   step   ==   0   )   
			{   
	        	 if   (   getsection(line,   sec,   LINE_LEN)   )   
				 {   
	              	if   (   strcmp(sec,   section)   ==   0   )   
	                 	 step   =   1;   						
	         	 }   
	      	}
			else   if   (step   ==   1)   
	      	{   
	      		if (getsection(line,   sec,   LINE_LEN))
					return true;
	        	 if   (   getkeyname(line,   ken,   LINE_LEN)   )   
				 {   	             
	                  rs   =   getkeyvalue(line,   kev,   LINE_LEN);   
	                  if   (   rs   >   0   )   
					  {   
	                      ConfigItem item;
						  item.name = ken;
						  item.value = kev;
						  RetList.push_back(item);
	                  } 
	              }   
	         }   	        
	    }
	 }   	
	return true;
}
bool TIniFile::GetSectionItem(const std::string& section,std::list<ConfigItem>& RetList)
{
	return GetSectionItem(section.c_str(),RetList);
}
