//---------------------------------------------------------------------------
#ifndef YQTcpMsgH
#define YQTcpMsgH
//---------------------------------------------------------------------------
//ACD��ACD���ý���\ACDHTTP����֮����Ϣ������Ϣ
#define YQTCP_ACDCFGCREATEDIR  0x300 //ACDCFG�����ļ�����������Ŀ¼,CALL���̲���,MsgStr:Ŀ¼��
#define YQTCP_ACDCFGDELETEDIR  0x301 //ACDCFG�����ļ�������ɾ��Ŀ¼,CALL���̲���,MsgStr:Ŀ¼��
#define YQTCP_ACDCHANGETAB     0x302 //ACDHTTP�޸ı�֪ͨ��CALL���̲���,MsgInt1:��ID
#define YQTCP_ACDHTTPSENDSM    0x303 //ACDHTTP���Ͷ��ţ�CALL���̲���,���͵��ⲿĿ�ĺ�,MsgInt1:��ҵID��MsgInt2:���ű��(���ݿ��ֶ�ΪDXBH)
#define YQTCP_ACDHTTPSENDIM    0x304 //ACDHTTP���ͼ�ʱ��Ϣ��CALL���̲���,֪ͨ����ҵ�ڲ���ӦSIP�ն�,MsgInt1:��ҵID��MsgInt2:��ʱ��Ϣ���(���ݿ��ֶ�ΪSMID)
#define YQTCP_WEBCREATECORP    0x305 //(�ֲ�ʽ)WEB������ҵ��ACDCFG������������ҵ��MsgInt1:��ҵID
#define YQTCP_WEBCREATEIAD     0x306 //(�ֲ�ʽ)WEB����IAD����CALL���̲�����ˢ���ڴ��������
#define YQTCP_SYNCBKSIVRAFILE  0x307 //ACDCFG����ACD����ĳ���ļ���BKSVR������ISVR֮���ͬ����MsgIntStr:�ļ�ȫ·��
#define YQTCP_ACDHTTPWEBMSG    0x308 //ACDHTTP������Ϣ���ͻ���
#define YQTCP_IVRCORPNEWORDEL  0x309 //ACDCFG����������IVR�ϴ���������ҵĿ¼,MsgInt1:��ҵID,MsgInt2=1��������MsgInt2=2:ɾ��

//������Ϣ 0x500
#define YQTCP_TEST              0x500  //����,�����ÿ���뷢һ��,������δ�յ�YQTCP_TESTACK�ر�����
#define YQTCP_TESTACK           0x501
#define YQTCP_OPEN              0x502 //�ͻ��˵������Int1:�û�ID,Str=�û���
#define YQTCP_CLOSE             0x503 //�յ�����Ϣ���ٷ�YQTCP_CLOSE���ر�����,Int1=���ùر�
#define YQTCP_STREAMSENDSTART   0x504
#define YQTCP_STREAMRECECMPL    0x505
#define YQTCP_ALWTEST           0x506  //�����Ͳ�����Ϣ
#define YQTCP_INHTEST           0x507  //��ֹ���Ͳ�����Ϣ
#define YQTCP_TIMEDATA          0x508  //�ͻ���ͬ��������ʱ��,Str=double�͵�ʱ��
#define YQTCP_SENDDBSTREAM      0x509  //����������,Int1=����
#define YQTCP_SYNCRESUL         0x50A  //����������,Int1=���,��������
#define YQTCP_STREAMCMPL        0x50B  //������,��Ϊ���ն˽�����Ϣ
#define YQTCP_SQL				0x50C  //����SQL��䣬Int1=Action,SQL������
										//��Ӧ��YQTCP_SENDDBSTREAM
#define YQTCP_GETFILE			0x50D  //��ȡָ���ļ�,������·�����ļ����ں�������
										//��Ӧ��YQTCP_SENDDBSTREAM,int1=-1�ļ�������
#define YQTCP_VERSION			0x50E  //�Ự�汾��,���յ��汾���뱾�����ò�һ��ʱ�ж�����
#define YQTCP_ACCTABLE			0x50F  //ͨ��MIDAS�������ݿ��
#define YQTCP_REDIRECT			0x510  //��¼�ض���,Int1=Port,MsgStr=IPAdr



//�մ������� 0x600
#define YQTCP_NEWFAX            0x600   //���´�����Ŀ
//���ŷ���� 0xA00
#define YQ_SM_CHECK		0xA04 //��ѯ���  MsgInt1��״̬ 0-�ɹ���1-���ڷ��ͣ�2-������
                                     //          MsgInt2�������ط�����  MsgStr:SMID ���ű��
