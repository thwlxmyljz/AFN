#include "stdafx.h"
#include "AFN02.h"
#include "YQErrCode.h"
AFN02Ack::AFN02Ack(Pkg_Afn_DataTag _what)
	:Pkg_Afn_Data(),WhatAckDataTag(_what)
{
	m_Tag.DA1 = 0;
	m_Tag.DA2 = 0;
	m_Tag.DT1 = 3;//F3ȷ��
	m_Tag.DT2 = 0;

	WhatAckAFN = 0x02;
	Err = 0x00;

	m_nLen = 6;//1+4+1
	m_pData = new BYTE[m_nLen];
	m_pData[0] = WhatAckAFN;	
	memcpy(m_pData+1,&WhatAckDataTag,4);
	m_pData[5] = Err;
}
AFN02Ack::~AFN02Ack()
{
}
//--------------------------------------------------------------------------------------------------------
AFN02::AFN02(void)
{
}
AFN02::~AFN02(void)
{
}
int AFN02::HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	//AFN02��֡
	AFNPackage* reqPkg = *(reqLst.begin());	
	if (reqPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_MAIN9 && \
		reqPkg->pAfn->afnHeader.AFN == Pkg_Afn_Header::AFN02)// && \
		//reqPkg->pAfn->afnHeader.SEQ._SEQ.CON == Pkg_Afn_Header::SEQ_CON_MBANSWER)
	{
		//��·�ӿڼ��,������Ӧ֡,��������֡���PSEQΪ��Ӧ֡��ʼ���			
		if (reqPkg->pAfn->pAfnData->m_Tag.DA1 == 0 && reqPkg->pAfn->pAfnData->m_Tag.DA2 == 0)
		{
			if (reqPkg->pAfn->pAfnData->m_Tag.DT1 == 1 && reqPkg->pAfn->pAfnData->m_Tag.DT2 == 0){
				//��¼,������
				AFNPackage::s_RSEQ = reqPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ;
				//���ݰ�F3��Ӧ��ERR�������£�=0��ȷ��=1��������=2���ַ�ظ���3~255����
				AFNPackage* ackPkg = new AFNPackage();
				if (!ackPkg){
					return YQER_NOMEMORY;
				}
				ackPkg->userHeader.C._C.DIR = 0x00;
				ackPkg->userHeader.C._C.PRM = 0x00;
				ackPkg->userHeader.C._C.FCV = 0x00;
				ackPkg->userHeader.C._C.FCB = 0x00;
				ackPkg->userHeader.C._C.FUN = 11;//��·״̬
				ackPkg->userHeader.A3._A3.TAG = 0;//����ַ
				ackPkg->userHeader.A3._A3.MSA = AFNPackage::s_MSA;
				ackPkg->userHeader.A1 = reqPkg->userHeader.A1;
				ackPkg->userHeader.A2 = reqPkg->userHeader.A2;
				ackPkg->pAfn->afnHeader.AFN = Pkg_Afn_Header::AFN00;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_NOANSWER;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.FIN = 1;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.FIR = 1;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;		
				ackPkg->pAfn->pAfnData = new AFN02Ack(reqPkg->pAfn->pAfnData->m_Tag);

				ackPkg->SetRSEQ();
				ackPkg->SetL();
				ackPkg->CreateCS();
				ackLst.push_back(ackPkg);

				return YQER_OK;
			}
			else if (reqPkg->pAfn->pAfnData->m_Tag.DT1 == 2 && reqPkg->pAfn->pAfnData->m_Tag.DT2 == 0){
				//�˳���¼,������
			}
			else if (reqPkg->pAfn->pAfnData->m_Tag.DT1 == 3 && reqPkg->pAfn->pAfnData->m_Tag.DT2 == 0){
				//����,6�ֽ��ն�ʱ������
			}
		}
	}
	return -1;
}
