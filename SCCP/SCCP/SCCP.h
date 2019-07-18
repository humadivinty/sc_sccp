// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SCCPDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SCCP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SCCP_EXPORTS
#define SCCP_API extern "C" __declspec(dllexport)
#else
#define SCCP_API extern "C" __declspec(dllimport)
#endif

#define DLL_SPEC __stdcall

#include "SCCP_commenDef.h"

/***********
*Уʱ����
*LONG lHandle ����¼����󣬻�ȡ������handle
*NET_DVR_TIME  CurTime , ָ��ʱ��
***********/
SCCP_API BOOL DLL_SPEC SCLS_Settime(LONG lHandle, NET_DVR_TIME  CurTime);

/************
*�����ʼ��
*����Ҫ�˲���ģ�ֱ�ӷ���true
************/
SCCP_API BOOL DLL_SPEC SCLS_DVR_Init();

/*********
*�����¼���˺������������ĵ�¼�����ӹ���
*char *sIP   ���IP��ַ
*WORD wPort  ����˿ںţ����ޣ���NULL
*char *sUserName  ��¼����û��������ޣ���NULL
*char *sPassword   ��¼������룬���ޣ���NULL
****����ֵ LONG  �����¼��ı�ʶ���ɹ�����ֵ>=0��ʧ�ܷ���ֵ<0
**********/
SCCP_API LONG DLL_SPEC SCLS_DVR_Login(char *sIP, WORD wPort, char *sUserName, char *sPassword);

/********
*����ص���������
*CALLBACK *fMessCallBack  �ص���������
*LONG lHandle    ������ӵ�lHandle
*����ֵ BOOL   true �ɹ��� false ʧ��
*********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_SetDVRMessCallBack(MessageCallback fMessCallBack, LONG lHandle);

/*********
*����˳�����
*LONG lHandle  ������ӵ�lHandle
*����ֵ BOOL   true �ɹ��� false ʧ��
**********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_Logout(LONG lHandle);

/**********
*��ȡ����ͼ
*char *ImgBuf ַ���ݣ��洢ͼƬ,�û�����
*int bufLen   ���ڴ洢ͼƬ����ĳ���
*int *ImgSize ַ���ݣ����ڼ�¼ͼƬ��ʵ�ʴ�С
*����ֵ int   0 �ɹ���-4���������ȡͼƬ����ʵ�ʷ���洢�ռ䲻��
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetBigImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/**********
*��ȡ���ƶ�ֵͼ
*char *ImgBuf ַ���ݣ��洢ͼƬ,�û�����
*int bufLen   ���ڴ洢ͼƬ����ĳ���
*int *ImgSize ַ���ݣ����ڼ�¼ͼƬ��ʵ�ʴ�С
*����ֵ int   0 �ɹ���-4���������ȡͼƬ����ʵ�ʷ���洢�ռ䲻��
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetBinaryImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/**********
*��ȡ���Ʋ�ɫͼƬ
*char *ImgBuf ַ���ݣ��洢ͼƬ,�û�����
*int bufLen   ���ڴ洢ͼƬ����ĳ���
*int *ImgSize ַ���ݣ����ڼ�¼ͼƬ��ʵ�ʴ�С
*����ֵ int   0 �ɹ���-4���������ȡͼƬ����ʵ�ʷ���洢�ռ䲻��
***********/
SCCP_API int DLL_SPEC SCLS_DVR_GetSmallImage(LONG lHandle, char *ImgBuf, int bufLen, int * ImgSize);

/*********
*char *pPlateNo ַ���ݣ��洢������ɫ�ͳ��ƺ���
*int bufLen      *pPlateNo�ķ���ĳ��ȣ�
*int *plateNoLen  ַ���ݣ����ڼ�¼����ʵ�ʴ�С
*����ֵ int   0 �ɹ��� -4 ��ȡ�ĳ��Ƴ��ȣ���������ĳ���
**********/
SCCP_API int DLL_SPEC SCLS_DVR_GetPlateNo(LONG lHandle, char *pPlateNo, int bufLen, int *plateNoLen);

/*********
*���ô˺����󣬻ᴥ���ص�����
*LONG lHandle  �������
*����ֵ BOOL   true �ɹ��� false ʧ��
**********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_ForceCapture(LONG lHandle);

/*********
*��Ƶ����
*LONG lHandle �������
*HWND FormHwnd   C#���Ŵ���
*����ֵ��LONG ��Ƶ���ŵ�handle  ����ͨ�����handle �����ر���Ƶ
**********/
SCCP_API LONG DLL_SPEC SCLS_DVR_RealPlay(LONG lHandle, HWND FormHwnd);

/*********
*ֹͣ��Ƶ����
*LONG lHandle ��Ƶ���ŵ�handle
********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_StopRealPlay(LONG lHandle);

/********
*��Ƶ¼���ܣ��ù���������Ƶ��Ԥ�����ܺ󣬲ſɿ���
*LONG lHandle  ��Ƶ���ŵ�handle
*char *sFileName   ��Ƶ�ļ�����·��
*����ֵ  true �ɹ� �� false ʧ��
*********/
SCCP_API BOOL DLL_SPEC SCLS_DVR_StartRecord(LONG lHandle, char *sFileName);

/********
*ֹͣ��Ƶ¼��
*LONG lHandle ��Ƶ���ŵ�handle
*����ֵ  true �ɹ� �� false ʧ��
*********/
SCCP_API  BOOL DLL_SPEC SCLS_DVR_StopRecord(LONG lHandle);