//����ֻ������ 0xC00
#define YQTCP_GETALLFJST        0xC00   //���յ�OPENʱ����,MsgInt1=�ֻ���,-1����
#define YQTCP_SENDSYSVAR        0xC01   //���ͷֻ���ϵͳ����,MsgInt1=˳���,-1������MsgInt1=����ֵ��MsgStr=�ַ���ֵ
#define YQTCP_SENDFJDATA        0xC02   //���ͷֻ����ݣ�MsgInt1=SEQNO,-1������MsgInt2=SERVICE��MsgStr=�ֻ���
#define YQTCP_LOCKFJ            0xC04   //�ҵ��ֻ�,Int1-Addr,Int2=0��ʱ�Ҷϣ�1���ùҶϣ�2����
//����ֻ��ͻ��� 0xC80
#define YQTCP_SENDALLFJST       0xC80   //���յ�OPENʱ����,MsgInt1=�ֻ���,-1����
#define YQTCP_FJST              0xC81   //Int1-Addr, Int2-State
#define YQTCP_GETSYSVAR         0xC84   //��ȡ�ֻ���ϵͳ����,MsgInt1=˳���,-1����
#define YQTCP_GETFJDATA         0xC82   //��ȡ�ֻ����ݣ�MsgInt1=SEQNO,-1������MsgInt2=SERVICE��MsgStr=�ֻ���
#define YQTCP_FJRINGTIMEOUT     0xC83   //�ֻ����峬ʱ,MsgStr=�ֻ���
#define YQTCP_ANLDIALOUT        0xC85   //������������
#define YQTCP_BILL              0xC86   //�����ʵ�:Int1=ʱ��1,Int2=ʱ��2
#define YQTCP_LINECOUNT         0xC88   //�����ʵ�:Int1=����

//�澯�������� 0xD00
#define YQTCP_SENDALMDATA       0xD00   //ͨ�������͸澯����,Int1=0-�ֻ�,1-�豸,2-תGETALMST����,Int2=��¼��
#define YQTCP_SENDALMEQST       0xD01   //�豸״̬��Int1=ID,Int2Lo=-2ֹͣ(��������״̬ʱ��)��-1��������һ���豸״̬��>=0˳���,Int2Hi=״̬
#define YQTCP_SENDALMFJST       0xD02   //�ֻ�״̬��Int1=�ֻ���,Int2Lo=-2ֹͣ(��������״̬ʱ��)��-1��������һ���豸״̬��>=0˳���,Int2Hi=״̬
#define YQTCP_SENDALMATNDST     0xD03   //�ֻ�״̬��Int1=�ֻ���,Int2Lo=-2ֹͣ(��������״̬ʱ��)��-1��������һ���豸״̬��>=0˳���,Int2Hi=״̬
#define YQTCP_ALMRESTART        0xD20   //�������¿�ʼ
#define YQTCP_ALMSENDCALLER     0xD21   //���������,Int1=�ֻ���,Int2=������볤��,Str=�����
//�澯����ͻ��� 0xD80
#define YQTCP_GETALMDATA        0xD80   //��ȡ�澯���ݣ�Int1=0-�豸��1-�ֻ�
#define YQTCP_GETALMST          0xD81   //��ȡ�豸�澯״̬, Int1=0���豸��1����·��2����̨��Int2=˳���




//API 0xE00
#define YQTCP_API		0xE00	//�㲥��Ϣ: �������к���,MsgStr[0~15]�ֻ���,MsgStr[16~47]�����
//�����м̷���� 0xF01
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
#define SWTCP_SENDDB            0xF01   //�������ݿ�����,Int1=����,Int2=˳���,���ʱ�����һ��,����ʱΪ-2
//�����м̿ͻ��� 0xF81
#define SWTCP_CLN               0xF80
#define SWTCP_GETDB             0xF81   //ȡ���ݿ�����,Int1=����,SWTAB_NOTABLEֹͣ

#define SWTCP_CHMSG             0xFA1   //���ͽ�������,Int1=EQID,Int2_Lo=CHID,Int2_Hi=Col
        #define CHMSG_ST        4
        #define CHMSG_CALLER    5
        #define CHMSG_CALLED    6
        #define CHMSG_DTMF      7

//����������Ϣ
#define YQUPG_START		0x1000
//�м��API��Ϣ
#define YQMIDL_START		0x1100
//���п���API��Ϣ
#define YQCC_START		0x2000
//egotom API��Ϣ
#define EGT_START		0x3000
//��ϯ����Ϣ
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
//API���Ͷ�����
typedef struct _TApiSM
{
 char IP[20];
 char Dest[81];
 char Message[129];
 unsigned short DestCount;
 double SendTime;
} TApiSM;
//---------------------------------------------------------------------------
//API���ն�����
typedef struct _TApiRecSM
{
  char Sender[32];
  char SendTime[32];
  char Msg[128];
} TApiRecSM;
//---------------------------------------------------------------------------
//API�����û���������
typedef struct _TUser
{
 int ID;
 char Name[20];
} TUser;
//---------------------------------------------------------------------------
//API���մ�����
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
	int CorpID;         //��ҵID
    int UserID;         //�û�ID
    bool P2PMsg;        //��Ե㷢�ͱ�־
    TYQTcpMsg YQTcpMsg; //������Ϣ
} TAtndSvrMsg;
//III��ACD��ACD��Ӧ�ò���Ϣ
typedef struct _TAtndSvrMsgEx
{
	int CorpID;         //��ҵID
    char SipUser[32];   //III��MTSC��SIP�ʺ�
    bool P2PMsg;        //��Ե㷢�ͱ�־
    TYQTcpMsg YQTcpMsg; //������Ϣ
} TAtndSvrMsgEx;

#endif
