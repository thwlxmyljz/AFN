//�ֽ��򣬵�λ��ǰ����λ�ں󣬵��ֽ���ǰ�����ֽ��ں�,��С�˴洢,����x86ƽ̨����
//�ն˵�¼ץ����   68 32 00 32 00 68 c9 00 10 4d 04 00 02 75 00 00 01 00 a2 16
//EXE���Ե�¼������68 32 00 32 00 68 49 00 00 00 00 02 02 62 00 00 01 00 B0 16
/*
---------------------------------------------
| ��ʼ�ַ�(0x68) |
---------------
| ����L(2�ֽ�)   |
---------------            �̶���ͷ����(6�ֽ�)
| ����L(2�ֽ�)   |          L��ʾ�û�����������
---------------
| ��ʼ�ַ�(0x68) |
----------------------------------------------
| ������C(1�ֽڣ�	|
---------------
| ��ַ��A(5�ֽ�)	|          �û�������(CSУ����)
---------------
| ��·�û�����	|
----------------------------------------------
| У���CS		|
---------------
| �����ַ�E(0x16)	|
----------------------------------------------
*/
#ifndef __PROTO_H__
#define __PROTO_H__

#include "YQDefs.h"

#pragma pack(1) //1�ֽڶ���

#define PKG_HEADLEN 6 //�̶���ͷ6�ֽڳ���
#define PKG_USER_HEADLEN 6 //�û�������ͷ6�ֽڳ���
#define PKG_TAILLEN 2 //��β2�ֽڳ���

#define PKG_AFN_HEADLEN 2 //AFNӦ��������ͷ2�ֽڳ���
#define PKG_AFN_DATATAGLEN 4 //AFNӦ�����������ݱ�ʶ����4�ֽ�(DA,DT)

class Pkg_Header{
public:
	/*
	�̶����Ȱ�ͷ,6BYTE
	*/
	BYTE S1;//��ʼ�ַ�0x68
	/*
	������+��ַ��+�û����ݳ���ֵ
	L,L1:����D0-D15��λ˵��
	D0=0,D1=0:����
	D0=1,D1=0:Q/GDW 130-2005ʹ��
	D0=0,D1=1:��Э��ʹ��
	D0=1,D1=1:����
	D2-D15:������+��ַ��+�û����ݳ���ֵ,BIN����
	*/
	union {
		WORD l;
		struct {
			WORD TAG:2;
			WORD LEN:14;		
		} _L;//����1
	} L;
	union {
		WORD l1;
		struct {
			WORD TAG:2;
			WORD LEN:14;
		} _L1;//����2
	} L1;

