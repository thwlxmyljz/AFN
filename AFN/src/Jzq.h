#pragma once

#include <list>
#include <map>
#include <string>
#include "Element.h"
#include "EventConnection.h"

using namespace std;

class Jzq{
public:
	class JzqA1A2{	
	public:
		//行政区号,0无效
		WORD m_areacode;
		//地址编码,0无效
		WORD m_number;
		void Invalid(){
			m_areacode = 0;
			m_number = 0;
		}
		BOOL isOK(){
			return m_areacode != 0 && m_number != 0;
		}
		bool operator==(const JzqA1A2& o){
			return m_areacode == o.m_areacode && m_number == o.m_number;
		}
	} m_a1a2;

	//名称
	string m_name;
	//按位表示属性
	/*
	bit0:1已存在数据库配置,0未配置
	bit1:1已登录，0未登录
	*/
	BYTE m_tag;	
	//响应帧序号
	BYTE m_RSEQ;
	//请求帧序号
	BYTE m_PSEQ;
	//启动帧帧序号计数器PFC
	BYTE m_PFC;
	//最近的心跳时间
	DWORD m_heartTimer;
	//最近的采集时间
	DWORD m_getTimer;
	//主站MSA值
	static BYTE s_MSA;
	//集中器带的电表
	std::list<Element> eleLst;
	typedef std::list<Element>::iterator EleIter;
public:
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
	std::string printInfo();
	/*
	登录函数
	_Fn:1(login in),2(login out),3(heart beart)
	_pseq:login in时记录帧序号
	_log:日志记录
	*/
	void LoginState(WORD _Fn,BYTE _pseq,BOOL _log=TRUE);
	/*
	返回TRUE:timeout
	*/
	BOOL CheckTimeout();
};

