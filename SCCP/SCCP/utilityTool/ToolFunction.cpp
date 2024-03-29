#include "stdafx.h"
#include "ToolFunction.h"
#include <string>
#include<shellapi.h>
//#include <afx.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")

#define MAX_LOG_FILE_SIZE (8*1024*1024)

TiXmlElement Tool_SelectElementByName(const char* InputInfo, char* pName, int iXMLType)
{
    //注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;
    if (iXMLType == 1)
    {
        if (!cXmlDoc.LoadFile(InputInfo))
        {
            printf("parse XML file failed \n");
            return TiXmlElement("");
        }
    }
    else if (iXMLType == 2)
    {
        if (!cXmlDoc.Parse(InputInfo))
        {
            printf("parse XML failed \n");
            return TiXmlElement("");
        }
    }

    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        printf("no have root Element\n");
        return TiXmlElement("");
    }
    else
    {
        TiXmlElement* pTempElement = NULL;
        pTempElement = Tool_ReadElememt(pRootElement, pName);
        if (pTempElement)
        {
            printf("find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
            return *pTempElement;
        }
        else
        {
            return TiXmlElement("");
        }
    }
}

TiXmlElement* Tool_ReadElememt(TiXmlElement* InputElement, char* pName)
{
    TiXmlElement* ptemp = NULL;
    if (InputElement && 0 == strcmp(pName, InputElement->Value()))
    {
        printf("Find the element :%s \n", InputElement->Value());
        ptemp = InputElement;
        return ptemp;
    }
    else
    {
        printf("%s \n", InputElement->Value());
    }

    TiXmlElement* tmpElement = InputElement;
    if (tmpElement->FirstChildElement())
    {
        ptemp = Tool_ReadElememt(tmpElement->FirstChildElement(), pName);
    }
    if (!ptemp)
    {
        tmpElement = tmpElement->NextSiblingElement();
        if (tmpElement)
        {
            ptemp = Tool_ReadElememt(tmpElement, pName);
        }
    }
    return ptemp;
}

void Tool_ReadKeyValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
    if (strlen(keyValue) > bufferSize)
    {
        return;
    }
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);
    PathRemoveFileSpecA(FileName);
    char iniFileName[MAX_PATH] = { 0 };
    MY_SPRINTF(iniFileName, sizeof(iniFileName), "%s\\%s", FileName, IniFileName);

    char chTemp[256] = { 0 };
    GetPrivateProfileStringA(nodeName, keyName, "0", chTemp, bufferSize, iniFileName);
    if (strcmp(chTemp, "0") == 0)
    {
        WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
    }
    else
    {
        strcpy_s(keyValue, bufferSize, chTemp);
    }
}

void Tool_ReadIntValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, int&keyValue)
{
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);
    PathRemoveFileSpecA(FileName);
    char iniFileName[MAX_PATH] = { 0 };
    MY_SPRINTF(iniFileName, sizeof(iniFileName), "%s\\%s", FileName, IniFileName);

    int iValue = GetPrivateProfileIntA(nodeName, keyName, keyValue, iniFileName);
    keyValue = iValue;

    char chTemp[128] = { 0 };
    sprintf_s(chTemp, sizeof(chTemp), "%d", iValue);
    WritePrivateProfileStringA(nodeName, keyName, chTemp, iniFileName);
}

void Tool_WriteKeyValueFromConfigFile(const char* INIFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
    if (strlen(keyValue) > bufferSize)
    {
        return;
    }
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);
    PathRemoveFileSpecA(FileName);

    char iniFileName[MAX_PATH] = { 0 };
    strcat_s(iniFileName, FileName);
    strcat_s(iniFileName, INIFileName);

    //GetPrivateProfileStringA(nodeName, keyName, "172.18.109.97", keyValue, bufferSize, iniFileName);

    WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
}

int Tool_checkIP(const char* p)
{
    int n[4];
    char c[4];
    //if (sscanf(p, "%d%c%d%c%d%c%d%c",
    //	&n[0], &c[0], &n[1], &c[1],
    //	&n[2], &c[2], &n[3], &c[3])
    //	== 7)
    if (sscanf_s(p, "%d%c%d%c%d%c%d%c",
        &n[0], &c[0], 1,
        &n[1], &c[1], 1,
        &n[2], &c[2], 1,
        &n[3], &c[3], 1)
        == 7)
    {
        int i;
        for (i = 0; i < 3; ++i)
        if (c[i] != '.')
            return 0;
        for (i = 0; i < 4; ++i)
        if (n[i] > 255 || n[i] < 0)
            return 0;
        if (n[0] == 0 && n[1] == 0 && n[2] == 0 && n[3] == 0)
        {
            return 0;
        }
        return 1;
    }
    else
        return 0;
}

