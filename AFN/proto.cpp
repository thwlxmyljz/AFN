#include "proto.h"
#include "YQErrCode.h"
//------------------------------------------------------------------------------------
Pkg_Afn::Pkg_Afn()
		:pAfnData(NULL),pAux(NULL)
{
}
Pkg_Afn::~Pkg_Afn()
{
	delete pAfnData;
	pAfnData = NULL;
	delete pAux;
	pAux = NULL;
}
Pkg_Afn::Pkg_Afn(BYTE* _data,DWORD _len)
		:pAfnData(NULL),pAux(NULL)
{
	//上行解包
	unPackData(_data,_len);
}
void Pkg_Afn::unPackData(BYTE* _data,DWORD _len)
{
	//解包
	if (_data && _len >= PKG_AFN_HEADLEN)
	{
		memcpy(&afnHeader,_data,PKG_AFN_HEADLEN);
		_len -= PKG_AFN_HEADLEN;
		_data += PKG_AFN_HEADLEN;
		if (afnHeader.SEQ._SEQ.TPV == 0x01)
		{
			//带时间戳,AUX的长度为8字节
			//_len -= 8;
			//pAux = new 
		}
		else{
			//不带时间戳,AUX的长度为2字节
			//_len -= 2;
		}	
		if (_len >= PKG_AFN_DATATAGLEN){
			//带单元数据数组
			pAfnData = new Pkg_Afn_Data(_data,_len);
		}					
	}
}
DWORD Pkg_Afn::PackData(BYTE* _data,DWORD _len)
{
	DWORD pkLen = 0x0;
	//AFN包头
	_len -= PKG_AFN_HEADLEN;
	if (_len < 0){
		return 0x0;
	}
	memcpy(_data,&afnHeader,PKG_AFN_HEADLEN);
	_data += PKG_AFN_HEADLEN;
	pkLen += PKG_AFN_HEADLEN;
	//AFN用户数据
	if (pAfnData){
		int afxDataLen = pAfnData->PackData(_data,_len);
		_data += afxDataLen;
		pkLen += afxDataLen;
	}
	//AUX附加域
	if (pAux){
		int auxDataLen = pAux->PackData(_data,_len);
		_data += auxDataLen;
		pkLen += auxDataLen;
	}
	return pkLen;
}
DWORD Pkg_Afn::GetDataLen()
{
	DWORD nLen = PKG_AFN_HEADLEN;
	if (pAfnData){
		nLen += pAfnData->GetDataLen();
	}
	if (pAux){
		nLen += pAux->GetDataLen();
	}
	return nLen;
}
//------------------------------------------------------------------------------------
Pkg_Afn_Data::Pkg_Afn_Data()
		:m_pData(NULL),m_nLen(0)
{
}
Pkg_Afn_Data::Pkg_Afn_Data(const Pkg_Afn_Data& _origin)
{
	m_Tag = _origin.m_Tag;
	m_pData = _origin.m_pData;
	m_nLen = _origin.m_nLen;
}
Pkg_Afn_Data::~Pkg_Afn_Data()
{
	delete m_pData;
	m_pData = NULL;
	m_nLen=0;
}
Pkg_Afn_Data::Pkg_Afn_Data(BYTE* _data,DWORD _len)
		:m_pData(NULL),m_nLen(0)
{
	unPackData(_data,_len);
}

void Pkg_Afn_Data::unPackData(BYTE* _data,DWORD _len)
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
DWORD Pkg_Afn_Data::PackData(BYTE* _data,DWORD _len)
{
	if (_len >= m_nLen + PKG_AFN_DATATAGLEN){
		memcpy(_data,&m_Tag,PKG_AFN_DATATAGLEN);
		memcpy(_data+PKG_AFN_DATATAGLEN,m_pData,m_nLen);
		return m_nLen + PKG_AFN_DATATAGLEN;
	}
	return 0x0;
}
DWORD Pkg_Afn_Data::GetDataLen()
{
	return PKG_AFN_DATATAGLEN+m_nLen;
}
//------------------------------------------------------------------------------------
void Pkg_Afn_Aux::unPackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = Pkg_Afn_Aux::GetDataLen();
	if (_len >= myLen){
		if (hasTp){			
			TP.PFC = _data[0];
			TP.TM[0] = _data[1];
			TP.TM[1] = _data[2];
			TP.TM[2] = _data[3];
			TP.TM[3] = _data[4];
			TP.DELAY = _data[5];
		} 
	}
}
DWORD Pkg_Afn_Aux::PackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = Pkg_Afn_Aux::GetDataLen();
	if (_len >= myLen){
		if (hasTp){			
			_data[0] = TP.PFC;
			_data[1] = TP.TM[0];
			_data[2] = TP.TM[1];
			_data[3] = TP.TM[2];
			_data[4] = TP.TM[3];
			_data[5] = TP.DELAY;
		} 
	}
	return myLen;
}
DWORD Pkg_Afn_Aux::GetDataLen()
{
	if (hasTp)
		return 6;//1+1+1+4+1
	return 0;
}
//------------------------------------------------------------------------------------
void Pkg_Afn_Aux_Up::unPackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = GetDataLen();
	if (_len >= myLen){
		EC.EC1 = _data[0];
		EC.EC2 = _data[1];
		Pkg_Afn_Aux::unPackData(_data+2,_len-2);
	}
}
DWORD Pkg_Afn_Aux_Up::PackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = GetDataLen();
	if (_len >= myLen){
		_data[0] = EC.EC1;
		_data[1] = EC.EC2;
		Pkg_Afn_Aux::PackData(_data+2,_len-2);
	}
	return myLen;
}
DWORD Pkg_Afn_Aux_Up::GetDataLen()
{
	return 2+Pkg_Afn_Aux::GetDataLen();
}
//------------------------------------------------------------------------------------
void Pkg_Afn_Aux_Down::unPackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = GetDataLen();
	if (_len >= myLen){
		for (int i = 0; i < 16; i++){
			PW[i] = _data[i];
		}
		Pkg_Afn_Aux::unPackData(_data+16,_len-16);
	}
}
DWORD Pkg_Afn_Aux_Down::PackData(BYTE* _data,DWORD _len)
{
	DWORD myLen = GetDataLen();
	if (_len >= myLen){
		if (hasPW){
			for (int i = 0; i < 16; i++){
				_data[i] = PW[i];
			}
		}
		Pkg_Afn_Aux::PackData(_data+16,_len-16);
	}
	return myLen;
}
DWORD Pkg_Afn_Aux_Down::GetDataLen()
{
	if (hasPW)
		return 16+Pkg_Afn_Aux::GetDataLen();
	return Pkg_Afn_Aux::GetDataLen();
}