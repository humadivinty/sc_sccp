#ifndef MYH264SAVER_H
#define MYH264SAVER_H

#include "CusH264Struct.h"
#include "CameraModule/ThreadSafeList.h"
#include "libAVI/cAviLib.h"
#include <memory>


#define SAVING_FLAG_NOT_SAVE 0
#define SAVING_FLAG_SAVING 1
#define SAVING_FLAG_SHUT_DOWN 2

class MyH264Saver
{
public:
    MyH264Saver();
    ~MyH264Saver();

    bool addDataStruct(CustH264Struct* pDataStruct);
    bool StartSaveH264(INT64  beginTimeStamp, const char* pchFilePath);
    bool StopSaveH264();

    static DWORD WINAPI  H264DataProceesor( LPVOID lpThreadParameter);
    DWORD processH264Data();

private:
    void SetIfExit(bool bValue);
    bool GetIfExit();

    void SetSaveFlag(int iValue);
    int GetSaveFlag();

    void SetTimeFlag(INT64 iValue);
    INT64 GetTimeFlag();

    void SetIfFirstSave(bool bValue);
    bool GetIfFirstSave();

    void SetSavePath(const char* filePath, size_t bufLength);
    const char* GetSavePath();

private:

    bool m_bExit;    
    bool m_bFirstSave;
    int m_iSaveH264Flag;        //   0--not save  ; 1--saving ; 2--shut down saving
    INT64 m_iTimeFlag;

    char m_chFilePath[256];

    TemplateThreadSafeList<std::shared_ptr<CustH264Struct > > m_lDataStructList;
    CRITICAL_SECTION m_Locker;
    HANDLE m_hThreadSaveH264;
    CAviLib m_264AviLib;
};

#endif // MYH264SAVER_H