#ifndef Api_H
#define Api_H

// ������̬��
extern "C" int YQOpen(void* LogFunc, int MsgQueneID, void *Userdata);

// �رն�̬��
extern "C" void YQClose();

// MG���̻�ÿ��10ms���ô˺���
extern "C" void YQLoop();

// ��ȡģ����
extern "C" const char * YQGetName();

// ��ȡ�汾��
extern "C" const char * YQVersion();

#endif

