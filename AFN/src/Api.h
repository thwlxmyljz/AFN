#ifndef Api_H
#define Api_H

// 启动动态库
extern "C" int YQOpen(void* LogFunc, int MsgQueneID, void *Userdata);

// 关闭动态库
extern "C" void YQClose();

// MG进程会每隔10ms调用此函数
extern "C" void YQLoop();

// 获取模块名
extern "C" const char * YQGetName();

// 获取版本号
extern "C" const char * YQVersion();

#endif

