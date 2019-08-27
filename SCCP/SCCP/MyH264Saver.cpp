#include "stdafx.h"
#include "MyH264Saver.h"
#include "utilityTool/ToolFunction.h"


MyH264Saver::MyH264Saver():
m_bExit(false),
m_iSaveH264Flag(0),
m_iTimeFlag(0),
m_bFirstSave(false),
//m_pLocker(NULL),
m_hThreadSaveH264(NULL)
{
    InitializeCriticalSection(&m_Locker);

    m_hThreadSaveH264 = CreateThread(NULL, 0, H264DataProceesor, this, 0, NULL);
}


MyH264Saver::~MyH264Saver()
{
    SetIfExit(true);
    SetSaveFlag(false);
    Tool_SafeCloseThread(m_hThreadSaveH264);

    DeleteCriticalSection(&m_Locker);

}

bool MyH264Saver::addDataStruct(CustH264Struct* pDataStruct)
{
    if (pDataStruct == NULL)
    {
        return false;
    }
    std::shared_ptr<CustH264Struct> pData = std::shared_ptr<CustH264Struct>(pDataStruct);
    if (m_lDataStructList.size() > 400)
    {
        m_lDataStructList.pop_front();
    }
    m_lDataStructList.push_back(pData);

    return true;
}

bool MyH264Saver::StartSaveH264(INT64 beginTimeStamp, const char* pchFilePath)
{
    SetSaveFlag(SAVING_FLAG_SAVING);
    SetIfFirstSave(true);
    SetTimeFlag(beginTimeStamp);
    SetSavePath(pchFilePath, strlen(pchFilePath));
    return true;
}

bool MyH264Saver::StopSaveH264()
{
    SetSaveFlag(SAVING_FLAG_SHUT_DOWN);
    return true;
}

DWORD WINAPI MyH264Saver::H264DataProceesor(LPVOID lpThreadParameter)
{
    if (NULL == lpThreadParameter)
    {
        return 0;
    }
    MyH264Saver* pSaver = (MyH264Saver*)lpThreadParameter;
    return pSaver->processH264Data();
}

DWORD MyH264Saver::processH264Data()
{
    int  iSaveFlag = 0;
    int iVideoWidth = -1;
    int iVideoHeight = -1;
    while (!GetIfExit())
    {
        //Sleep(50);
        if (m_lDataStructList.size() <= 0)
        {
            Sleep(50);
            continue;
        }

        iSaveFlag = GetSaveFlag();
        std::shared_ptr<CustH264Struct > pData = nullptr;
        switch (iSaveFlag)
        {
        case SAVING_FLAG_NOT_SAVE:
            Sleep(10);
            break;
        case SAVING_FLAG_SAVING:
            pData = m_lDataStructList.front();
            if (pData == nullptr)
            {
                break;
            }
            m_lDataStructList.pop_front();
            iVideoWidth = pData->m_iWidth;
            iVideoHeight = pData->m_iHeight;
            if (GetIfFirstSave())
            {
                if (!m_264AviLib.IsNULL())
                {
                    m_264AviLib.close();
                }
                if (iVideoWidth > 0
                    && iVideoHeight > 0)
                {
                    m_264AviLib.setAviInfo((char*)GetSavePath(), iVideoWidth, iVideoHeight, 25, "H264");
                    SetIfFirstSave(false);
                }
            }
            if (!m_264AviLib.IsNULL()
                && pData->m_llFrameTime > GetTimeFlag()
                && 0== m_264AviLib.writeFrame((char*)pData->m_pbH264FrameData, pData->m_iDataSize, pData->m_isIFrame))
            {
                //SetIfFirstSave(false);
            }
            break;
        case SAVING_FLAG_SHUT_DOWN:
            if (!m_264AviLib.IsNULL())
            {
                m_264AviLib.close();
                SetSaveFlag(SAVING_FLAG_NOT_SAVE);
            }
            break;
        default:
            break;
        }       

    }
    return 0;
}

void MyH264Saver::SetIfExit(bool bValue)
{
    EnterCriticalSection(&m_Locker);
    m_bExit = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfExit()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bExit;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSaveFlag(int iValue)
{
    EnterCriticalSection(&m_Locker);
    m_iSaveH264Flag = iValue;
    LeaveCriticalSection(&m_Locker);
}

int MyH264Saver::GetSaveFlag()
{
    int iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iSaveH264Flag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetTimeFlag(INT64 iValue)
{
    EnterCriticalSection(&m_Locker);
    m_iTimeFlag = iValue;
    LeaveCriticalSection(&m_Locker);
}

INT64 MyH264Saver::GetTimeFlag()
{
    INT64 iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iTimeFlag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetIfFirstSave(bool bValue)
{
    EnterCriticalSection(&m_Locker);
    m_bFirstSave = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfFirstSave()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bFirstSave;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSavePath(const char* filePath, size_t bufLength)
{
    EnterCriticalSection(&m_Locker);
    if (bufLength < sizeof(m_chFilePath))
    {
        memcpy(m_chFilePath, filePath, bufLength);
        m_chFilePath[bufLength] = '\0';
    }
    LeaveCriticalSection(&m_Locker);
}

const char* MyH264Saver::GetSavePath()
{
    const char* pValue = NULL;
    EnterCriticalSection(&m_Locker);
    pValue = m_chFilePath;
    LeaveCriticalSection(&m_Locker);
    return pValue;
}
