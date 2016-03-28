//字节序，低位在前，高位在后，低字节在前，高字节在后,即小端存储,请在x86平台编译
//终端登录抓包：   68 32 00 32 00 68 c9 00 10 4d 04 00 02 75 00 00 01 00 a2 16
//EXE测试登录发包：68 32 00 32 00 68 49 00 00 00 00 02 02 62 00 00 01 00 B0 16
/*
---------------------------------------------
| 起始字符(0x68) |
---------------
| 长度L(2字节)   |
---------------            固定包头长度(6字节)
| 长度L(2字节)   |          L表示用户数据区长度
---------------
| 起始字符(0x68) |
----------------------------------------------
| 控制于C(1字节）	|
---------------
| 地址域A(5字节)	|          用户数据区(CS校验区)
---------------
| 链路用户数据	|
----------------------------------------------
| 校验和CS		|
---------------
| 结束字符E(0x16)	|
----------------------------------------------
*/
#ifndef __PROTO_H__
#define __PROTO_H__

#include "YQDefs.h"

#pragma pack(1) //1字节对齐

#define PKG_HEADLEN 6 //固定包头6字节长度
#define PKG_USER_HEADLEN 6 //用户数据区头6字节长度
#define PKG_TAILLEN 2 //包尾2字节长度

#define PKG_AFN_HEADLEN 2 //AFN应用数据区头2字节长度
#define PKG_AFN_DATATAGLEN 4 //AFN应用数据区数据标识长度4字节(DA,DT)

class Pkg_Header{
public:
	/*
	固定长度包头,6BYTE
	*/
	BYTE S1;//起始字符0x68
	/*
	控制域+地址域+用户数据长度值
	L,L1:长度D0-D15按位说明
	D0=0,D1=0:禁用
	D0=1,D1=0:Q/GDW 130-2005使用
	D0=0,D1=1:本协议使用
	D0=1,D1=1:保留
	D2-D15:控制域+地址域+用户数据长度值,BIN编码
	*/
	union {
		WORD l;
		struct {
			WORD TAG:2;
			WORD LEN:14;		
		} _L;//长度1
	} L;
	union {
		WORD l1;
		struct {
			WORD TAG:2;
			WORD LEN:14;
		} _L1;//长度2
	} L1;

