#include "EventConnection.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "Log.h"
#include "PackageBuilder.h"
#include <string.h>

#define BUF_SIZE 4000
EventConnection::EventConnection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa,PackageBuilder& _builder)
	:m_buffer(NULL),m_buffer_size(0),bev(_bev),fd(_fd),m_remoteAddr(*(sockaddr_in*)sa),builder(_builder)
{	
}
EventConnection::~EventConnection(void)
{
#ifdef _WIN32
	in_addr addr;   
	memcpy(&addr, &m_remoteAddr.sin_addr.S_un.S_addr, sizeof(m_remoteAddr.sin_addr.S_un.S_addr));   
	string strIp = inet_ntoa(addr); 

	LogFile->FmtLog(LOG_INFORMATION,"delete Connection[%s,%d]",strIp.c_str(),m_remoteAddr.sin_port); 
#else
	LogFile->FmtLog(LOG_INFORMATION,"delete Connection"); 
#endif
	//bufferevent_free(bev);
}
int EventConnection::SendBuf(const void* cmd,unsigned int cmdlen)
{
	LogFile->FmtLog(LOG_INFORMATION,"snd pkg:%s", TYQUtils::Byte2Hex(cmd,cmdlen).c_str());
    return bufferevent_write(bev, cmd, cmdlen);
}
int EventConnection::SendPkg(const IPackage* pkg)
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
int EventConnection::SendPkg(std::list<IPackage*>& pkgLst)
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
void EventConnection::appendData2Buffer(BYTE* msg, DWORD msgLen)
{
	m_buffer_size = m_buffer_size+msgLen;
	BYTE* nBuffer = new BYTE[m_buffer_size];
	memcpy(nBuffer,m_buffer,m_buffer_size);
	memcpy(nBuffer+m_buffer_size,msg,msgLen);
	delete[] m_buffer;
	m_buffer = nBuffer;
}
int EventConnection::RecBuf()
{
	int errCode = YQER_OK;
	BOOL parseBuffer = FALSE;
	BOOL bufferedData = FALSE;
	BYTE msg[BUF_SIZE];
	BYTE* pMsg = msg;
	DWORD msgLen = (DWORD)bufferevent_read(bev, msg, BUF_SIZE-1 );
	LOG(LOG_INFORMATION,"rec pkg len[%d] data:%s",msgLen, TYQUtils::Byte2Hex(msg,msgLen).c_str());
	if (m_buffer){
		appendData2Buffer(msg,msgLen);
		pMsg = m_buffer;		
		msgLen = m_buffer_size;
		parseBuffer = TRUE;
	}
	while(msgLen > 0) 
	{
		IPackage* pkg = createPackage();
		if (!pkg)
			break;
		DWORD eatLen = 0;
		errCode = pkg->ParseProto(pMsg,msgLen,eatLen);
		pMsg += eatLen;
		msgLen -= eatLen;
		if (errCode == YQER_OK){
			//由handlePackage进行删除pkg
			errCode = handlePackage(pkg);
		}
		else if (errCode == YQER_PKG_Err(1)){
			//半截包，缓存起来继续接收
			YQLogMin("RecBuf,half pkg, bufferred and wait left data...");
			if (parseBuffer){
				memmove(m_buffer,pMsg,msgLen);
				m_buffer_size = msgLen;
			}
			else{
				m_buffer_size = msgLen;
				m_buffer = new BYTE[m_buffer_size];
				memcpy(m_buffer,pMsg,msgLen);
			}
			delete pkg;
			bufferedData = TRUE;
			break;
		}
		else{
			YQLogMin("RecBuf, pkg invalid!");
			delete pkg;
		}
	}

	if (!bufferedData && m_buffer){
		delete[] m_buffer;
		m_buffer = NULL;
		m_buffer_size = 0;
	}

	return errCode;
}
BOOL EventConnection::Compare(struct bufferevent *_bev)
{
	return bev==_bev;
}
void EventConnection::ClearRecPkgList()
{
	ClearPkgList(m_pkgList);
}
void EventConnection::ClearPkgList(std::list<IPackage*>& lst)
{
	for (Iter it = lst.begin();  it != lst.end(); it++){
		delete (*it);
	}
	lst.clear();
}
