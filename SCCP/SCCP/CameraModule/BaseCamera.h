#ifndef CAMERA6467_H
#define CAMERA6467_H
#include "CameraResult.h"
#include <string>
#include <list>
#include <comdef.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")

#include "libAVI/cAviLib.h"
#include "MyH264Saver.h"

#define CMD_DEL_VEH_HEAD 1
#define CMD_GET_VEH_LENGTH 2
#define  CMD_DEL_ALL_VEH 3
#define CMD_GET_VHE_HEAD 4


#define  COLOR_UNKNOW (9)
#define  COLOR_BLUE 0
#define  COLOR_YELLOW 1
#define  COLOR_BLACK 2
#define  COLOR_WHITE 3
#define  COLOR_GRADIENT_CREEN 4
#define  COLOR_YELLOW_GREEN 5
#define  COLOR_BLUE_WHITE 6
#define  COLOR_RED 7

#define  BUFFERLENTH 256

#define SAFE_DELETE_OBJ(obj) \
if (NULL != obj)                                  \
{                                           \
    delete obj;                        \
    obj = NULL;                      \
}

#define SAFE_DELETE_ARRAY(arrayObj) \
if (NULL != arrayObj)                                  \
{                                           \
    delete[] arrayObj;                        \
    arrayObj = NULL;                      \
}

unsigned int __stdcall  Camera_StatusCheckThread(LPVOID lpParam);

struct CameraInfo
{
    char chIP[20];                          //deviceIP
    char chDeviceID[3];
    char chStationID[7];
    char chLaneID[3];
    bool bLogEnable;
    bool bSynTimeEnable;
    bool bIsInit;
    int iDirection;
    CameraInfo()
    {
        memset(chIP, 0, 20);
        memset(chDeviceID, 0, 3);
        memset(chStationID, 0, 7);
        memset(chLaneID, 0, 3);
        bLogEnable = true;
        bSynTimeEnable = false;
        bIsInit = false;
        iDirection = 0;
    }
};

struct SaveModeInfo
{
    int iSafeModeEnable;
    char chBeginTime[260];
    char chEndTime[260];
    int iIndex;
    int iDataType;
    SaveModeInfo()
    {
        iSafeModeEnable = 0;
        iIndex = 0;
        iDataType = 0;
        memset(chBeginTime, 0, 260);
        memset(chEndTime, 0, 260);
    }
};

struct DeviceTime
{
    int iYear;
    int iMonth;
    int iDay;
    int iHour;
    int iMinutes;
    int iSecond;
    int iMilisecond;

    DeviceTime()
    {
        iYear = 0;
        iMonth = 0;
        iDay = 0;
        iHour = 0;
        iMinutes = 0;
        iSecond = 0;
        iMilisecond = 0;
    }
};

typedef struct _BasicInfo
{
    char szIP[64];              //设备IP，适用于所有设备类型
    char szMask[64];            //设备子网掩码，适用于所有设备类型
    char szGateway[64];         //设备网关，适用于所有设备类型
    char szMac[128];            //设备物理地址，适用于所有设备类型
    char szModelVersion[128]; //设备模型版本，此字段保留
    char szSN[128];             //设备编号，适用于所有设备类型
    char szWorkMode[128];       //设备工作模式，仅适用于PCC200、PCC600、PCC200A
    char szDevType[128];        //设备类型，仅适用于PCC200、PCC600、PCC200A
    char szDevVersion[128];  //设备版本，仅适用于PCC200、PCC600、PCC200A
    char szMode[128];           //设备运行模式，如正常模式，适用于所有设备类型
    char szRemark[128];    //保留字段，仅适用于PCC200、PCC600
    char szBackupVersion[128];  //备份版本，仅适用于PCC200、PCC600 
    char szFPGAVersion[128]; //FPGA版本，仅适用于PCC200、PCC600
    char szKernelVersion[128];  //Kernel版本，仅适用于PCC200、PCC600
    char szUbootVersion[128]; //Uboot版本，仅适用于PCC200、PCC600
    char szUBLVersion[128];//UBL版本，仅适用于PCC200、PCC600
}BasicInfo;

