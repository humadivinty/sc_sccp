#include "stdafx.h"
#include "SCCP.h"
#include "CameraModule/Camera6467_plate.h"
#include "CameraModule/DeviceListManager.h"
#include "utilityTool/ToolFunction.h"

#define MAX_CAMERA_COUNT (20)

#define WRITE_LOG(fmt, ...) Tool_WriteFormatLog("%s:: "fmt, __FUNCTION__, ##__VA_ARGS__);

#define ERROR_CODE_PARA_INVALID -100
#define ERROR_CODE_TIMEOUT -101
#define ERROR_CODE_DISCONNECT -102
#define ERROR_CODE_BUFFER_NOTENOUGH -4

extern int g_iVideoChannelID;


SCCP_API BOOL DLL_SPEC SCLS_Settime(LONG lHandle, NET_DVR_TIME CurTime)
{
    WRITE_LOG(" begin, nHandle = %d, CurTime.year = %d, month = %d, day = %d, hour = %d, minute = %d, second = %d .",
        lHandle, 
        CurTime.dwYear,
        CurTime.dwMonth,
        CurTime.dwDay,
        CurTime.dwHour,
        CurTime.dwMinute,
        CurTime.dwSecond);

    if (!DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        WRITE_LOG("camera id is not found.");
        return FALSE;
    }
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera != NULL)
    {
        if (pCamera->SynTime(CurTime.dwYear,
            CurTime.dwMonth,
            CurTime.dwDay,
            CurTime.dwHour,
            CurTime.dwMinute,
            CurTime.dwSecond,
            0))
        {
            WRITE_LOG("SynTime success.");
            return TRUE;
        }
        else
        {
            WRITE_LOG("SynTime failed.");
            return FALSE;
        }        
    }
    else
    {
        WRITE_LOG(" the camera of nHandle is invalid.");
        return FALSE;
    }
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_Init()
{
    WRITE_LOG("return true directory");
    return TRUE;
}

SCCP_API LONG DLL_SPEC SCLS_DVR_Login(char *sIP, WORD wPort, char *sUserName, char *sPassword)
{
    WRITE_LOG("Begin, sIP = %s, wport = %u, sUserName = %s, sPassword = %s", sIP, wPort, sUserName, sPassword);
    int iCheckIp = Tool_checkIP(sIP);
    if (iCheckIp != 1)
    {
        WRITE_LOG("the Parameters is invalid,  IP address is invalid.");
        return ERROR_CODE_PARA_INVALID;
    }

    long iHandle = ERROR_CODE_DISCONNECT;
    bool bFindSuccess = false;
    BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceByIpAddress(sIP);
    if (pCamera != NULL)
    {
        iHandle = DeviceListManager::GetInstance()->GetDeviceIdByIpAddress(sIP);
        WRITE_LOG("find device %s, ID = %ld", sIP, iHandle);
        bFindSuccess = true;
    }
    else
    {
        for (int i = 0; i < MAX_CAMERA_COUNT; i++)
        {
            if (NULL == DeviceListManager::GetInstance()->GetDeviceById(i))
            {
                Camera6467_plate* pCamera = new Camera6467_plate();
                pCamera->SetCameraIP(sIP);
                pCamera->SetLoginID(i);
                if (pCamera->ConnectToCamera() == 0)
                {
                    WRITE_LOG("connect to camera success.");
                }
                else
                {
                    WRITE_LOG("connect to camera failed.");
                }
                DeviceListManager::GetInstance()->AddOneDevice(i, pCamera);

                iHandle = i;
                WRITE_LOG("create camera success, device id= %d, ip = %s", i, sIP);
                bFindSuccess = true;
                break;
            }
        }
    }
    WRITE_LOG("finish, iHandle= %ld ", iHandle);
    return iHandle;
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_SetDVRMessCallBack(MessageCallback fMessCallBack, LONG lHandle)
{
    WRITE_LOG(" begin, nHandle = %d, MessageCallback = %p, .", lHandle, fMessCallBack);
    if (!DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        WRITE_LOG("camera id is not found.");
        return FALSE;
    }
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera != NULL)
    {
        pCamera->SetResultCallback(fMessCallBack, NULL);
        WRITE_LOG("SetResultCallBack success.");
        return TRUE;
    }
    else
    {
        WRITE_LOG(" end ,the camera of nHandle is invalid.");
        return FALSE;
    }
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_Logout(LONG lHandle)
{
    WRITE_LOG(" begin, nHandle = %d.", lHandle);
    if (DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        DeviceListManager::GetInstance()->EraseDevice(lHandle);
    }
    WRITE_LOG("finish.");
    return TRUE;
}

SCCP_API int DLL_SPEC SCLS_DVR_GetBigImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize)
{
    WRITE_LOG(" begin,lHandle = %d, ImgBuf = %p, bufLen = %d, ImgSize = %p", lHandle, ImgBuf, bufLen, ImgSize);
    //int iDeviceID = -1;
    //Camera6467_plate* pCamera = NULL;
    //for (int i = 0; i < MAX_CAMERA_COUNT; i++)
    //{
    //    pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(i);
    //    if (NULL != pCamera)
    //    {
    //        iDeviceID = i;
    //        break;
    //    }
    //}
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera == NULL)
    {
        WRITE_LOG("finish , can not find the camera, please connect first.");
        return ERROR_CODE_DISCONNECT;
    }
    int iRet = -1;
    CameraResult* pReult = pCamera->GetOneResult();
    if (pReult== NULL)
    {
        WRITE_LOG("finish ,result is not ready.");
        return ERROR_CODE_TIMEOUT;
    }
    WRITE_LOG(" get result success, image data = %p, length = %lu.", 
        pReult->CIMG_LastSnapshot.pbImgData,
        pReult->CIMG_LastSnapshot.dwImgSize);

    if (pReult->CIMG_LastSnapshot.dwImgSize <= 0
        || pReult->CIMG_LastSnapshot.dwImgSize > bufLen
        || pReult->CIMG_LastSnapshot.pbImgData == NULL)
    {       
        iRet = ERROR_CODE_BUFFER_NOTENOUGH;
        WRITE_LOG("finish , get result success, but the image data is not valid, error code = %d.", iRet);
    }
    else
    {
        WRITE_LOG("begin to copy data to buffer.");
        memcpy(ImgBuf, pReult->CIMG_LastSnapshot.pbImgData, pReult->CIMG_LastSnapshot.dwImgSize);
        WRITE_LOG("finish copy data.");
        iRet = 0;
    }

    SAFE_DELETE_OBJ(pReult);

    WRITE_LOG("finish, return code = %d.", iRet);
    return iRet;
}

