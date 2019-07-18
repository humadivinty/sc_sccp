#ifndef CAMERRESULT
#define CAMERRESULT

/* ʶ������ͼ���Ͷ��� */
#define RECORD_BIGIMG_BEST_SNAPSHOT			0x0001	/**< ������ʶ��ͼ */
#define RECORD_BIGIMG_LAST_SNAPSHOT			0x0002	/**< ���ʶ��ͼ */
#define RECORD_BIGIMG_BEGIN_CAPTURE			0x0003	/**< ��ʼץ��ͼ */
#define RECORD_BIGIMG_BEST_CAPTURE			0x0004	/**< ������ץ��ͼ */
#define RECORD_BIGIMG_LAST_CAPTURE			0x0005	/**<  ���ץ��ͼ */

#define  MAX_IMG_SIZE 10*1024*1024

#define UNKOWN_TYPE 0
#define BUS_TYPE_1 1
#define BUS_TYPE_2 2
#define BUS_TYPE_3 3
#define BUS_TYPE_4 4
#define BUS_TYPE_5 5
#define TRUCK_TYPE_1 5
#define TRUCK_TYPE_2 6
#define TRUCK_TYPE_3 7
#define TRUCK_TYPE_4 8
#define TRUCK_TYPE_5 9

class CameraIMG
{
public:
	CameraIMG();
	CameraIMG(const CameraIMG& CaIMG);
	~CameraIMG();
	unsigned long wImgWidth;
	unsigned long wImgHeight;
	DWORD dwImgSize;
	unsigned long  wImgType;
	char chSavePath[256];
	PBYTE pbImgData;

	CameraIMG& operator = (const CameraIMG& CaIMG);
};

class CameraResult
{
public:

	CameraResult();
	CameraResult(const CameraResult& CaRESULT);
	~CameraResult();

	DWORD dwCarID;
    DWORD dwArriveTime;
	int iDeviceID;
	int iPlateColor;
	int iPlateTypeNo;
	DWORD64 dw64TimeMS;
	int iSpeed;
	int iResultNo;
	int iVehTypeNo;		//���ʹ���: ��1--1 ��������4--4�� ��1--5  ��������4--8
	int iVehBodyColorNo;	
	int iVehBodyDeepNo;	
	int iAreaNo;
	int iRoadNo;
	int iLaneNo;
	int iDirection;
	int iWheelCount;		//����
	long iAxletreeCount;		//����
	int iAxletreeGroupCount;//������
	int iAxletreeType;		//����
    int iReliability;       //���Ŷ�
	float fVehLenth;			//����
	float fDistanceBetweenAxles;		//���
	float fVehHeight;		//����
    float fConfidenceLevel;     //���Ŷ�
	bool bBackUpVeh;		//�Ƿ񵹳�


	char chDeviceIp[64];
	char chPlateNO[64];
	char chPlateColor[64];
	char chListNo[256];
	char chPlateTime[256];
	char chSignStationID[256];
	char chSignStationName[256];
	char pcAppendInfo[10240];

	char chDeviceID[256];
	char chLaneID[256];

	CameraIMG CIMG_BestSnapshot;	/**< ������ʶ��ͼ */
	CameraIMG CIMG_LastSnapshot;	/**< ���ʶ��ͼ */
	CameraIMG CIMG_BeginCapture;	/**< ��ʼץ��ͼ */
	CameraIMG CIMG_BestCapture;		/**< ������ץ��ͼ */
	CameraIMG CIMG_LastCapture;		/**< ���ץ��ͼ */
	CameraIMG CIMG_PlateImage;		/**< ����Сͼ */
	CameraIMG CIMG_BinImage;			/**< ���ƶ�ֵͼ */

	CameraResult& operator = (const CameraResult& CaRESULT);

	friend bool SerializationResultToDisk(const char* chFilePath, const CameraResult& CaResult);
	friend bool SerializationFromDisk(const char* chFilePath, CameraResult& CaResult);

	friend bool SerializationAsConfigToDisk(const char* chFilePath, const CameraResult& CaResult);
	friend bool SerializationAsConfigFromDisk(const char* chFilePath, CameraResult& CaResult);
};

typedef struct _tagSafeModeInfo
{
	int iEableSafeMode;
	char chBeginTime[256];
	char chEndTime[256];
	int index;
	int DataInfo;
	_tagSafeModeInfo()
	{
		iEableSafeMode = 0;
		memset(chBeginTime, 0, sizeof(chBeginTime));
		memset(chEndTime, 0, sizeof(chEndTime));
		index = 0;
		DataInfo = 0;
	}
}_tagSafeModeInfo;

#endif