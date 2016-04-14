#ifndef DIR_H
#define DIR_H

#define BASEDIR      "/home/yiqiao/" //YIQIAO根目录
#define MTSDIR       "/home/yiqiao/" //MTS根目录
#define MTSDATADIR   "/home/yiqiao/data/" //MTS数据目录
#define INIFILE      "/home/yiqiao/mts.ini" //配置文件

//以下是相对路径，相对MTSDIR
#define DIR_SYSLANG0 "data/lang0" //系统语音lang0
#define DIR_SYSLANG1 "data/lang1" //系统语音lang1
#define DIR_SYSLANG2 "data/lang2" //系统语音lang2
#define DIR_SYSLANG3 "data/lang3" //系统语音lang3
#define DIR_SYSLANG4 "data/lang4" //系统语音lang4
#define DIR_IVRTMP   "data/ivrtmp" //动态语音文件
#define DIR_CORP     "data/corp%d" //企业工作目录
#define DIR_CORP_T   "corp%d"
#define DIR_VOC      "data/corp%d/voc" //企业提示音、IVR提示音目录
#define DIR_VOC_T    "/voc" 
#define DIR_VOCOWN   "data/corp%d/vocown" //机主留言
#define DIR_VOCOWN_T "/vocown"

#define DIR_VOCMAIL  "data/corp%d/vocmail" //语音邮箱
#define DIR_VOCMAIL_T "/vocmail" 
#define DIR_FAXGET   "data/corp%d/faxget" //索取传真
#define DIR_FAXGET_T   "/faxget"
#define DIR_FAXSEND  "data/corp%d/faxsend" //发送传真
#define DIR_FAXSEND_T  "/faxsend"
#define DIR_FAXRECE  "data/corp%d/faxrece" //接收传真
#define DIR_FAXRECE_T  "/faxrece"
#define DIR_VOCMON   "data/corp%d/vocmon" //监听语音
#define DIR_VOCMON_T   "/vocmon"
#define DIR_QJ       "data/corp%d/qj" //情景目录
#define DIR_QJ_T       "/qj"
#define DIR_PHONE    "data/corp%d/phone" //照片目录
#define DIR_PHONE_T    "/phone"
#define DIR_OFFFILE  "data/corp%d/offfile" //离线文件目录
#define DIR_OFFFILE_T  "/offfile"
#define DIR_NHD      "data/corp%d/nhd"     //网络硬盘目录
#define DIR_NHD_T    "/nhd"


//文件名格式
#define FILE_REC          "record/%d-%d[%s]" //录音目录
#define FILE_VOCOWN       "data/corp%d/vocown/%d.voc" //机主留言文件
#define FILE_VM           "data/corp%d/vocmail/%d.voc" //留言
#define FILE_VMID         "data/corp%d/vocmail/%s" //留言
#define FILE_FAXRECE      "data/corp%d/faxrece/%s" //接收传真
#define FILE_FAXSEND      "data/corp%d/faxsend/%s" //发送传真
#define FILE_FAXSENDID    "data/corp%d/faxsend/%d.tif" //发送传真
#define FILE_FAXRECVID    "data/corp%d/faxrece/%d.tif" //接收传真
#define FILE_FAXGET       "data/corp%d/faxget/%s" //索取传真
#define FILE_OFFFILE      "data/corp%d/offfile/%s" //离线文件
#define FILE_VOCFILE      "data/corp%d/voc/%s"
#define FILE_TEMPFILE     "data/corp%d/%d.tmp"
#define FILE_TEMPFAX      "data/corp%d/%d.tif"
#define FILE_FULLVOCTIMER FILE_VOCFILE
#define FILE_LANGFIL      "data/lang%d/%s"
#define FILE_LEV_VOC      "lev%d.voc"
#define FILE_BREAK_VOC    "break.voc"
#define FILE_VOCTIMER     "timcal%d.voc"
#define FILE_NHDFile      "data/corp%d/nhd/%s"     //网络硬盘目录




	
//录音服务器的录音目录
#define DIR_REC       "record" //录音目录

//TTS转换临时目录
#define DIR_TTSTMP  "/home/yiqiao/ttstmp"
#endif

