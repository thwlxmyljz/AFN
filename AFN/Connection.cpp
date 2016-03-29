#include "stdafx.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"

#define BUF_SIZE 16384

Connection::Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:bev(_bev),fd(_fd),m_remoteAddr((*(sockaddr_in*)sa)),m_jzq(NULL)
{	
	LogFile->FmtLog(LOG_INFORMATION,"new Connection(%s)",m_remoteAddr.Convert(true)); 
}
Connection::~Connection(void)
{
	LogFile->FmtLog(LOG_INFORMATION,"delete Connection(%s)",m_remoteAddr.Convert(true)); 
	bufferevent_free(bev);
}

int Connection::SendBuf(const void* cmd,unsigned int cmdlen)
{
    return bufferevent_write(bev, cmd, cmdlen);
}
int Connection::SendPkg(const AFNPackage* pkg)
{
	return 0;
}
int Connection::RecBuf()
{
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
		if (m_jzq){
			m_jzq->m_areacode = pkg->userHeader.A1;
			m_jzq->m_number = pkg->userHeader.A2;
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
		YQLogInfo("rec mul pkg , middle");
		if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 1)
		{
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
	BYTE msg[BUF_SIZE];
	for (Iter it = pkgLst.begin(); it != pkgLst.end(); it++){
		int cnt = (*it)->Serialize(msg,BUF_SIZE);
		if (cnt > 0 && cnt < BUF_SIZE){
			LogFile->FmtLog(LOG_INFORMATION,"send pkg:%s",TYQUtils::Byte2Hex(msg,cnt).c_str());
			if (bufferevent_write(bev, msg, cnt) != 0){
				YQLogMaj("send pkg, bufferevent_write error");
			}
		}else{
			YQLogMaj("send pkg, Serialize pkg error");
		}
	}
	return YQER_OK;
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
BYTE Jzq::s_MSA = 0x01;
//------------------------------------------------------------------------------------
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
	//加载集中器
	Jzq* p = new Jzq("test",0xffff,0xffff,0x01);
	m_jzq.push_back(p);
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
		YQLogMin("bev read, but no connection");/*XXX win32*/
		bufferevent_free(bev);
	}
}
void JzqList::conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        ;//发送数据完成
    }
}

void JzqList::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF)  {
        YQLogInfo("Connection closed.");
    } 
    else if (events & BEV_EVENT_ERROR) {
        YQLogMin("Got an error on the connection");/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
	g_JzqConList->delConnection(bev);
}
int JzqList::newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa)
{
	struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)  {
		YQLogMaj("Error constructing bufferevent!");        
        return YQER_CON_Err(1);
    }

	Connection* con = new Connection(base,bev,fd,sa);
	push_back(con);	

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	//注册写事件
    //bufferevent_enable(bev, EV_WRITE);
	//注册读事件
    bufferevent_enable(bev, EV_READ);
	bufferevent_enable(bev, EV_PERSIST);
	return YQER_OK;
}
void JzqList::delConnection(struct bufferevent *bev)
{
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++)
	{
		Connection* con = (*it);
		if (con->Compare(bev))
		{			
			g_JzqConList->erase(it);			
			delete con;
			break;
		}
	}    
}
Connection* JzqList::getConnection(struct bufferevent *bev)
{
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++)
	{
		Connection* con = (*it);
		if (con->Compare(bev))
		{			
			return con;
			break;
		}
	} 
	return NULL;
}
void JzqList::ReportLoginState(WORD _areacode,WORD _number,BYTE _Fn,BYTE _pseq)
{
	Jzq *p = NULL;
	for (jzqIter it = m_jzq.begin(); it != m_jzq.end(); it++){
		if ((*it)->m_areacode == _areacode && (*it)->m_number == _number){			
			p = (*it);
			break;
		}
	}
	if (!p){
		YQLogInfo("new jzq");
		p = new Jzq("",_areacode,_number,0x0);//Fn=1已登录,=0退出登录，未在数据库内配置
		if (!p)
			return;
		m_jzq.push_back(p);
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
BYTE JzqList::GetRSEQ(WORD _areacode,WORD _number)
{
	for (jzqIter it = m_jzq.begin(); it != m_jzq.end(); it++)
	{
		Jzq* p = (*it);
		if (p->m_areacode == _areacode && p->m_number == _number)
		{			
			BYTE n = p->m_RSEQ;
			if (++p->m_RSEQ > 15){
				p->m_RSEQ = 0;
			}
			return n;
		}
	}
	return 0x0;
}