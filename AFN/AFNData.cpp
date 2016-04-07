#include "stdafx.h"
#include "AFNData.h"

int AFNData::parseDateTime(const BYTE* _data,int _len,std::string& dt)
{
	if (_len >= 5){
		char buf[64];
		int year = ((_data[4]>>4)&0x0f)*10+(_data[4]&0x0f);
		int mon = ((_data[3]>>4)&0x0f)*10+(_data[3]&0x0f);
		int day = ((_data[2]>>4)&0x0f)*10+(_data[2]&0x0f);
		int hour = ((_data[1]>>4)&0x0f)*10+(_data[1]&0x0f);
		int min = ((_data[0]>>4)&0x0f)*10+(_data[0]&0x0f);
		sprintf_s(buf,"%02d-%02d-%02d %02d:%02d:00",year,mon,day,hour,min);
		dt = buf;
		return 5;
	}
	return 0;
}
int AFNData::parseTable14(const BYTE* _data,int _len,float& val)
{
	if (_len >= 5){
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
		val =   (float)(((_data[3]>>4)&0x0f)*100000+(_data[3]&0x0f)*10000+
				((_data[2]>>4)&0x0f)*1000+(_data[2]&0x0f)*100+
				((_data[1]>>4)&0x0f)*10+(_data[1]&0x0f)+
				((_data[0]>>4)&0x0f)*0.1+(_data[0]&0x0f)*0.01);
		return 4;
	}
	return 0;
}