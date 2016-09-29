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
		//��������,0��Ч
		WORD m_areacode;
		//��ַ����,0��Ч
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

	//����
	string m_name;
	//��λ��ʾ����
	/*
	bit0:1�Ѵ������ݿ�����,0δ����
	bit1:1�ѵ�¼��0δ��¼
	*/
	BYTE m_tag;	
	//��Ӧ֡���
	BYTE m_RSEQ;
	//����֡���
	BYTE m_PSEQ;
	//����֡֡��ż�����PFC
	BYTE m_PFC;
	//���������ʱ��
	DWORD m_heartTimer;
	//����Ĳɼ�ʱ��
	DWORD m_getTimer;
	//��վMSAֵ
	static BYTE s_MSA;
	//���������ĵ��
	std::list<Element> eleLst;
	typedef std::list<Element>::iterator EleIter;
public:
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
	std::string printInfo();
	/*
	��¼����
	_Fn:1(login in),2(login out),3(heart beart)
	_pseq:login inʱ��¼֡���
	_log:��־��¼
	*/
	void LoginState(WORD _Fn,BYTE _pseq,BOOL _log=TRUE);
	/*
	����TRUE:timeout
	*/
	BOOL CheckTimeout();
};