class BaseCamera
{
public:
    BaseCamera();
    BaseCamera(const char* chIP, HWND  hWnd, int Msg);
    //Camera6467(const char* chIP, HWND*  hWnd,int Msg);
    virtual ~BaseCamera() = 0;

    //void SetList(Result_lister* ResultList);
    bool SetCameraInfo(CameraInfo& camInfo);
    void SetLoginID(int iID);

    int GetCamStatus();
    int GetNetSatus();
    int GetLoginID();
    char* GetStationID();
    char* GetDeviceID();
    char*  GetLaneID();
    const char* GetCameraIP();
    void SetCameraIP(const char* ipAddress);
    void SetWindowsWnd(HWND  hWnd);
    void SetResultMsg(int msgNum);
    void SetCameraIndex(int iIndex);
    bool GetDeviceTime(DeviceTime& deviceTime);
    bool GetStreamLength(IStream* pStream, ULARGE_INTEGER* puliLenth);

    int ConnectToCamera();
    void	InterruptionConnection();
    bool DisConnectCamera();

    virtual void ReadConfig();

    void WriteFormatLog(const char* szfmt, ...);
    bool WriteLog(const char* chlog);

    bool TakeCapture();
    bool SynTime();
    bool SynTime(int Year, int Month, int Day, int Hour, int Minute, int Second, int MilientSecond);
    bool SaveImgToDisk(char* chImgPath, BYTE* pImgData, DWORD dwImgSize);
    bool SaveImgToDisk(char* chImgPath, BYTE* pImgData, DWORD dwImgSize, int iWidth, int iHeight, int iType = 0);

    void SetMsg(UINT iConMsg, UINT iDsiConMsg);
    void SaveResultToBufferPath(CameraResult* pResult);

    void CompressImg(CameraIMG& camImg, DWORD requireSize);

    void SendMessageToPlateServer(int iMessageType = 1);
    //向设备发送特定命令，只对中海小黄人对接有效
    bool SenMessageToCamera(int iMessageType, int& iReturnValue, int& iErrorCode, int iArg = 0);

    //设置设备的叠加字符大小和颜色
    bool SetOverlayVedioFont(int iFontSize, int iColor);
    bool SetOverlayVideoText(int streamId, char* overlayText, int textLength);
    bool SetOverlayVideoTextPos(int streamId, int posX, int posY);
    bool SetOverlayVideoTextEnable(int streamId, bool enable);
    bool SetOverlayTimeEnable(int streamID, bool enable);
    bool SetOverlayTimeFormat(int streamId, int iformat);
    bool SetOverlayTimePos(int streamId, int posX, int posY);

    //获取设备的硬件版本信息
    bool GetHardWareInfo(BasicInfo& info);
    bool CheckDeviceIfOldVersion();

    //视频保存
    bool SetH264Callback(int iStreamID, DWORD64 dwBeginTime, DWORD64 dwEndTime, DWORD RecvFlag);
    bool SetH264CallbackNULL(int iStreamID, DWORD RecvFlag);

    bool StartToSaveAviFile(int iStreamID, const char* fileName, DWORD64 beginTimeTick = 0);
    bool StopSaveAviFile(int iStreamID);

private:
    int SaveH264Frame(
        BYTE* H264FrameData,
        LONG DataSize, 
        LONG Width, 
        LONG Height,
        int isIFrame,
        LONGLONG FrameTime, 
        int IsHistory);

    void setAviFilePath(const char* chPath);
    char* getAviPath();
protected:
    void* m_hHvHandle;
    //void* m_hWnd;
    HWND m_hWnd;
    int m_iMsg;
    int m_iConnectMsg;
    int m_iDisConMsg;
    int m_iConnectStatus;
    int m_iLoginID;
    int m_iCompressQuality;
    int m_iDirection;
    int m_iIndex;

    bool m_bLogEnable;
    bool m_bSynTime;
    bool m_bDeviceTypeNew;
    bool m_bFirstH264Frame;

    char m_chDeviceID[3];
    char m_chStationID[7];
    char m_chLaneID[3];
    char m_chAviFilePath[256];

