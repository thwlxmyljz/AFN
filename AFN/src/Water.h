#pragma once

#include <list>
#include <map>
#include <string>
#include "EventConnection.h"

using namespace std;

class Water{
public:
	//名称
	string m_name;
	//最近的心跳时间
	DWORD m_heartTimer;
	//最近的采集时间
	DWORD m_getTimer;
	
public:
	Water();
	Water(string _name);
	~Water();
	BOOL operator==(const Water& o);
	std::string printInfo();
	
	/*
	返回TRUE:timeout
	*/
	BOOL CheckTimeout();
};

