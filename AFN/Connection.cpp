#include "stdafx.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "Lock.h"
#include <sstream>

#define BUF_SIZE 16384

Connection::Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:bev(_bev),fd(_fd),m_remoteAddr((*(sockaddr_in*)sa)),m_jzq(NULL)
{	
	in_addr addr;   
	memcpy(&addr, &m_remoteAddr.sin_addr.S_un.S_addr, sizeof(m_remoteAddr.sin_addr.S_un.S_addr));   
	string strIp = inet_ntoa(addr); 

	LogFile->FmtLog(LOG_INFORMATION,"new Connection[%s,%d]",strIp.c_str(),m_remoteAddr.sin_port); 
}
Connection::~Connection(void)
{
	in_addr addr;   
	memcpy(&addr, &m_remoteAddr.sin_addr.S_un.S_addr, sizeof(m_remoteAddr.sin_addr.S_un.S_addr));   
	string strIp = inet_ntoa(addr); 

	LogFile->FmtLog(LOG_INFORMATION,"delete Connection[%s,%d]",strIp.c_str(),m_remoteAddr.sin_port); 
	bufferevent_free(bev);
}
int Connection::SendBuf(const void* cmd,unsigned int cmdlen)
{
	LogFile->FmtLog(LOG_INFORMATION,"snd pkg:%s", TYQUtils::Byte2Hex(cmd,cmdlen).c_str());
    return bufferevent_write(bev, cmd, cmdlen);
}
int Connection::SendPkg(const AFNPackage* pkg)
{
	BYTE sndBuf[BUF_SIZE];
	int size = pkg->Serialize(sndBuf,BUF_SIZE);
	if (size > 0 && size < BUF_SIZE){
		return SendBuf(sndBuf,size);
	}
	else{
		LogFile->FmtLog(LOG_MINOR,"SendPkg Serialize error, error code:%d",size); 
	}
	return size;
}
int Connection::RecBuf()
{
	//每帧响应确认模式
	int nRet = YQER_OK;
	BYTE msg[BUF_SIZE];
	size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );
	msg[len] = '\0';
	LogFile->FmtLog(LOG_INFORMATION,"rec pkg:%s", TYQUtils::Byte2Hex(msg,len).c_str());

	AFNPackage* pkg = new AFNPackage();
	int errCode = pkg->ParseProto(msg,len);
	if (errCode != YQER_OK){		
		YQLogMin("RecBuf, pkg invalid!");
		delete pkg;
		return errCode;
	}
	std::list<AFNPackage* > ackLst;	
	if (pkg->userHeader.A3._A3.TAG == 0){
		//单地址
		if (!m_jzq){
			//查找集中器，如果集中器配置在数据库内，则会查找到此集中器
			//如果未配置在数据库内，在处理登录帧的时候，会加入到集中器列表，再下一帧数据过来的时候，会查找到此集中器
			m_jzq = g_JzqConList->getJzq(pkg->userHeader.A1,pkg->userHeader.A2);
			if (m_jzq){
				//设置集中器的网络连接
				m_jzq->m_conn = this;
			}
		}
	}
	if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 1  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
		//单帧
		nRet = AFNPackageBuilder::Instance().HandlePkg(pkg,ackLst);
		if (nRet == YQER_OK && ackLst.size() > 0){
			SendPkg(ackLst);
		}
		delete pkg;
		return nRet;
	}
	else if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 0  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
		//多帧，第一帧
		ClearRecPkgList();
		YQLogInfo("rec mul pkg , first");
		m_pkgList.push_back(pkg);
	}
	else if (pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 0) {
		//多帧，中间帧
		m_pkgList.push_back(pkg);		
		if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 0){
			YQLogInfo("rec mul pkg , middle");
		}
		else{
			//多帧，结束帧
			YQLogInfo("rec mul pkg , end");
			nRet = AFNPackageBuilder::Instance().HandlePkg(m_pkgList,ackLst);
			if (nRet == YQER_OK && ackLst.size() > 0){
				SendPkg(ackLst);
			}	
			ClearRecPkgList();			
			return nRet;
		}		
	}
	return nRet;
}
int Connection::SendPkg(std::list<AFNPackage*>& pkgLst)
{
	int ret = YQER_OK;
	for (Iter it = pkgLst.begin(); it != pkgLst.end(); it++){
		int as;
		if ((as=SendPkg(*it)) != YQER_OK){
			ret = as;
		}
	}
	return ret;
}
void Connection::ClearRecPkgList()
{
	for (Iter it = m_pkgList.begin();  it != m_pkgList.end(); it++){
		delete (*it);
	}
	m_pkgList.clear();
}

