#ifndef YQCOMFUN_H
#define YQCOMFUN_H
#include <string>
#include <list>
class COM
{
public:
static std::string timetostr(time_t ttm);
static std::string timetostr(double ttm);
static int getsubstring(std::string orgstr, std::string split, std::list<std::string>* retlist);
static void itoa(unsigned long val,char *buf,unsigned radix);
static double timestrtodouble(std::string tmstr);
static std::string doubletotimestr(double dtm);
};
#endif

