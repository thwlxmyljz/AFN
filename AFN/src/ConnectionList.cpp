#include "ConnectionList.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "Lock.h"
#include "YQErrCode.h"
#include <sstream>
#include "DBManager.h"
#include "Log.h"
//------------------------------------------------------------------------------------
Mutex g_JzqConList_Mutex;
JzqList* g_JzqConList = NULL;

#define TM_GETKWH 60000 //60 secs

JzqList::JzqList()
{
}
JzqList::~JzqList()
{
	//清空连接
	conIter it = begin();
	while (it != end()) {
		EventConnection* con = (*it);
		erase(it);
		delete con;
		it = begin();
	}
}
void JzqList::Load()
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		delete (*it);
	}
	for (waterIter it = m_waterList.begin(); it != m_waterList.end(); it++){
		delete (*it);
	}
	m_jzqList.clear();
	m_waterList.clear();
	YQLogInfo("LoadJzq...");
	LoadJzq();
	YQLogInfo("LoadJzq over");
	YQLogInfo("LoadWater");
	LoadWater();
	YQLogInfo("LoadWater over");
}
std::string JzqList::Print()
{
	std::string ss;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		ss += (*it)->printInfo();
	}
	for (waterIter it = m_waterList.begin(); it != m_waterList.end(); it++){
		ss += (*it)->printInfo();
	}
	return ss;
}
EventConnection* JzqList::getConnection(struct bufferevent *bev)
{
	for (conIter it = begin(); it != end(); it++){
		EventConnection* con = (*it);
		if (con->Compare(bev)){			
			return con;
		}
	} 
	return NULL;
}
void JzqList::delConnection(struct bufferevent *bev)
{
	AUTO_LOCK()
	for (conIter it = begin(); it != end(); it++){
		EventConnection* con = (*it);
		if (con->Compare(bev)){
			YQLogInfo("delete connection");
			erase(it);			
			delete con;
			break;
		}
	}    
}
/*
心跳超时检测
*/
void JzqList::CheckConnection()
{
	std::string ss;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){		
		ss += (*it)->printInfo();
		(*it)->CheckTimeout();
	}
	for (waterIter it = m_waterList.begin(); it != m_waterList.end(); it++){		
		ss += (*it)->printInfo();
		(*it)->CheckTimeout();
	}
	YQLogInfo(ss.c_str());
}
/**
libevent event
*/
void JzqList::conn_readcb(struct bufferevent *bev, void *user_data)
{
	EventConnection* p = g_JzqConList->getConnection(bev);
	if (p){
		p->RecBuf();
	}
	else{
		YQLogMin("bev read, but no connection,Free bev");/*XXX win32*/
		bufferevent_free(bev);
	}
}
void JzqList::conn_writecb(struct bufferevent *bev, void *user_data)
{
}