    std::string m_strIP;
    std::string m_strDeviceID;

    CLSID m_jpgClsid;
    CLSID m_bmpClsid;

    SaveModeInfo m_SaveModelInfo;

    CRITICAL_SECTION m_csLog;    

    CAviLib m_264AviLib;
    MyH264Saver m_h264Saver;

    void ReadHistoryInfo();
    void WriteHistoryInfo(SaveModeInfo& SaveInfo);

    inline void CopyDataToIMG(CameraIMG& DestImg,
        PBYTE pImgData,
        unsigned long width,
        unsigned long height, 
        unsigned long dataLenth, 
        WORD wImgType)
    {
        if (NULL != DestImg.pbImgData)
        {
            delete[] DestImg.pbImgData;
            DestImg.pbImgData = NULL;
        }

        DestImg.pbImgData = new(std::nothrow) BYTE[dataLenth];
        DestImg.dwImgSize = 0;
        if (NULL != DestImg.pbImgData)
        {
            memcpy(DestImg.pbImgData, pImgData, dataLenth);
            DestImg.wImgWidth = width;
            DestImg.wImgHeight = height;
            DestImg.dwImgSize = dataLenth;
            DestImg.wImgType = wImgType;
        }
    }
    
//protected:
    CRITICAL_SECTION m_csFuncCallback;

public:
    static int  RecordInfoBeginCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoBegin(dwCarID);
    };
    virtual int RecordInfoBegin(DWORD dwCarID) = 0;

    static int  RecordInfoEndCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoEnd(dwCarID);
    };
    virtual int RecordInfoEnd(DWORD dwCarID) = 0;

    static int  RecordInfoPlateCallBack(PVOID pUserData,
        DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoPlate(dwCarID, pcPlateNo, pcAppendInfo, dwRecordType, dw64TimeMS);
    }
    virtual int RecordInfoPlate(DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS) = 0;

    static int  RecordInfoBigImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD  wImgType,
        WORD  wWidth,
        WORD  wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoBigImage(dwCarID, wImgType, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    virtual int RecordInfoBigImage(DWORD dwCarID,
        WORD  wImgType,
        WORD  wWidth,
        WORD  wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS) = 0;

    static int  RecordInfoSmallImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoSmallImage(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    virtual int RecordInfoSmallImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS) = 0;

    static int  RecordInfoBinaryImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->RecordInfoBinaryImage(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    virtual int RecordInfoBinaryImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS) = 0;

    static int  JPEGStreamCallBack(
        PVOID pUserData,
        PBYTE pbImageData,
        DWORD dwImageDataLen,
        DWORD dwImageType,
        LPCSTR szImageExtInfo
        )
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->DeviceJPEGStream(pbImageData, dwImageDataLen, dwImageType, szImageExtInfo);

    };
    virtual int DeviceJPEGStream(
        PBYTE pbImageData,
        DWORD dwImageDataLen,
        DWORD dwImageType,
        LPCSTR szImageExtInfo) = 0;

    virtual void CheckStatus() = 0;

    static INT HVAPI_CALLBACK_H264_EX(
        PVOID pUserData,
        DWORD dwVedioFlag,
        DWORD dwVideoType,
        DWORD dwWidth,
        DWORD dwHeight,
        DWORD64 dw64TimeMS,
        PBYTE pbVideoData,
        DWORD dwVideoDataLen,
        LPCSTR szVideoExtInfo
        )
    {
        if (pUserData == NULL)
            return 0;

        BaseCamera* pThis = (BaseCamera*)pUserData;
        return pThis->handleH264Frame(
            dwVedioFlag,
            dwVideoType,
            dwWidth,
            dwHeight,
            dw64TimeMS,
            pbVideoData,
            dwVideoDataLen,
            szVideoExtInfo);
    };

    int handleH264Frame(
        DWORD dwVedioFlag,
        DWORD dwVideoType,
        DWORD dwWidth,
        DWORD dwHeight,
        DWORD64 dw64TimeMS,
        PBYTE pbVideoData,
        DWORD dwVideoDataLen,
        LPCSTR szVideoExtInfo
        );
};

#endif