	BYTE S2;//��ʼ�ַ�0x68
};
/*
�û���������ͷ,����C,A����
*/
class Pkg_User_Header{
public:		
	/*
	C:������,��λ˵������
		D7:���䷽��λDIR,DIR=0���У�DIR=1����
		D6:������ʶλPRM,RPM=1��ʾ��֡������������վ,PRM=0��ʾ��֡�������ԴӶ�վ
		D5:���б�ʾ֡����λFCB�����б�ʾ�������λACD
		D4:���б�ʾ֡������ЧλFCV,���б���δ��
			֡����λFCV,FCB˵����
				FCV=1,��ʾFCBλ��Ч��FCV=0����ʾFCBλ��Ч
				��֡������ЧλFCV=1ʱ��FCB��ʾÿ��վ�����ķ���/ȷ�ϻ�������/��Ӧ����ı仯λ��FCBλ������ֹ��Ϣ����Ķ�ʧ���ظ�
				����վ��ͬһ�Ӷ�վ�����µķ���/ȷ�ϻ�������/��Ӧ�������ʱ����FCBȡ�෴ֵ������վ����ÿ���Ӷ�վ��FCBֵ������ʱδ
				�յ��Ӷ�վ�ı��ģ����߽��ճ��ֲ��������վ���ı�FCB��״̬���ظ�ԭ���ķ���/ȷ�ϻ�������/��Ӧ����
			�������λACD˵����
				������������Ӧ�����У���վ������
				ACD=1,��ʾ�ն�����Ҫ�¼��ȴ����ʣ��򸽼���Ϣ���д����¼�������EC(EC˵������)
				ACD=0,��ʾ�ն�����Ҫ�¼��ȴ�����
				�ն����ù���
				���ϴ��յ����ĺ����µ���Ҫ�¼���ACD��λ1
				�յ���վ�����¼���ִ����ɺ�ACD��λ0
				
		D0-D3:������

			PRM=1ʱ��������������վ��
			
			ֵ			֡����			������
			0			����
			1			����/ȷ��		��λ����
			2-3			����
			4			����/�޻ش�		�û�����
			5-8			����
			9			������Ӧ֡		��·����
			10			������Ӧ֡		����1������
			11			������Ӧ֡		����2������
			12-15		����
			
			PRM=0ʱ���������ԴӶ�վ��
			
			ֵ			֡����			������
			0			ȷ��				�Ͽ�
			1-7			����
			8			��Ӧ֡			�û�����
			9			��Ӧ֡			���ϣ������ٻ�������
			10			����
			11			��Ӧ֡			��·״̬
			12-15		����
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
	/*FUNC����*/
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
	A:��ַ��,����A1,A2,A3
		A1:��������,������ִ��,2�ֽ�
		A2���ն˵�ַ��2�ֽڣ�=0��Ч��=0xFFFF,��A3��D0λ���壬A3(D0=1)��ʾϵͳ�㲥��ַ
		A3:��վ��ַ���ն����ַ��ʶ��1�ֽڣ�D0=0��ʾA2Ϊ����ַ��D0=1��ʾA2λ�鲥��ַ��D1-D7���0-127����վ��ַMSA
			1����վ�����ķ���֡��MSAӦΪ��0ֵ�����ն���Ӧ֡��MSA����վ��ͬ
			2���ն������ķ���֡��MSAΪ0,��վ��Ӧ֡��MSAҲҪ����Ϊ0
	*/
	WORD A1;//��ַ����������(BCD����)
	WORD A2;//��ַ���ն˵�ַ(BIN)
	union {
		BYTE a3;
		struct {
			BYTE TAG:1;
			BYTE MSA:7;
		} _A3;//��ַ����վ��ַ���ն����ַ��ʶ(BIN)
	} A3;
};
/*
��β
*/
class Pkg_Tail{
public:	
	BYTE CS;//�û�������У���,��8λ��������,���������
	BYTE E;//�����ַ�0x16
};
/*
Ӧ�ò�������ͷ
*/
class Pkg_Afn_Header{
public:
	/*
	������
	AFNֵ		���ܶ���
	0x00		ȷ��/����
	0x01		��λ
	0x02		��·�ӿڼ��
	0x03		�м�վ����
	0x04		���ò���
	0x05		��������
	0x06		�����֤����ԿЭ��
	0x07		����
	0x08		���󱻼����ն������ϱ�
	0x09		�����ն�����
	0x0A		��ѯ����
	0x0B		������������
	0x0C		����1������(ʵʱ����)
	0x0D		����2������(��ʷ����)
	0x0E		����3������(�¼�����)
	0x0F		�ļ�����
	0x10		����ת��
	0x11-0xFF	����

	AFN=0x02ʱ
		F1:��¼
		F2:�˳���¼
		F3:����
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
	���к�
	D7:ʱ���ǩ��ЧλTPV,=1������Ϣ�����ʱ�����=0��ʱ���
	D6:��֡��ʶFIR,��һ֡
	D5:ĩ֡��ʶFIN,���һ֡,(D5,D6)���ʹ�ã����±�
		D6	D5	˵��
		0	0	��֡���м�֡
		0	1	��֡������֡
		1	0	��֡����һ֡���к���֡
		1	1	��֡
	D4:����ȷ�ϱ�ʶλCON��=1��ʾ�յ����ĺ���Ҫ�Ըñ���ȷ�ϣ�=0��ȷ��
	D0~D3:����֡���PSEQ/��Ӧ֡���RSEQ
	����:	
	*/
	union {
		BYTE seq;
		struct {
			//PSEQʱΪPFC��4λ,ȡֵ(0~15)��RSEQʱ��ʼΪPSEQ���Ժ����1,��������յ�RSEQ��ͬ��֡���򲻴��������յ�PSEQ�����ط���Ӧ
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
���ݵ�Ԫ��ʶ
*/
class Pkg_Afn_DataTag{
public:
	/*
	DA���ݵ�Ԫ��ʶ,2�ֽ�
	DA1,DA2����ʾ8*255=2040����Ϣ�㣬��Ϊpn,(n=[0~2040]),p0��ʾ�ն���Ϣ��,p1��ʼ��ʾȷ����Ϣ��
	DA1=0xff,DA2=0x00��ʾ������Ч������(������p0)
	*/
	BYTE DA1;//���ݵ�Ԫ��ʶDA1����λ��ʾÿ����Ϣ����ģ�0-8������Ϣ��Ԫ
	BYTE DA2;//���ݵ�Ԫ��ʶDA2����ֵ��0-255����ʾ��Ϣ����
	