void JzqList::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF)  {
        YQLogInfo("Connection closed.");
    } 
    else if (events & BEV_EVENT_ERROR) {
        YQLogMin("Got an error on the connection");/*XXX win32*/
    }
	g_JzqConList->delConnection(bev);
}
void JzqList::LoadJzq()
{
	//数据库加载集中器
	char sqlBuf[128];
	snprintf(sqlBuf,128,"select * from gc_equipmentjzq");
	qopen(sqlBuf);
	query()->First();
	LOG(LOG_INFORMATION,"jzq count(%d)",query()->RecordCount);
	while (query()->RecordCount > 0 && !(query()->IsEof())){		
		int areacode = query()->FieldByName("EquipmentCjqArea")->AsInteger();
		int address = query()->FieldByName("EquipmentCjqAddr")->AsInteger();
		std::string name = query()->FieldByName("EquipmentName")->AsString();
		LOG(LOG_INFORMATION,"load jzq(%s,%d,%d)",name.c_str(),areacode,address);
		Jzq* p = new Jzq(name,areacode,address,0x01);
		m_jzqList.push_back(p);
		query()->Next();
	}
	qclose();
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		Jzq* p = (*it);
		snprintf(sqlBuf,128,"select * from gc_equipmentelect where EquipmentCjqArea=%d and EquipmentCjqAddr=%d",p->m_a1a2.m_areacode,p->m_a1a2.m_number);
		qopen(sqlBuf);
		query()->First();
		LOG(LOG_INFORMATION,"jzq(%s) ele count(%d)",p->m_name.c_str(),query()->RecordCount);
		while (query()->RecordCount > 0 && !(query()->IsEof())){		
			int pn = query()->FieldByName("EquipmentCjqPn")->AsInteger();
			int id = query()->FieldByName("ID")->AsInteger();
			LOG(LOG_INFORMATION,"load ele(%s,%d)",p->m_name.c_str(),pn);
			Element ele;
			ele.pn = pn;
			ele.ID = id;
			p->eleLst.push_back(ele);
			query()->Next();
		}
		qclose();
	}
}
Jzq* JzqList::getJzq(WORD _areacode,WORD _number)
{
	Jzq *p = NULL;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		if ((*it)->m_a1a2.m_areacode == _areacode && (*it)->m_a1a2.m_number == _number){			
			p = (*it);
			break;
		}
	}
	return p;
}
Jzq* JzqList::getJzq(const std::string& _name)
{
	Jzq *p = NULL;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		if ((*it)->m_name == _name){			
			p = (*it);
			break;
		}
	}
	return p;
}
void JzqList::ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq,BOOL _add)
{
	Jzq *p = getJzq(_areacode,_number);
	if (!p && _add){
		static int noneTag = 1;
		std::string noneName = "none";
		ostringstream os;
		os << "none" << noneTag++;
		p = new Jzq(os.str(),_areacode,_number,0x0);
		if (!p)
			return;
		m_jzqList.push_back(p);
		LogFile->FmtLog(LOG_INFORMATION,"new jzq(%s)",p->m_name.c_str());
	}
	if (p){
		p->LoginState(_Fn,_pseq);
	}
}
BYTE JzqList::GetRSEQ(WORD _areacode,WORD _number,BOOL _increase)
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++)
	{
		Jzq* p = (*it);
		if (p->m_a1a2.m_areacode == _areacode && p->m_a1a2.m_number == _number)
		{			
			BYTE n = p->m_RSEQ;
			if (_increase && ++p->m_RSEQ > 15){
				p->m_RSEQ = 0;
			}
			return n;
		}
	}
	return 0x0;
}
BYTE JzqList::GetPSEQ(WORD _areacode,WORD _number,BOOL _increase)
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++)
	{
		Jzq* p = (*it);
		if (p->m_a1a2.m_areacode == _areacode && p->m_a1a2.m_number == _number)
		{			
			BYTE n = p->m_PFC;
			if (_increase){
				++p->m_PFC;				
			}
			return n&0x0f;
		}
	}
	return 0x0;
}

int JzqList::GetEleID(WORD areacode,WORD addr, WORD pn)
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){	
		Jzq* p = (*it);
		if (p->m_a1a2.m_areacode == areacode && p->m_a1a2.m_number == addr){
			for (Jzq::EleIter eleIt = p->eleLst.begin(); eleIt != p->eleLst.end(); eleIt++){
				Element& ele = (*eleIt);
				if (ele.pn == pn)
					return ele.ID;
			}
		}
	}
	return -1;
}

int JzqList::newJzqConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa)
{
	struct bufferevent *bev;
#ifdef _WIN32	
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
#else
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
#endif
    if (!bev)  {
		YQLogMaj("Error constructing bufferevent!");        
        return YQER_CON_Err(1);
    }

	ConnectionPower* con = new ConnectionPower(base,bev,fd,sa);
	push_back(con);

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

	return YQER_OK;
}
ConnectionPower* JzqList::getJzqConnection(WORD _areacode,WORD _number)
{
	ConnectionPower *p = NULL;
	for (conIter it = begin(); it != end(); it++){
		ConnectionPower* old = dynamic_cast<ConnectionPower*>(*it);
		if (old){
			if (old->m_jzq.m_areacode == _areacode && old->m_jzq.m_number == _number){			
				p = old;
				break;
			}
		}
	}
	return p;
}
ConnectionPower* JzqList::getJzqConnection(const std::string& _name)
{
	Jzq *p = getJzq(_name);
	if ( p ){
		return getJzqConnection(p->m_a1a2.m_areacode,p->m_a1a2.m_number);
	}
	return NULL;
}
/*
定时采集集中器数据
*/
void JzqList::AutoGetAllKwh()
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){		
		Jzq* pJzq = (*it);
		if (TYQUtils::TimeElapse(pJzq->m_getTimer) > TM_GETKWH){
			//采集此集中器所带电表数据
			TYQUtils::TimeStart(pJzq->m_getTimer);
			AFNPackageBuilder::Instance().getallkwh_async(pJzq);
		}
	}
}
//-----------------------------------------------------------------------------------