#include "ConnectionList.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "Lock.h"
#include <sstream>
//------------------------------------------------------------------------------------
Mutex g_JzqConList_Mutex;
JzqList* g_JzqConList = NULL;
JzqList::~JzqList()
{
	//清空连接
	conIter it = begin();
	while (it != end()) {
		Connection* con = (*it);
		erase(it);
		delete con;
		it = begin();
	}
}
void JzqList::LoadJzq()
{
	//数据库加载集中器
	Jzq* p = new Jzq("test01",0xffff,0xffff,0x01);
	m_jzqList.push_back(p);
	p = new Jzq("test",0x1000,0x44d,0x01);
	m_jzqList.push_back(p);
}
std::string JzqList::printJzq()
{
	std::string ss;
	jzqIter it = m_jzqList.begin();
	while (it != m_jzqList.end()){
		Jzq* p = (*it);
		ss += p->printInfo();
		it++;
	}
	return ss;
}
void JzqList::checkConnection()
{
	std::string ss;
	jzqIter it = m_jzqList.begin();
	while (it != m_jzqList.end()){
		Jzq* p = (*it);
		ss += p->printInfo();
		if (TYQUtils::TimeElapse(p->m_heart) >= 120){
			p->LoginState(2/*退出登录*/,0,FALSE);
		}
		it++;
	}
	YQLogInfo(ss.c_str());
}
/**
libevent event
*/
void JzqList::conn_readcb(struct bufferevent *bev, void *user_data)
{
	Connection* p = g_JzqConList->getConnection(bev);
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
int JzqList::newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa)
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

	Connection* con = new Connection(base,bev,fd,sa);
	push_back(con);

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

	return YQER_OK;
}
void JzqList::delConnection(struct bufferevent *bev)
{
	AUTO_LOCK()
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++){
		Connection* con = (*it);
		if (con->Compare(bev))
		{			
			g_JzqConList->ReportLoginState(con->m_jzq.m_areacode,con->m_jzq.m_number,2,0,FALSE);
			YQLogInfo("delete connection");
			g_JzqConList->erase(it);			
			delete con;
			break;
		}
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
Connection* JzqList::getConnection(struct bufferevent *bev)
{
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++){
		Connection* con = (*it);
		if (con->Compare(bev)){			
			return con;
		}
	} 
	return NULL;
}
Connection* JzqList::getConnection(WORD _areacode,WORD _number)
{
	Connection *p = NULL;
	for (conIter it = begin(); it != end(); it++){
		if ((*it)->m_jzq.m_areacode == _areacode && (*it)->m_jzq.m_number == _number){			
			p = (*it);
			break;
		}
	}
	return p;
}
Connection* JzqList::getConnection(const std::string& _name)
{
	Jzq *p = getJzq(_name);
	if ( p ){
		return getConnection(p->m_a1a2.m_areacode,p->m_a1a2.m_number);
	}
	return NULL;
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
			if (_increase){
				if (++p->m_RSEQ > 15){
					p->m_RSEQ = 0;
				}
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