	BYTE S2;//起始字符0x68
};
/*
用户数据区包头,包括C,A部分
*/
class Pkg_User_Header{
public:		
	/*
	C:控制域,按位说明如下
		D7:传输方向位DIR,DIR=0下行，DIR=1上行
		D6:启动标识位PRM,RPM=1表示此帧报文来自启动站,PRM=0表示此帧报文来自从动站
		D5:下行表示帧计数位FCB，上行表示请求访问位ACD
		D4:下行表示帧计数有效位FCV,上行保留未用
			帧计数位FCV,FCB说明：
				FCV=1,表示FCB位有效，FCV=0，表示FCB位无效
				当帧计数有效位FCV=1时，FCB表示每个站连续的发送/确认或者请求/响应服务的变化位，FCB位用来防止信息传输的丢失和重复
				启动站向同一从动站传输新的发送/确认或者请求/响应传输服务时，将FCB取相反值。启动站保存每个从动站的FCB值，若超时未
				收到从动站的报文，或者接收出现差错，则启动站不改变FCB的状态，重复原来的发送/确认或者请求/响应服务。
			请求访问位ACD说明：
				出现在上行响应报文中（主站分析）
				ACD=1,表示终端有重要事件等待访问，则附加信息域中带有事件计数器EC(EC说明见下)
				ACD=0,表示终端无重要事件等待访问
				终端设置规则：
				自上次收到报文后发生新的重要事件，ACD置位1
				收到主站请求事件并执行完成后，ACD置位0
				
		D0-D3:功能码

			PRM=1时（报文来自启动站）
			
			值			帧类型			服务功能
			0			保留
			1			发送/确认		复位命令
			2-3			保留
			4			发送/无回答		用户数据
			5-8			保留
			9			请求响应帧		链路测试
			10			请求响应帧		请求1级数据
			11			请求响应帧		请求2级数据
			12-15		保留
			
			PRM=0时（报文来自从动站）
			
			值			帧类型			服务功能
			0			确认				认可
			1-7			保留
			8			响应帧			用户数据
			9			响应帧			否认：无所召唤的数据
			10			保留
			11			响应帧			链路状态
			12-15		保留
	*/
	union {
		BYTE c;
		struct {
			BYTE FUN:4;
			BYTE FCV:1;
			BYTE FCB:1;
			BYTE PRM:1;
			BYTE DIR:1;
		} _C;
	} C;
	/*FUNC定义*/
	enum UH_FUNC_MAIN{
		UH_FUNC_MAIN1 = 1,
		UH_FUNC_MAIN4 = 4,
		UH_FUNC_MAIN9 = 9,
		UH_FUNC_MAIN10 = 10,
		UH_FUNC_MAIN11 = 11
	};
	enum UH_FUNC_SUB{
		UH_FUNC_SUB0 = 0,
		UH_FUNC_SUB8 = 8,
		UH_FUNC_SUB9 = 9,
		UH_FUNC_SUB11 = 11
	};
	/*
	A:地址域,包含A1,A2,A3
		A1:行政区码,按国标执行,2字节
		A2：终端地址，2字节，=0无效，=0xFFFF,视A3的D0位定义，A3(D0=1)表示系统广播地址
		A3:主站地址和终端组地址标识，1字节，D0=0表示A2为单地址，D0=1表示A2位组播地址，D1-D7组成0-127个主站地址MSA
			1、主站启动的发送帧的MSA应为非0值，其终端响应帧的MSA与主站相同
			2、终端启动的发送帧的MSA为0,主站响应帧的MSA也要设置为0
	*/
	WORD A1;//地址域，行政区码(BCD编码)
	WORD A2;//地址域，终端地址(BIN)
	union {
		BYTE a3;
		struct {
			BYTE TAG:1;
			BYTE MSA:7;
		} _A3;//地址域，主站地址和终端组地址标识(BIN)
	} A3;
};
/*
包尾
*/
class Pkg_Tail{
public:	
	BYTE CS;//用户数据区校验和,按8位组算术和,不考虑溢出
	BYTE E;//结束字符0x16
};
/*
应用层数据区头
*/
class Pkg_Afn_Header{
public:
	/*
	功能码
	AFN值		功能定义
	0x00		确认/否认
	0x01		复位
	0x02		链路接口检测
	0x03		中继站命令
	0x04		设置参数
	0x05		控制命令
	0x06		身份认证及密钥协商
	0x07		备用
	0x08		请求被级联终端主动上报
	0x09		请求终端配置
	0x0A		查询参数
	0x0B		请求任务数据
	0x0C		请求1类数据(实时数据)
	0x0D		请求2类数据(历史数据)
	0x0E		请求3类数据(事件数据)
	0x0F		文件传输
	0x10		数据转发
	0x11-0xFF	备用

	AFN=0x02时
		F1:登录
		F2:退出登录
		F3:心跳
	*/
	BYTE AFN;
	enum AFN_CODE{
		AFN00 = 0,
		AFN01,
		AFN02,
		AFN03,
		AFN04,
		AFN05,
		AFN06,
		AFN07,
		AFN08,
		AFN09,
		AFN0A,
		AFN0B,
		AFN0C,
		AFN0D,
		AFN0E,
		AFN0F,
		AFN10,
	};
	/*
	序列号
	D7:时间标签有效位TPV,=1附加信息域带有时间戳，=0无时间戳
	D6:首帧标识FIR,第一帧
	D5:末帧标识FIN,最后一帧,(D5,D6)组合使用，如下表
		D6	D5	说明
		0	0	多帧，中间帧
		0	1	多帧，结束帧
		1	0	多帧，第一帧，有后续帧
		1	1	单帧
	D4:请求确认标识位CON，=1表示收到报文后需要对该报文确认，=0不确认
	D0~D3:启动帧序号PSEQ/响应帧序号RSEQ
	举例:	
	*/
	union {
		BYTE seq;
		struct {
			//PSEQ时为PFC低4位,取值(0~15)，RSEQ时初始为PSEQ，以后递增1,如果连续收到RSEQ相同的帧，则不处理，连续收到PSEQ，则重发响应
			BYTE PRSEQ:4;
			BYTE CON:1;
			BYTE FIN:1;
			BYTE FIR:1;
			BYTE TPV:1;
		} _SEQ;
	} SEQ;
	enum SEQ_CON{
		SEQ_CON_NOANSWER=0,
		SEQ_CON_MBANSWER
	};
	enum SEQ_TPV{
		SEQ_TPV_NO=0,
		SEQ_CON_YES
	};	
};
/*
数据单元标识
*/
class Pkg_Afn_DataTag{
public:
	/*
	DA数据单元标识,2字节
	DA1,DA2共表示8*255=2040个信息点，记为pn,(n=[0~2040]),p0表示终端信息点,p1开始表示确切信息点
	DA1=0xff,DA2=0x00表示所有有效测量点(不包含p0)
	*/
	BYTE DA1;//数据单元标识DA1，按位表示每个信息点组的（0-8个）信息点元
	BYTE DA2;//数据单元标识DA2，按值（0-255）表示信息点组
	
