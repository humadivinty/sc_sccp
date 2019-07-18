// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SCCPDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SCCP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SCCP_EXPORTS
#define SCCP_API extern "C" __declspec(dllexport)
#else
#define SCCP_API extern "C" __declspec(dllimport)
#endif

#define DLL_SPEC __stdcall

#include "SCCP_commenDef.h"

/***********
*校时函数
*LONG lHandle ，登录相机后，获取的连接handle
*NET_DVR_TIME  CurTime , 指定时间
***********/
SCCP_API BOOL DLL_SPEC SCLS_Settime(LONG lHandle, NET_DVR_TIME  CurTime);

/************
*相机初始化
*不需要此步骤的，直接返回true
************/
SCCP_API BOOL DLL_SPEC SCLS_DVR_Init();

/*********
*相机登录，此函数，完成相机的登录、连接功能
*char *sIP   相机IP地址
*WORD wPort  相机端口号，如无，则传NULL
*char *sUserName  登录相机用户名，如无，则传NULL
*char *sPassword   登录相机密码，如无，则传NULL
****返回值 LONG  相机登录后的标识，成功返回值>=0，失败返回值<0
**********/
SCCP_API LONG DLL_SPEC SCLS_DVR_Login(char *sIP, WORD wPort, char *sUserName, char *sPassword);

/********
*相机回调函数设置
*CALLBACK *fMessCallBack  回调函数名称
*LONG lHandle    相机连接的lHandle
*返回值 BOOL   true 成功， false 失败
*********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_SetDVRMessCallBack(MessageCallback fMessCallBack, LONG lHandle);

/*********
*相机退出函数
*LONG lHandle  相机连接的lHandle
*返回值 BOOL   true 成功， false 失败
**********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_Logout(LONG lHandle);

/**********
*获取近景图
*char *ImgBuf 址传递，存储图片,用户返回
*int bufLen   用于存储图片缓存的长度
*int *ImgSize 址传递，用于记录图片的实际大小
*返回值 int   0 成功，-4代表相机截取图片过大，实际分配存储空间不足
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetBigImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/**********
*获取车牌二值图
*char *ImgBuf 址传递，存储图片,用户返回
*int bufLen   用于存储图片缓存的长度
*int *ImgSize 址传递，用于记录图片的实际大小
*返回值 int   0 成功，-4代表相机截取图片过大，实际分配存储空间不足
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetBinaryImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/**********
*获取车牌彩色图片
*char *ImgBuf 址传递，存储图片,用户返回
*int bufLen   用于存储图片缓存的长度
*int *ImgSize 址传递，用于记录图片的实际大小
*返回值 int   0 成功，-4代表相机截取图片过大，实际分配存储空间不足
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetSmallImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/*********
*char *pPlateNo 址传递，存储车牌颜色和车牌号码
*int bufLen      *pPlateNo的分配的长度，
*int *plateNoLen  址传递，用于记录车牌实际大小
*返回值 int   0 成功， -4 获取的车牌长度，超过分配的长度
**********/
SCCP_API int DLL_SPEC SCLS_DVR_GetPlateNo(LONG lHandle, char *pPlateNo, int bufLen, int *plateNoLen);

/*********
*调用此函数后，会触发回调函数
*LONG lHandle  相机连接
*返回值 BOOL   true 成功， false 失败
**********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_ForceCapture(LONG lHandle);

/*********
*视频播放
*LONG lHandle 相机连接
*HWND FormHwnd   C#播放窗口
*返回值，LONG 视频播放的handle  可以通过这个handle 单独关闭视频
**********/
SCCP_API LONG DLL_SPEC SCLS_DVR_RealPlay(LONG lHandle, HWND FormHwnd);

/*********
*停止视频播放
*LONG lHandle 视频播放的handle
********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_StopRealPlay(LONG lHandle);

/********
*视频录像功能，该功能是在视频打开预览功能后，才可开启
*LONG lHandle  视频播放的handle
*char *sFileName   视频文件保存路径
*返回值  true 成功 ， false 失败
*********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_StartRecord(LONG lHandle, char *sFileName);

/********
*停止视频录像
*LONG lHandle 视频播放的handle
*返回值  true 成功 ， false 失败
*********/
SCCP_API  BOOL DLL_SPEC SCLS_DVR_StopRecord(LONG lHandle);