SCCP_API int DLL_SPEC SCLS_DVR_GetBinaryImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize)
{
    WRITE_LOG(" begin, lHandle = %d, ImgBuf = %p, bufLen = %d, ImgSize = %p", lHandle, ImgBuf, bufLen, ImgSize);
    //int iDeviceID = -1;
    //Camera6467_plate* pCamera = NULL;
    //for (int i = 0; i < MAX_CAMERA_COUNT; i++)
    //{
    //    pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(i);
    //    if (NULL != pCamera)
    //    {
    //        iDeviceID = i;
    //        break;
    //    }
    //}
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera == NULL)
    {
        WRITE_LOG("finish , can not find the camera, please connect first.");
        return ERROR_CODE_DISCONNECT;
    }
    int iRet = -1;
    CameraResult* pReult = pCamera->GetOneResult();
    if (pReult == NULL)
    {
        WRITE_LOG("finish ,result is not ready.");
        return ERROR_CODE_TIMEOUT;
    }
    WRITE_LOG(" get result success, image data = %p, length = %lu.",
        pReult->CIMG_BinImage.pbImgData,
        pReult->CIMG_BinImage.dwImgSize);

    if (pReult->CIMG_BinImage.dwImgSize <= 0
        || pReult->CIMG_BinImage.dwImgSize > bufLen
        || pReult->CIMG_BinImage.pbImgData == NULL)
    {
        iRet = ERROR_CODE_BUFFER_NOTENOUGH;
        WRITE_LOG("finish , get result success, but the image data is not valid, error code = %d.", iRet);
    }
    else
    {
        WRITE_LOG("begin to copy data to buffer.");
        memcpy(ImgBuf, pReult->CIMG_BinImage.pbImgData, pReult->CIMG_BinImage.dwImgSize);
        WRITE_LOG("finish copy data.");
        iRet = 0;
    }

    SAFE_DELETE_OBJ(pReult);

    WRITE_LOG("finish, return code = %d.", iRet);
    return iRet;
}

SCCP_API int DLL_SPEC SCLS_DVR_GetSmallImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize)
{
    WRITE_LOG(" begin,lHandle = %d,  ImgBuf = %p, bufLen = %d, ImgSize = %p", lHandle, ImgBuf, bufLen, ImgSize);
    int iDeviceID = -1;
    Camera6467_plate* pCamera = NULL;
    for (int i = 0; i < MAX_CAMERA_COUNT; i++)
    {
        pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(i);
        if (NULL != pCamera)
        {
            iDeviceID = i;
            break;
        }
    }
    if (pCamera == NULL)
    {
        WRITE_LOG("finish , can not find the camera, please connect first.");
        return ERROR_CODE_DISCONNECT;
    }
    int iRet = -1;
    CameraResult* pReult = pCamera->GetOneResult();
    if (pReult == NULL)
    {
        WRITE_LOG("finish ,result is not ready.");
        return ERROR_CODE_TIMEOUT;
    }
    WRITE_LOG(" get result success, image data = %p, length = %lu.",
        pReult->CIMG_PlateImage.pbImgData,
        pReult->CIMG_PlateImage.dwImgSize);

    if (pReult->CIMG_PlateImage.dwImgSize <= 0
        || pReult->CIMG_PlateImage.dwImgSize > bufLen
        || pReult->CIMG_PlateImage.pbImgData == NULL)
    {
        iRet = ERROR_CODE_BUFFER_NOTENOUGH;
        WRITE_LOG("finish , get result success, but the image data is not valid, error code = %d.", iRet);
    }
    else
    {
        WRITE_LOG("begin to copy data to buffer.");
        memcpy(ImgBuf, pReult->CIMG_PlateImage.pbImgData, pReult->CIMG_PlateImage.dwImgSize);
        WRITE_LOG("finish copy data.");
        iRet = 0;
    }

    SAFE_DELETE_OBJ(pReult);

    WRITE_LOG("finish, return code = %d.", iRet);
    return iRet;
}