	/*
	DT���ݵ�Ԫ��ʶ,2�ֽ�
	DT1,DT2����ʾ8*31=248����Ϣ���ͣ���ΪFn,(n=[1~248]),�ն�������ʱ,DTλ��0������������ʱ����������ȱʧ������д0xEE	
	*/
	BYTE DT1;//���ݵ�Ԫ��ʶDT1,��λ��ʾÿ����Ϣ�����(0-8��)��Ϣ��Ԫ
	BYTE DT2;//���ݵ�Ԫ��ʶDT2,��ֵ(ֻȡ0-31)��ʾ��Ϣ����
};
/*
���ݵ�Ԫ�ӿ�
*/
class Pkg_UserDataInterface{
public:
	//���
	virtual void unPackData(BYTE* _data,DWORD _len) = 0;
	//���,���ط���ֽ���
	virtual int PackData(BYTE* _data,DWORD _len) = 0;
	//���ݳ���
	virtual int GetDataLen() = 0;
};
/*
���ݵ�Ԫ
*/
class Pkg_Afn_Data : public Pkg_UserDataInterface {
	friend class Pkg_Afn;
	friend class Pkg;

public:
	//DA,DT,4�ֽڵ�Ԫ��ʶ
	Pkg_Afn_DataTag m_Tag;

	//��Ԫ��������
	BYTE* m_pData;
	DWORD m_nLen;

public:
	Pkg_Afn_Data();
	virtual ~Pkg_Afn_Data();
	Pkg_Afn_Data(BYTE* _data,DWORD _len);

	//���ݴ���
	virtual int HandleData();

	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
	{
		return PKG_AFN_DATATAGLEN+m_nLen;
	}
};
/*
������Ϣ��,��������
*/
class Pkg_Afn_Aux : public Pkg_UserDataInterface{
};
class Pkg_Afn_Aux_Up : public Pkg_Afn_Aux {
public:
	//����,EC,2�ֽ�
	struct {
		BYTE EC1;//�¼�������EC1����Ҫ�¼�������,��Χ0~255��,����,ACD=1ʱ��Ч
		BYTE EC2;//�¼�������EC2��һ���¼�������,��Χ0~255��,����,ACD=1ʱ��Ч
	} EC;
	//ʱ���,6�ֽ�
	struct {
		BYTE PFC;//����֡������PFC,BIN����,����վÿ����1֡������+1����0��255ѭ�����ӣ��ط�֡����PFC
		BYTE TM[4];//����֡����ʱ��,��¼����֡���͵�ʱ��,��λ���ʱ��
		BYTE DELAY;//�����ʹ�����ʱʱ��,��λ:����,BIN����
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
	//��Ϣ��֤��PW,���з���,����վ�㷨���㣬��������ݣ����ն˱�����֤�����ݣ�ͨ������Ӧ���������Ӧ����
	BYTE PW[16];
	//ʱ���,6�ֽ�
	struct {
		BYTE PFC;//ʱ���TPV(����֡������PFC,BIN����)
		BYTE TM[4];//ʱ���TPV(����֡����ʱ��,��¼����֡���͵�ʱ��,��λ���ʱ��)
		BYTE DELAY;//ʱ���TPV(�����ʹ�����ʱʱ��,��λ:����,BIN����)
	} TP;
	virtual void unPackData(BYTE* _data,DWORD _len);
	virtual int PackData(BYTE* _data,DWORD _len);
	virtual int GetDataLen()
	{
		return 22;
	}
};
/*
Ӧ������������ʽ
*/
class Pkg_Afn {
public:
	/*
	������+SEQ,2�ֽ�
	*/
	Pkg_Afn_Header afnHeader;	
	/*
	Ӧ�����ݵ�Ԫ����
	��p0-p2040����,ÿ��pn�ڰ�F1-F248����
	�ն���Ӧ����ʱ���ĳ������������DT�Ķ�Ӧ��ʶλ��0(Fn=0),ĳ�����ݲ���ȱʧ��ȱʧ���ݲ�����0xEE��
	*/
	Pkg_Afn_Data *pAfnData;
	/*
	������Ϣ��
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
������
*/
class Pkg {
public:
	/*��ͷ*/
	Pkg_Header pkgHeader;
	/*�û�����ͷ*/
	Pkg_User_Header userHeader;
	/*Ӧ������*/
	Pkg_Afn* pAfn;
	/*��β*/
	Pkg_Tail pkgTail;
};
#endif