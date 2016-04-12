#include "Connection.h"
#include "ConnectionList.h"
#include "AFN02.h"
#include "AFNPackage.h"
#include "YQErrCode.h"
#include "LogFileu.h"


AFN02Ack_Data::AFN02Ack_Data(Pkg_Afn_DataTag _what)
	:Pkg_Afn_Data(),WhatAckDataTag(_what)
{
	m_Tag.DA1 = 0;
	m_Tag.DA2 = 0;
	AFNPackage::GetDT(3/*F3*/,m_Tag.DT1,m_Tag.DT2);

	WhatAckAFN = 0x02;
	Err = 0x00;

	m_nLen = 6;//1+4+1
	m_pData = new BYTE[m_nLen];
	m_pData[0] = WhatAckAFN;	
	memcpy(m_pData+1,&WhatAckDataTag,4);
	m_pData[5] = Err;
}
AFN02Ack_Data::~AFN02Ack_Data()
{
}
int AFN02::HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{	
	if (reqLst.size() == 0){
		return YQER_OK;
	}
	//AFN02单帧
	AFNPackage* reqPkg = *(reqLst.begin());	
	if (reqPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_MAIN9)
	{
		//链路接口检测,发送响应帧,保存启动帧序号PSEQ为响应帧起始序号			
		if (reqPkg->pn == 0){
			if (reqPkg->Fn == 1/*F1登录*/ || reqPkg->Fn == 2/*F2退出登录*/ || reqPkg->Fn == 3/*F3心跳,数据单元带终端时间*/)
			{
				g_JzqConList->ReportLoginState(reqPkg->userHeader.A1,reqPkg->userHeader.A2,reqPkg->Fn,reqPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ);

				if (reqPkg->pAfn->afnHeader.SEQ._SEQ.CON != Pkg_Afn_Header::SEQ_CON_MBANSWER){
					//终端测试软件的CON=0，实际设备CON=1								
					return YQER_OK;
				}
				//数据按F3回应，ERR定义如下，=0正确，=1其他错误，=2表地址重复，3~255备用
				AFNPackage* ackPkg = new AFNPackage();
				ackPkg->userHeader.C._C.DIR = 0x00;
				ackPkg->userHeader.C._C.PRM = 0x00;
				ackPkg->userHeader.C._C.FCV = 0x00;
				ackPkg->userHeader.C._C.FCB = 0x00;
				ackPkg->userHeader.C._C.FUN = 11;//链路状态
				ackPkg->userHeader.A3._A3.TAG = 0;//单地址
				ackPkg->userHeader.A3._A3.MSA = Jzq::s_MSA;
				ackPkg->userHeader.A1 = reqPkg->userHeader.A1;
				ackPkg->userHeader.A2 = reqPkg->userHeader.A2;
				ackPkg->pAfn->afnHeader.AFN = Pkg_Afn_Header::AFN00;//确认
				ackPkg->pAfn->afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_NOANSWER;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.FIN = 1;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.FIR = 1;
				ackPkg->pAfn->afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;						
				ackPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetRSEQ(reqPkg->userHeader.A1,reqPkg->userHeader.A2);	
				ackPkg->pAfn->pAfnData = new AFN02Ack_Data(reqPkg->pAfn->pAfnData->m_Tag);
				ackPkg->okPkg();

				ackLst.push_back(ackPkg);
				return YQER_OK;
			}			
		}
	}
	return -1;
}