bool Tool_IsFileExist(const char* FilePath)
{
    if (FilePath == NULL)
    {
        return false;
    }
    FILE* tempFile = NULL;
    bool bRet = false;
    //tempFile = fopen(FilePath, "r");
    fopen_s(&tempFile, FilePath, "r");
    if (tempFile)
    {
        bRet = true;
        fclose(tempFile);
        tempFile = NULL;
    }
    return bRet;
}

bool Tool_MakeDir(const char* chImgPath)
{
    if (NULL == chImgPath)
    {
        //WriteLog("the path is null ,Create Dir failed.");
        return false;
    }
    std::string tempFile(chImgPath);
    size_t iPosition = tempFile.rfind("\\");
    std::string tempDir = tempFile.substr(0, iPosition + 1);
    if (MakeSureDirectoryPathExists(tempDir.c_str()))
    {
        return true;
    }
    else
    {
        //WriteLog("Create Dir failed.");
        return false;
    }
}

long Tool_GetFileSize(const char *FileName)
{
    //FILE* tmpFile = fopen(FileName, "rb");
    FILE* tmpFile = NULL;
    fopen_s(&tmpFile, FileName, "rb");
    if (tmpFile)
    {
        fseek(tmpFile, 0, SEEK_END);
        long fileSize = ftell(tmpFile);
        fclose(tmpFile);
        tmpFile = NULL;
        return fileSize;
    }
    else
    {
        //"open file failed.";
        return 0;
    }
}

bool Tool_PingIPaddress(const char* IpAddress)
{
    //FILE* pfile;
    //char chBuffer[1024] = {0};
    char chCMD[256] = { 0 };
    sprintf_s(chCMD, sizeof(chCMD), "ping %s -n 1", IpAddress);
    //std::string strPingResult;
    //pfile = _popen(chCMD, "r");
    //if (pfile != NULL)
    //{
    //	while(fgets(chBuffer, 1024, pfile) != NULL)
    //	{
    //		strPingResult.append(chBuffer);
    //	}
    //}
    //else
    //{
    //	printf("popen failed. \n");
    //	return false;
    //}
    //_pclose(pfile);
    //printf("%s", strPingResult.c_str());
    //if (std::string::npos != strPingResult.find("TTL") || std::string::npos != strPingResult.find("ttl"))
    //{
    //	return true;
    //}
    //else
    //{
    //	return false;
    //}


    char pbuf[1024]; // 缓存  
    DWORD len;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE hRead1 = NULL, hWrite1 = NULL;  // 管道读写句柄  
    BOOL b;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE; // 管道句柄是可被继承的  
    saAttr.lpSecurityDescriptor = NULL;

    // 创建匿名管道，管道句柄是可被继承的  
    b = CreatePipe(&hRead1, &hWrite1, &saAttr, 1024);
    if (!b)
    {
        //MessageBox(hwnd, "管道创建失败。","Information",0);  
        printf("管道创建失败\n");
        return false;
    }

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite1; // 设置需要传递到子进程的管道写句柄  


    // 创建子进程，运行ping命令，子进程是可继承的  
    if (!CreateProcess(NULL, chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        //itoa(GetLastError(), pbuf, 10); 
        sprintf_s(pbuf, sizeof(pbuf), "%d", GetLastError());
        //MessageBox(hwnd, pbuf,"Information",0);
        printf("%s\n", pbuf);
        CloseHandle(hRead1);
        hRead1 = NULL;
        CloseHandle(hWrite1);
        hWrite1 = NULL;
        return false;
    }

    // 写端句柄已被继承，本地则可关闭，不然读管道时将被阻塞  
    CloseHandle(hWrite1);
    hWrite1 = NULL;

    // 读管道内容，并用消息框显示  
    len = 1000;
    DWORD l;

    std::string strInfo;
    while (ReadFile(hRead1, pbuf, len, &l, NULL))
    {
        if (l == 0) break;
        pbuf[l] = '\0';
        //MessageBox(hwnd, pbuf, "Information",0);  
        //printf("Information2:\n%s\n", pbuf);
        strInfo.append(pbuf);
        len = 1000;
    }

    //MessageBox(hwnd, "ReadFile Exit","Information",0);  
    printf("finish ReadFile buffer = %s\n", strInfo.c_str());
    CloseHandle(hRead1);
    hRead1 = NULL;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    pi.hThread = NULL;
    CloseHandle(pi.hProcess);
    pi.hProcess = NULL;

    if (std::string::npos != strInfo.find("TTL") || std::string::npos != strInfo.find("ttl"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality)
{
    if (pbSrc == NULL || iSrcLen <= 0)
    {
        return false;
    }
    if (pbDst == NULL || iDstLen == NULL || *iDstLen <= 0)
    {
        return false;
    }
    if (iDstWidth <= 0 || iDstHeight <= 0)
    {
        return false;
    }

    // init gdi+
    ULONG_PTR gdiplusToken = NULL;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建流
    IStream *pstmp = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pstmp);
    if (pstmp == NULL)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 初始化流
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };
    pstmp->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pstmp->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    pstmp->Write(pbSrc, iSrcLen, &ulRealSize);

    // 从流创建位图
    Bitmap bmpSrc(pstmp);
    Bitmap bmpDst(iDstWidth, iDstHeight, PixelFormat24bppRGB);

    // 创建画图对象
    Graphics grDraw(&bmpDst);

    // 绘图
    //grDraw.DrawImage(&bmpSrc, 0, 0, bmpSrc.GetWidth(), bmpSrc.GetHeight());
    Rect destRect(0, 0, iDstWidth, iDstHeight);
    grDraw.DrawImage(&bmpSrc, destRect);
    if (Ok != grDraw.GetLastStatus())
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 创建输出流
    IStream* pStreamOut = NULL;
    if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK)
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);

    // 初始化输出流
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将位图按照JPG的格式保存到输出流中
    int iQuality = compressQuality % 100;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    //bmpDst.Save(pStreamOut, &jpgClsid, 0);

    // 获取输出流大小
    bool bRet = false;
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小 
    if (*iDstLen < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        *iDstLen = libNewPos.LowPart;
        bRet = false;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(pbDst, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        *iDstLen = ulRealSize;
        bRet = true;
    }


    // 释放内存
    if (pstmp != NULL)
    {
        pstmp->Release();
        pstmp = NULL;
    }
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    GdiplusShutdown(gdiplusToken);
    gdiplusToken = NULL;

    return bRet;
}

