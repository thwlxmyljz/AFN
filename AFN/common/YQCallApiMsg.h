/****************************************************************************
公司名称：深圳市亿桥科技有限公司
版权申明：版权所有 (C) 2008
单元名称：YQCallApiMsg.h
文件标识： 
创建时间：2008-3-24 18:42:06
作    者：陈群祥
版    本：V1.0
摘    要：所有网络模块的公共消息与结构

当前版本：V1.1
修改者：  唐宏文
修改日期：持续更新中...
修改摘要：在原始版上增加
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef YQCallApiMsgH
#define YQCallApiMsgH
#include "YQTcpMsg.h"
//---------------------------------------------------------------------------
//进程间通信ID
#define	PSTYP_ALL	  0
#define PSTYP_DBA	  1
#define PSTYP_CALL	  2
#define PSTYP_ACDCFG  3 //ACDCFG模块消息
#define PSTYP_ACDHTTP 4 //ACDHTTP模块消息
#define PSTYP_WATCHDOG 5 //WatchDog模块消息
#define PSTYP_SYNC     6 //同步进程
#define PSTYP_ADM	1314
//---------------------------------------------------------------------------
//进程间通信消息ID
#define PROC_MSG_HEART_BEAT			0
#define PROC_MSG_CFG_REFRESH		1
//---------------------------------------------------------------------------
//SIP部分
#define YQTCP_GETUSERSTATUS     0x800   // 查询用户状态
#define YQTCP_SENDUSERSTATUS    0x801   // ACD主动告诉用户状态
#define YQTCP_DELNHDFILE        0x802   // 删除网络硬盘文件
#define YQTCP_UIVERSION         0x803   // 取服务器支持的客户端版本
//---------------------------------------------------------------------------
//CALL和DB之间通讯用
#define YQTCP_BILLDATA             0x860    // 生成话单
#define YQTCP_NEWORDER             0x861    // 新订单
//CALL和DB之间通讯用END
//看门狗程序和同步进程间消息
#define YQTCP_SWAP               0x900 //由看门程序发来的发生倒换事件
#define YQTCP_SYNCDATA           0x901 //需要同步数据给备用ACD
#define YQTCP_REQUESTBATCHSYNC   0x902 //请求批量同步数
#define YQTCP_WEBSWAP            0x903 //由网管程序发来的发生倒换事件
#define YQTCP_WEBDBSWAP          0x904 // 由网管程序发来的数据库倒换
#define YQTCP_RESTORESYNCDATA    0x905 //恢复同步数据

//网管MTSA与EGT交互
#define YQTCP_MTSASNDONECORP    0x9FC   //网管发送某个企业的信息
#define YQTCP_MTSACORPINFO      0x9FD   //获取到网管MTSA的管理企业的详细信息，保存到EGT
#define YQTCP_MTSAORGINFO       0x9FE   //获取到网管MTSA的组织结构信息，保存到EGT
#define YQTCP_MTSAONLINE        0x9FF   //网管连接上EGT，接下来EGT开始获取网管内相关数据
//MTSC与会议(Media服务器\Record服务器)的交互消息
#define YQTCP_DELRECFILE        0xAEC   //Record:删除某个录制文件
#define YQTCP_ReplayEnd         0xAED   //Record:某个录制回放完成
#define YQTCP_SetRecPlayCurTime 0xAEE   //Record:设置某个录制回放的当前时间
#define YQTCP_GetRecPlayCurTime 0xAEF   //Record:获取某个录制回放的当前时间
#define YQTCP_StopRec           0xAF0   //Record:停止某个录制的回放
#define YQTCP_PlayRec           0xAF1   //Record:开始回放某个录制
#define YQTCP_GetReplayRes      0xAF2   //Record:获取回放录制的端口资源
#define YQTCP_ReportAVInfo      0xAF3   //Media:音视频资源汇报,Record:报告最小端口与资源数量
#define YQTCP_ClearLink         0xAF4   //(Media与Record共用)清除连接
#define YQTCP_LinkOneToAnother  0xAF5   //(Media与Record共用)设置2个资源的连接
#define YQTCP_SetLink           0xAF6   //(Media与Record共用)设置2个资源的连接
#define YQTCP_GetLinkCount      0xAF7   //获取某个会议的资源连接数
#define YQTCP_GetConfCount      0xAF8   //获取当前会议数量
#define YQTCP_ReleaseConf       0xAF9   //Media:销毁会议,Record:销毁一个录制
#define YQTCP_CreateConf        0xAFA   //Media:创建会议,Record:创建一个录制
#define YQTCP_GetSupportRes     0xAFB   //获取某会议服务器支持的资源
#define YQTCP_SetVideoConf      0xAFC   //某会议服务器的视频资源汇报
#define YQTCP_SetAudioConf      0xAFD   //某会议服务器的音频资源汇报
//SQL语句转发
#define YQTCP_SQLSTREAM         0xAFE   //SQL语句流，转发到客户端
//VOIP消息
#define YQTCP_VOIP              0xAFF   //VOIP消息，需转发
//情景交互
#define YQTCP_QJCOUNT           0xBFA   //本地用户获取自己的情景个数
#define YQTCP_QJGET             0xBFB   //获取一个情景
#define YQTCP_QJDELETE          0xBFC   //删除情景连接
#define YQTCP_QJCANCEL          0xBFD   //取消情景
#define YQTCP_QJSEND            0xBFE   //给对方发送一个情景
#define YQTCP_QJCALLUSER        0xBFF   //当对方MTSC主机不是本地MTSC主机，开始一个情景交互，
                                        //在连接到对方MTSC后，通知对方，让对方连接到我的MTSC主机
//---------------------------------------------------------------------------
//话务台MTSC与客户端MTSU交互
#define YQTCP_WEBMSG            0xCE2   //306版之后的WEB的所有表通知消息到客户端
#define YQTCP_GETID2            0xCE3   //从话务台上获取ASYSVAR某个变量ID,获取的类型由MsgStr决定
#define YQTCP_GETID             0xCE4   //从话务台上获取CSYSVAR某个变量ID,获取的类型由MsgStr决定
#define YQTCP_IMMSG             0xCE5   //发送即时消息[插入到短信,作为短信送到客户端]
#define YQTCP_SETCORPID         0xCE6   //设置企业ID[用于支持企业总机],Int1=CorpID
#define YQTCP_SETAT				0xCE7   //设置话务员状态,Int1=UserID, Int2Lo=1:签入,2:工作,4:示闲,In2Hi=0:否,1:是
#define YQTCP_CONFUSERSTATE     0xCE8   //会议用户的状态改变
#define YQTCP_CONFENDALM2       0xCE9   //会议到结束第二次告警
#define YQTCP_CONFENDALM1       0xCEA   //会议到结束第一次告警
#define YQTCP_SETCHAIRMAN       0xCEB   //设置主持人
#define YQTCP_CONFDELETEPREUSER 0xCEC   //删除某个会议的预订用户
#define YQTCP_CONFREQUESTUSER   0xCED   //给某个会议预订用户
#define YQTCP_RECORDINGOFFLINE  0xCEE   //一个录制结束
#define YQTCP_RECORDINGONLINE   0xCEF   //新建立一个录制
#define YQTCP_GETRECORDERINFO   0xCF0   //获取所有正在录制信息
#define YQTCP_CONFUSEROUT       0xCF1   //某个用户退出会议
#define YQTCP_CONFUSERIN        0xCF2   //某个用户进入会议
#define YQTCP_KICKUSER          0xCF3   //踢掉某个会议用户
#define YQTCP_GETCONFUSERS      0xCF4   //获取会议的用户信息，MsgInt2=0:在线用户信息,MsgInt2=1:预订用户信息
#define YQTCP_GETCENTREXINFO    0xCF5   //获取话务台的一些配置信息
#define YQTCP_UPCONFINFO        0xCF6   //更新某个会议信息
#define YQTCP_GETCONFINFO       0xCF7   //获取会议信息
#define YQTCP_DESTROYCONF       0xCF8   //销毁一个会议
#define YQTCP_CREATECONF        0xCF9   //创建一个会议
#define YQTCP_REQUESTCONF       0xCFA   //请求一个会议
#define YQTCP_GETBACKUPFILES    0xCFB   //mtsu->mtsc表示客户端请求备份MTSC关键数据，mtsc->mtsu表示客户端收到备份状态
#define YQTCP_GETFJSTATE        0xCFC   //MTSU获取分机状态
#define YQTCP_APICARDCHANGE     0xCFD   //名片修改通知话务台
	//与YQSwitch1.0兼容
#define YQTCP_APIIPCALLDROP     0xCFE   //VOIP处理相关，MTSC要求客户端挂断
#define YQTCP_APIIPCALLIN       0xCFF   //VOIP处理相关，MTSC接收到呼叫，通知到客户端
//---------------------------------------------------------------------------
//话务台MTSC与中心MTSA交互
#define YQTCP_MTSADBX           0xDEA    //网管切换DB主备用
#define YQTCP_MTSAACDX          0xDEB   //网管切换ACD主备用
#define YQTCP_DBONLINE          0xDEC   //mtsc报告某个数据库上线
#define YQTCP_ATNDMSGTOAT       0xDED   //MTSC发送TAtndSvrMsg消息到AT服务端，放在流内发送
#define YQTCP_SETATHOOKOF       0xDEE   //设置AT服务端某个AT话务员的状态
#define YQTCP_GETATCLIENT       0xDEF   //从AT服务端获取一个空闲的AT话务员
#define YQTCP_UPLOAD_IMMSG      0xDF0   //网管YQNWADM或者WEBAPI发送即时消息给某个企业
#define YQTCP_UPLOAD_LOGO       0xDF1   //话务台上传公司LOGO到网管YQDB
#define YQTCP_DOWNLOAD_FILE     0xDF2   //从话务台上获取某个文件 
#define YQTCP_SENDOUTPORT1      0xDF3   //发送话务台出口端口号到中心，这里发送了WEB访问MTSC的端口号 
#define YQTCP_ALARMMSG          0xDF4   //话务台告警消息
#define YQTCP_UPLOAD_VOICELEVM  0xDF5   //上传语音树改动数据----目前改成SQL语句，没有使用----
#define YQTCP_CLIENTSHOWMONWND  0xDF6   //客户端显示或者隐藏通道监视窗口
#define YQTCP_CENTREXDOWNLINE   0xDF7   //话务台离线消息          
#define YQTCP_SYNCAIDLINKCORP   0xDF8   //同步WEB(企业话务台绑定表)与话务台(内部企业表)
#define YQTCP_GETCORPSYSVAR     0xDF9   //获取系统变量----目前改成SQL语句，没有使用----
#define YQTCP_SETCORPSYSVAR     0xDFA   //设置系统变量----目前改成SQL语句，没有使用----
#define YQTCP_UPLOAD_FILE       0xDFB   //上传某文件到话务台
#define YQTCP_SETCHANNELLINK    0xDFC   //设置话务台某通道的连接企业
#define YQTCP_GETCHANNELLINK    0xDFD   //获取话务台某通道的连接企业!!!目前改成SQL语句，没有使用!!!
#define YQTCP_GETCHANNELCOUNT   0xDFE   //获取话务台通道数
#define YQTCP_SENDOUTPORT       0xDFF   //发送话务台出口端口号到中心，这里发送了API和UDP侦听端口号
//---------------------------------------------------------------------------
#define YQTCP_SENDCID		    0xE01	//广播消息: 发送主叫号码,MsgStr[0~15]分机号,MsgStr[16~47]来电号
#define YQTCP_APIFJST           0xE02   //广播消息: Int1=State,Int2=顺序号,-1无效 MsgStr=分机号
#define YQTCP_APISENDBILL       0xE03   //发送话单,Int1=话单数,话单通过流的方式发送
#define YQTCP_APISENDPROP       0xE04   //发送属性,Int1=特服;Int2=权限,Str=分机号码,空值表示分机号码不正确
#define YQTCP_APISETRESUL       0xE05   //设置结果,Int2=0失败,1成功,-1分机号码不存在,MsgStr=分机号码
#define YQTCP_APISENDFJDATA     0xE06   //通过流发送所有分机数据,Int1=记录数
//---------------------------------------------------------------------------
//发送短信
#define YQTCP_APISENDSMID       0xE07   //发送短信ID,Int1=SMID
#define YQTCP_APISENDSMST       0xE08   //发送短信状态,Int1=-1 ID不存在，>-1 SMID,Int2=Lo:CLBZ,Hi:CSCX
//---------------------------------------------------------------------------
//帐号
#define YQTCP_APISNDUSER        0xE09   //发送用户名称及密码,Int1=UserID,Str[0~15]=名称，Str[17~32]密码
#define YQTCP_APISNDUSERCOUNT   0xE0A   //发送用户数,流发送用户内容
#define YQTCP_APISETUSERRESUL   0xE0B   //Int1=-1 ID不存在,0=OK
//---------------------------------------------------------------------------
//接收短信
#define YQTCP_APINEWSM          0xE0D   //收到新短信,Int1=用户ID
//---------------------------------------------------------------------------
//发传真
#define YQTCP_APISENDFAXST      0xE10   //发送传真状态,Int1=FaxSubID,Int2=状态
//---------------------------------------------------------------------------
//接收传真
#define YQTCP_APINEWFAX         0xE20   //收到新传真,Int1=用户ID
//---------------------------------------------------------------------------
//API客户端 0xE80
#define YQTCP_APILOCKFJ         0xE80   //挂掉分机,Int1=0临时挂断，1永久挂断，2接续
#define YQTCP_APIGETFJST        0xE81   //获取分机状态,Int2=顺序号,-1无效
#define YQTCP_APIGETBILL        0xE82   //获取取话单,Int2=0按时段,1分机新话单,2所有新话单,MsgStr[0~15]:分号号串,MsgStr[16]开始时间,MsgStr[24]结束时间
#define YQTCP_APIGETPROP        0xE83   //获取属性,MsgStr=分机号码
#define YQTCP_APISETSERVICE     0xE84   //获取特服,Int1=特服;Int2=标志:0取消,1激活,Str=分机号
#define YQTCP_APISETCLASS       0xE85   //设置特服,Int1=级别;Str=分机号
#define YQTCP_APIGETFJDATA      0xE86   //获取所有分机数据
//---------------------------------------------------------------------------
//发送短信
#define YQTCP_APISENDSM         0xE87   //发送短信,号码及内容在流中,Int1=UserID
#define YQTCP_APIGETSENDSMST    0xE88   //获取发送短信状态,Int1=SMID
#define YQTCP_APISMCANCEL       0xE89   //获取发送短信状态,Int1=SMID
//---------------------------------------------------------------------------
//帐号
#define YQTCP_APIGETUSER        0xE8A   //获取用户名称及密码,Int1=UserID
#define YQTCP_APIGETALLUSER     0xE8B   //获取用户ID及名称
#define YQTCP_APISETUSER        0xE8C   //设置用户名称及密码,Int1=UserID,Str[0~15]=名称，Str[17~32]密码
#define YQTCP_APIGETALLDEP      0xE8D   //获取所有部门ID及名称
//---------------------------------------------------------------------------
//接收短信
#define YQTCP_APIGETRECSM       0xE8E   //取UserID的所有未收到的短信,Int1=UserID
#define YQTCP_APIGETALLRECSM    0xE8F   //收取所有用户的新短信
//---------------------------------------------------------------------------
//短信功能交互
#define YQTCP_STARTSMVR         0xE90   //启动与停止SM交互
#define YQTCP_SENDSMVR          0xE91   //服务器收到交互短信转发给客户端
//---------------------------------------------------------------------------
//接收传真
#define YQTCP_APIGETRECFAX      0xEA0   //取UserID的所有未收到的传真,Int1=UserID
#define YQTCP_APIRECFAX         0xEA1   //取FaxID的传真,Int1=FaxID
#define YQTCP_APIFAXTOCORP      0xEA2   //传真转发给本公司,用流发送
//---------------------------------------------------------------------------
//发传真
#define YQTCP_APISENDFAX        0xEB0   //发传真，Int1=Flag,Int2=(UserID or FaxID,) Str[0~7] = 时间, Str[8~47]号码
                                        //Flag=1后续发送文件流
#define YQTCP_APIGETSENDFAXST   0xEB1   //发传真状态，Int1=SubID
#define YQTCP_APIDELSENDFAX     0xEB2   //取消传真发送,Int1=FAXID
#define YQTCP_APISENDFAXREDIAL  0xEB3   //重拨,Int1=SubID
#define YQTCP_APISENDFAXSTREAM	0xEB4   //通话中发送传真,Int1=CallID,Int2=AlwRetry,内容在流中 
//---------------------------------------------------------------------------
//接收语音留言
//#define YQTCP_APINEWVOICE       0xEC0   //服务器通知客户端有新语音留言
#define YQTCP_APIGETRECVOICE    0xEC1   //客户端向服务器发出获取新语音留言信息的请求（数据库保存的信息）
#define YQTCP_APIRECVOICE       0xEC2   //客户端获取服务器上的某条新语音留言的具体语音数据
#define YQTCP_RECVOICEOK        0xEC3   //客户端接收某条语音留言成功，通知服务器
//---------------------------------------------------------------------------
//数据库数据同步(WEB与话务台)
#define YQTCP_SYNC_DATABASE     0xED0   //同步
#define YQTCP_LOAD_DATABASE     0xED1   //请求某个数据下载
#define YQTCP_UPLOAD_DATABASE   0xED2   //上传来某个数据
#define YQTCP_CORPCONNECTED     0xED3   //有话务台连接上来
#define YQTCP_ONLY_MSG          0xED4   //话务台仅发送的是一个通知消息
#define YQTCP_CENTREXOUTPORT    0xED5   //话务台发送自己的对外连接端口号给中心
#define YQTCP_UPLOAD_ASYS       0xED6   //话务台发送系统变量表的WEB地址与出口端口到中心
//---------------------------------------------------------------------------
#define YQTCP_SYNC_YH            0xEF0      //同步用户表
#define YQTCP_SYNC_BMB           0xEF1      //同步部门表
#define YQTCP_SYNC_BMYH          0xEF2      //同步部门用户表
//---------------------------------------------------------------------------
#define YQTCP_UPLOAD_FAX         0xEF3      //上传来FAX表数据
#define YQTCP_UPLOAD_VOICE       0xEF4      //上传来语音邮件数据
#define YQTCP_UPLOAD_PHONE       0xEF5      //上传来话务统计数据
//---------------------------------------------------------------------------
#define YQTCP_MSG_UPLOADFAX      0xEF6      //WEB中心通知话务台需要上传话务台传真数据，在话务台刚连上WEB中心时，中心发送给话务台的消息
#define YQTCP_MSG_UPLOADVOICE    0xEF7      //WEB中心通知话务台需要上传话务台语音邮件数据，在话务台刚连上WEB中心时，中心发送给话务台的消息
#define YQTCP_MSG_NEEDPHONE      0xEF8
//---------------------------------------------------------------------------
#define YQTCP_UPLOAD_BOOKDIR     0xEF9      //上传上来企业通讯录(表AD_BOOKDIR)
#define YQTCP_UPLOAD_BOOKTYPE    0xEFA      //上传上来企业通讯录(表AD_BOOKTYPE)
#define YQTCP_UPLOAD_INFOCLASS   0xEFB      //上传上来企业通讯录(表AD_INFOCLASS)
#define YQTCP_UPLOAD_INFOTYPE    0xEFC      //上传上来企业通讯录(表AD_INFOTYPE)
#define YQTCP_UPLOAD_MUCHINFO    0xEFD      //上传上来企业通讯录(表AD_MUCHINFO)
#define YQTCP_UPLOAD_PERSON      0xEFE      //上传上来企业通讯录(表AD_PERSON)
#define YQTCP_UPLOAD_PERSONLINK  0xEFF      //上传上来企业通讯录(表AD_PERSONLINK)
//---------------------------------------------------------------------------
//客户端管理系统加入(2008-07:E3x已用户完)
#define YQTCP_APILOGIN          0xE30   //登录,Int1=UserID,Str密码
#define YQTCP_APIMKBUSY         0xE31   //话机置忙,Int1=IsBusy,Int2=Type,Str=号码
#define YQTCP_APIMKCALL         0xE32   //发起呼叫,Int1=UserID,Int2=Type,Str[0~15]:分机号，Str[16~47]被叫号
#define YQTCP_APIMKCBK          0xE33   //发起回叫,内容在流中，Int1=语音文件长度,Int2=语音级别，Stream=带路径语音文件
#define YQTCP_APICTF            0xE34   //话务台来话转移,Int1=Channel, Int2=DestType, Str=目的号
#define YQTCP_NEWVMAIL          0xE35   //收到新语音邮件,Int1=UserID
#define YQTCP_APIGETTB			0xE36   //接收数据库的表流,Int1=表名(1:分机号码表,2:连选群表,3:部门表,4:用户表)
#define YQTCP_APISENDTB			0xE37   //发送数据库的表流,Int1=表名(同上)
#define YQTCP_APIIVRINI			0xE38   //IVR,Int1:1启动,0退出
#define YQTCP_APIIVR			0xE39   //IVR
#define YQTCP_APINETMON			0xE3A   //网络远程监听
#define YQTCP_APICLINREC		0xE3B   //来电记录生成通知,Int1=ID,STR=分机号
#define YQTCP_LINESTATE			0xE3C   //线路状态
#define YQTCP_LINESPERCARD      0xE3D   //话务台每板卡的线数
#define YQTCP_APIMKCBKFJ        0xE3E   //先呼外线再呼分机,Str[0~15]:分机号，Str[16~47]外部号
#define YQTCP_CENTREXINFO       0xE3F   //话务台信息，比如板卡数，每板卡通道数等
//(2008-03-17 ChenQX added)
//---------------------------------------------------------------------------
//IP呼叫
#define YQTCP_IPMKCALL       	0xE40   //INT1=CALLID/USERID,INT2=TYPE+PORT,STR=[0~23]DN+[24~47]IP
#define YQTCP_IPALERT       	0xE41   //INT1=CALLID,INT2=PORT
#define YQTCP_IPANSWER       	0xE42   //INT1=CALLID
#define YQTCP_IPRELEASE   	    0xE43   //INT1=CALLID，INT2=REASION
#define YQTCP_IPUPDATE       	0xE44   //INT1=CALLID，INT2_LO=PORT,INT2_HI=0语音；2视频,Str=IP地址
#define YQTCP_MEDIASVRDISCON    0xE45   //媒体服务器与话务台MTSC断开消息
//---------------------------------------------------------------------------
//语音留言错误代码
#define YQTCP_ERRORCODE_SYSVAREMPTY       (0-0x1)   //服务器没有保存语音留言的目录，数据库被破坏
#define YQTCP_ERRORCODE_NOSUCHFILE        (0-0x3)   //服务器没有此语音文件
#define YQTCP_ERRORCODE_STREAMERROR       (0-0x4)   //协议交互过程出错
#define YQTCP_ERRORCODE_NOMEMORY          (0-0x5)   //内存不足
#define YQTCP_ERRORCODE_NOTCONNCECTED     (0-0x6)   //没有和服务器建立连接
#define YQTCP_ERRORCODE_WRITELOCALFILE    (0-0x7)   //不能写本地文件
#define YQTCP_ERRORCODE_PCMTOWAV          (0-0x8)   //服务器不能转换PCM文件成WAV
#define YQTCP_ERRORCODE_NONE                 0x1    //成功
//---------------------------------------------------------------------------
//话务台连接错误代码
#define YQTCP_ERRORCODE_CORPIDINVALID        (0-0x4)  //企业ID错误，不能找到对应话务台
#define YQTCP_ERRORCODE_CENTREXDISCONNECTED  (0-0xA)  //企业ID对应话务台没有连接到中心
//---------------------------------------------------------------------------
//语音的状态
#define TTS_STATUS_NOTHANDL 0   //不用提交
#define TTS_STATUS_HANDLING 1   //新加的，需要上传到中心
#define TTS_STATUS_OK       2   //审批通过了
#define TTS_STATUS_FAILED   3   //没有审批通过
#define TTS_STATUS_USING    4   //已生成，使用中
#define TTS_UPLOADOK        5   //已上传
//---------------------------------------------------------------------------

#define ALMCLASS_CENTREX    1   //话务台
#define ALMCLASS_BOARD      2   //板卡
/*通道告警时，TYQTcpMsg.MsgStr 为CR_TAMEM表中TAGRP,TAMEM,
* 都为整数,有效值为二个整数*/
#define ALMCLASS_CHANNEL    3   //通道
/*MG告警时.TYQTcpMsg.MsgStr 为CR_TAGRP表中TAGRP,有效值为1个整数*/
#define ALMCLASS_MG         4   //MG
/*ACD告警时，TYQTcpMsg.MsgStr 为AID，和ACDNO TYPE=0备用。其它为主用*/
#define ALMCLASS_ACD        5
#define ALMCLASS_DB         6
//---------------------------------------------------------------------------
//告警具体
#define ALM_SMDLG               0xFFF0        //外挂系统状态
#define ALM_BORD		        0x0000        //电路板告警
#define ALM_LINE		        0x0010        //电路板上的线路告警
#define ALM_LINE_B0		        0x0010        //0电路板上的线路告警
#define ALM_LINE_B1		        0x0011        //1电路板上的线路告警
#define ALM_LINE_B2		        0x0012        //2电路板上的线路告警
#define ALM_LINE_B3		        0x0013        //3电路板上的线路告警
#define ALM_LINE_B4		        0x0014        //4电路板上的线路告警
#define ALM_LINE_B5		        0x0015        //5电路板上的线路告警
#define ALM_LINE_B6		        0x0016        //6电路板上的线路告警
#define ALM_LINE_B7		        0x0017        //7电路板上的线路告警
#define ALM_LINE_B8		        0x0018        //8电路板上的线路告警
#define ALM_LINE_B9		        0x0019        //9电路板上的线路告警
#define ALM_LINE_BA		        0x001A        //10电路板上的线路告警
#define ALM_LINE_BB		        0x001B        //11电路板上的线路告警
#define ALM_LINE_BC		        0x001C        //12电路板上的线路告警
#define ALM_LINE_BD		        0x001D        //13电路板上的线路告警
#define ALM_LINE_BE		        0x001E        //14电路板上的线路告警
#define ALM_LINE_BF		        0x001F        //15电路板上的线路告警