SCCP_API int DLL_SPEC SCLS_DVR_GetPlateNo(LONG lHandle, char *pPlateNo, int bufLen, int *plateNoLen)
{
    WRITE_LOG(" begin,lHandle = %d, ImgBuf = %p, bufLen = %d, ImgSize = %p", lHandle, pPlateNo, bufLen, plateNoLen);
    //int iDeviceID = -1;
    //Camera6467_plate* pCamera = NULL;
    //for (int i = 0; i < MAX_CAMERA_COUNT; i++)
    //{
    //    pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(i);
    //    if (NULL != pCamera)
    //    {
    //        iDeviceID = i;
    //        break;
    //    }
    //}
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera == NULL)
    {
        WRITE_LOG("finish , can not find the camera, please connect first.");
        return ERROR_CODE_DISCONNECT;
    }
    int iRet = -1;
    CameraResult* pReult = pCamera->GetOneResult();
    if (pReult == NULL)
    {
        WRITE_LOG("finish ,result is not ready.");
        return ERROR_CODE_TIMEOUT;
    }
    iRet = 0;
    WRITE_LOG(" get result success, plate number = %s, length = %d.", pReult->chPlateNO,  strlen(pReult->chPlateNO)+1);
    if (bufLen >= strlen(pReult->chPlateNO) + 1)
    {
        sprintf_s(pPlateNo, bufLen, "%s", pReult->chPlateNO);
    }
    else
    {
        WRITE_LOG("buffer is not enough,use buffer size.");
        memcpy(pPlateNo, pReult->chPlateNO, bufLen);
        pPlateNo[bufLen] = '\0';
        iRet = ERROR_CODE_BUFFER_NOTENOUGH;
    }
    WRITE_LOG("get plate finish."); 

    SAFE_DELETE_OBJ(pReult);

    WRITE_LOG("finish, return code = %d.", iRet);
    return iRet;
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_ForceCapture(LONG lHandle)
{
    WRITE_LOG(" begin, nHandle = %d .", lHandle);
    if (!DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        WRITE_LOG("camera id is not found.");
        return FALSE;
    }
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera != NULL)
    {
        if ( pCamera->TakeCapture())
        {
            WRITE_LOG("TakeCapture success.");
            return TRUE;
        }
        else
        {
            WRITE_LOG("TakeCapture failed.");
            return FALSE;
        }
    }
    else
    {
        WRITE_LOG("end ,the camera of nHandle is invalid.");
        return FALSE;
    }
}

SCCP_API LONG DLL_SPEC SCLS_DVR_RealPlay(LONG lHandle, HWND FormHwnd)
{
    WRITE_LOG(" begin, nHandle = %d FormHwnd = %p.", lHandle, FormHwnd);
    if (!DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        WRITE_LOG("camera id is not found.");
        return FALSE;
    }
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera != NULL)
    {
        pCamera->StopPlayVideoByChannel(0);
        pCamera->StopPlayVideoByChannel(1);


        pCamera->StartPlayVideoByChannel(g_iVideoChannelID, FormHwnd);

        WRITE_LOG("end ,set play video success.");
        return TRUE;
    }
    else
    {
        WRITE_LOG("end ,the camera of nHandle is invalid.");
        return FALSE;
    }
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_StopRealPlay(LONG lHandle)
{
    WRITE_LOG(" begin, nHandle = %d.", lHandle);
    if (!DeviceListManager::GetInstance()->FindIfExsit(lHandle))
    {
        WRITE_LOG("camera id is not found.");
        return FALSE;
    }
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(lHandle);
    if (pCamera != NULL)
    {
        pCamera->StopPlayVideoByChannel(0);
        pCamera->StopPlayVideoByChannel(1);

        WRITE_LOG("end ,set play video success.");
        return TRUE;
    }
    else
    {
        WRITE_LOG("end ,the camera of nHandle is invalid.");
        return FALSE;
    }
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_StartRecord(LONG lHandle, char *sFileName)
{
    WRITE_LOG("return false directory");
    return FALSE;
}

SCCP_API BOOL DLL_SPEC SCLS_DVR_StopRecord(LONG lHandle)
{
    WRITE_LOG("return false directory");
    return FALSE;
}
