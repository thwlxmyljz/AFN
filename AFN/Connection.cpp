#include "stdafx.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"

static const char MESSAGE[] = "Hello, World!\n";

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
int Connection::SendBuf(const void* cmd,unsigned int cmdlen)
{
    return bufferevent_write(bev, cmd, cmdlen);
}
int Connection::SendPkg(const AFNPackage* pkg)
{
	return 0;
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
	//�������
	cIter it = begin();
	while (it != end())
	{
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
    if (!bev) 
    {
		YQLogMaj("Error constructing bufferevent!");        
        return YQER_CON_Err(1);
    }

	Connection* con = new Connection(base,bev,fd,sa);
	push_back(con);	

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	//ע��д�¼�
    //bufferevent_enable(bev, EV_WRITE);
	//ע����¼�
    bufferevent_enable(bev, EV_READ);
	bufferevent_enable(bev, EV_PERSIST);
	return YQER_OK;
}
/**
libevent event
*/
void ZjqList::conn_readcb(struct bufferevent *bev, void *user_data)
{
	char msg[4096];
	size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );
	msg[len] = '\0';
	printf("server read the data:%s\n", msg);
	/*
	char reply[] = "hello client";
	bufferevent_write(bev, reply, strlen(reply) );
	printf("server send the data:%s\n", reply);
	*/
}
void ZjqList::conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        //�����������
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