	/*
	DT数据单元标识,2字节
	DT1,DT2共表示8*31=248个信息类型，记为Fn,(n=[1~248]),终端无数据时,DT位清0，部分无数据时，将数据项缺失内容填写0xEE	
	*/
	BYTE DT1;//数据单元标识DT1,按位表示每个信息类组的(0-8种)信息类元
	BYTE DT2;//数据单元标识DT2,按值(只取0-31)表示信息类组
};
/*
数据单元接口
*/
class Pkg_UserDataInterface{
public:
	//解包
	virtual void unPackData(BYTE* _data,DWORD _len) = 0;
	//封包,返回封包字节数
	virtual int PackData(BYTE* _data,DWORD _len) = 0;
	//数据长度
	virtual int GetDataLen() = 0;
};
/*
数据单元
*/
class Pkg_Afn_Data : public Pkg_UserDataInterface {
	friend class Pkg_Afn;
	friend class Pkg;

public:
	//DA,DT,4字节单元标识
	Pkg_Afn_DataTag m_Tag;

	//单元数据内容
	BYTE* m_pData;
	DWORD m_nLen;

public:
	Pkg_Afn_Data();
	virtual ~Pkg_Afn_Data();
	Pkg_Afn_Data(BYTE* _data,DWORD _len);

	//数据处理
	virtual int HandleData();

	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
	{
		return PKG_AFN_DATATAGLEN+m_nLen;
	}
};
/*
附加信息域,分上下行
*/
class Pkg_Afn_Aux : public Pkg_UserDataInterface{
};
class Pkg_Afn_Aux_Up : public Pkg_Afn_Aux {
public:
	//上行,EC,2字节
	struct {
		BYTE EC1;//事件计数器EC1（重要事件计数器,范围0~255）,上行,ACD=1时有效
		BYTE EC2;//事件计数器EC2（一般事件计数器,范围0~255）,上行,ACD=1时有效
	} EC;
	//时间戳,6字节
	struct {
		BYTE PFC;//启动帧计数器PFC,BIN编码,启动站每发送1帧数据则+1，从0～255循环增加，重发帧不变PFC
		BYTE TM[4];//启动帧发送时标,记录启动帧发送的时间,单位秒分时日
		BYTE DELAY;//允许发送传输延时时间,单位:分钟,BIN编码
	} TP;
	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
	{
		return 8;
	}
};
class Pkg_Afn_Aux_Down : Pkg_Afn_Aux {
public:
	//消息认证码PW,下行发送,由主站算法计算，如带此数据，则终端必须认证此数据，通过则响应命令，否则不响应命令
	BYTE PW[16];
	//时间戳,6字节
	struct {
		BYTE PFC;//时间戳TPV(启动帧计数器PFC,BIN编码)
		BYTE TM[4];//时间戳TPV(启动帧发送时标,记录启动帧发送的时间,单位秒分时日)
		BYTE DELAY;//时间戳TPV(允许发送传输延时时间,单位:分钟,BIN编码)
	} TP;
	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
	{
		return 22;
	}
};
/*
应用数据区包格式
*/
class Pkg_Afn {
public:
	/*
	功能码+SEQ,2字节
	*/
	Pkg_Afn_Header afnHeader;	
	/*
	应用数据单元数组
	按p0-p2040排列,每个pn内按F1-F248排列
	终端响应数据时如果某项无数据则在DT的对应标识位置0(Fn=0),某项数据部分缺失，缺失数据部分填0xEE。
	*/
	Pkg_Afn_Data *pAfnData;
	/*
	附加信息域
	*/
	Pkg_Afn_Aux  *pAux;
public:
	Pkg_Afn();
	~Pkg_Afn();
	Pkg_Afn(BYTE* _data,DWORD _len);	

	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
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
};
/*
包定义
*/
class Pkg {
public:
	/*包头*/
	Pkg_Header pkgHeader;
	/*用户数据头*/
	Pkg_User_Header userHeader;
	/*应用数据*/
	Pkg_Afn* pAfn;
	/*包尾*/
	Pkg_Tail pkgTail;
};
#endif