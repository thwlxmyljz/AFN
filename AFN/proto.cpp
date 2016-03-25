#include "proto.h"

Pkg_Afn::Pkg_Afn(BYTE* _data,DWORD _len)
		:pAfnData(NULL),pAux(NULL)
{
	//上行解包
	if (_len >= PKG_AFN_HEADLEN){
		memcpy(&afnHeader,_data,PKG_AFN_HEADLEN);
		_len -= PKG_AFN_HEADLEN;
		_data += PKG_AFN_HEADLEN;
		if (afnHeader.SEQ.TPV == 0x01){
			//带时间戳,AUX的长度为8字节
			//_len -= 8;
			//pAux = new 
		}
		else{
			//不带时间戳,AUX的长度为2字节
			//_len -= 2;
		}	
		if (_len >= PKG_AFN_DATATAGLEN){
			//带单元数据
			pAfnData = new Pkg_Afn_Data(_data,_len);
		}					
	}
}
Pkg_Afn::~Pkg_Afn()
{
	delete pAfnData;
	pAfnData = NULL;
	delete pAux;
	pAux = NULL;
}
Pkg_Afn_Data::Pkg_Afn_Data(BYTE* _data,DWORD _len)
		:m_pData(NULL),m_nLen(0)
{
	if (_len >= PKG_AFN_DATATAGLEN){
		//单元标识
		memcpy(&m_Tag,_data,PKG_AFN_DATATAGLEN);
		_len -= PKG_AFN_DATATAGLEN;
		_data += PKG_AFN_DATATAGLEN;			
	}
	if (_len > 0){
		//数据单元
		m_pData = new BYTE[_len];
		if (m_pData){
			m_nLen = _len;
			memcpy(m_pData,_data,m_nLen);
		}
	}
}
Pkg_Afn_Data::~Pkg_Afn_Data()
{
	delete m_pData;
	m_pData=NULL;
	m_nLen=0;
}