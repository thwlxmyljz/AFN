#include "AFNData.h"
#include <stdlib.h>
#include <stdio.h>
int AFNData::parseDateTime5(const BYTE* _data,int _len,std::string& dt)
{
	if (_len >= 5){
		if (_data[0] == 0xee){
			dt = "00-00-00 00:00:00";
			return 5;
		}
		char buf[64];
		int year = ((_data[4]>>4)&0x0f)*10+(_data[4]&0x0f);
		int mon = ((_data[3]>>4)&0x0f)*10+(_data[3]&0x0f);
		int day = ((_data[2]>>4)&0x0f)*10+(_data[2]&0x0f);
		int hour = ((_data[1]>>4)&0x0f)*10+(_data[1]&0x0f);
		int min = ((_data[0]>>4)&0x0f)*10+(_data[0]&0x0f);
#ifdef _WIN32
		sprintf_s(buf,"20%02d-%02d-%02d %02d:%02d:00",year,mon,day,hour,min);
#else
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:00",year,mon,day,hour,min);
#endif
		dt = buf;
		return 5;
	}
	return 0;
}
int AFNData::parseDateTime6(const BYTE* _data,int _len,std::string& dt)
{
	if (_len >= 6){
		if (_data[0] == 0xee){
			dt = "00-00-00 00:00:00";
			return 6;
		}
		char buf[64];
		int year = ((_data[5]>>4)&0x0f)*10+(_data[5]&0x0f);
		int mon = ((_data[4]>>4)&0x1)*10+(_data[4]&0x0f);
		int day = ((_data[3]>>4)&0x0f)*10+(_data[3]&0x0f);
		int hour = ((_data[2]>>4)&0x0f)*10+(_data[2]&0x0f);
		int min = ((_data[1]>>4)&0x0f)*10+(_data[1]&0x0f);
		int sec = ((_data[0]>>4)&0x0f)*10+(_data[0]&0x0f);
#ifdef _WIN32
		sprintf_s(buf,"20%02d-%02d-%02d %02d:%02d:%02d",year,mon,day,hour,min,sec);
#else
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",year,mon,day,hour,min,sec);
#endif
		dt = buf;
		return 6;
	}
	return 0;
}
int AFNData::parseTable14(const BYTE* _data,int _len,float& val)
{
	if (_len >= 5){
		if (_data[0] == 0xee){
			val = -1;
			return 5;
		}
		val =   (float)(((_data[4]>>4)&0x0f)*100000+(_data[4]&0x0f)*10000+
				((_data[3]>>4)&0x0f)*1000+(_data[3]&0x0f)*100+
				((_data[2]>>4)&0x0f)*10+(_data[2]&0x0f)+
				((_data[1]>>4)&0x0f)*0.1+(_data[1]&0x0f)*0.01+
				((_data[0]>>4)&0x0f)*0.001+(_data[0]&0x0f)*0.0001);

		return 5;
	}
	return 0;
}
int AFNData::parseTable11(const BYTE* _data,int _len,float& val)
{
	if (_len >= 4){
		if (_data[0] == 0xee){
			val = -1;
			return 4;
		}
		val =   (float)(((_data[3]>>4)&0x0f)*100000+(_data[3]&0x0f)*10000+
				((_data[2]>>4)&0x0f)*1000+(_data[2]&0x0f)*100+
				((_data[1]>>4)&0x0f)*10+(_data[1]&0x0f)+
				((_data[0]>>4)&0x0f)*0.1+(_data[0]&0x0f)*0.01);
		return 4;
	}
	return 0;
}