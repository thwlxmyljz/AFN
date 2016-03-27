#include "proto.h"
#include "YQErrCode.h"
Pkg_Afn::Pkg_Afn()
		:pAfnData(NULL),pAux(NULL)
{
}
Pkg_Afn::Pkg_Afn(BYTE* _data,DWORD _len)
		:pAfnData(NULL),pAux(NULL)
{
	//���н��
	unPackData(_data,_len);
}
void Pkg_Afn::unPackData(BYTE* _data,DWORD _len)
{
	if (_len >= PKG_AFN_HEADLEN){
		memcpy(&afnHeader,_data,PKG_AFN_HEADLEN);
		_len -= PKG_AFN_HEADLEN;
		_data += PKG_AFN_HEADLEN;
		if (afnHeader.SEQ._SEQ.TPV == 0x01){
			//��ʱ���,AUX�ĳ���Ϊ8�ֽ�
			//_len -= 8;
			//pAux = new 
		}
		else{
			//����ʱ���,AUX�ĳ���Ϊ2�ֽ�
			//_len -= 2;
		}	
		if (_len >= PKG_AFN_DATATAGLEN){
			//����Ԫ��������
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
Pkg_Afn_Data::Pkg_Afn_Data()
		:m_pData(NULL),m_nLen(0)
{

}
Pkg_Afn_Data::Pkg_Afn_Data(BYTE* _data,DWORD _len)
		:m_pData(NULL),m_nLen(0)
{
	if (_len >= PKG_AFN_DATATAGLEN){
		//��Ԫ��ʶ
		memcpy(&m_Tag,_data,PKG_AFN_DATATAGLEN);
		_len -= PKG_AFN_DATATAGLEN;
		_data += PKG_AFN_DATATAGLEN;			
	}
	if (_len > 0){
		//���ݵ�Ԫ
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
int Pkg_Afn_Data::HandleData()
{
	return YQER_OK;
}
int Pkg_Afn_Data::PackData(BYTE* _data,DWORD _len)
{
	if (_len >= m_nLen + PKG_AFN_DATATAGLEN){
		memcpy(_data,&m_Tag,PKG_AFN_DATATAGLEN);
		memcpy(_data+PKG_AFN_DATATAGLEN,m_pData,m_nLen);
		return m_nLen + PKG_AFN_DATATAGLEN;
	}
	return 0;
}