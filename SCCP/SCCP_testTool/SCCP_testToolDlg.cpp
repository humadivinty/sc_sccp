
// SCCP_testToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SCCP_testTool.h"
#include "SCCP_testToolDlg.h"
#include "afxdialogex.h"
#include "../SCCP/SCCP_commenDef.h"
#include "../SCCP/SCCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef DEBUG
#pragma comment(lib, "../debug/SCCP.lib")
#else
#pragma comment(lib, "../release/SCCP.lib")
#endif
#include <string>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#define  MAX_IMAGE_LENGTH (10*1024*1024)
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void* pDlg = NULL;
bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSCCP_testToolDlg 对话框



CSCCP_testToolDlg::CSCCP_testToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSCCP_testToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSCCP_testToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DATETIMEPICKER_Date, m_dateTimeDate);
    DDX_Control(pDX, IDC_DATETIMEPICKER_Time, m_dateTime_time);
}

BEGIN_MESSAGE_MAP(CSCCP_testToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_Init, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrInit)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_Login, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrLogin)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_Logout, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrLogout)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_Settime, &CSCCP_testToolDlg::OnBnClickedButtonSclsSettime)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_SetDVRMessCallBack, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrSetdvrmesscallback)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_GetBigImage, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetbigimage)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_GetBinaryImage, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetbinaryimage)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_GetSmallImage, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetsmallimage)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_GetPlateNo, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetplateno)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_ForceCapture, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrForcecapture)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_RealPlay, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrRealplay)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_StopRealPlay, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStoprealplay)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_StartRecord, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStartrecord)
    ON_BN_CLICKED(IDC_BUTTON_SCLS_DVR_StopRecord, &CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStoprecord)
END_MESSAGE_MAP()


// CSCCP_testToolDlg 消息处理程序

