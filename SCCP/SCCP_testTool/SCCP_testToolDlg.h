
// SCCP_testToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxdtctl.h"
#include <memory>
#include "../SCCP/SCCP_commenDef.h"
#include "../SCCP/SCCP.h"

// CSCCP_testToolDlg �Ի���
class CSCCP_testToolDlg : public CDialogEx
{
// ����
public:
	CSCCP_testToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCCP_TESTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSclsDvrInit();
    afx_msg void OnBnClickedButtonSclsDvrLogin();
    afx_msg void OnBnClickedButtonSclsDvrLogout();
    afx_msg void OnBnClickedButtonSclsSettime();
    afx_msg void OnBnClickedButtonSclsDvrSetdvrmesscallback();
    afx_msg void OnBnClickedButtonSclsDvrGetbigimage();
    afx_msg void OnBnClickedButtonSclsDvrGetbinaryimage();
    afx_msg void OnBnClickedButtonSclsDvrGetsmallimage();
    afx_msg void OnBnClickedButtonSclsDvrGetplateno();
    afx_msg void OnBnClickedButtonSclsDvrForcecapture();
    afx_msg void OnBnClickedButtonSclsDvrRealplay();
    afx_msg void OnBnClickedButtonSclsDvrStoprealplay();
    afx_msg void OnBnClickedButtonSclsDvrStartrecord();
    afx_msg void OnBnClickedButtonSclsDvrStoprecord();

public:
    void ShowMessage(CString strMsg);
private:
    void ShowMsg(CEdit *pEdit, CString strMsg);
    bool GetItemText(int ItemID, char* buffer, size_t bufSize);

    unsigned char* getImageBuferPointer();

public:

    static void __stdcall ResultCallBack(int iCommand, char* pInfo);
    static void __stdcall CBFun_Result_Callback(LONG iCommand, NET_DVR_PLATE_RESULT *pInfo);
    static void __stdcall CBFun_Extrat_Callbackt(int iCommand, T_EXTRAVLPINFO * pInfo);

    
private:
    long m_iDeviceID;
    std::shared_ptr<uint8_t> m_pbImgData;

    CDateTimeCtrl m_dateTimeDate;
    CDateTimeCtrl m_dateTime_time;
public:
    afx_msg void OnBnClickedButtonSetextracallback();
};
