#pragma once

#include "proto.h"
#include <string>

class AFNData 
{
public:
	static int parseDateTime5(const BYTE* _data,int _len,std::string& dt);
	static int parseDateTime6(const BYTE* _data,int _len,std::string& dt);
	static int parseTable14(const BYTE* _data,int _len,float& val);
	static int parseTable11(const BYTE* _data,int _len,float& val);
};

