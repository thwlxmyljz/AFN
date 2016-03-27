#include "stdafx.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"

#define BUF_SIZE 16384

Connection::Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:bev(_bev),fd(_fd),m_remoteAddr((*(sockaddr_in*)sa)),\
	m_name(""),m_areacode(0),m_number(0)
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
	//暂不考虑多帧同一event内收到
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
	m_areacode = pkg->userHeader.A1;
	if (pkg->userHeader.A3._A3.TAG == 0){
		//单地址
		m_number = pkg->userHeader.A2;
	}
	if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 1  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
		//单帧
		nRet = pkg->HandlePkg(ackLst);
		if (nRet == YQER_OK){
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
			nRet = pkg->HandlePkg(m_pkgList,ackLst);
			if (nRet == YQER_OK){
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
BOOL Connection::Compare(const string& name)
{
	return m_name==name;
}
BOOL Connection::Compare(const string& areaCode,const string& number)
{
	return TRUE;
}
BOOL Connection::Compare(struct bufferevent *_bev)
{
	return bev==_bev;
}

ZjqList* zjqList = NULL;
ZjqList::~ZjqList()
{
	//清空连接
	cIter it = begin();
	while (it != end()) {
		Connection* con = (*it);
		erase(it);
		delete con;
		it = begin();
	}
}
int ZjqList::newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa)
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
/**
libevent event
*/
void ZjqList::conn_readcb(struct bufferevent *bev, void *user_data)
{
	Connection* p = zjqList->getConnection(bev);
	if (p){
		p->RecBuf();
	}
	else{
		YQLogMin("bev read, but no connection");/*XXX win32*/
		bufferevent_free(bev);
	}
}
void ZjqList::conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        ;//发送数据完成
    }
}

void ZjqList::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF)  {
        YQLogInfo("Connection closed.");
    } 
    else if (events & BEV_EVENT_ERROR) {
        YQLogMin("Got an error on the connection");/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
	zjqList->delConnection(bev);
}
void ZjqList::delConnection(const string& name)
{
	for (cIter it = zjqList->begin(); it != zjqList->end(); it++)
	{
		Connection* con = (*it);
		if (con->Compare(name))
		{			
			zjqList->erase(it);			
			delete con;
			break;
		}
	}    
}
void ZjqList::delConnection(const string& areaCode,const string& number)
{
	for (cIter it = zjqList->begin(); it != zjqList->end(); it++)
	{
		Connection* con = (*it);
		if (con->Compare(areaCode,number))
		{			
			zjqList->erase(it);			
			delete con;
			break;
		}
	}   
}
void ZjqList::delConnection(struct bufferevent *bev)
{
	for (cIter it = zjqList->begin(); it != zjqList->end(); it++)
	{
		Connection* con = (*it);
		if (con->Compare(bev))
		{			
			zjqList->erase(it);			
			delete con;
			break;
		}
	}    
}
Connection* ZjqList::getConnection(struct bufferevent *bev)
{
	for (cIter it = zjqList->begin(); it != zjqList->end(); it++)
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
