#pragma once

#include "AFNPackage.h"
#include <map>

typedef int (*pfnDoHandleRequest)(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
typedef int (*pfnDoHandleAck)(std::list<AFNPackage*>& ackLst);

struct PkgHandler{
	pfnDoHandleRequest reqHander;
	pfnDoHandleAck ackHandler;
};

class AFNPackageBuilder
{
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);
public:		
	//ע�ᴦ����
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);

	//����֡����,reqLst:��֡����ackLst:������Ӧ֡������0����֡��,�����������Ӧ֡������Ҫ���͵��ն�
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

	/*
	��������֡
	*/
	//�ٲ������,pn�������,0���в�����
	int setpointparams(std::string name,WORD pn);
	int setpointstatus(std::string name);

	//��ȡ������ʱ��
	int getclock(std::string name);
	//�ն˼��г���״̬��Ϣ
	int getstatus(std::string name);
	//������Ϣ
	int getallkwh(std::string name,WORD pn);
public:
	static AFNPackageBuilder& Instance(){
		if (single == NULL){
			single = new AFNPackageBuilder;
		}
		return (*single);
	}	
protected:
	//����֡����
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//��Ӧ֡����
	int DoHandleAck(std::list<AFNPackage*>& reqLst);
private:
	//Builder��һ����
	static AFNPackageBuilder* single;
	//���Ĵ���MAP
	std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler> handlerMap;
	typedef std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler>::iterator mapIter;
};

