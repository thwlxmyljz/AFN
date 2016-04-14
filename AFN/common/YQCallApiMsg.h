/****************************************************************************
��˾���ƣ����������ſƼ����޹�˾
��Ȩ��������Ȩ���� (C) 2008
��Ԫ���ƣ�YQCallApiMsg.h
�ļ���ʶ�� 
����ʱ�䣺2008-3-24 18:42:06
��    �ߣ���Ⱥ��
��    ����V1.0
ժ    Ҫ����������ģ��Ĺ�����Ϣ��ṹ

��ǰ�汾��V1.1
�޸��ߣ�  �ƺ���
�޸����ڣ�����������...
�޸�ժҪ����ԭʼ��������
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef YQCallApiMsgH
#define YQCallApiMsgH
#include "YQTcpMsg.h"
//---------------------------------------------------------------------------
//���̼�ͨ��ID
#define	PSTYP_ALL	  0
#define PSTYP_DBA	  1
#define PSTYP_CALL	  2
#define PSTYP_ACDCFG  3 //ACDCFGģ����Ϣ
#define PSTYP_ACDHTTP 4 //ACDHTTPģ����Ϣ
#define PSTYP_WATCHDOG 5 //WatchDogģ����Ϣ
#define PSTYP_SYNC     6 //ͬ������
#define PSTYP_ADM	1314
//---------------------------------------------------------------------------
//���̼�ͨ����ϢID
#define PROC_MSG_HEART_BEAT			0
#define PROC_MSG_CFG_REFRESH		1
//---------------------------------------------------------------------------
//SIP����
#define YQTCP_GETUSERSTATUS     0x800   // ��ѯ�û�״̬
#define YQTCP_SENDUSERSTATUS    0x801   // ACD���������û�״̬
#define YQTCP_DELNHDFILE        0x802   // ɾ������Ӳ���ļ�
#define YQTCP_UIVERSION         0x803   // ȡ������֧�ֵĿͻ��˰汾
//---------------------------------------------------------------------------
//CALL��DB֮��ͨѶ��
#define YQTCP_BILLDATA             0x860    // ���ɻ���
#define YQTCP_NEWORDER             0x861    // �¶���
//CALL��DB֮��ͨѶ��END
//���Ź������ͬ�����̼���Ϣ
#define YQTCP_SWAP               0x900 //�ɿ��ų������ķ��������¼�
#define YQTCP_SYNCDATA           0x901 //��Ҫͬ�����ݸ�����ACD
#define YQTCP_REQUESTBATCHSYNC   0x902 //��������ͬ����
#define YQTCP_WEBSWAP            0x903 //�����ܳ������ķ��������¼�
#define YQTCP_WEBDBSWAP          0x904 // �����ܳ����������ݿ⵹��
#define YQTCP_RESTORESYNCDATA    0x905 //�ָ�ͬ������

//����MTSA��EGT����
#define YQTCP_MTSASNDONECORP    0x9FC   //���ܷ���ĳ����ҵ����Ϣ
#define YQTCP_MTSACORPINFO      0x9FD   //��ȡ������MTSA�Ĺ�����ҵ����ϸ��Ϣ�����浽EGT
#define YQTCP_MTSAORGINFO       0x9FE   //��ȡ������MTSA����֯�ṹ��Ϣ�����浽EGT
#define YQTCP_MTSAONLINE        0x9FF   //����������EGT��������EGT��ʼ��ȡ�������������
//MTSC�����(Media������\Record������)�Ľ�����Ϣ
#define YQTCP_DELRECFILE        0xAEC   //Record:ɾ��ĳ��¼���ļ�
#define YQTCP_ReplayEnd         0xAED   //Record:ĳ��¼�ƻط����
#define YQTCP_SetRecPlayCurTime 0xAEE   //Record:����ĳ��¼�ƻطŵĵ�ǰʱ��
#define YQTCP_GetRecPlayCurTime 0xAEF   //Record:��ȡĳ��¼�ƻطŵĵ�ǰʱ��
#define YQTCP_StopRec           0xAF0   //Record:ֹͣĳ��¼�ƵĻط�
#define YQTCP_PlayRec           0xAF1   //Record:��ʼ�ط�ĳ��¼��
#define YQTCP_GetReplayRes      0xAF2   //Record:��ȡ�ط�¼�ƵĶ˿���Դ
#define YQTCP_ReportAVInfo      0xAF3   //Media:����Ƶ��Դ�㱨,Record:������С�˿�����Դ����
#define YQTCP_ClearLink         0xAF4   //(Media��Record����)�������
#define YQTCP_LinkOneToAnother  0xAF5   //(Media��Record����)����2����Դ������
#define YQTCP_SetLink           0xAF6   //(Media��Record����)����2����Դ������
#define YQTCP_GetLinkCount      0xAF7   //��ȡĳ���������Դ������
#define YQTCP_GetConfCount      0xAF8   //��ȡ��ǰ��������
#define YQTCP_ReleaseConf       0xAF9   //Media:���ٻ���,Record:����һ��¼��
#define YQTCP_CreateConf        0xAFA   //Media:��������,Record:����һ��¼��
#define YQTCP_GetSupportRes     0xAFB   //��ȡĳ���������֧�ֵ���Դ
#define YQTCP_SetVideoConf      0xAFC   //ĳ�������������Ƶ��Դ�㱨
#define YQTCP_SetAudioConf      0xAFD   //ĳ�������������Ƶ��Դ�㱨
//SQL���ת��
#define YQTCP_SQLSTREAM         0xAFE   //SQL�������ת�����ͻ���
//VOIP��Ϣ
#define YQTCP_VOIP              0xAFF   //VOIP��Ϣ����ת��
//�龰����
#define YQTCP_QJCOUNT           0xBFA   //�����û���ȡ�Լ����龰����
#define YQTCP_QJGET             0xBFB   //��ȡһ���龰
#define YQTCP_QJDELETE          0xBFC   //ɾ���龰����
#define YQTCP_QJCANCEL          0xBFD   //ȡ���龰
#define YQTCP_QJSEND            0xBFE   //���Է�����һ���龰
#define YQTCP_QJCALLUSER        0xBFF   //���Է�MTSC�������Ǳ���MTSC��������ʼһ���龰������
                                        //�����ӵ��Է�MTSC��֪ͨ�Է����öԷ����ӵ��ҵ�MTSC����
//---------------------------------------------------------------------------
//����̨MTSC��ͻ���MTSU����
#define YQTCP_WEBMSG            0xCE2   //306��֮���WEB�����б�֪ͨ��Ϣ���ͻ���
#define YQTCP_GETID2            0xCE3   //�ӻ���̨�ϻ�ȡASYSVARĳ������ID,��ȡ��������MsgStr����
#define YQTCP_GETID             0xCE4   //�ӻ���̨�ϻ�ȡCSYSVARĳ������ID,��ȡ��������MsgStr����
#define YQTCP_IMMSG             0xCE5   //���ͼ�ʱ��Ϣ[���뵽����,��Ϊ�����͵��ͻ���]
#define YQTCP_SETCORPID         0xCE6   //������ҵID[����֧����ҵ�ܻ�],Int1=CorpID
#define YQTCP_SETAT				0xCE7   //���û���Ա״̬,Int1=UserID, Int2Lo=1:ǩ��,2:����,4:ʾ��,In2Hi=0:��,1:��
#define YQTCP_CONFUSERSTATE     0xCE8   //�����û���״̬�ı�
#define YQTCP_CONFENDALM2       0xCE9   //���鵽�����ڶ��θ澯
#define YQTCP_CONFENDALM1       0xCEA   //���鵽������һ�θ澯
#define YQTCP_SETCHAIRMAN       0xCEB   //����������
#define YQTCP_CONFDELETEPREUSER 0xCEC   //ɾ��ĳ�������Ԥ���û�
#define YQTCP_CONFREQUESTUSER   0xCED   //��ĳ������Ԥ���û�
#define YQTCP_RECORDINGOFFLINE  0xCEE   //һ��¼�ƽ���
#define YQTCP_RECORDINGONLINE   0xCEF   //�½���һ��¼��
#define YQTCP_GETRECORDERINFO   0xCF0   //��ȡ��������¼����Ϣ
#define YQTCP_CONFUSEROUT       0xCF1   //ĳ���û��˳�����
#define YQTCP_CONFUSERIN        0xCF2   //ĳ���û��������
#define YQTCP_KICKUSER          0xCF3   //�ߵ�ĳ�������û�
#define YQTCP_GETCONFUSERS      0xCF4   //��ȡ������û���Ϣ��MsgInt2=0:�����û���Ϣ,MsgInt2=1:Ԥ���û���Ϣ
#define YQTCP_GETCENTREXINFO    0xCF5   //��ȡ����̨��һЩ������Ϣ
#define YQTCP_UPCONFINFO        0xCF6   //����ĳ��������Ϣ
#define YQTCP_GETCONFINFO       0xCF7   //��ȡ������Ϣ
#define YQTCP_DESTROYCONF       0xCF8   //����һ������
#define YQTCP_CREATECONF        0xCF9   //����һ������
#define YQTCP_REQUESTCONF       0xCFA   //����һ������
#define YQTCP_GETBACKUPFILES    0xCFB   //mtsu->mtsc��ʾ�ͻ������󱸷�MTSC�ؼ����ݣ�mtsc->mtsu��ʾ�ͻ����յ�����״̬
#define YQTCP_GETFJSTATE        0xCFC   //MTSU��ȡ�ֻ�״̬
#define YQTCP_APICARDCHANGE     0xCFD   //��Ƭ�޸�֪ͨ����̨
	//��YQSwitch1.0����
#define YQTCP_APIIPCALLDROP     0xCFE   //VOIP������أ�MTSCҪ��ͻ��˹Ҷ�
#define YQTCP_APIIPCALLIN       0xCFF   //VOIP������أ�MTSC���յ����У�֪ͨ���ͻ���
//---------------------------------------------------------------------------
//����̨MTSC������MTSA����
#define YQTCP_MTSADBX           0xDEA    //�����л�DB������
#define YQTCP_MTSAACDX          0xDEB   //�����л�ACD������
#define YQTCP_DBONLINE          0xDEC   //mtsc����ĳ�����ݿ�����
#define YQTCP_ATNDMSGTOAT       0xDED   //MTSC����TAtndSvrMsg��Ϣ��AT����ˣ��������ڷ���
#define YQTCP_SETATHOOKOF       0xDEE   //����AT�����ĳ��AT����Ա��״̬
#define YQTCP_GETATCLIENT       0xDEF   //��AT����˻�ȡһ�����е�AT����Ա
#define YQTCP_UPLOAD_IMMSG      0xDF0   //����YQNWADM����WEBAPI���ͼ�ʱ��Ϣ��ĳ����ҵ
#define YQTCP_UPLOAD_LOGO       0xDF1   //����̨�ϴ���˾LOGO������YQDB
#define YQTCP_DOWNLOAD_FILE     0xDF2   //�ӻ���̨�ϻ�ȡĳ���ļ� 
#define YQTCP_SENDOUTPORT1      0xDF3   //���ͻ���̨���ڶ˿ںŵ����ģ����﷢����WEB����MTSC�Ķ˿ں� 
#define YQTCP_ALARMMSG          0xDF4   //����̨�澯��Ϣ
#define YQTCP_UPLOAD_VOICELEVM  0xDF5   //�ϴ��������Ķ�����----Ŀǰ�ĳ�SQL��䣬û��ʹ��----
#define YQTCP_CLIENTSHOWMONWND  0xDF6   //�ͻ�����ʾ��������ͨ�����Ӵ���
#define YQTCP_CENTREXDOWNLINE   0xDF7   //����̨������Ϣ          
#define YQTCP_SYNCAIDLINKCORP   0xDF8   //ͬ��WEB(��ҵ����̨�󶨱�)�뻰��̨(�ڲ���ҵ��)
#define YQTCP_GETCORPSYSVAR     0xDF9   //��ȡϵͳ����----Ŀǰ�ĳ�SQL��䣬û��ʹ��----
#define YQTCP_SETCORPSYSVAR     0xDFA   //����ϵͳ����----Ŀǰ�ĳ�SQL��䣬û��ʹ��----
#define YQTCP_UPLOAD_FILE       0xDFB   //�ϴ�ĳ�ļ�������̨
#define YQTCP_SETCHANNELLINK    0xDFC   //���û���̨ĳͨ����������ҵ
#define YQTCP_GETCHANNELLINK    0xDFD   //��ȡ����̨ĳͨ����������ҵ!!!Ŀǰ�ĳ�SQL��䣬û��ʹ��!!!
#define YQTCP_GETCHANNELCOUNT   0xDFE   //��ȡ����̨ͨ����
#define YQTCP_SENDOUTPORT       0xDFF   //���ͻ���̨���ڶ˿ںŵ����ģ����﷢����API��UDP�����˿ں�
//---------------------------------------------------------------------------
#define YQTCP_SENDCID		    0xE01	//�㲥��Ϣ: �������к���,MsgStr[0~15]�ֻ���,MsgStr[16~47]�����
#define YQTCP_APIFJST           0xE02   //�㲥��Ϣ: Int1=State,Int2=˳���,-1��Ч MsgStr=�ֻ���
#define YQTCP_APISENDBILL       0xE03   //���ͻ���,Int1=������,����ͨ�����ķ�ʽ����
#define YQTCP_APISENDPROP       0xE04   //��������,Int1=�ط�;Int2=Ȩ��,Str=�ֻ�����,��ֵ��ʾ�ֻ����벻��ȷ
#define YQTCP_APISETRESUL       0xE05   //���ý��,Int2=0ʧ��,1�ɹ�,-1�ֻ����벻����,MsgStr=�ֻ�����
#define YQTCP_APISENDFJDATA     0xE06   //ͨ�����������зֻ�����,Int1=��¼��
//---------------------------------------------------------------------------
//���Ͷ���
#define YQTCP_APISENDSMID       0xE07   //���Ͷ���ID,Int1=SMID
#define YQTCP_APISENDSMST       0xE08   //���Ͷ���״̬,Int1=-1 ID�����ڣ�>-1 SMID,Int2=Lo:CLBZ,Hi:CSCX
//---------------------------------------------------------------------------
//�ʺ�
#define YQTCP_APISNDUSER        0xE09   //�����û����Ƽ�����,Int1=UserID,Str[0~15]=���ƣ�Str[17~32]����
#define YQTCP_APISNDUSERCOUNT   0xE0A   //�����û���,�������û�����
#define YQTCP_APISETUSERRESUL   0xE0B   //Int1=-1 ID������,0=OK
//---------------------------------------------------------------------------
//���ն���
#define YQTCP_APINEWSM          0xE0D   //�յ��¶���,Int1=�û�ID
//---------------------------------------------------------------------------
//������
#define YQTCP_APISENDFAXST      0xE10   //���ʹ���״̬,Int1=FaxSubID,Int2=״̬
//---------------------------------------------------------------------------
//���մ���
#define YQTCP_APINEWFAX         0xE20   //�յ��´���,Int1=�û�ID
//---------------------------------------------------------------------------
//API�ͻ��� 0xE80
#define YQTCP_APILOCKFJ         0xE80   //�ҵ��ֻ�,Int1=0��ʱ�Ҷϣ�1���ùҶϣ�2����
#define YQTCP_APIGETFJST        0xE81   //��ȡ�ֻ�״̬,Int2=˳���,-1��Ч
#define YQTCP_APIGETBILL        0xE82   //��ȡȡ����,Int2=0��ʱ��,1�ֻ��»���,2�����»���,MsgStr[0~15]:�ֺźŴ�,MsgStr[16]��ʼʱ��,MsgStr[24]����ʱ��
#define YQTCP_APIGETPROP        0xE83   //��ȡ����,MsgStr=�ֻ�����
#define YQTCP_APISETSERVICE     0xE84   //��ȡ�ط�,Int1=�ط�;Int2=��־:0ȡ��,1����,Str=�ֻ���
#define YQTCP_APISETCLASS       0xE85   //�����ط�,Int1=����;Str=�ֻ���
#define YQTCP_APIGETFJDATA      0xE86   //��ȡ���зֻ�����
//---------------------------------------------------------------------------
//���Ͷ���
#define YQTCP_APISENDSM         0xE87   //���Ͷ���,���뼰����������,Int1=UserID
#define YQTCP_APIGETSENDSMST    0xE88   //��ȡ���Ͷ���״̬,Int1=SMID
#define YQTCP_APISMCANCEL       0xE89   //��ȡ���Ͷ���״̬,Int1=SMID
//---------------------------------------------------------------------------
//�ʺ�
#define YQTCP_APIGETUSER        0xE8A   //��ȡ�û����Ƽ�����,Int1=UserID
#define YQTCP_APIGETALLUSER     0xE8B   //��ȡ�û�ID������
#define YQTCP_APISETUSER        0xE8C   //�����û����Ƽ�����,Int1=UserID,Str[0~15]=���ƣ�Str[17~32]����
#define YQTCP_APIGETALLDEP      0xE8D   //��ȡ���в���ID������
//---------------------------------------------------------------------------
//���ն���
#define YQTCP_APIGETRECSM       0xE8E   //ȡUserID������δ�յ��Ķ���,Int1=UserID
#define YQTCP_APIGETALLRECSM    0xE8F   //��ȡ�����û����¶���
//---------------------------------------------------------------------------
//���Ź��ܽ���
#define YQTCP_STARTSMVR         0xE90   //������ֹͣSM����
#define YQTCP_SENDSMVR          0xE91   //�������յ���������ת�����ͻ���
//---------------------------------------------------------------------------
//���մ���
#define YQTCP_APIGETRECFAX      0xEA0   //ȡUserID������δ�յ��Ĵ���,Int1=UserID
#define YQTCP_APIRECFAX         0xEA1   //ȡFaxID�Ĵ���,Int1=FaxID
#define YQTCP_APIFAXTOCORP      0xEA2   //����ת��������˾,��������
//---------------------------------------------------------------------------
//������
#define YQTCP_APISENDFAX        0xEB0   //�����棬Int1=Flag,Int2=(UserID or FaxID,) Str[0~7] = ʱ��, Str[8~47]����
                                        //Flag=1���������ļ���
#define YQTCP_APIGETSENDFAXST   0xEB1   //������״̬��Int1=SubID
#define YQTCP_APIDELSENDFAX     0xEB2   //ȡ�����淢��,Int1=FAXID
#define YQTCP_APISENDFAXREDIAL  0xEB3   //�ز�,Int1=SubID
#define YQTCP_APISENDFAXSTREAM	0xEB4   //ͨ���з��ʹ���,Int1=CallID,Int2=AlwRetry,���������� 
//---------------------------------------------------------------------------
//������������
//#define YQTCP_APINEWVOICE       0xEC0   //������֪ͨ�ͻ���������������
#define YQTCP_APIGETRECVOICE    0xEC1   //�ͻ����������������ȡ������������Ϣ���������ݿⱣ�����Ϣ��
#define YQTCP_APIRECVOICE       0xEC2   //�ͻ��˻�ȡ�������ϵ�ĳ�����������Եľ�����������
#define YQTCP_RECVOICEOK        0xEC3   //�ͻ��˽���ĳ���������Գɹ���֪ͨ������
//---------------------------------------------------------------------------
//���ݿ�����ͬ��(WEB�뻰��̨)
#define YQTCP_SYNC_DATABASE     0xED0   //ͬ��
#define YQTCP_LOAD_DATABASE     0xED1   //����ĳ����������
#define YQTCP_UPLOAD_DATABASE   0xED2   //�ϴ���ĳ������
#define YQTCP_CORPCONNECTED     0xED3   //�л���̨��������
#define YQTCP_ONLY_MSG          0xED4   //����̨�����͵���һ��֪ͨ��Ϣ
#define YQTCP_CENTREXOUTPORT    0xED5   //����̨�����Լ��Ķ������Ӷ˿ںŸ�����
#define YQTCP_UPLOAD_ASYS       0xED6   //����̨����ϵͳ�������WEB��ַ����ڶ˿ڵ�����
//---------------------------------------------------------------------------
#define YQTCP_SYNC_YH            0xEF0      //ͬ���û���
#define YQTCP_SYNC_BMB           0xEF1      //ͬ�����ű�
#define YQTCP_SYNC_BMYH          0xEF2      //ͬ�������û���
//---------------------------------------------------------------------------
#define YQTCP_UPLOAD_FAX         0xEF3      //�ϴ���FAX������
#define YQTCP_UPLOAD_VOICE       0xEF4      //�ϴ��������ʼ�����
#define YQTCP_UPLOAD_PHONE       0xEF5      //�ϴ�������ͳ������
//---------------------------------------------------------------------------
#define YQTCP_MSG_UPLOADFAX      0xEF6      //WEB����֪ͨ����̨��Ҫ�ϴ�����̨�������ݣ��ڻ���̨������WEB����ʱ�����ķ��͸�����̨����Ϣ
#define YQTCP_MSG_UPLOADVOICE    0xEF7      //WEB����֪ͨ����̨��Ҫ�ϴ�����̨�����ʼ����ݣ��ڻ���̨������WEB����ʱ�����ķ��͸�����̨����Ϣ
#define YQTCP_MSG_NEEDPHONE      0xEF8
//---------------------------------------------------------------------------
#define YQTCP_UPLOAD_BOOKDIR     0xEF9      //�ϴ�������ҵͨѶ¼(��AD_BOOKDIR)
#define YQTCP_UPLOAD_BOOKTYPE    0xEFA      //�ϴ�������ҵͨѶ¼(��AD_BOOKTYPE)
#define YQTCP_UPLOAD_INFOCLASS   0xEFB      //�ϴ�������ҵͨѶ¼(��AD_INFOCLASS)
#define YQTCP_UPLOAD_INFOTYPE    0xEFC      //�ϴ�������ҵͨѶ¼(��AD_INFOTYPE)
#define YQTCP_UPLOAD_MUCHINFO    0xEFD      //�ϴ�������ҵͨѶ¼(��AD_MUCHINFO)
#define YQTCP_UPLOAD_PERSON      0xEFE      //�ϴ�������ҵͨѶ¼(��AD_PERSON)
#define YQTCP_UPLOAD_PERSONLINK  0xEFF      //�ϴ�������ҵͨѶ¼(��AD_PERSONLINK)
//---------------------------------------------------------------------------
//�ͻ��˹���ϵͳ����(2008-07:E3x���û���)
#define YQTCP_APILOGIN          0xE30   //��¼,Int1=UserID,Str����
#define YQTCP_APIMKBUSY         0xE31   //������æ,Int1=IsBusy,Int2=Type,Str=����
#define YQTCP_APIMKCALL         0xE32   //�������,Int1=UserID,Int2=Type,Str[0~15]:�ֻ��ţ�Str[16~47]���к�
#define YQTCP_APIMKCBK          0xE33   //����ؽ�,���������У�Int1=�����ļ�����,Int2=��������Stream=��·�������ļ�
#define YQTCP_APICTF            0xE34   //����̨����ת��,Int1=Channel, Int2=DestType, Str=Ŀ�ĺ�
#define YQTCP_NEWVMAIL          0xE35   //�յ��������ʼ�,Int1=UserID
#define YQTCP_APIGETTB			0xE36   //�������ݿ�ı���,Int1=����(1:�ֻ������,2:��ѡȺ��,3:���ű�,4:�û���)
#define YQTCP_APISENDTB			0xE37   //�������ݿ�ı���,Int1=����(ͬ��)
#define YQTCP_APIIVRINI			0xE38   //IVR,Int1:1����,0�˳�
#define YQTCP_APIIVR			0xE39   //IVR
#define YQTCP_APINETMON			0xE3A   //����Զ�̼���
#define YQTCP_APICLINREC		0xE3B   //�����¼����֪ͨ,Int1=ID,STR=�ֻ���
#define YQTCP_LINESTATE			0xE3C   //��·״̬
#define YQTCP_LINESPERCARD      0xE3D   //����̨ÿ�忨������
#define YQTCP_APIMKCBKFJ        0xE3E   //�Ⱥ������ٺ��ֻ�,Str[0~15]:�ֻ��ţ�Str[16~47]�ⲿ��
#define YQTCP_CENTREXINFO       0xE3F   //����̨��Ϣ������忨����ÿ�忨ͨ������
//(2008-03-17 ChenQX added)
//---------------------------------------------------------------------------
//IP����
#define YQTCP_IPMKCALL       	0xE40   //INT1=CALLID/USERID,INT2=TYPE+PORT,STR=[0~23]DN+[24~47]IP
#define YQTCP_IPALERT       	0xE41   //INT1=CALLID,INT2=PORT
#define YQTCP_IPANSWER       	0xE42   //INT1=CALLID
#define YQTCP_IPRELEASE   	    0xE43   //INT1=CALLID��INT2=REASION
#define YQTCP_IPUPDATE       	0xE44   //INT1=CALLID��INT2_LO=PORT,INT2_HI=0������2��Ƶ,Str=IP��ַ
#define YQTCP_MEDIASVRDISCON    0xE45   //ý��������뻰��̨MTSC�Ͽ���Ϣ
//---------------------------------------------------------------------------
//�������Դ������
#define YQTCP_ERRORCODE_SYSVAREMPTY       (0-0x1)   //������û�б����������Ե�Ŀ¼�����ݿⱻ�ƻ�
#define YQTCP_ERRORCODE_NOSUCHFILE        (0-0x3)   //������û�д������ļ�
#define YQTCP_ERRORCODE_STREAMERROR       (0-0x4)   //Э�齻�����̳���
#define YQTCP_ERRORCODE_NOMEMORY          (0-0x5)   //�ڴ治��
#define YQTCP_ERRORCODE_NOTCONNCECTED     (0-0x6)   //û�кͷ�������������
#define YQTCP_ERRORCODE_WRITELOCALFILE    (0-0x7)   //����д�����ļ�
#define YQTCP_ERRORCODE_PCMTOWAV          (0-0x8)   //����������ת��PCM�ļ���WAV
#define YQTCP_ERRORCODE_NONE                 0x1    //�ɹ�
//---------------------------------------------------------------------------
//����̨���Ӵ������
#define YQTCP_ERRORCODE_CORPIDINVALID        (0-0x4)  //��ҵID���󣬲����ҵ���Ӧ����̨
#define YQTCP_ERRORCODE_CENTREXDISCONNECTED  (0-0xA)  //��ҵID��Ӧ����̨û�����ӵ�����
//---------------------------------------------------------------------------
//������״̬
#define TTS_STATUS_NOTHANDL 0   //�����ύ
#define TTS_STATUS_HANDLING 1   //�¼ӵģ���Ҫ�ϴ�������
#define TTS_STATUS_OK       2   //����ͨ����
#define TTS_STATUS_FAILED   3   //û������ͨ��
#define TTS_STATUS_USING    4   //�����ɣ�ʹ����
#define TTS_UPLOADOK        5   //���ϴ�
//---------------------------------------------------------------------------

#define ALMCLASS_CENTREX    1   //����̨
#define ALMCLASS_BOARD      2   //�忨
/*ͨ���澯ʱ��TYQTcpMsg.MsgStr ΪCR_TAMEM����TAGRP,TAMEM,
* ��Ϊ����,��ЧֵΪ��������*/
#define ALMCLASS_CHANNEL    3   //ͨ��
/*MG�澯ʱ.TYQTcpMsg.MsgStr ΪCR_TAGRP����TAGRP,��ЧֵΪ1������*/
#define ALMCLASS_MG         4   //MG
/*ACD�澯ʱ��TYQTcpMsg.MsgStr ΪAID����ACDNO TYPE=0���á�����Ϊ����*/
#define ALMCLASS_ACD        5
#define ALMCLASS_DB         6
//---------------------------------------------------------------------------
//�澯����
#define ALM_SMDLG               0xFFF0        //���ϵͳ״̬
#define ALM_BORD		        0x0000        //��·��澯
#define ALM_LINE		        0x0010        //��·���ϵ���·�澯
#define ALM_LINE_B0		        0x0010        //0��·���ϵ���·�澯
#define ALM_LINE_B1		        0x0011        //1��·���ϵ���·�澯
#define ALM_LINE_B2		        0x0012        //2��·���ϵ���·�澯
#define ALM_LINE_B3		        0x0013        //3��·���ϵ���·�澯
#define ALM_LINE_B4		        0x0014        //4��·���ϵ���·�澯
#define ALM_LINE_B5		        0x0015        //5��·���ϵ���·�澯
#define ALM_LINE_B6		        0x0016        //6��·���ϵ���·�澯
#define ALM_LINE_B7		        0x0017        //7��·���ϵ���·�澯
#define ALM_LINE_B8		        0x0018        //8��·���ϵ���·�澯
#define ALM_LINE_B9		        0x0019        //9��·���ϵ���·�澯
#define ALM_LINE_BA		        0x001A        //10��·���ϵ���·�澯
#define ALM_LINE_BB		        0x001B        //11��·���ϵ���·�澯
#define ALM_LINE_BC		        0x001C        //12��·���ϵ���·�澯
#define ALM_LINE_BD		        0x001D        //13��·���ϵ���·�澯
#define ALM_LINE_BE		        0x001E        //14��·���ϵ���·�澯
#define ALM_LINE_BF		        0x001F        //15��·���ϵ���·�澯


