#ifndef SCCP_COMMENTDEF_H
#define SCCP_COMMENTDEF_H
#include <windef.h>
//车牌检测结果
typedef struct tagNET_DVR_PLATE_RESULT
{
    BYTE    byDriveChan;    //触发车道号,默认为0
    DWORD   dwPicLen;        //图片长度(近景图)
    DWORD   dwPicPlateLen;  //车牌小图片长度（车牌彩图）
    DWORD   dwBinPicLen;    //车牌二值图长度
    BYTE    *pBuffer1;      // 近景图
    BYTE    *pBuffer2;      // 车牌彩图
    BYTE    *pBuffer3;      //车牌二值图
    char sLicense[20]; //车牌信息，颜色+车牌，无结果时，值为“未检测”
    char vlpId[32];      //识别车辆Id, 格式“yyyyMMddHHmmss+3位自增
}NET_DVR_PLATE_RESULT, *LPNET_DVR_PLATE_RESULT;
//结构体内容，可根据相机厂商可提供的数值，自行添加其他字段，但以上结构拥有的东西//是现在项目必须有的
/***********
*回调函数
*LONG lCommand 命令类型，或者报警类型，或者相机消息类型，用于针对特定消息，重写*处理过程
*char *pInfo 存储车牌信息、车辆照片，可以按照下面方式，直接赋值给*NET_DVR_PLATE_RESULT类型的变量。
***********/
//typedef void (CALLBACK *MessageCallback)(int, char *);
//{
//    //回调函数调用
//    NET_DVR_PLATE_RESULT struPlateResult;
//    memcpy(&struPlateResult, pInfo, sizeof(struPlateResult));
//    .....
//}

typedef void(__stdcall *CBFun_GetRegResult)(LONG, NET_DVR_PLATE_RESULT *);


typedef struct _extra_vlp_info
{
    char vlpId[32];                   //识别车辆Id, 格式“yyyyMMddHHmmss+3位自增数字”确保不会重复； 
    int vlpExtraType;				//额外识别数据类型 1：车身图片 2：车尾图片 3：额外识别信息
    unsigned int imageLength;	//识别数据长度
    unsigned char* image;		//识别数据
} T_EXTRAVLPINFO;

typedef void(__stdcall *CBFun_GetExtraRegResult)(int, T_EXTRAVLPINFO *);



typedef struct _NET_DVR_TIME
{
    DWORD dwYear;        //年
    DWORD dwMonth;        //月
    DWORD dwDay;        //日
    DWORD dwHour;        //时
    DWORD dwMinute;        //分
    DWORD dwSecond;        //秒
}NET_DVR_TIME, *LPNET_DVR_TIME;

#endif