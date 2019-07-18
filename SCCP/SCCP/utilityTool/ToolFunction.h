#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>

//#define MY_SPRINTF sprintf_s

//ͨ���ڵ������Ҳ�������Ӧ�ڵ�
//ע��XMLTYPE Ϊ1ʱ��InputInfoΪXML·������Ϊ2ʱ,InputInfoΪ�������ļ�����
TiXmlElement Tool_SelectElementByName(const char* InputInfo, char* pName, int iXMLType);

TiXmlElement* Tool_ReadElememt(TiXmlElement* InputElement, char* pName);

void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

void Tool_ReadIntValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, int&keyValue);

void Tool_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//���IP����Ч��
int Tool_checkIP(const char* p);

bool Tool_IsFileExist(const char* FilePath);

bool Tool_MakeDir(const char* chImgPath);

long Tool_GetFileSize(const char *FileName);

bool Tool_PingIPaddress(const char* IpAddress);

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);

int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void Tool_ExcuteShellCMD(char* pChCommand);

bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality);

bool Tool_GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

void Tool_ExcuteCMD(char* pChCommand);

std::wstring Img_string2wstring(std::string strSrc);

#ifdef  USE_MFC
//ɾ��ָ���ļ���
bool DeleteDirectory(const char* strDirName);

//ɾ��ָ��Ŀ¼���� 'yyyy-mm-dd' ��ʽ�������ļ��У������޶�������Ϊʱ������ָ������
int CirclelaryDelete(const char* folderPath, int iBackUpDays);
#endif


int Tool_SafeCloseThread(HANDLE& threadHandle);

const char* Tool_GetCurrentPath();

SYSTEMTIME Tool_GetCurrentTime();

bool Tool_DimCompare(const char *szSrcPlateNo, const char *szDesPlateNo);

void Tool_WriteLog(const char*);

void Tool_WriteFormatLog(const char* szfmt, ...);

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize);

bool Tool_GetTextNodeFromXML(const char* XmlInfoBuf, size_t xmlLength, const char* NodeName, char* ValueBuf, size_t bufLength);
