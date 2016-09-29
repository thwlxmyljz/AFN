#include "ConnectionPower.h"
#include "ConnectionList.h"
#include "AFN02.h"
#include "AFNPackage.h"
#include "YQErrCode.h"
#include "LogFileu.h"
#include "AFNPackageBuilder.h"
#include "IPackage.h"

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
int AFN02::HandleRequest(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst)
{	
	if (reqLst.size() == 0){
		return YQER_OK;
	}
	//AFN02单帧
	AFNPackage* reqPkg = (AFNPackage*)(*(reqLst.begin()));	
	if (reqPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_MAIN9)
	{
		//链路接口检测,发送响应帧,保存启动帧序号PSEQ为响应帧起始序号			
		if (reqPkg->pn == 0){
			if (reqPkg->Fn == 1/*F1登录*/ || reqPkg->Fn == 2/*F2退出登录*/ || reqPkg->Fn == 3/*F3心跳,数据单元带终端时间*/)
			{
				//通知应用层
				g_JzqConList->ReportLoginState(reqPkg->userHeader.A1,reqPkg->userHeader.A2,reqPkg->Fn,reqPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ,TRUE);

				if (reqPkg->pAfn->afnHeader.SEQ._SEQ.CON != Pkg_Afn_Header::SEQ_CON_MBANSWER){
					//终端测试软件的CON=0，实际设备CON=1								
					return YQER_OK;
				}
				//数据按F3回应，ERR定义如下，=0正确，=1其他错误，=2表地址重复，3~255备用
				ackLst.push_back(AFNPackageBuilder::Instance().CreateAck(reqPkg,new AFN02Ack_Data(reqPkg->pAfn->pAfnData->m_Tag)));
				return YQER_OK;
			}			
		}
	}
	return -1;
}