int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }
    free(pImageCodecInfo);
    return -1;  // Failure
}

void Tool_ExcuteShellCMD(char* pChCommand)
{
    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);
}

bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality)
{
    if (!pImgsrc || !pImgDest || srcSize <= 0 || DestSize <= 0)
    {
        //WriteLog("传入参数为非法值");
        return false;
    }
    if (wcslen(DestString) <= 0 || x < 0 || y < 0)
    {
        //WriteLog("字符串长度为0");
        return false;
    }

    //构造图像	
    IStream *pSrcStream = NULL;
    IStream *pDestStream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pSrcStream);
    CreateStreamOnHGlobal(NULL, TRUE, &pDestStream);
    if (!pSrcStream || !pDestStream)
    {
        //WriteLog("流创建失败.");
        return false;
    }
    LARGE_INTEGER liTemp = { 0 };
    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pSrcStream->Write(*pImgsrc, srcSize, NULL);
    Bitmap bmp(pSrcStream);
    int iImgWith = bmp.GetWidth();
    int iImgHeight = bmp.GetHeight();

    Graphics grp(&bmp);

    SolidBrush brush(Color(colorR, colorG, colorB));
    FontFamily fontFamily(L"宋体");
    //Gdiplus::Font font(&fontFamily, (REAL)FontSize);
    Gdiplus::Font font(&fontFamily, (REAL)FontSize, FontStyleRegular, UnitPixel);

    RectF layoutRect(x, y, iImgWith - x, 0);
    RectF FinalRect;
    INT codePointsFitted = 0;
    INT linesFitted = 0;
    int strLenth = wcslen(DestString);
    grp.MeasureString(DestString, strLenth, &font, layoutRect, NULL, &FinalRect, &codePointsFitted, &linesFitted);
    grp.DrawString(DestString, -1, &font, FinalRect, NULL, &brush);
    Gdiplus::Status iState = grp.GetLastStatus();
    if (iState == Ok)
    {
        //WriteLog("字符叠加成功");
    }
    else
    {
        //char chLog[260] = { 0 };
        //sprintf(chLog, "字符叠加失败， 错误码为%d", iState);
        //WriteLog(chLog);
    }

    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);

    // 将位图按照JPG的格式保存到输出流中
    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    int iQuality = compressQuality;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmp.Save(pDestStream, &jpgClsid, &encoderParameters);

    ULARGE_INTEGER uiSize;
    pDestStream->Seek(liTemp, STREAM_SEEK_CUR, &uiSize);
    long iFinalSize = (long)uiSize.QuadPart;
    if (iFinalSize <= DestSize)
    {
        pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
        pDestStream->Read(*pImgDest, iFinalSize, NULL);
        DestSize = iFinalSize;
    }
    else
    {
        DestSize = 0;
        if (pSrcStream)
        {
            pSrcStream->Release();
            pSrcStream = NULL;
        }
        if (pDestStream)
        {
            pDestStream->Release();
            pDestStream = NULL;
        }
        //WriteLog("传入空间不足，字符叠加失败");
        return false;
    }

    if (pSrcStream)
    {
        pSrcStream->Release();
        pSrcStream = NULL;
    }
    if (pDestStream)
    {
        pDestStream->Release();
        pDestStream = NULL;
    }
    return true;
}