//---------------------------------------------------------------------------
//ͨ���澯ֵ
#define LINE_OK                 0x0000            //ͨ������
#define LINE_FAILED             0x0001            //ͨ������


//---------------------------------------------------------------------------
#define MAXFILENAMELEN 14  //�����ʼ���ʶ�ļ�����󳤶�
#define MAXFJHLEN 20       //�ֻ�����󳤶�
#define MAXGHLEN  12       //������󳤶�
//---------------------------------------------------------------------------
//��������������Ϣ�ֶγ���
#define CONFPWDLEN 64      //����������󳤶�
#define CONFMAXLEN 128     //����������󳤶�
#define SUBNUMBERLEN 20    //�ֻ�����󳤶�
#define USERINFOLEN 128    //�����û���ʶ��󳤶�
//---------------------------------------------------------------------------
//MTSU����MTSC���ݵ�����
#define FileType_Mtsf 0     //MTSĿ¼�ļ���
#define FileType_Sys  1     //ϵͳsystem32Ŀ¼�µ��ļ�
#define FileType_Win  2     //ϵͳWINDOWSĿ¼�µ��ļ�
#define FileType_Mts  3     //MTSCĿ¼�µ��ļ�
#define FileType_DB   4     //���ݿ�����
//---------------------------------------------------------------------------
//����Ԥ���û�����״̬��ʧ�ܵ�״̬���Ǹ���
#define CONFUSERSTATE_NONE  0 //�ȴ�����
#define CONFUSERSTATE_ING   1 //���ں���
#define CONFUSERSTATE_OK    2 //���гɹ����ȴ��������
#define CONFUSERSTATE_OKOK  3 //���гɹ����Ѽ������
#define CONFUSERSTATE_OKNO  4 //���гɹ����������ʧ��
#define CONFUSERSTATE_OVER  5 //�û����н���
//---------------------------------------------------------------------------
//����֪ͨ�澯
#define CONF_ALM1      10         //��������ǰ10���ӵĵ�һ�θ澯
#define CONF_ALM2      5          //��������ǰ5���ӵĵڶ��θ澯
#define CONF_ALMPLAY   3          //��������ǰ3���Ӳ��Ÿ澯����
#define USERINFOSPLIT  "$"        //�����û�֮��ķָ���
//---------------------------------------------------------------------------
//�����������
#define CONFTYPE_AUDIO 1  //������Ƶ����
#define CONFTYPE_VIDEO 2  //������Ƶ����
#define CONFTYPE_AV    3  //��������Ƶ����

