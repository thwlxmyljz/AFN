//---------------------------------------------------------------------------
#ifndef YQResourceH
#define YQResourceH
//---------------------------------------------------------------------------
#define YQRES_NONERES			0x0000		// ����Դ
#define YQRES_AUDIO_CAPS		0x0001		// ��Ƶ�ɼ��豸
#define YQRES_AUDIO_OUT			0x0002		// ��Ƶ����豸
#define YQRES_VIDEO_CAPS		0x0003		// ͼ��ɼ��豸
#define YQRES_VIDEO_OUT			0x0004		// ͼ�����
#define YQRES_WAV_PLAY			0x0005		// ���ⲥ��wav�����ļ�
#define YQRES_WAV_RECORD		0x0006		// ¼���յ�������wav�ļ�
#define YQRES_DTMFCODE			0x0007		// DTMF��
#define YQRES_REC_PLAY			0x0008		// ����¼�������������еĲ���
#define YQRES_VOLUME_TEST		0x0009		// ��Ƶ�ɼ�������豸����

#define YQRES_RTPVOICE			0x0010		// RTP����
#define YQRES_RTPVIDEO			0x0020		// RTP��Ƶ

#define YQRES_RTPPROXY			0x0030		// RTP����

#define YQRES_RTPVCCONF			0x0040		// ����RTP����
#define YQRES_RTPVDCONF			0x0050		// ��ƵRTP����

#define YQRES_RECORDDIR			0x0060		// ¼��������¼��Ŀ¼
#define YQRES_AUDIO_RECSVR		0x0061		// ��Ƶ¼�Ʒ�����
#define YQRES_VIDEO_RECSVR		0x0062		// ��Ƶ¼�Ʒ�����

#define YQRES_UDTF				0x0070		//  UDT����ܵ�
#define YQRES_UDTFILE			0x0071		//  ��������ļ�
#define YQRES_SDPFILE			0x0072		//  ��������ļ������SDP

#define YQRES_UPDATE			0x0080		//  ���µ�ǰ�Ự����Դ

#define YQRES_T38FAX			0x0090		//  T.38����

#define YQRES_CARDCH			0x0100		// �����м�
#define YQRES_CARDFAX			0x0200		// ���ഫ��
#define YQRES_CARDCONF			0x0300		// �������

typedef struct _YQRes
{
	int Type;
    int ID;
    union
    {
        char Param[56];
        struct
        {
        	int IntPam;
            char StrPam[52];
		};
	

        struct
        {
        	char IP[32];
            unsigned short Port;
            union
            {
            	int PayloadType;
				int Payloads[5];
			};
		
		};   
    };
} YQRes;

typedef YQRes* LPYQRes;
typedef const YQRes* LPCYQRes;

enum
{
	yq_a_alaw = 8,
	yq_v_h263 = 98
};
//---------------------------------------------------------------------------
#endif
