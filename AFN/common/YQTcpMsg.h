//---------------------------------------------------------------------------
#ifndef YQTcpMsgH
#define YQTcpMsgH
//---------------------------------------------------------------------------
//ACD与ACD配置进程\ACDHTTP进程之间消息队列消息
#define YQTCP_ACDCFGCREATEDIR  0x300 //ACDCFG请求文件服务器建立目录,CALL进程操作,MsgStr:目录名
#define YQTCP_ACDCFGDELETEDIR  0x301 //ACDCFG请求文件服务器删除目录,CALL进程操作,MsgStr:目录名
#define YQTCP_ACDCHANGETAB     0x302 //ACDHTTP修改表通知，CALL进程操作,MsgInt1:表ID
#define YQTCP_ACDHTTPSENDSM    0x303 //ACDHTTP发送短信，CALL进程操作,发送到外部目的号,MsgInt1:企业ID，MsgInt2:短信编号(数据库字段为DXBH)
#define YQTCP_ACDHTTPSENDIM    0x304 //ACDHTTP发送即时消息，CALL进程操作,通知到企业内部对应SIP终端,MsgInt1:企业ID，MsgInt2:即时消息编号(数据库字段为SMID)
#define YQTCP_WEBCREATECORP    0x305 //(分布式)WEB创建企业，ACDCFG操作，建立企业，MsgInt1:企业ID
#define YQTCP_WEBCREATEIAD     0x306 //(分布式)WEB建立IAD机柜，CALL进程操作，刷新内存机柜数据
#define YQTCP_SYNCBKSIVRAFILE  0x307 //ACDCFG请求ACD进行某个文件的BKSVR和所有ISVR之间的同步，MsgIntStr:文件全路径
#define YQTCP_ACDHTTPWEBMSG    0x308 //ACDHTTP发送消息到客户端
#define YQTCP_IVRCORPNEWORDEL  0x309 //ACDCFG请求在所有IVR上创建或者企业目录,MsgInt1:企业ID,MsgInt2=1：建立，MsgInt2=2:删除

//公共消息 0x500
#define YQTCP_TEST              0x500  //测试,服务端每五秒发一次,五秒内未收到YQTCP_TESTACK关闭连接
#define YQTCP_TESTACK           0x501
#define YQTCP_OPEN              0x502 //客户端到服务端Int1:用户ID,Str=用户名
#define YQTCP_CLOSE             0x503 //收到此消息后再发YQTCP_CLOSE并关闭连接,Int1=永久关闭
#define YQTCP_STREAMSENDSTART   0x504
#define YQTCP_STREAMRECECMPL    0x505
#define YQTCP_ALWTEST           0x506  //允许发送测试信息
#define YQTCP_INHTEST           0x507  //禁止发送测试信息
#define YQTCP_TIMEDATA          0x508  //客户端同步服务器时间,Str=double型的时间
#define YQTCP_SENDDBSTREAM      0x509  //发送数据流,Int1=数量
#define YQTCP_SYNCRESUL         0x50A  //发送数据流,Int1=结果,后续无流
#define YQTCP_STREAMCMPL        0x50B  //流结束,作为接收端较验消息
#define YQTCP_SQL				0x50C  //发送SQL语句，Int1=Action,SQL在流中
										//响应：YQTCP_SENDDBSTREAM
#define YQTCP_GETFILE			0x50D  //读取指定文件,带绝对路径的文件名在后续流中
										//响应：YQTCP_SENDDBSTREAM,int1=-1文件不存在
#define YQTCP_VERSION			0x50E  //会话版本号,接收到版本号与本端设置不一致时中断连接
#define YQTCP_ACCTABLE			0x50F  //通过MIDAS访问数据库表
#define YQTCP_REDIRECT			0x510  //登录重定向,Int1=Port,MsgStr=IPAdr



//收传真服务端 0x600
#define YQTCP_NEWFAX            0x600   //含新传真数目
//短信服务端 0xA00
#define YQ_SM_CHECK		0xA04 //查询结果  MsgInt1：状态 0-成功，1-正在发送，2-待发送
                                     //          MsgInt2：短信重发次数  MsgStr:SMID 短信编号
//网络分机服务端 0xC00
#define YQTCP_GETALLFJST        0xC00   //在收到OPEN时发出,MsgInt1=分机号,-1结束
#define YQTCP_SENDSYSVAR        0xC01   //发送分机功系统变量,MsgInt1=顺序号,-1结束，MsgInt1=整型值，MsgStr=字符串值
#define YQTCP_SENDFJDATA        0xC02   //发送分机数据，MsgInt1=SEQNO,-1结束，MsgInt2=SERVICE，MsgStr=分机号
#define YQTCP_LOCKFJ            0xC04   //挂掉分机,Int1-Addr,Int2=0临时挂断，1永久挂断，2接续
//网络分机客户端 0xC80
#define YQTCP_SENDALLFJST       0xC80   //在收到OPEN时发出,MsgInt1=分机号,-1结束
#define YQTCP_FJST              0xC81   //Int1-Addr, Int2-State
#define YQTCP_GETSYSVAR         0xC84   //获取分机功系统变量,MsgInt1=顺序号,-1结束
#define YQTCP_GETFJDATA         0xC82   //获取分机数据，MsgInt1=SEQNO,-1结束，MsgInt2=SERVICE，MsgStr=分机号
#define YQTCP_FJRINGTIMEOUT     0xC83   //分机振铃超时,MsgStr=分机号
#define YQTCP_ANLDIALOUT        0xC85   //分析拨出号码
#define YQTCP_BILL              0xC86   //发送帐单:Int1=时间1,Int2=时间2
#define YQTCP_LINECOUNT         0xC88   //发送帐单:Int1=线数