BOOL Connection::Compare(struct bufferevent *_bev)
{
	return bev==_bev;
}
//------------------------------------------------------------------------------------
Jzq::Jzq()
	:m_name(""),
	m_areacode(0),
	m_number(0),
	m_tag(0),
	m_conn(NULL),
	m_heart(0),
	m_RSEQ(0x0),
	m_PSEQ(0x0),
	m_PFC(0x0)
{
}
Jzq::Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag)
	:m_name(_name),
	m_areacode(_areaCode),
	m_number(_number),
	m_tag(_tag),
	m_conn(NULL),
	m_heart(0),
	m_RSEQ(0x0),
	m_PSEQ(0x0),
	m_PFC(0x0)
{
}
Jzq::~Jzq()
{
}
BOOL Jzq::operator==(const Jzq& o)
{
	return (o.m_areacode == m_areacode && o.m_number == m_number);
}
//------------------------------------------------------------------------------------
BYTE Jzq::s_MSA = 0x01;
//------------------------------------------------------------------------------------
Mutex g_JzqConListMutex;
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
		std::ostringstream os;
		os <<"name(" << p->m_name << "),areacode(" << p->m_areacode << "),address(" << p->m_number << "),state("\
			<< ((p->m_tag&0x1)?"db:yes,":"db:no,") << ((p->m_tag&0x2)?"online:yes)":"online:no)") ;
		os << "\r\n--------------------------------------------------------\r\n";
		ss += os.str();
		it++;
	}
	return ss;
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
	/*
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        ;
    }
	*/
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
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
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
	Lock lock(g_JzqConListMutex);
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++){
		Connection* con = (*it);
		if (con->Compare(bev))
		{		
			//清除集中器的网络连接
			if (con->m_jzq){
				con->m_jzq->m_conn = NULL;
			}
			g_JzqConList->erase(it);			
			delete con;
			break;
		}
	}    
}
Connection* JzqList::getConnection(struct bufferevent *bev)
{
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++){
		Connection* con = (*it);
		if (con->Compare(bev))
		{			
			return con;
			break;
		}
	} 
	return NULL;
}
Jzq* JzqList::getJzq(WORD _areacode,WORD _number)
{
	Jzq *p = NULL;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		if ((*it)->m_areacode == _areacode && (*it)->m_number == _number){			
			p = (*it);
			break;
		}
	}
	return p;
}
Jzq* JzqList::getJzq(std::string _name)
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
void JzqList::ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq)
{
	Jzq *p = getJzq(_areacode,_number);
	if (!p){
		YQLogInfo("new jzq");
		p = new Jzq("",_areacode,_number,0x0);
		if (!p)
			return;
		m_jzqList.push_back(p);
	}
	if (_Fn==1){
		p->m_tag |= (0x1<<1);
		p->m_RSEQ = _pseq;//响应帧起始=登录帧的请求帧起始序号,之后响应+1循环(0~15)
		LogFile->FmtLog(LOG_INFORMATION,"jzq(%d,%d) login in",_areacode,_number);		
	}
	else if (_Fn == 2){
		p->m_tag &= ~(0x1<<1);
		LogFile->FmtLog(LOG_INFORMATION,"jzq(%d,%d) login out",_areacode,_number);
	}
	else if (_Fn==3){
		p->m_tag |= (0x1<<1);
		LogFile->FmtLog(LOG_INFORMATION,"jzq(%d,%d) heartbeat",_areacode,_number);
	}
	TYQUtils::TimeStart(p->m_heart);
}
BYTE JzqList::GetRSEQ(WORD _areacode,WORD _number,BOOL _increase)
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++)
	{
		Jzq* p = (*it);
		if (p->m_areacode == _areacode && p->m_number == _number)
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
		if (p->m_areacode == _areacode && p->m_number == _number)
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
