#include "YQComFun.h"
#include <string.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
std::string COM::timetostr(time_t ttm)
{
	struct tm *local = NULL;
	local = localtime(&ttm);
	char tmbuf[32];
	memset(tmbuf,0,32);
	/*2008-09-18 18:46:34*/
	sprintf(tmbuf,"%d-%d-%d %d:%d:%d",local->tm_year+1900,\
			local->tm_mon+1,local->tm_mday,local->tm_hour,\
			local->tm_min,local->tm_sec);
	return tmbuf;
}
std::string COM::timetostr(double ttm)
{
	time_t mm = (time_t)(ttm);
	return timetostr(mm);
}
int COM::getsubstring(std::string orgstr, std::string split, std::list<std::string>* retlist)
{
	int nRet = 0;
	int nPos = 0;
	if (!retlist)
		return nRet;
	while((nPos = orgstr.find(split)) != -1)
	{
		nRet++;
		retlist->push_back(orgstr.substr(0,nPos));
		
		if (nPos != (int)(orgstr.size()-1))
			orgstr = orgstr.substr(nPos+1,orgstr.size()-nPos-1);
		else
			break;
	}
	if (retlist->size() <= 0)
	{
		nRet++;
		retlist->push_back(orgstr);
	}
	if (nRet > 1)
	{
		nRet++;
		retlist->push_back(orgstr);
	}
	return nRet;
}
void COM::itoa(unsigned long val,char *buf,unsigned radix)
{     
	char   *p;                                 /*   pointer   to   traverse   string   */     
	char   *firstdig;                   /*   pointer   to   first   digit   */     
	char   temp;                             /*   temp   char   */     
	unsigned   digval;                 /*   value   of   digit   */     

	p   =   buf;     
	firstdig   =   p;                       /*   save   pointer   to   first   digit   */     

	do   {     
	      digval   =   (unsigned)   (val   %   radix);     
	      val   /=   radix;               /*   get   next   digit   */     

	      /*   convert   to   ascii   and   store   */     
	      if   (digval   >   9)     
	              *p++   =   (char   )   (digval   -   10   +   'a');     /*   a   letter   */     
	      else     
	              *p++   =   (char   )   (digval   +   '0');               /*   a   digit   */     
	}   while   (val   >   0);     

	/*   We   now   have   the   digit   of   the   number   in   the   buffer,   but   in   reverse     
	    order.     Thus   we   reverse   them   now.   */     

	*p--   =   '\0';                         /*   terminate   string;   p   points   to   last   digit   */     

	do   {     
	      temp   =   *p;     
	      *p   =   *firstdig;     
	      *firstdig   =   temp;       /*   swap   *p   and   *firstdig   */     
	      --p;     
	      ++firstdig;                   /*   advance   to   next   two   digits   */     
	}   while   (firstdig   <   p);   /*   repeat   until   halfway   */     
}     

double COM::timestrtodouble(std::string tmstr)
{
	int year,mon,day,hour,min,sec;
	sscanf(tmstr.c_str(),"%d-%d-%d %d:%d:%d",&year,&mon,&day,&hour,&min,&sec);
	struct tm temp;
	time_t first,second;   

	temp.tm_year = year - 1900;   
    temp.tm_mon = mon -1;   
    temp.tm_mday = day;   
    temp.tm_hour = hour;   
    temp.tm_sec = sec;   
    temp.tm_min = min;   
    first = mktime(&temp);   

	temp.tm_year = 2000 - 1900;   
    temp.tm_mon = 1 -1;   
    temp.tm_mday = 1;   
    temp.tm_hour = 0;   
    temp.tm_sec = 0;   
    temp.tm_min = 0;   
    second = mktime(&temp);   

	int days = (first-second)/(24*60*60);
	days += 36526;//客户端BCB从1899年算起,36526为2000.1.1~1899.12.30之间的天数

	double ff = (((double)hour)+(((double)min)+((double)sec)/60)/60)/24;
	ff += days;
	
	return ff;
}
std::string COM::doubletotimestr(double dtm)
{
	dtm-=36526;//36526为2000.1.1~1899.12.30之间的天数，本地从2000年开始算，BCB从1899开始算
    int day = (int)dtm;
    float ff = dtm-day;
    double dhour = ff*24;
    int hour = (int)dhour;
    ff = dhour-hour;
    double dmin = ff*60;
    int min = (int)dmin;
    ff = dmin-min;
    double dsec = ff*60;
    int sec = (int)dsec;


	time_t first,second;   
  
    struct tm temp;   
    temp.tm_year = 2000-1900;   //客户端从2000年开始算
    temp.tm_mon = 1-1;   
    temp.tm_mday = 1;   
    temp.tm_hour = 0;   
    temp.tm_sec = 0;   
    temp.tm_min = 0;   
    first = mktime(&temp);   
	
	second = first+(24*60*60*day);
	struct tm *local = localtime(&second);
	char tmbuf[32];
	memset(tmbuf,0,32);
	sprintf(tmbuf,"%d-%d-%d %d:%d:%d",local->tm_year+1900,\
			local->tm_mon+1,local->tm_mday,hour,\
			min,sec);
	return tmbuf;
}