bool Tool_GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    std::string strAppendInfo = pszAppendInfo;
    size_t siStart = strAppendInfo.find(strItemName);
    if (siStart == std::string::npos)
    {
        return false;
    }
    siStart = strAppendInfo.find("\"", siStart + 1);
    if (siStart == std::string::npos)
    {
        return false;
    }
    size_t siEnd = strAppendInfo.find("\"", siStart + 1);
    if (siEnd == std::string::npos)
    {
        return false;
    }

    std::string strRst = strAppendInfo.substr(siStart + 1, siEnd - siStart - 1);
    if (*piRstBufLen < (int)strRst.length())
    {
        *piRstBufLen = (int)strRst.length();
        return false;
    }

    strncpy_s(pszRstBuf, *piRstBufLen, strRst.c_str(), (int)strRst.length());
    *piRstBufLen = (int)strRst.length();
    return true;
}

void Tool_ExcuteCMD(char* pChCommand)
{
#ifdef WIN32

    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);

#endif // WIN32
}

std::wstring Img_string2wstring(std::string strSrc)
{
    std::wstring wstrDst;
    int iWstrLen = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), NULL, 0);
    wchar_t* pwcharBuf = new wchar_t[iWstrLen + sizeof(wchar_t)];   // 多一个结束符
    if (pwcharBuf == NULL || iWstrLen <= 0)
    {
        return L"";
    }
    memset(pwcharBuf, 0, iWstrLen*sizeof(wchar_t)+sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), pwcharBuf, iWstrLen);
    pwcharBuf[iWstrLen] = L'\0';
    wstrDst.append(pwcharBuf);
    delete[] pwcharBuf;
    pwcharBuf = NULL;
    return wstrDst;
}

