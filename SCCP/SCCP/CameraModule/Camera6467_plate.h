#pragma once

#define USE_VIDEO 1

#include "BaseCamera.h"
#include "CameraModule/ResultListManager.h"

class Camera6467_plate :    public BaseCamera
{
public:
    Camera6467_plate();
    Camera6467_plate(const char* chIP, HWND  hWnd, int Msg);
    virtual ~Camera6467_plate();

    bool GetOneJpegImg(CameraIMG &destImg);    

    //��ȡ����״̬�ص���ʱ����
    int GetTimeInterval();
    void SetDisConnectCallback(void* funcDisc, void* pUser);
    void SetReConnectCallback(void* funcReco, void* pUser);
    void SetConnectStatus_Callback(void* func, void* pUser, int TimeInterval);
    void SendConnetStateMsg(bool isConnect);

    void SetResultCallback(void* funcResult, void* pUser);

    bool GetResultComplete();
    void SetResultComplete(bool bfinish);

    CameraResult* GetOneResult();
#ifdef USE_VIDEO
    void StartPlayVideoByChannel(int iChannelID, const HWND winHandle);
    int StopPlayVideoByChannel(int iChannelID);
    void* GetVideoHandleByChannel(int iChannerlID);
    int GetChannelIDByHandle(void* handle);
    bool TakeOnePictureFromVedio(int channelID, CameraIMG& camImg, int iImgType = 0);
    int GetChanelIDByWinHandle(void* handle);
    void recordChannelWinHandle(int channelID, void* handle);
#endif

private:
    void AnalysisAppendXML(CameraResult* CamResult);
    int AnalysisVelchType(const char* vehType);
    int AnalysisVelchTypeWithPlateColor(const char* vehType, int iPlateColor);
    int AnalysisPlateColorNo(const char *szPlateNo);
    void ReadConfig();

    void SendResultByCallback();

#ifdef USE_VIDEO
    int StartPlayVideo(int iChannelID, HANDLE& playHandle, const HWND winHandle);
    void StopPlayVideo(HANDLE& playHandle);
#endif

    void SetCheckThreadExit(bool  bExit);
    bool GetCheckThreadExit();

    void SaveOverlayResult();

    bool CheckIfBackUpVehicle(CameraResult* CamResult);
    bool CheckIfSuperLength(CameraResult* CamResult);
private:

    int m_iTimeInvl;
    int m_iCompressBigImgSize;
    int m_iCompressSamllImgSize;
    int m_iSuperLenth;
    int m_iWaitVfrTimeOut;

    DWORD m_dwLastCarID;

    bool m_bResultComplete;
    bool m_bJpegComplete;
    bool m_bSaveToBuffer;
    bool m_bOverlay;
    bool m_bCompress;
    bool m_bStatusCheckThreadExit;


    PBYTE m_pTempBin;
    PBYTE m_pTempBig1;
    PBYTE m_pCaptureImg;
    PBYTE m_pTempBig;

    CameraResult* m_CameraResult;
    CameraResult* m_BufferResult;

    CameraIMG m_CIMG_StreamJPEG;

    CRITICAL_SECTION m_csResult;
    CameraIMG m_Bin_IMG_Temp;
    CameraIMG m_Small_IMG_Temp;

    void* g_pUser;
    void* g_func_ReconnectCallback;
    void* g_ConnectStatusCallback;
    void* g_func_DisconnectCallback;

    void* g_pFunc_Result;
    void* g_pUser_Result;

    void* g_pFunc_ResultExtraInfo;
    void* g_pUserData_ExtraInfo;

    void* m_hFirstWinHandle;
    void* m_hSecondWinHandle;

    HANDLE m_hPlayFirstH264;
    HANDLE m_hPlaySecondh264;

    HANDLE m_hStatusCheckThread;			//״̬����߳�

    ResultListManager m_resultList;

public:
    DWORD getResultWaitTime();
    unsigned int SendResultThreadFunc();
    static unsigned int WINAPI s_SendResultThreadFunc(void* parameter)
    {
        Camera6467_plate* pThis = (Camera6467_plate* )parameter;
        if (pThis)
        {
            return pThis->SendResultThreadFunc();
        }
        return 0;
    }
private:

    virtual int RecordInfoBegin(DWORD dwCarID);

    virtual int RecordInfoEnd(DWORD dwCarID);

    virtual int RecordInfoPlate(DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoBigImage(DWORD dwCarID,
        WORD  wImgType,
        WORD  wWidth,
        WORD  wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoSmallImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoBinaryImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int DeviceJPEGStream(
        PBYTE pbImageData,
        DWORD dwImageDataLen,
        DWORD dwImageType,
        LPCSTR szImageExtInfo);

    virtual void CheckStatus();
};

