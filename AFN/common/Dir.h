#ifndef DIR_H
#define DIR_H

#define BASEDIR      "/home/yiqiao/" //YIQIAO��Ŀ¼
#define MTSDIR       "/home/yiqiao/" //MTS��Ŀ¼
#define MTSDATADIR   "/home/yiqiao/data/" //MTS����Ŀ¼
#define INIFILE      "/home/yiqiao/mts.ini" //�����ļ�

//���������·�������MTSDIR
#define DIR_SYSLANG0 "data/lang0" //ϵͳ����lang0
#define DIR_SYSLANG1 "data/lang1" //ϵͳ����lang1
#define DIR_SYSLANG2 "data/lang2" //ϵͳ����lang2
#define DIR_SYSLANG3 "data/lang3" //ϵͳ����lang3
#define DIR_SYSLANG4 "data/lang4" //ϵͳ����lang4
#define DIR_IVRTMP   "data/ivrtmp" //��̬�����ļ�
#define DIR_CORP     "data/corp%d" //��ҵ����Ŀ¼
#define DIR_CORP_T   "corp%d"
#define DIR_VOC      "data/corp%d/voc" //��ҵ��ʾ����IVR��ʾ��Ŀ¼
#define DIR_VOC_T    "/voc" 
#define DIR_VOCOWN   "data/corp%d/vocown" //��������
#define DIR_VOCOWN_T "/vocown"

#define DIR_VOCMAIL  "data/corp%d/vocmail" //��������
#define DIR_VOCMAIL_T "/vocmail" 
#define DIR_FAXGET   "data/corp%d/faxget" //��ȡ����
#define DIR_FAXGET_T   "/faxget"
#define DIR_FAXSEND  "data/corp%d/faxsend" //���ʹ���
#define DIR_FAXSEND_T  "/faxsend"
#define DIR_FAXRECE  "data/corp%d/faxrece" //���մ���
#define DIR_FAXRECE_T  "/faxrece"
#define DIR_VOCMON   "data/corp%d/vocmon" //��������
#define DIR_VOCMON_T   "/vocmon"
#define DIR_QJ       "data/corp%d/qj" //�龰Ŀ¼
#define DIR_QJ_T       "/qj"
#define DIR_PHONE    "data/corp%d/phone" //��ƬĿ¼
#define DIR_PHONE_T    "/phone"
#define DIR_OFFFILE  "data/corp%d/offfile" //�����ļ�Ŀ¼
#define DIR_OFFFILE_T  "/offfile"
#define DIR_NHD      "data/corp%d/nhd"     //����Ӳ��Ŀ¼
#define DIR_NHD_T    "/nhd"


//�ļ�����ʽ
#define FILE_REC          "record/%d-%d[%s]" //¼��Ŀ¼
#define FILE_VOCOWN       "data/corp%d/vocown/%d.voc" //���������ļ�
#define FILE_VM           "data/corp%d/vocmail/%d.voc" //����
#define FILE_VMID         "data/corp%d/vocmail/%s" //����
#define FILE_FAXRECE      "data/corp%d/faxrece/%s" //���մ���
#define FILE_FAXSEND      "data/corp%d/faxsend/%s" //���ʹ���
#define FILE_FAXSENDID    "data/corp%d/faxsend/%d.tif" //���ʹ���
#define FILE_FAXRECVID    "data/corp%d/faxrece/%d.tif" //���մ���
#define FILE_FAXGET       "data/corp%d/faxget/%s" //��ȡ����
#define FILE_OFFFILE      "data/corp%d/offfile/%s" //�����ļ�
#define FILE_VOCFILE      "data/corp%d/voc/%s"
#define FILE_TEMPFILE     "data/corp%d/%d.tmp"
#define FILE_TEMPFAX      "data/corp%d/%d.tif"
#define FILE_FULLVOCTIMER FILE_VOCFILE
#define FILE_LANGFIL      "data/lang%d/%s"
#define FILE_LEV_VOC      "lev%d.voc"
#define FILE_BREAK_VOC    "break.voc"
#define FILE_VOCTIMER     "timcal%d.voc"
#define FILE_NHDFile      "data/corp%d/nhd/%s"     //����Ӳ��Ŀ¼




	
//¼����������¼��Ŀ¼
#define DIR_REC       "record" //¼��Ŀ¼

//TTSת����ʱĿ¼
#define DIR_TTSTMP  "/home/yiqiao/ttstmp"
#endif

