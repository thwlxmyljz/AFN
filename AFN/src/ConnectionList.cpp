#include "ConnectionList.h"
#include "Connection.h"
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

#define TM_GETKWH 10000 //10 secs

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
	/*
	CREATE TABLE `gc_equipmentjzq` (
	  `ID` int(24) NOT NULL AUTO_INCREMENT COMMENT '序号',
	  `EquipmentName` varchar(24) NOT NULL COMMENT '集中器名称',
	  `EquipmentCjqArea` int(11) NOT NULL COMMENT '采集器 行政区划码',
	  `EquipmentCjqAddr` int(11) NOT NULL COMMENT '采集器 终端地址',
	  `EquipmentXH` varchar(24) DEFAULT NULL COMMENT '集中器型号',
	  `EquipmentPower` float DEFAULT NULL COMMENT '集中器功率',
	  `UnitCode` varchar(32) NOT NULL COMMENT '大单位序号',
	  `UnitDepCode` varchar(32) NOT NULL COMMENT '小单位序号',
	  `EquipmentPlace` varchar(24) DEFAULT NULL COMMENT '设备所安装的位置',
	  `SetTime` datetime(6) NOT NULL COMMENT '设备安装时间',
	  `State` int(8) DEFAULT NULL COMMENT '设备的使用状态',
	  `Remarks` varchar(64) DEFAULT NULL COMMENT '备注信息',
	  PRIMARY KEY (`ID`),
	  KEY `INDEX1` (`EquipmentCjqArea`,`EquipmentCjqAddr`)
	) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
	*/
	std::string sql= "select * from gc_equipmentjzq";
	qopen(sql);
	query()->First();
	LOG(LOG_INFORMATION,"jzq count(%d)",query()->RecordCount);
	while (!(query()->IsEof())){		
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
		/*
		CREATE TABLE `gc_equipmentelect` (
		  `ID` int(24) NOT NULL AUTO_INCREMENT COMMENT '序号',
		  `EquipmentName` varchar(24) NOT NULL COMMENT '电仪器名称',
		  `EquipmentCjqPn` int(11) NOT NULL COMMENT '电仪器编号 电表标识',
		  `EquipmentCjqArea` int(11) NOT NULL COMMENT '采集器 行政区划码',
		  `EquipmentCjqAddr` int(11) NOT NULL COMMENT '采集器 终端地址',
		  `EquipmentXH` varchar(24) DEFAULT NULL COMMENT '电仪器型号',
		  `EquipmentPower` float DEFAULT NULL COMMENT '电仪器功率',
		  `UnitName` varchar(32) NOT NULL COMMENT '大单位名称',
		  `UnitCode` varchar(32) NOT NULL COMMENT '大单位序号',
		  `UnitDepName` varchar(32) NOT NULL COMMENT '小单位名称',
		  `UnitDepCode` varchar(32) NOT NULL COMMENT '小单位序号',
		  `EquipmentPlace` varchar(24) DEFAULT NULL COMMENT '设备所安装的位置',
		  `SetTime` datetime(6) NOT NULL COMMENT '设备安装时间',
		  `State` int(8) DEFAULT NULL COMMENT '设备的使用状态',
		  `Remarks` varchar(64) DEFAULT NULL COMMENT '备注信息',
		  PRIMARY KEY (`ID`),
		  UNIQUE KEY `EquipmentCjqPn` (`EquipmentCjqPn`),
		  KEY `INDEX1` (`EquipmentCjqPn`,`EquipmentCjqArea`,`EquipmentCjqAddr`)
		) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
		*/
		Jzq* p = (*it);
		char sqlBuf[128];
		snprintf(sqlBuf,128,"select * from gc_equipmentelect where EquipmentCjqArea=%d and EquipmentCjqAddr=%d",p->m_a1a2.m_areacode,p->m_a1a2.m_number);
		qopen(sqlBuf);
		query()->First();
		LOG(LOG_INFORMATION,"jzq count(%d)",query()->RecordCount);
		while (!(query()->IsEof())){		
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
	/*
	Jzq* p = new Jzq("test01",0xffff,0xffff,0x01);
	m_jzqList.push_back(p);
	p = new Jzq("test",0x1000,0x44d,0x01);
	m_jzqList.push_back(p);*/
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
std::string JzqList::printJzq()
{
	std::string ss;
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){
		ss += (*it)->printInfo();
	}
	return ss;
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
	YQLogInfo(ss.c_str());
}
/*
定时采集集中器数据
*/
void JzqList::AutoGetAllKwh()
{
	for (jzqIter it = m_jzqList.begin(); it != m_jzqList.end(); it++){		
		Jzq* pJzq = (*it);
		if (TYQUtils::TimeElapse(pJzq->m_kwhTimer) > TM_GETKWH){
			if (!getConnection(pJzq->m_a1a2.m_areacode,pJzq->m_a1a2.m_number)){
				LOG(LOG_INFORMATION,"auto getkwh(%s), jzq not connected",pJzq->m_name.c_str());
				continue;
			}
			Pkg_Afn_Data* p = NULL;
			AFNPackageBuilder::Instance().getallkwh_async(&p,pJzq->m_name,1/*集中器定义的测试测量点*/);
		}
	}
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
void JzqList::delConnection(struct bufferevent *bev)
{
	AUTO_LOCK()
	for (conIter it = g_JzqConList->begin(); it != g_JzqConList->end(); it++){
		Connection* con = (*it);
		if (con->Compare(bev)){			
			g_JzqConList->ReportLoginState(con->m_jzq.m_areacode,con->m_jzq.m_number,2,0,FALSE);
			YQLogInfo("delete connection");
			g_JzqConList->erase(it);			
			delete con;
			break;
		}
	}    
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
