#include "stdafx.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"


Connection::Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:bev(_bev),fd(_fd),m_remoteAddr((*(sockaddr_in*)sa)),\
	m_name(""),m_areacode(""),m_number("")
{	
	LogFile->FmtLog(LOG_INFORMATION,"new Connection(%s)",m_remoteAddr.Convert(true)); 
}
Connection::~Connection(void)
{
	LogFile->FmtLog(LOG_INFORMATION,"delete Connection(%s)",m_remoteAddr.Convert(true)); 
	bufferevent_free(bev);
}
string Connection::printHex(void* data,int len)
{
	static char ss[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	string str;
	unsigned char* p = (unsigned char*)data;
	while(len-- > 0){
		unsigned char c = (*p++);		
		str += ss[(c&0xf0)>>4];
		str += ss[c&0x0f];
		str += " ";
	}
	return str;
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
	//暂不考虑多帧，待做...
	BYTE msg[16384];
	size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );
	msg[len] = '\0';
	LogFile->FmtLog(LOG_INFORMATION,"rec pkg:%s", printHex(msg,len).c_str());

	AFNPackage* pkg = new AFNPackage();
	int errCode = pkg->parseProto(msg,len);
	if (errCode != YQER_OK){
		YQLogMin("RecBuf, but parse pkg invalid");/*XXX win32*/
		return errCode;
	}

	return pkg->HandlePkg();
}
BOOL Connection::Compare(const string& name)
{
	return m_name==name;
}
BOOL Connection::Compare(const string& areaCode,const string& number)
{
	return (m_areacode==areaCode)&&(m_number==number);
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
