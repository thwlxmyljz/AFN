#pragma once

#include "proto.h"
#include <list>

class AFNPackage : public Pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;

	//处理来自启动站请求报文,ackPkg返回响应帧（可能0~多帧）
	int HandleRequest(std::list<AFNPackage*>& ackLst);
	//作为从动站,处理响应报文,ackPkg返回响应帧（可能0~多帧）
	int HandleAck(std::list<AFNPackage*>& ackLst);

	//填写ACK包的相关字段
	void SetAckType2ZJQ();
	//填写REQ包的相关字段
	void SetReqType2ZJQ();
public:
	AFNPackage(void);
	~AFNPackage(void);

	//数据分析包
	int ParseProto(BYTE* data,DWORD len);
	//包序列化到buf中
	int Serialize(BYTE* buf,DWORD bufLen);

	//单帧处理,ackLst返回响应帧（可能0~多帧）
	int HandlePkg(std::list<AFNPackage*>& ackLst);
	//多帧处理,reqLst多帧请求，ackLst返回响应帧（可能0～多帧）
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

	//计算CS值
	static BYTE GetCS(BYTE* buf,DWORD len)
	{
		BYTE cs = 0x00;
		for (DWORD i = 0; i < len; i++){
			cs += buf[i];
		}
		return cs;
	}

	/*校验包合法性*/
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
	}
private:
	//应用层包唯一ID计数
	static DWORD s_pkgID;
	//响应帧序号
	static BYTE s_RSEQ;
	//请求帧序号
	static BYTE s_PSEQ;
	//主站MSA值
	static BYTE s_MSA;
private:
	//应用层包唯一ID
	DWORD m_nId;
	void DoSetRSEQ();
};

