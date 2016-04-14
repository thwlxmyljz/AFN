#ifndef YQMSGTYPEH
#define YQMSGTYPEH
//进程类，用于进程间通信
#define	PSTYP_ALL	0
#define PSTYP_DBA	1
#define PSTYP_CALL	2



//进程间通讯命令字
#define UPDATE_SQL    5
#define INSERT_SM     6
#define INSERT_VMAILE 7
#define UPDATE_SENDSM 8
#define MG_REGISTER   9
#define UPDATE_TIMER           10
#define UPDATE_CONFDATA        11 //刷新会议数据
#define UPDATE_CONFDATARESULT  12 //刷新会议数据结果
#define DELE_CONFDATA          13 //删除会议数据
#define SEND_INST_MSG          14 //发送即时消息
#define ACD_SEND_SM            15 //ACD发送短信
#define ACD_AUTOTIME 1000

#endif