//---------------------------------------------------------------------------
//通道告警值
#define LINE_OK                 0x0000            //通道正常
#define LINE_FAILED             0x0001            //通道故障


//---------------------------------------------------------------------------
#define MAXFILENAMELEN 14  //语音邮件标识文件名最大长度
#define MAXFJHLEN 20       //分机号最大长度
#define MAXGHLEN  12       //工号最大长度
//---------------------------------------------------------------------------
//申请会议的请求信息字段长度
#define CONFPWDLEN 64      //会议密码最大长度
#define CONFMAXLEN 128     //会议描述最大长度
#define SUBNUMBERLEN 20    //分机号最大长度
#define USERINFOLEN 128    //会议用户标识最大长度
//---------------------------------------------------------------------------
//MTSU备份MTSC数据到本地
#define FileType_Mtsf 0     //MTS目录文件夹
#define FileType_Sys  1     //系统system32目录下的文件
#define FileType_Win  2     //系统WINDOWS目录下的文件
#define FileType_Mts  3     //MTSC目录下的文件
#define FileType_DB   4     //数据库数据
//---------------------------------------------------------------------------
//会议预订用户呼叫状态，失败的状态都是负数
#define CONFUSERSTATE_NONE  0 //等待呼叫
#define CONFUSERSTATE_ING   1 //正在呼叫
#define CONFUSERSTATE_OK    2 //呼叫成功，等待加入会议
#define CONFUSERSTATE_OKOK  3 //呼叫成功，已加入会议
#define CONFUSERSTATE_OKNO  4 //呼叫成功，加入会议失败
#define CONFUSERSTATE_OVER  5 //用户呼叫结束
//---------------------------------------------------------------------------
//会议通知告警
#define CONF_ALM1      10         //会议快结束前10分钟的第一次告警
#define CONF_ALM2      5          //会议快结束前5分钟的第二次告警
#define CONF_ALMPLAY   3          //会议快结束前3分钟播放告警声音
#define USERINFOSPLIT  "$"        //会议用户之间的分隔符
//---------------------------------------------------------------------------
//请求会议类型
#define CONFTYPE_AUDIO 1  //请求音频会议
#define CONFTYPE_VIDEO 2  //请求视频会议
#define CONFTYPE_AV    3  //请求音视频会议

