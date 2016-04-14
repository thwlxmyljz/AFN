//---------------------------------------------------------------------------
#ifndef YQResourceH
#define YQResourceH
//---------------------------------------------------------------------------
#define YQRES_NONERES			0x0000		// 无资源
#define YQRES_AUDIO_CAPS		0x0001		// 音频采集设备
#define YQRES_AUDIO_OUT			0x0002		// 音频输出设备
#define YQRES_VIDEO_CAPS		0x0003		// 图像采集设备
#define YQRES_VIDEO_OUT			0x0004		// 图像输出
#define YQRES_WAV_PLAY			0x0005		// 对外播放wav语音文件
#define YQRES_WAV_RECORD		0x0006		// 录制收到语音到wav文件
#define YQRES_DTMFCODE			0x0007		// DTMF码
#define YQRES_REC_PLAY			0x0008		// 播放录音服务器过程中的操作
#define YQRES_VOLUME_TEST		0x0009		// 音频采集和输出设备测试

#define YQRES_RTPVOICE			0x0010		// RTP语音
#define YQRES_RTPVIDEO			0x0020		// RTP视频

#define YQRES_RTPPROXY			0x0030		// RTP代理

#define YQRES_RTPVCCONF			0x0040		// 语音RTP会议
#define YQRES_RTPVDCONF			0x0050		// 视频RTP会议

#define YQRES_RECORDDIR			0x0060		// 录音服务器录制目录
#define YQRES_AUDIO_RECSVR		0x0061		// 音频录制服务器
#define YQRES_VIDEO_RECSVR		0x0062		// 视频录制服务器

#define YQRES_UDTF				0x0070		//  UDT传输管道
#define YQRES_UDTFILE			0x0071		//  待传输的文件
#define YQRES_SDPFILE			0x0072		//  待传输的文件翻译成SDP

#define YQRES_UPDATE			0x0080		//  更新当前会话的资源

#define YQRES_T38FAX			0x0090		//  T.38传真

#define YQRES_CARDCH			0x0100		// 卡类中继
#define YQRES_CARDFAX			0x0200		// 卡类传真
#define YQRES_CARDCONF			0x0300		// 卡类会议

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
