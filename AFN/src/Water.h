#pragma once

#include <list>
#include <map>
#include <string>
#include "EventConnection.h"

using namespace std;

class Water{
public:
	//����
	string m_name;
	//���������ʱ��
	DWORD m_heartTimer;
	//����Ĳɼ�ʱ��
	DWORD m_getTimer;
	
public:
	Water();
	Water(string _name);
	~Water();
	BOOL operator==(const Water& o);
	std::string printInfo();
	
	/*
	����TRUE:timeout
	*/
	BOOL CheckTimeout();
};