#ifdef USE_MFC
bool DeleteDirectory(char* strDirName)
{
    CFileFind tempFind;

    char strTempFileFind[MAX_PATH];

    MY_SPRINTF(strTempFileFind, sizeof(strTempFileFind), "%s//*.*", strDirName);

    BOOL IsFinded = tempFind.FindFile(strTempFileFind);

    while (IsFinded)
    {
        IsFinded = tempFind.FindNextFile();

        if (!tempFind.IsDots())
        {
            char strFoundFileName[MAX_PATH];

            //strcpy(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
            strcpy_s(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

            if (tempFind.IsDirectory())
            {
                char strTempDir[MAX_PATH];

                MY_SPRINTF(strTempDir, sizeof(strTempDir), "%s//%s", strDirName, strFoundFileName);

                DeleteDirectory(strTempDir);
            }
            else
            {
                char strTempFileName[MAX_PATH];

                MY_SPRINTF(strTempFileName, sizeof(strTempFileName), "%s//%s", strDirName, strFoundFileName);

                DeleteFile(strTempFileName);
            }
        }
    }

    tempFind.Close();

    if (!RemoveDirectory(strDirName))
    {
        return FALSE;
    }

    return TRUE;
}

int CirclelaryDelete(const char* folderPath, int iBackUpDays)
{
    printf("进入环覆盖线程主函数,开始查找制定目录下的文件夹");
    char myPath[MAX_PATH] = { 0 };
    //sprintf(myPath, "%s\\*", folderPath);
    MY_SPRINTF(myPath,sizeof(myPath),  "%s\\*", folderPath);

    CTime tmCurrentTime = CTime::GetCurrentTime();
    CTime tmLastMonthTime = tmCurrentTime - CTimeSpan(iBackUpDays, 0, 0, 0);
    int Last_Year = tmLastMonthTime.GetYear();
    int Last_Month = tmLastMonthTime.GetMonth();
    int Last_Day = tmLastMonthTime.GetDay();
    //cout<<Last_Year<<"-"<<Last_Month<<"-"<<Last_Day<<endl;

    CFileFind myFileFind;
    BOOL bFinded = myFileFind.FindFile(myPath);
    char DirectoryName[MAX_PATH] = { 0 };
    while (bFinded)
    {
        bFinded = myFileFind.FindNextFileA();
        if (!myFileFind.IsDots())
        {
            MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s", myFileFind.GetFileName().GetBuffer());
            if (myFileFind.IsDirectory())
            {
                int iYear, iMonth, iDay;
                iYear = iMonth = iDay = 0;
                //sscanf(DirectoryName,"%d-%d-%d",&iYear, &iMonth, &iDay);
                sscanf_s(DirectoryName, "%d-%d-%d", &iYear, &iMonth, &iDay);
                if (iYear == 0 && iMonth ==0 && iDay == 0)
                {
                    continue;
                }
                if (iYear < Last_Year)
                {
                    MY_SPRINTF(DirectoryName,sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                    printf("delete the DirectoryB :%s\n", DirectoryName);
                    DeleteDirectory(DirectoryName);

                    char chLog[MAX_PATH] = { 0 };
                    MY_SPRINTF(chLog,sizeof(chLog), "年份小于当前年份，删除文件夹%s", DirectoryName);
                    printf(chLog);
                }
                else if (iYear == Last_Year)
                {
                    if (iMonth < Last_Month)
                    {
                        MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                        printf("delete the DirectoryB :%s\n", DirectoryName);
                        DeleteDirectory(DirectoryName);

                        char chLog[MAX_PATH] = { 0 };
                        MY_SPRINTF(chLog,sizeof(chLog), "月份小于上一月，删除文件夹%s", DirectoryName);
                        printf(chLog);
                    }
                    else if (iMonth == Last_Month)
                    {
                        if (iDay < Last_Day)
                        {
                            MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                            printf("delete the DirectoryB :%s\n", DirectoryName);
                            DeleteDirectory(DirectoryName);

                            char chLog[MAX_PATH] = { 0 };
                            MY_SPRINTF(chLog, sizeof(chLog), "日号小于指定天数，删除文件夹%s", DirectoryName);
                            printf(chLog);
                        }
                    }
                }
            }
        }
    }
    myFileFind.Close();
    printf("查询结束，退出环覆盖线程主函数..");
    return 0;
}

#endif

int Tool_SafeCloseThread(HANDLE& threadHandle)
{
    if (threadHandle == NULL)
    {
        return -1;
    }
    MSG msg;
    DWORD dwRet = -1;
    while (NULL != threadHandle && WAIT_OBJECT_0 != dwRet) // INFINITE
    {
        dwRet = MsgWaitForMultipleObjects(1, &threadHandle, FALSE, 100, QS_ALLINPUT);
        if (dwRet == WAIT_OBJECT_0 + 1)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    CloseHandle(threadHandle);
    threadHandle = NULL;
    return 0;
}

const TCHAR* Tool_GetCurrentPath()
{
    static TCHAR szPath[256] = { 0 };
    if (strlen(szPath) <= 0)
    {
        GetModuleFileName(NULL, szPath, MAX_PATH - 1);
        PathRemoveFileSpec(szPath);
    }
    return szPath;
}

SYSTEMTIME Tool_GetCurrentTime()
{
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间
    return systime;
}


bool Tool_DimCompare(const char *szSrcPlateNo, const char *szDesPlateNo)
{

    if (!szSrcPlateNo || !szDesPlateNo)
        return false;

    if (strstr(szSrcPlateNo, "无") || strstr(szDesPlateNo, "无"))
    {
        printf("Info: NoPlate not Compare!!!!!!!!!!");
        return false;
    }
    char chLog[MAX_PATH] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "DimCompare(%s, %s)", szSrcPlateNo, szDesPlateNo);
    printf(chLog);
    //获取6字节标准车牌
    char strStandardCarChar[10] = { 0 };
    int  nPlateNo = strlen(szSrcPlateNo);
    if (nPlateNo > 10)
    {
        printf("Error: szPlateNo!!!!!!!!!!");
        return false;
    }
    else if (nPlateNo > 6)
    {
        strcpy_s(strStandardCarChar, szSrcPlateNo + 2);
        //WriteLog("strcpy iCurPlateNo > 6 ");
    }
    else
    {
        strcpy_s(strStandardCarChar, szSrcPlateNo);
        //WriteLog("strcpy iCurPlateNo <= 6 ");
    }

    int iMaxMatchCnt = 0;
    int iMaxMatchRate = 0;
    int iStandardPlateLen = strlen(strStandardCarChar);//比对车牌长度
    int i = 0;
    int j = 0;
    int nFlagCompare = -1;
    char strComparePlateChar[10] = { 0 };
    int  iComparePlateLen = 0;
    int  iCurPlateNo = 0;

    iCurPlateNo = strlen(szDesPlateNo);
    if (iCurPlateNo > 10)
    {
        printf("Error: szPlateNo!!!!!!!!!!");
        return false;
    }
    else if (iCurPlateNo > 6)
    {
        strcpy_s(strComparePlateChar, szDesPlateNo + 2);
        //WriteLog("strcpy iCurPlateNo > 6 ");
    }
    else
    {
        strcpy_s(strComparePlateChar, szDesPlateNo);
        //WriteLog("strcpy iCurPlateNo <= 6 ");
    }

    //取出list中需要作对比的牌号字符数组
    iComparePlateLen = strlen(strComparePlateChar);//list选中车牌长度

    //取少位数的来遍历,同位匹配
    int iLoopTimes = iComparePlateLen < iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
    int iEqualCount = 0;
    for (j = 0; j < iLoopTimes; j++)
    {
        if (strComparePlateChar[j] == strStandardCarChar[j])
        {
            ++iEqualCount; //匹配数
        }
    }

    if (iEqualCount >= 5 && iEqualCount > iMaxMatchCnt) //车牌匹配5个或以上，算是匹配了，但仍然循环完，已查找最佳匹配率的记录
    {
        iMaxMatchCnt = iEqualCount;

        int iDenominator = iComparePlateLen > iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
        int iMatchRate = (int)(iMaxMatchCnt * 100 / iDenominator);

        if (iMatchRate > iMaxMatchRate)
        {
            iMaxMatchRate = iMatchRate;
            nFlagCompare = i;
        }
    }

    //同位匹配5个以上直接认为是同个车牌
    if (nFlagCompare != -1)
    {
        printf("nFlagCompare != -1（同位匹配5个以上直接认为是同个车牌） return true.");
        return true;
    }

    //同位匹配不上,只能继续错位匹配
    iMaxMatchCnt = 0;
    iMaxMatchRate = 0;
    nFlagCompare = -1;
    iLoopTimes = 4; //错位比较中间连续的4位
    if (iStandardPlateLen < 6 || iComparePlateLen < 6)
    {
        printf("iStandardPlateLen < 6 or iComparePlateLen < 6");
        return false;
    }

    //A12345
    //假如车辆有三辆，分别为粤G15678、川B23456、云C12456
    int iEqualCount1 = 0, iEqualCount2 = 0;
    bool bCompare = false;
    for (j = 0; j < iLoopTimes; j++)
    {
        // A1234与15678、23456、12456比较,后5位与主的前5位比较,但中间连续4位必须全匹配
        if (strComparePlateChar[j + 2] == strStandardCarChar[j + 1])
        {
            ++iEqualCount1; //匹配数
            if (iEqualCount1 == iLoopTimes) bCompare = true;
        }

        // 12345与G15678、B23456、C12456比较,前5位与主的后5位比较,但中间连续4位必须全匹配
        if (strComparePlateChar[j] == strStandardCarChar[j + 1])
        {
            ++iEqualCount2; //匹配数
            if (iEqualCount2 == iLoopTimes) bCompare = true;
        }
    }
    if (strComparePlateChar[1] == strStandardCarChar[0])
        ++iEqualCount1;
    if (strComparePlateChar[4] == strStandardCarChar[5])
        ++iEqualCount2;


    //车牌匹配4个或以上，算是匹配了，但仍然循环完，已查找最佳匹配率的记录
    if ((iEqualCount1 >= 4 && iEqualCount1 > iMaxMatchCnt && bCompare)
        || (iEqualCount2 >= 4 && iEqualCount2 > iMaxMatchCnt && bCompare))
    {
        iMaxMatchCnt = iEqualCount1 > iEqualCount2 ? iEqualCount1 : iEqualCount2;

        int iDenominator = iComparePlateLen > iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
        int iMatchRate = (int)(iMaxMatchCnt * 100 / iDenominator);

        if (iMatchRate > iMaxMatchRate)
        {
            iMaxMatchRate = iMatchRate;
            nFlagCompare = i;
        }
    }
    if (nFlagCompare == -1)
    {
        printf("DimCompare failed.\n");
        return false;
    }
    else
    {
        printf("DimCompare success.\n");
        return true;
    }
}

void Tool_WriteLog(const char* chlog)
{
    //取得当前的精确毫秒的时间
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间

    char chLogPath[512] = { 0 };

    char chLogRoot[256] = { 0 };
    Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Log", "Path", chLogRoot, sizeof(chLogRoot));
    if (strlen(chLogRoot) > 0)
    {
        sprintf_s(chLogPath, sizeof(chLogPath), "%s\\%04d-%02d-%02d\\",
            chLogRoot,
            systime.wYear,
            systime.wMonth,
            systime.wDay);
    }
    else
    {
        sprintf_s(chLogPath, sizeof(chLogPath), "%s\\XLWLog\\%04d-%02d-%02d\\",
            Tool_GetCurrentPath(),
            systime.wYear,
            systime.wMonth,
            systime.wDay);
    }
    MakeSureDirectoryPathExists(chLogPath);

    //每次只保留10天以内的日志文件
    CTime tmCurrentTime = CTime::GetCurrentTime();
    CTime tmLastMonthTime = tmCurrentTime - CTimeSpan(30, 0, 0, 0);
    int Last_Year = tmLastMonthTime.GetYear();
    int Last_Month = tmLastMonthTime.GetMonth();
    int Last_Day = tmLastMonthTime.GetDay();

    char chOldLogFileName[MAX_PATH] = { 0 };
    //sprintf_s(chOldLogFileName, "%s\\XLWLog\\%04d-%02d-%02d\\",szFileName, Last_Year, Last_Month, Last_Day);
    sprintf_s(chOldLogFileName, sizeof(chOldLogFileName), "%s\\XLWLog\\%04d-%02d-%02d\\",
        Tool_GetCurrentPath(),
        Last_Year,
        Last_Month,
        Last_Day);

    if (PathFileExists(chOldLogFileName))
    {
        char chCommand[512] = { 0 };
        //sprintf_s(chCommand, "/c rd /s/q %s", chOldLogFileName);
        sprintf_s(chCommand, sizeof(chCommand), "/c rd /s/q %s", chOldLogFileName);
        Tool_ExcuteCMD(chCommand);
    }

    char chLogFileName[512] = { 0 };
    //sprintf_s(chLogFileName, "%s\\CameraLog-%d-%02d_%02d.log",chLogPath, pTM->tm_year + 1900, pTM->tm_mon+1, pTM->tm_mday);
    sprintf_s(chLogFileName, sizeof(chLogFileName), "%s\\%s", chLogPath, DLL_LOG_NAME);

    FILE *file = NULL;
    //file = fopen(chLogFileName, "a+");
    fopen_s(&file, chLogFileName, "a+");
    if (file)
    {
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d:%03d [DLLVersion:%s] : %s\n",
            systime.wYear,
            systime.wMonth,
            systime.wDay,
            systime.wHour,
            systime.wMinute,
            systime.wSecond,
            systime.wMilliseconds,
            DLL_VERSION,
            chlog);
        fclose(file);
        file = NULL;
    }
}

void Tool_WriteBackupLog(const char* chLog)
{
    int iLogEnable = 0;
    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Log", "gLogEnable", iLogEnable);
    //if (iLogEnable == 0)
    //{
    //    return;
    //}

    //取得当前的精确毫秒的时间
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间

    char chLogPath[512] = { 0 };
    //char chBackupPath[256] = {0};
    char chLogRoot[256] = { 0 };
    Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Log", "Path", chLogRoot, sizeof(chLogRoot));
    if (strlen(chLogRoot) > 0)
    {
        sprintf_s(chLogPath, sizeof(chLogPath), "%s\\", chLogRoot);
    }
    else
    {
        sprintf_s(chLogPath, sizeof(chLogPath), "%s\\XLWLog\\", Tool_GetCurrentPath());
    }

    int iLastBackUpNum = 0;
    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Log", "LastBackUpNum", iLastBackUpNum);
    [&iLastBackUpNum](int& iCount) ->void
    {
        if (iCount <= 0)
        {
            iCount = 30;
        }
        else if (iCount >= 30)
        {
            iCount = 1;
        }
        else
        {
             --iCount;
        }
    };
    char chLastBackUpLogFileName[256] = {0};
    sprintf_s(chLastBackUpLogFileName, sizeof(chLastBackUpLogFileName), "%s/AVC_XLW%02d.log", chLogPath, iLastBackUpNum);

    static int s_iTryTime = 0;
    FILE* pfLogFile = NULL;
    char chLogName[256] = {0};
    sprintf_s(chLogName, sizeof(chLogName), "%s/AVC_XLW.log", chLogPath);
    if (s_iTryTime++ > 100
        &&Tool_GetFileSize(chLogName) >= MAX_LOG_FILE_SIZE
        && CopyFile(chLogName, chLastBackUpLogFileName, FALSE))
    {        
        s_iTryTime = 0;

        char chTemp[32] = {0};
        sprintf_s(chTemp, sizeof(chTemp), "%d", iLastBackUpNum);
        Tool_WriteKeyValueFromConfigFile(INI_FILE_NAME, "Log", "LastBackUpNum", chTemp, sizeof(chTemp));

        fopen_s(&pfLogFile, chLogName, "wb");
    }
    else
    {
        fopen_s(&pfLogFile, chLogName, "ab");
    }
    
    if (pfLogFile)
    {
        fprintf(pfLogFile, "%04d-%02d-%02d %02d:%02d:%02d:%03d [%s] : %s\n",
            systime.wYear,
            systime.wMonth,
            systime.wDay,
            systime.wHour,
            systime.wMinute,
            systime.wSecond,
            systime.wMilliseconds,
            DLL_VERSION,
            chLog);
        fclose(pfLogFile);
        pfLogFile = NULL;
    }
}

void Tool_WriteFormatLog(const char* szfmt, ...)
{
    static char g_szPbString[10240] = { 0 };
    memset(g_szPbString, 0, sizeof(g_szPbString));

    va_list arg_ptr;
    va_start(arg_ptr, szfmt);
    vsnprintf_s(g_szPbString, sizeof(g_szPbString), szfmt, arg_ptr);

    Tool_WriteLog(g_szPbString);
    //Tool_WriteBackupLog(g_szPbString);

    va_end(arg_ptr);
}

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize)
{
    //printf("begin Tool_SaveFileToPath");
    if (NULL == fileData || NULL == szPath || fileSize == 0)
    {
        printf("Tool_SaveFileToPath, failed.NULL == pImgData || NULL == chImgPath");
        return false;
    }
    char chLogBuff[MAX_PATH] = { 0 };
    bool bRet = false;

    if (NULL != strstr(szPath, "\\") || NULL != strstr(szPath, "/"))
    {
        std::string tempFile(szPath);
        size_t iPosition = std::string::npos;
        if (NULL != strstr(szPath, "\\"))
        {
            iPosition = tempFile.rfind("\\");
        }
        else
        {
            iPosition = tempFile.rfind("/");
        }
        std::string tempDir = tempFile.substr(0, iPosition + 1);
        if (!MakeSureDirectoryPathExists(tempDir.c_str()))
        {
            memset(chLogBuff, '\0', sizeof(chLogBuff));
            //sprintf(chLogBuff, "%s save failed", szPath);
            sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save failed, create path failed.", szPath);
            printf(chLogBuff);
            return false;
        }
    }

    size_t iWritedSpecialSize = 0;
    FILE* fp = NULL;
    //fp = fopen(szPath, "wb+");
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&fp, szPath, "wb+");
    if (fp)
    {
        //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
        iWritedSpecialSize = fwrite(fileData, sizeof(byte), fileSize, fp);
        fflush(fp);
        fclose(fp);
        fp = NULL;
        bRet = true;
    }
    else
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf(chLogBuff, "%s save failed", szPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s open failed, error code = %d", szPath, errCode);
        printf(chLogBuff);
    }
    if (iWritedSpecialSize == fileSize)
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf(chLogBuff, "%s save success", szPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save success", szPath);
        printf(chLogBuff);
    }
    else
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf(chLogBuff, "%s save failed.", szPath);
        _get_errno(&errCode);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s write no match, size = %lu, write size = %lu, error code = %d.",
            szPath,
            fileSize,
            iWritedSpecialSize,
            errCode);
        printf(chLogBuff);
    }

    //printf("end SaveImgToDisk");
    return bRet;
}

