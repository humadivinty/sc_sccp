#ifndef SCCP_COMMENTDEF_H
#define SCCP_COMMENTDEF_H
#include <windef.h>
//���Ƽ����
typedef struct tagNET_DVR_PLATE_RESULT
{
    BYTE    byDriveChan;    //����������,Ĭ��Ϊ0
    DWORD   dwPicLen;        //ͼƬ����(����ͼ)
    DWORD   dwPicPlateLen;  //����СͼƬ���ȣ����Ʋ�ͼ��
    DWORD   dwBinPicLen;    //���ƶ�ֵͼ����
    BYTE    *pBuffer1;      // ����ͼ
    BYTE    *pBuffer2;      // ���Ʋ�ͼ
    BYTE    *pBuffer3;      //���ƶ�ֵͼ
    char sLicense[20]; //������Ϣ����ɫ+���ƣ��޽��ʱ��ֵΪ��δ��⡱
}NET_DVR_PLATE_RESULT, *LPNET_DVR_PLATE_RESULT;
//�ṹ�����ݣ��ɸ���������̿��ṩ����ֵ��������������ֶΣ������Ͻṹӵ�еĶ���//��������Ŀ�����е�
/***********
*�ص�����
*LONG lCommand �������ͣ����߱������ͣ����������Ϣ���ͣ���������ض���Ϣ����д*�������
*char *pInfo �洢������Ϣ��������Ƭ�����԰������淽ʽ��ֱ�Ӹ�ֵ��*NET_DVR_PLATE_RESULT���͵ı�����
***********/
typedef void (CALLBACK *MessageCallback)(int, char *);
//{
//    //�ص���������
//    NET_DVR_PLATE_RESULT struPlateResult;
//    memcpy(&struPlateResult, pInfo, sizeof(struPlateResult));
//    .....
//}

typedef struct _NET_DVR_TIME
{
    DWORD dwYear;        //��
    DWORD dwMonth;        //��
    DWORD dwDay;        //��
    DWORD dwHour;        //ʱ
    DWORD dwMinute;        //��
    DWORD dwSecond;        //��
}NET_DVR_TIME, *LPNET_DVR_TIME;

#endif