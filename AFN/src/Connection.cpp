#include "Connection.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "Lock.h"
#include "Log.h"
#include <sstream>

#define BUF_SIZE 16384

//------------------------------------------------------------------------------------
Jzq::Jzq()
	:m_name(""),
	m_tag(0),
	m_RSEQ(0x0),
	m_PSEQ(0x0),
	m_PFC(0x0),
	m_heartTimer(0),
	m_kwhTimer(0)
{
	m_a1a2.Invalid();
}
Jzq::Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag)
	:m_name(_name),
	m_tag(_tag),
	m_RSEQ(0x0),
	m_PSEQ(0x0),
	m_PFC(0x0),
	m_heartTimer(0),
	m_kwhTimer(0)
{
	m_a1a2.m_areacode = _areaCode;
	m_a1a2.m_number = _number;
}
Jzq::~Jzq()
{
}
BOOL Jzq::operator==(const Jzq& o)
{
	return (m_a1a2 == o.m_a1a2);
}
std::string Jzq::printInfo()
{
	std::ostringstream os;
	os <<"totalEle("<<eleLst.size()<<"),name(" << m_name << "),areacode(" << m_a1a2.m_areacode << "),address(" << m_a1a2.m_number << "),state(";
	os << ((m_tag&0x1)?"config:yes,":"config:no,") << ((m_tag&0x2)?"online:yes)":"online:no)");
	os << "\r\n-------------------------------------------------------------------------------\r\n";
	return os.str();
}
void Jzq::LoginState(WORD _Fn,BYTE _pseq,BOOL _log)
{
	if (_Fn==1){

		//集中器发送登录消息
		m_tag |= (0x1<<1);
		m_RSEQ = _pseq;//响应帧起始=登录帧的请求帧起始序号,之后响应+1循环(0~15)
		LOG(LOG_INFORMATION,"jzq(%s,%d,%d) login in",m_name.c_str(),m_a1a2.m_areacode,m_a1a2.m_number);	
	}
	else if (_Fn == 2){
		//超时、或者集中器发送退出消息
		m_tag &= ~(0x1<<1);
		LOG(LOG_INFORMATION,"jzq(%s,%d,%d) login out",m_name.c_str(),m_a1a2.m_areacode,m_a1a2.m_number);
	}
	else if (_Fn==3){
		//集中器发送心跳消息
		m_tag |= (0x1<<1);
		//记录心跳时间
		TYQUtils::TimeStart(m_heartTimer);
		LOG(LOG_INFORMATION,"jzq(%s,%d,%d) heartbeat(%d)",m_name.c_str(),m_a1a2.m_areacode,m_a1a2.m_number,m_heartTimer);		
	}
}
BOOL Jzq::CheckTimeout()
{
	DWORD dwElapse = TYQUtils::TimeElapse(m_heartTimer);
	if ( dwElapse > 180000/*3分钟，3次heartbeart*/)
	{
		TYQUtils::TimeStart(m_heartTimer);
		ostringstream os;
		os << m_name << " timeout, from " << m_heartTimer << " + " << dwElapse;			
		YQLogInfo(os.str().c_str());
		LoginState(2/*退出登录*/,0,FALSE);
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------
BYTE Jzq::s_MSA = 0x01;
Connection::Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:bev(_bev),fd(_fd),m_remoteAddr((*(sockaddr_in*)sa))
{	
	m_jzq.Invalid();
#ifdef _WIN32
	in_addr addr; 
	memcpy(&addr, &m_remoteAddr.sin_addr.S_un.S_addr, sizeof(m_remoteAddr.sin_addr.S_un.S_addr));   
	string strIp = inet_ntoa(addr); 

	LogFile->FmtLog(LOG_INFORMATION,"new Connection[%s,%d]",strIp.c_str(),m_remoteAddr.sin_port); 
#else
	LogFile->FmtLog(LOG_INFORMATION,"new Connection"); 
#endif
}
Connection::~Connection(void)
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
	LOG(LOG_INFORMATION,"rec pkg len[%d] data:%s",len, TYQUtils::Byte2Hex(msg,len).c_str());
	BYTE* pMsg = msg;
	do 
	{
		AFNPackage* pkg = new AFNPackage();
		int errCode = pkg->ParseProto(pMsg,len);	
		if (errCode != YQER_OK){
			if (errCode == YQER_PKG_Err(1)){
				//半截包，继续接受...,待做
				YQLogMin("RecBuf, pkg invalid!");
				delete pkg;
				return errCode;
			}
			else{
				YQLogMin("RecBuf, pkg invalid!");
				delete pkg;
				return errCode;
			}
		}
		pMsg += pkg->GetFrameL();
		len -= pkg->GetFrameL();
		std::list<AFNPackage* > ackLst;	
		if (pkg->userHeader.A3._A3.TAG == 0){
			//单地址
			m_jzq.m_areacode = pkg->userHeader.A1;
			m_jzq.m_number = pkg->userHeader.A2;				
		}
		if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 1  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
			//单帧
			YQLogInfo("rec single pkg ");
			nRet = AFNPackageBuilder::Instance().HandlePkg(pkg,ackLst);
			if (nRet == YQER_OK && ackLst.size() > 0){
				SendPkg(ackLst);
				ClearPkgList(ackLst);
			}
			delete pkg;
		}
		else if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 0  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
			//多帧，第一帧
			ClearPkgList(m_pkgList);
			YQLogInfo("rec mul pkg , first");
			m_pkgList.push_back(pkg);

			//in test, no another pkg after this first pkg, so handle this pkg
			nRet = AFNPackageBuilder::Instance().HandlePkg(m_pkgList,ackLst);
			if (nRet == YQER_OK && ackLst.size() > 0){
				SendPkg(ackLst);
				ClearPkgList(ackLst);
			}	
			ClearPkgList(m_pkgList);
			//
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
					ClearPkgList(ackLst);
				}	
				ClearPkgList(m_pkgList);
			}		
		}
		else{
			YQLogMin("rec pkg,FIN|FIR error");
		}
		LOG(LOG_INFORMATION,"parse one pkg over, left %d bytes in buffer",len);
	}while(len > 0);

	return YQER_OK;
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
	ClearPkgList(m_pkgList);
}
void Connection::ClearPkgList(std::list<AFNPackage*>& lst)
{
	for (Iter it = lst.begin();  it != lst.end(); it++){
		delete (*it);
	}
	lst.clear();
}

BOOL Connection::Compare(struct bufferevent *_bev)
{
	return bev==_bev;
}