//告警管理服务端 0xD00
#define YQTCP_SENDALMDATA       0xD00   //通过流发送告警数据,Int1=0-分机,1-设备,2-转GETALMST流程,Int2=记录数
#define YQTCP_SENDALMEQST       0xD01   //设备状态，Int1=ID,Int2Lo=-2停止(主动发送状态时用)，-1结束换下一个设备状态，>=0顺序号,Int2Hi=状态
#define YQTCP_SENDALMFJST       0xD02   //分机状态，Int1=分机号,Int2Lo=-2停止(主动发送状态时用)，-1结束换下一个设备状态，>=0顺序号,Int2Hi=状态
#define YQTCP_SENDALMATNDST     0xD03   //分机状态，Int1=分机号,Int2Lo=-2停止(主动发送状态时用)，-1结束换下一个设备状态，>=0顺序号,Int2Hi=状态
#define YQTCP_ALMRESTART        0xD20   //流程重新开始
#define YQTCP_ALMSENDCALLER     0xD21   //发送来电号,Int1=分机号,Int2=来电号码长度,Str=来电号
//告警管理客户端 0xD80
#define YQTCP_GETALMDATA        0xD80   //读取告警数据，Int1=0-设备，1-分机
#define YQTCP_GETALMST          0xD81   //读取设备告警状态, Int1=0读设备，1读线路，2话务台，Int2=顺序号




//API 0xE00
#define YQTCP_API		0xE00	//广播消息: 发送主叫号码,MsgStr[0~15]分机号,MsgStr[16~47]来电号
//数字中继服务端 0xF01
#define SWTAB_COUNT             7
#define SWTAB_VOICELEV_MAX      5
#define SWTAB_ONETABLE          -2
#define SWTAB_NOTABLE           -1
#define SWTAB_SUBNUMBER         0
#define SWTAB_VOICELEV_1        1
#define SWTAB_VOICELEV_2        2
#define SWTAB_VOICELEV_3        3
#define SWTAB_VOICELEV_4        4
#define SWTAB_VOICELEV_5        SWTAB_VOICELEV_MAX
#define SWTAB_SYSVAR            SWTAB_VOICELEV_MAX + 1

#define SWTCP_SVR               0xF00
#define SWTCP_SENDDB            0xF01   //发送数据库数据,Int1=表名,Int2=顺序号,多表时与表名一致,单表时为-2
//数字中继客户端 0xF81
#define SWTCP_CLN               0xF80
#define SWTCP_GETDB             0xF81   //取数据库数据,Int1=表名,SWTAB_NOTABLE停止

#define SWTCP_CHMSG             0xFA1   //发送接续内容,Int1=EQID,Int2_Lo=CHID,Int2_Hi=Col
        #define CHMSG_ST        4
        #define CHMSG_CALLER    5
        #define CHMSG_CALLED    6
        #define CHMSG_DTMF      7

//在线升级消息
#define YQUPG_START		0x1000
//中间层API消息
#define YQMIDL_START		0x1100
//呼叫控制API消息
#define YQCC_START		0x2000
//egotom API消息
#define EGT_START		0x3000
//座席卡消息
#define YQAC_START		0x4000

typedef struct _TYQTcpMsg
{
 int Flow;
 int MsgInt1;
 int MsgInt2;
 char MsgStr[48];
} TYQTcpMsg;
//---------------------------------------------------------------------------
typedef struct _TYQBill
{
        char ZJ[13];
        char BJ[35];
        double SJ;
        int THSC;
        float FY;
        char JB;
        char JBMC[15];
} TYQBill;
//---------------------------------------------------------------------------
//API发送短信流
typedef struct _TApiSM
{
 char IP[20];
 char Dest[81];
 char Message[129];
 unsigned short DestCount;
 double SendTime;
} TApiSM;
//---------------------------------------------------------------------------
//API接收短信流
typedef struct _TApiRecSM
{
  char Sender[32];
  char SendTime[32];
  char Msg[128];
} TApiRecSM;
//---------------------------------------------------------------------------
//API传送用户及部门流
typedef struct _TUser
{
 int ID;
 char Name[20];
} TUser;
//---------------------------------------------------------------------------
//API接收传真流
typedef struct _TRecFaxInfo
{
  	int FaxID;
        char Remarks[100];
	char SendTime[32];
        char Calling[24];
        char Caller[16];
} TRecFaxInfo;
//---------------------------------------------------------------------------

typedef struct _TAtndSvrMsg
{
	int CorpID;         //企业ID
    int UserID;         //用户ID
    bool P2PMsg;        //点对点发送标志
    TYQTcpMsg YQTcpMsg; //具体消息
} TAtndSvrMsg;
//III代ACD与ACD上应用层消息
typedef struct _TAtndSvrMsgEx
{
	int CorpID;         //企业ID
    char SipUser[32];   //III代MTSC的SIP帐号
    bool P2PMsg;        //点对点发送标志
    TYQTcpMsg YQTcpMsg; //具体消息
} TAtndSvrMsgEx;

#endif
