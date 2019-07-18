// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "coredump/MiniDumper.h"

bool  g_bLogEnable = false;
int g_iVideoChannelID = 0;
CMiniDumper g_MiniDumper(true);

bool g_ReadConfig();
void g_WriteConfig();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    char chLog[256] = { 0 };
    DWORD TID = 0;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        TID = GetCurrentThreadId();

        sprintf_s(chLog, "Video_car DLL_PROCESS_ATTACH current thread ID = %lu\n", TID);
        OutputDebugStringA(chLog);
        g_ReadConfig();
        break;
	case DLL_THREAD_ATTACH:
        TID = GetCurrentThreadId();
        sprintf_s(chLog, "Video_car  DLL_THREAD_ATTACH current thread ID = %lu\n", TID);
        OutputDebugStringA(chLog);
        break;
	case DLL_THREAD_DETACH:
        TID = GetCurrentThreadId();
        sprintf_s(chLog, "Video_car  DLL_THREAD_DETACH current thread ID = %lu\n", TID);
        OutputDebugStringA(chLog);
        break;
	case DLL_PROCESS_DETACH:
        TID = GetCurrentThreadId();
        sprintf_s(chLog, "Video_car  DLL_PROCESS_DETACH current thread ID = %lu\n", TID);
        OutputDebugStringA(chLog);
		break;
	}
	return TRUE;
}


bool g_ReadConfig()
{
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);

    PathRemoveFileSpecA(FileName);
    char iniFileName[MAX_PATH] = { 0 };
    char iniDeviceInfoName[MAX_PATH] = { 0 };
    strcat_s(iniFileName, FileName);
    //strcat_s(iniFileName,"\\XLW_Plate.ini");
    strcat_s(iniFileName, INI_FILE_NAME);

    //读取可靠性配置文件
    int iLogEnable = GetPrivateProfileIntA("Log", "Enable", 0, iniFileName);
    if (iLogEnable == 0)
    {
        g_bLogEnable = false;
    }
    else
    {
        g_bLogEnable = true;
    }
    //g_iBackupCount = GetPrivateProfileIntA("Log", "BackupCount", 1, iniFileName);

    g_iVideoChannelID = GetPrivateProfileIntA("Video", "channelID", 0, iniFileName);
    return true;
}

void g_WriteConfig()
{
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);

    PathRemoveFileSpecA(FileName);
    char iniFileName[MAX_PATH] = { 0 };
    char iniDeviceInfoName[MAX_PATH] = { 0 };
    strcat_s(iniFileName, FileName);
    //strcat_s(iniFileName,"\\XLW_Plate.ini");
    strcat_s(iniFileName, INI_FILE_NAME);

    char chTemp[260] = { 0 };
    //sprintf_s(chTemp, "%d", g_iBackupCount);
    //WritePrivateProfileStringA("Log", "BackupCount", chTemp, iniFileName);

    memset(chTemp, 0, sizeof(chTemp));
    sprintf_s(chTemp, "%d", g_bLogEnable);
    WritePrivateProfileStringA("Log", "Enable", chTemp, iniFileName);

    memset(chTemp, 0, sizeof(chTemp));
    sprintf_s(chTemp, "%d", g_iVideoChannelID);
    WritePrivateProfileStringA("Video", "channelID", chTemp, iniFileName);
}