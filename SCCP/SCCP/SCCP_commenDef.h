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
    char vlpId[32];      //ʶ����Id, ��ʽ��yyyyMMddHHmmss+3λ����
}NET_DVR_PLATE_RESULT, *LPNET_DVR_PLATE_RESULT;
//�ṹ�����ݣ��ɸ���������̿��ṩ����ֵ��������������ֶΣ������Ͻṹӵ�еĶ���//��������Ŀ�����е�
/***********
*�ص�����
*LONG lCommand �������ͣ����߱������ͣ����������Ϣ���ͣ���������ض���Ϣ����д*�������
*char *pInfo �洢������Ϣ��������Ƭ�����԰������淽ʽ��ֱ�Ӹ�ֵ��*NET_DVR_PLATE_RESULT���͵ı�����
***********/
//typedef void (CALLBACK *MessageCallback)(int, char *);
//{
//    //�ص���������
//    NET_DVR_PLATE_RESULT struPlateResult;
//    memcpy(&struPlateResult, pInfo, sizeof(struPlateResult));
//    .....
//}

typedef void(__stdcall *CBFun_GetRegResult)(LONG, NET_DVR_PLATE_RESULT *);


typedef struct _extra_vlp_info
{
    char vlpId[32];                   //ʶ����Id, ��ʽ��yyyyMMddHHmmss+3λ�������֡�ȷ�������ظ��� 
    int vlpExtraType;				//����ʶ���������� 1������ͼƬ 2����βͼƬ 3������ʶ����Ϣ
    unsigned int imageLength;	//ʶ�����ݳ���
    unsigned char* image;		//ʶ������
} T_EXTRAVLPINFO;

typedef void(__stdcall *CBFun_GetExtraRegResult)(int, T_EXTRAVLPINFO *);



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