BOOL CSCCP_testToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    m_iDeviceID = 0;
    //m_pbImageData = new unsigned char[MAX_IMAGE_LENGTH];
    //memset(m_pbImageData, 0, MAX_IMAGE_LENGTH);
    pDlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSCCP_testToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSCCP_testToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSCCP_testToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrInit()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_Init();
    
    CString strLog;
    strLog.Format("SCLS_DVR_Init  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrLogin()
{
    // TODO:  在此添加控件通知处理程序代码
    char chTemp[256] = {0};
    GetItemText(IDC_IPADDRESS1, chTemp, sizeof(chTemp));

    m_iDeviceID = SCLS_DVR_Login(chTemp, 80, "admin", "admin");

    CString strLog;
    strLog.Format("SCLS_DVR_Login(%s)  = %d ", chTemp, m_iDeviceID);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrLogout()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_Logout(m_iDeviceID);

    CString strLog;
    strLog.Format("SCLS_DVR_Logout  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsSettime()
{
    // TODO:  在此添加控件通知处理程序代码
    CTime timeDate;
    CTime timeTime;
    m_dateTimeDate.GetTime(timeDate);
    m_dateTime_time.GetTime(timeTime);

    int iYear = timeDate.GetYear();
    int iMonth = timeDate.GetMonth();
    int iDay = timeDate.GetDay();
    int iHour = timeTime.GetHour();
    int iMinute = timeTime.GetMinute();
    int iSecond = timeTime.GetSecond();

    NET_DVR_TIME timeStruct;
    timeStruct.dwYear = iYear;
    timeStruct.dwMonth = iMonth;
    timeStruct.dwDay = iDay;
    timeStruct.dwHour = iHour;
    timeStruct.dwMinute = iMinute;
    timeStruct.dwSecond = iSecond;

    BOOL bret = SCLS_Settime(m_iDeviceID, timeStruct);

    CString strLog;
    strLog.Format("SCLS_Settime(%04d-%02d-%02d %02d:%02d:%02d) = %d",
        timeDate.GetYear(), 
        timeDate.GetMonth(),
        timeDate.GetDay(),
        timeTime.GetHour(),
        timeTime.GetMinute(),
        timeTime.GetSecond(),
        bret );
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrSetdvrmesscallback()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bRet = SCLS_DVR_SetDVRMessCallBack(ResultCallBack, m_iDeviceID);

    CString strLog;
    strLog.Format("SCLS_DVR_SetDVRMessCallBack(%p)  = %d ", &ResultCallBack, bRet);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetbigimage()
{
    // TODO:  在此添加控件通知处理程序代码
    int iImageLength = MAX_IMAGE_LENGTH;
    int iRet = SCLS_DVR_GetBigImage(m_iDeviceID, (CHAR*)getImageBuferPointer(), iImageLength, &iImageLength);

    CString strLog;
    strLog.Format("SCLS_DVR_GetBigImage()  = %d , image length = %d", iRet, iImageLength);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetbinaryimage()
{
    // TODO:  在此添加控件通知处理程序代码
    int iImageLength = MAX_IMAGE_LENGTH;
    int iRet = SCLS_DVR_GetBinaryImage(m_iDeviceID, (CHAR*)getImageBuferPointer(), iImageLength, &iImageLength);

    CString strLog;
    strLog.Format("SCLS_DVR_GetBinaryImage()  = %d , image length = %d", iRet, iImageLength);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetsmallimage()
{
    // TODO:  在此添加控件通知处理程序代码
    int iImageLength = MAX_IMAGE_LENGTH;
    int iRet = SCLS_DVR_GetSmallImage(m_iDeviceID, (CHAR*)getImageBuferPointer(), iImageLength, &iImageLength);

    CString strLog;
    strLog.Format("SCLS_DVR_GetSmallImage()  = %d , image length = %d", iRet, iImageLength);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrGetplateno()
{
    // TODO:  在此添加控件通知处理程序代码
    char chPlateNumber[256] = { 0 };
    int iPlateLength = MAX_IMAGE_LENGTH;
    int iRet = SCLS_DVR_GetPlateNo(m_iDeviceID, chPlateNumber, iPlateLength, &iPlateLength);

    CString strLog;
    strLog.Format("SCLS_DVR_GetPlateNo()  = %d , chPlateNumber = %s", iRet, chPlateNumber);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrForcecapture()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_ForceCapture(m_iDeviceID);

    CString strLog;
    strLog.Format("SCLS_DVR_ForceCapture  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrRealplay()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_RealPlay(m_iDeviceID, GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd());

    CString strLog;
    strLog.Format("SCLS_DVR_RealPlay  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStoprealplay()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_StopRealPlay(m_iDeviceID);

    CString strLog;
    strLog.Format("SCLS_DVR_StopRealPlay  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStartrecord()
{
    // TODO:  在此添加控件通知处理程序代码
    CHAR szPath[256] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH - 1);
    PathRemoveFileSpec(szPath);

    char chFileName[256] = {0};
    sprintf_s(chFileName, sizeof(chFileName), "%s/%ld.avi", szPath, GetTickCount());

    BOOL bret = SCLS_DVR_StartRecord(m_iDeviceID, chFileName);

    CString strLog;
    strLog.Format("SCLS_DVR_StartRecord  = %d ", bret);
    ShowMessage(strLog);
}


void CSCCP_testToolDlg::OnBnClickedButtonSclsDvrStoprecord()
{
    // TODO:  在此添加控件通知处理程序代码
    BOOL bret = SCLS_DVR_StopRecord(m_iDeviceID);

    CString strLog;
    strLog.Format("SCLS_DVR_StopRecord  = %d ", bret);
    ShowMessage(strLog);
}

void CSCCP_testToolDlg::ShowMessage(CString strMsg)
{
    CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MSG);
    ShowMsg(pEdit, strMsg);
}

void CSCCP_testToolDlg::ShowMsg(CEdit *pEdit, CString strMsg)
{
    if (pEdit == NULL)
    {
        return;
    }

    CTime  time = CTime::GetCurrentTime();
    CString strTmp;
    pEdit->GetWindowText(strTmp);
    if (strTmp.IsEmpty() || strTmp.GetLength() > 4096)
    {
        strTmp = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
        strTmp += strMsg;
        pEdit->SetWindowText(strTmp);
        return;
    }

    strTmp += _T("\r\n");
    strTmp += time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
    strTmp += strMsg;
    pEdit->SetWindowText(strTmp);

    if (pEdit != NULL)
    {
        pEdit->LineScroll(pEdit->GetLineCount() - 1);
    }
}

bool CSCCP_testToolDlg::GetItemText(int ItemID, char* buffer, size_t bufSize)
{
    CString strTemp;
    GetDlgItem(ItemID)->GetWindowText(strTemp);
    if (strTemp.GetLength() < bufSize)
    {
        //sprintf(buffer, "%s", strTemp.GetBuffer());
        sprintf_s(buffer, bufSize, "%s", strTemp.GetBuffer());
        strTemp.ReleaseBuffer();
        return true;
    }
    return false;
}

unsigned char* CSCCP_testToolDlg::getImageBuferPointer()
{
    uint8_t* pData = NULL;
    if (!m_pbImgData)
    {
        m_pbImgData = std::shared_ptr<uint8_t>(new uint8_t[MAX_IMAGE_LENGTH], std::default_delete<uint8_t[]>());
    }
    if (m_pbImgData)
    {
        pData = m_pbImgData.get();
        memset(pData, 0, MAX_IMAGE_LENGTH);
    }
    return pData;
}

void __stdcall CSCCP_testToolDlg::ResultCallBack(int iCommand, char* pInfo)
{
    char chFilePath[256] = {0};
    if (pDlg)
    {
        CString strLog;
        NET_DVR_PLATE_RESULT* pResult = (NET_DVR_PLATE_RESULT*)pInfo;
        strLog.Format("ResultCallBack: byDriveChan = %d, PlateNO:%s", pResult->byDriveChan,  pResult->sLicense);
        ((CSCCP_testToolDlg*)pDlg)->ShowMessage(strLog);

        strLog.Format("ResultCallBack: dwPicLen = %lu, dwPicPlateLen:%lu, dwBinPicLen = %lu", pResult->dwPicLen, pResult->dwPicPlateLen, pResult->dwBinPicLen);
        ((CSCCP_testToolDlg*)pDlg)->ShowMessage(strLog);

        strLog.Format("ResultCallBack: pBuffer1= %p, pBuffer2= %p,pBuffer3= %p", pResult->pBuffer1, pResult->pBuffer2, pResult->pBuffer3);
        ((CSCCP_testToolDlg*)pDlg)->ShowMessage(strLog);

        char FileName[MAX_PATH];
        GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);
        PathRemoveFileSpecA(FileName);

        SYSTEMTIME systime;
        GetLocalTime(&systime);//本地时间
        sprintf_s(chFilePath, sizeof(chFilePath), "%s\\ResultBuffer\\%04d%02d%02d%02d%02d%02d%03d-%s-%d\\",
            FileName,
            systime.wYear,
            systime.wMonth,
            systime.wDay,
            systime.wHour,
            systime.wMinute,
            systime.wSecond,
            systime.wMilliseconds,
            pResult->sLicense,
            pResult->byDriveChan);

        char chFileName[256] = {0};
        sprintf_s(chFileName, sizeof(chFileName), "%s\\Pic.jpg", chFilePath);
        Tool_SaveFileToPath(chFileName, pResult->pBuffer1, pResult->dwPicLen);

        memset(chFileName, '\0', sizeof(chFileName));
        sprintf_s(chFileName, sizeof(chFileName), "%s\\PicPlate.bmp", chFilePath);
        Tool_SaveFileToPath(chFileName, pResult->pBuffer2, pResult->dwPicPlateLen);

        memset(chFileName, '\0', sizeof(chFileName));
        sprintf_s(chFileName, sizeof(chFileName), "%s\\BinPic.bin", chFilePath);
        Tool_SaveFileToPath(chFileName, pResult->pBuffer3, pResult->dwBinPicLen);
    }
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