//---------------------------------------------------------------------------
//会议录制的标志
#define ARECORDUSERINFO "RecorderA"  //会议录制的音频录制标识
#define VRECORDUSERINFO "RecorderV"  //会议录制的视频录制标识
//---------------------------------------------------------------------------
//分机监控的分机用户登陆信息
typedef struct _TFJLoginInfo
{
    //分机号
    char szFjNumber[MAXFJHLEN];
    //已登陆的工号
    char szLoginGHID[MAXGHLEN];
    //状态信息
    int  nState; //1:在线，0：不在线
}TFJLoginInfo;
//---------------------------------------------------------------------------
//语音邮件标识
typedef struct _TRecVoiceInfo
{
    //In database table VMAILFILE,szFileName size is 14
    char szFileName[MAXFILENAMELEN];
    //In database table VMAILFILE,szSubNumber size is 20
    char szSubNumber[MAXFJHLEN];
}TRecVoiceInfo;
//---------------------------------------------------------------------------
//传真上传结构
typedef struct _TRecFaxInfoEx
{
    TRecFaxInfo FBaseInfo;
    unsigned long FdwUserID;
}TRecFaxInfoEx;
//---------------------------------------------------------------------------
//交互短信
typedef struct _TSMVR
{
    int FnFuncID;
    int FnStep;
    TApiRecSM FsmInfo;
}TSMVR;
//---------------------------------------------------------------------------
//通道表示
typedef union _TChannelShow
{
    int CHID;
    struct
    {
        short BordNo;
        short LineNo;
    }BordLine;
}TChannelShow;
//---------------------------------------------------------------------------
//线路信息
typedef struct _TChannelInfo
{
    //话务台AID
    int AID;
    //板卡号(short)+线路号(short)
    TChannelShow BORDINFO;
    //通道类型编号
    int  TypeName_Int;
    //通道类型名称
    char TypeName[12];
    //通道的名称
    char ChName[12];
    //硬件模块类型编号
    int  ModuleName_Int;
    //硬件模块类型名称
    char ModuleName[12];
    //是否激活
    short Enabled;
    //连接企业ID
    int CorpID;
}TChannelInfo;
//---------------------------------------------------------------------------
//话务台内部企业表
typedef struct _TAIDLinkCorp
{
    //话务台ID
    int AID;
    //连接的企业ID
    int CorpID;
    //连接的企业名字
    char Name[32];
}TAIDLinkCorp;
//---------------------------------------------------------------------------
//分机号码信息
typedef struct _SubNumber
{
    //分机短号
    char ShortNumber[20];
    //分机长号
    char LongNumber[20];
}SubNumber;
//---------------------------------------------------------------------------
//会议状态
typedef enum _EConfState
{
    EM_CONF_SLEEPING, //未启动
    EM_CONF_STARTERR, //启动失败
    EM_CONF_MEETING,  //正在进行中
    EM_CONF_PAUSED,   //中断
    EM_CONF_DESTROY   //需要销毁
} EConfState;
//---------------------------------------------------------------------------
//申请会议的请求信息
typedef struct _TConfInfoRequest
{
    //企业ID，此会议的所有者
    int CorpID;
    //用户ID，此会议的所有者
    int UserID;
    //开始时间
    double StartTime;
    //结束时间
    double EndTime;
    //最大用户数
    unsigned short MaxUser;
    //会议类型，指音视频
    unsigned short Type;//1:音频，2：视频，3：音视频
    //是否录音
    int Record; //1:录音，0：不录音
    //会议密码
    char Password[CONFPWDLEN];
    //其他描述
    char Remark[USERINFOLEN];
}TConfInfoRequest;
//---------------------------------------------------------------------------
//客户端获取到的会议信息
typedef struct _TConfInfoRequestEx
{
    //基础信息
    TConfInfoRequest BaseInfo;
    //会议主ID
    int MainID;
    //目前状态
    int State;
    //在线人数
    int OnlineUsers;
	//对齐,暂时不用
	int NoUse;
}TConfInfoRequestEx;
//---------------------------------------------------------------------------
//正在录制信息
typedef struct _TRecordingInfo
{
    //录制ID
    int FRecID;
    //企业ID
    int FCorpID;
    //录制开始时间
	double FStartTime;
    //录制的分机号
    char SubNumber[SUBNUMBERLEN];
    //是否正在被侦听
    bool IsListening;
}TRecordingInfo;
//---------------------------------------------------------------------------
#endif