bool Tool_GetTextNodeFromXML(const char* XmlInfoBuf, size_t xmlLength, const char* NodeName, char* ValueBuf, size_t bufLength)
{
    if (XmlInfoBuf == NULL || NodeName == NULL || ValueBuf == NULL
        || xmlLength <= 0 || bufLength <= 0)
    {
        return false;
    }
    const char* pNodeName = NodeName;
    std::string strEndNodeName;
    strEndNodeName.append("<");
    strEndNodeName.append("/");
    strEndNodeName.append(pNodeName);
    strEndNodeName.append(">");

    char* pchTemp = new char[xmlLength + 1];
    memset(pchTemp, '\0', xmlLength + 1);
    memcpy(pchTemp, XmlInfoBuf, xmlLength);

    std::string strInfo(pchTemp);
    if (pchTemp)
    {
        delete[] pchTemp;
        pchTemp = NULL;
    }
    else
    {
        printf("malloc failed.");
        return false;
    }

    size_t iPos = strInfo.find(strEndNodeName);
    if (std::string::npos == iPos)
    {
        printf("can not find node %s .\n", strEndNodeName.c_str());
        return false;
    }
    std::string strSubBefore = strInfo.substr(0, iPos);
    size_t iPos2 = strSubBefore.rfind(">");
    if (std::string::npos == iPos)
    {
        printf("can not find text '>' .\n");
        return false;
    }

    std::string strValue = strInfo.substr(iPos2 + 1, iPos - iPos2 - 1);
    //printf("find then node ' %s'  text = '%s'", pNodeName, strValue.c_str());

    if (strValue.length() >= bufLength)
    {
        printf("value length %d is larger than buffer length %d.", strValue.length(), bufLength);
        return false;
    }
    else
    {
        memset(ValueBuf, '\0', bufLength);
        memcpy(ValueBuf, strValue.c_str(), strValue.length());

        return true;
    }
}