//---------------------------------------------------------------------------
//����¼�Ƶı�־
#define ARECORDUSERINFO "RecorderA"  //����¼�Ƶ���Ƶ¼�Ʊ�ʶ
#define VRECORDUSERINFO "RecorderV"  //����¼�Ƶ���Ƶ¼�Ʊ�ʶ
//---------------------------------------------------------------------------
//�ֻ���صķֻ��û���½��Ϣ
typedef struct _TFJLoginInfo
{
    //�ֻ���
    char szFjNumber[MAXFJHLEN];
    //�ѵ�½�Ĺ���
    char szLoginGHID[MAXGHLEN];
    //״̬��Ϣ
    int  nState; //1:���ߣ�0��������
}TFJLoginInfo;
//---------------------------------------------------------------------------
//�����ʼ���ʶ
typedef struct _TRecVoiceInfo
{
    //In database table VMAILFILE,szFileName size is 14
    char szFileName[MAXFILENAMELEN];
    //In database table VMAILFILE,szSubNumber size is 20
    char szSubNumber[MAXFJHLEN];
}TRecVoiceInfo;
//---------------------------------------------------------------------------
//�����ϴ��ṹ
typedef struct _TRecFaxInfoEx
{
    TRecFaxInfo FBaseInfo;
    unsigned long FdwUserID;
}TRecFaxInfoEx;
//---------------------------------------------------------------------------
//��������
typedef struct _TSMVR
{
    int FnFuncID;
    int FnStep;
    TApiRecSM FsmInfo;
}TSMVR;
//---------------------------------------------------------------------------
//ͨ����ʾ
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
//��·��Ϣ
typedef struct _TChannelInfo
{
    //����̨AID
    int AID;
    //�忨��(short)+��·��(short)
    TChannelShow BORDINFO;
    //ͨ�����ͱ��
    int  TypeName_Int;
    //ͨ����������
    char TypeName[12];
    //ͨ��������
    char ChName[12];
    //Ӳ��ģ�����ͱ��
    int  ModuleName_Int;
    //Ӳ��ģ����������
    char ModuleName[12];
    //�Ƿ񼤻�
    short Enabled;
    //������ҵID
    int CorpID;
}TChannelInfo;
//---------------------------------------------------------------------------
//����̨�ڲ���ҵ��
typedef struct _TAIDLinkCorp
{
    //����̨ID
    int AID;
    //���ӵ���ҵID
    int CorpID;
    //���ӵ���ҵ����
    char Name[32];
}TAIDLinkCorp;
//---------------------------------------------------------------------------
//�ֻ�������Ϣ
typedef struct _SubNumber
{
    //�ֻ��̺�
    char ShortNumber[20];
    //�ֻ�����
    char LongNumber[20];
}SubNumber;
//---------------------------------------------------------------------------
//����״̬
typedef enum _EConfState
{
    EM_CONF_SLEEPING, //δ����
    EM_CONF_STARTERR, //����ʧ��
    EM_CONF_MEETING,  //���ڽ�����
    EM_CONF_PAUSED,   //�ж�
    EM_CONF_DESTROY   //��Ҫ����
} EConfState;
//---------------------------------------------------------------------------
//��������������Ϣ
typedef struct _TConfInfoRequest
{
    //��ҵID���˻����������
    int CorpID;
    //�û�ID���˻����������
    int UserID;
    //��ʼʱ��
    double StartTime;
    //����ʱ��
    double EndTime;
    //����û���
    unsigned short MaxUser;
    //�������ͣ�ָ����Ƶ
    unsigned short Type;//1:��Ƶ��2����Ƶ��3������Ƶ
    //�Ƿ�¼��
    int Record; //1:¼����0����¼��
    //��������
    char Password[CONFPWDLEN];
    //��������
    char Remark[USERINFOLEN];
}TConfInfoRequest;
//---------------------------------------------------------------------------
//�ͻ��˻�ȡ���Ļ�����Ϣ
typedef struct _TConfInfoRequestEx
{
    //������Ϣ
    TConfInfoRequest BaseInfo;
    //������ID
    int MainID;
    //Ŀǰ״̬
    int State;
    //��������
    int OnlineUsers;
	//����,��ʱ����
	int NoUse;
}TConfInfoRequestEx;
//---------------------------------------------------------------------------
//����¼����Ϣ
typedef struct _TRecordingInfo
{
    //¼��ID
    int FRecID;
    //��ҵID
    int FCorpID;
    //¼�ƿ�ʼʱ��
	double FStartTime;
    //¼�Ƶķֻ���
    char SubNumber[SUBNUMBERLEN];
    //�Ƿ����ڱ�����
    bool IsListening;
}TRecordingInfo;
//---------------------------------------------------------------------------
#endif


