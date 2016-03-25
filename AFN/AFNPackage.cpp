#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

DWORD AFNPackage::s_pkgID = 1;

AFNPackage::AFNPackage()
	:m_nId(s_pkgID)
{
}
AFNPackage::~AFNPackage(void)
{
}
int AFNPackage::parseProto(BYTE* data,DWORD len)
{
	if (len < (PKG_HEADLEN+PKG_USER_HEADLEN+PKG_TAILLEN))
		return YQER_PKG_Err(1);
	//���ɰ�ͷ��
	memcpy(&pkgHeader,data,PKG_HEADLEN);
	//���ɰ�β��
	memcpy(&pkgTail,data+len-2,PKG_TAILLEN);

	//�жϰ��Ϸ���
	if (!valid()){
		//����ʶ�ַ��Ƿ�
		return YQER_PKG_Err(2);
	}
	if (len != pkgHeader.L.LEN+PKG_HEADLEN+PKG_TAILLEN){
		//�����Ȳ���ȷ
		return YQER_PKG_Err(3);
	}
	BYTE cs = GetCS(data+PKG_HEADLEN,pkgHeader.L.LEN);
	if (cs != pkgTail.CS){
		//У��ʹ���
		return YQER_PKG_Err(4);
	}

	//�����û�������
	data += PKG_HEADLEN;
	memcpy(&userHeader,data+PKG_HEADLEN,PKG_USER_HEADLEN);

	//����AFN����
	data += PKG_USER_HEADLEN;
	pAfn = new Pkg_Afn(data,pkgHeader.L.LEN-PKG_USER_HEADLEN);//Ӧ�����ݳ���=�û����ݳ���-�û�ͷ����
	
	return YQER_OK;
}
int AFNPackage::serialize(BYTE* buf,DWORD bufLen)
{
	return YQER_OK;
}
int AFNPackage::HandlePkg()
{
	return YQER_OK;
}