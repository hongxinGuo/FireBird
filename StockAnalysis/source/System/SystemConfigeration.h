//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֲ�����λ�ڴ��ļ��У�����ϵͳ���á�
//
// �洢��ʽʹ��json��ʽ���洢���ļ�SystemConfigeration.json�С�
//
// ʹ��boost�е�JSON��������֮��ʵ�����л��ͷ����л���
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<string>
#include<map>

#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

class CSystemConfigeration : CObject {
public:
	CSystemConfigeration();
	~CSystemConfigeration();

	bool LoadDB();
	bool SaveDB();
	void Update(void);
	void UpdateJson(void);

	void UpdateSystem(void); // ʹ�ñ�����ȥ����ϵͳ

	void SetDefaultFileName(CString fileName) { m_strFileName = fileName; }
	CString GetDefaultFileName(void) { return m_strFileName; }

	// ��ȡϵͳ���ò���
	bool IsUsingFastCPU(void) noexcept { return m_bUsingFastCPU; }
	void SetUsingFastCPU(bool bUsingFastCPU) noexcept { m_bUsingFastCPU = bUsingFastCPU; }
	CString GetDatabaseAccountName(void) noexcept { return m_strDatabaseAccountName; }
	void SetDatabaseAccountName(CString strAccountName) noexcept { m_strDatabaseAccountName = strAccountName; }
	CString GetDatabaseAccountPassword(void) noexcept { return m_strDatabaseAccountPassword; }
	void SetDatabaseAccountPassword(CString strPassword) noexcept { m_strDatabaseAccountPassword = strPassword; }

	int GetBackgroundThreadPermittedNumber(void) noexcept { return m_iBackgroundThreadPermittedNumber; }
	int GetSavingThreadPermittedNumber(void) noexcept { return m_iSavingThreadPermittedNumber; }

	// ȫ�ֲ���
	int GetChinaMarketRealtimeServer(void) noexcept { return m_iChinaMarketRealtimeServer; }
	void SetChinaMarketRealtimeServer(int iChinaMarketRealtimeServer) noexcept { m_fUpdate = true; m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer; }
	int GetChinaMarketRealtimeInquiryTime(void) noexcept { return m_iChinaMarketRealtimeInquiryTime; }
	void SetChinaMarketRealtimeInquiryTime(int iChinaMarketRealtimeInquiryTime) noexcept { m_fUpdate = true; m_iChinaMarketRealtimeInquiryTime = iChinaMarketRealtimeInquiryTime; }

	CString GetFinnhubToken(void) noexcept { return m_strFinnhubToken; }
	CString GetTiingoToken(void) noexcept { return m_strTiingoToken; }
	CString GetQuandlToken(void) noexcept { return m_strQuandlToken; }
	int GetWorldMarketFinnhubInquiryTime(void) noexcept { return m_iWorldMarketFinnhubInquiryTime; }
	void SetWorldMarketFinnhubInquiryTime(int iWorldMarketFinnhubInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime; }
	int GetWorldMarketTiingoInquiryTime(void) noexcept { return m_iWorldMarketTiingoInquiryTime; }
	void SetWorldMarketTiingoInquiryTime(int iWorldMarketTiingoInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime; }
	int GetWorldMarketQuandlInquiryTime(void) noexcept { return m_iWorldMarketQuandlInquiryTime; }
	void SetWorldMarketQuandlInquiryTime(int iWorldMarketQuandlInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime; }

	bool IsUsingSinaRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 0) return true; else return false; }
	bool IsUsingNeteaseRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 1) return true; else return false; }

	// WebSocket
	bool IsUsingFinnhubWebSocket(void) const noexcept { return m_bUsingFinnhubWebSocket; }
	void SetUsingFinnhubWebSocket(bool bUsingFinnhubWebSocket) noexcept { m_bUsingFinnhubWebSocket = bUsingFinnhubWebSocket; }
	bool IsUsingTiingoIEXWebSocket(void) const noexcept { return m_bUsingTiingoIEXWebSocket; }
	void SetUsingTiingoIEXWebSocket(bool bUsingTiingoIEXWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoIEXWebSocket = bUsingTiingoIEXWebSocket; }
	bool IsUsingTiingoCryptoWebSocket(void) const noexcept { return m_bUsingTiingoCryptoWebSocket; }
	void SetUsingTiingoCryptoWebSocket(bool bUsingTiingoCryptoWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoCryptoWebSocket = bUsingTiingoCryptoWebSocket; }
	bool IsUsingTiingoForexWebSocket(void) const noexcept { return m_bUsingTiingoForexWebSocket; }
	void SetUsingTiingoForexWebSocket(bool bUsingTiingoForexWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoForexWebSocket = bUsingTiingoForexWebSocket; }
	bool IsUsingQuandlWebSocket(void) const noexcept { return m_bUsingQuandlWebSocket; }
	void SetUsingQuandlWebSocket(bool bUsingQuandlWebSocket) noexcept { m_fUpdate = true; m_bUsingQuandlWebSocket = bUsingQuandlWebSocket; }

	// ChinaMarket
	int GetSavingChinaMarketStockDayLineThread(void) noexcept { return m_iSavingChinaMarketStockDayLineThread; }
	void SetSavingChinaMarketStockDayLineThread(int iSavingChinaMarketStockDayLineThread) noexcept { m_fUpdate = true; m_iSavingChinaMarketStockDayLineThread = iSavingChinaMarketStockDayLineThread; }
	bool IsFastInquiringRTData(void) noexcept { return m_bFastInquiringRTData; }
	void SetFastInquiringRTData(bool fFlag) noexcept { m_fUpdate = true; m_bFastInquiringRTData = fFlag; }

	// data update rate
	int GetStockProfileUpdateRate(void) noexcept { return m_iStockProfileUpdateRate; }
	void SetStockProfileUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockProfileUpdateRate = iRate; }
	int GetStockBasicFinancialUpdateRate(void) noexcept { return m_iStockBasicFinancialUpdateRate; }
	void SetStockBasicFinancialUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockBasicFinancialUpdateRate = iRate; }
	int GetInsideTransactionUpdateRate(void) noexcept { return m_iInsideTransactionUpdateRate; }
	void SetInsideTransactionUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iInsideTransactionUpdateRate = iRate; }
	int GetInsideSentimentUpdateRate(void) noexcept { return m_iInsideSentimentUpdateRate; }
	void SetInsideSentimentUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iInsideSentimentUpdateRate = iRate; }
	int GetStockPeerUpdateRate(void) noexcept { return m_iStockPeerUpdateRate; }
	void SetStockPeerUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockPeerUpdateRate = iRate; }

	void SetSystemDisplayRect(RECT rect) noexcept { m_rSystemDisplay = rect; }
	void SetSystemDisplayRect(int cx, int cy) noexcept { m_rSystemDisplay.left = 0; m_rSystemDisplay.right = cx; m_rSystemDisplay.top = 0; m_rSystemDisplay.bottom = cy; }
	RECT GetSystemDisplayRect(void) noexcept { return m_rSystemDisplay; }
	int GetSystemDisplayHeight(void) noexcept { return m_rSystemDisplay.bottom - m_rSystemDisplay.top; }
	int GetSystemDisplayWidth(void) noexcept { return m_rSystemDisplay.right - m_rSystemDisplay.left; }
	void SetCurrentWindowRect(RECT rect) noexcept { m_rCurrentWindow = rect; }
	void SetCurrentWindowRect(int cx, int cy) noexcept { m_rCurrentWindow.left = 0; m_rCurrentWindow.right = cx; m_rCurrentWindow.top = 0; m_rCurrentWindow.bottom = cy; }
	RECT GetCurrentWindowRect(void) noexcept { return m_rCurrentWindow; }
	int GetCurrentWindowHeight(void) noexcept { return m_rCurrentWindow.bottom - m_rCurrentWindow.top; }
	int GetCurrentWindowWidth(void) noexcept { return m_rCurrentWindow.right - m_rCurrentWindow.left; }

	bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	CString m_strFileName; // �����ļ�����

	// ϵͳ����
	bool m_bUsingFastCPU; //  �Ƿ�ʹ�ÿ���CPU
	CString m_strDatabaseAccountName; // ���ݿ��˻�����
	CString m_strDatabaseAccountPassword; // ���ݿ��˻�����
	int m_iBackgroundThreadPermittedNumber; // ��̨�߳��������ֵ
	int m_iSavingThreadPermittedNumber; // ��������洢�߳����� Ĭ��Ϊ4

	// ϵͳ����
	int m_iChinaMarketRealtimeServer; // �й��г�ʵʱ���ݷ�����
	int m_iChinaMarketRealtimeInquiryTime; // �й��г�ʵʱ���ݲ�ѯ���ʱ��

	int m_iWorldMarketFinnhubInquiryTime;
	int m_iWorldMarketTiingoInquiryTime;
	int m_iWorldMarketQuandlInquiryTime;

	CString m_strFinnhubToken;
	CString m_strTiingoToken;
	CString m_strQuandlToken;

	// WebSocket
	bool m_bUsingFinnhubWebSocket;
	bool m_bUsingTiingoIEXWebSocket;
	bool m_bUsingTiingoCryptoWebSocket;
	bool m_bUsingTiingoForexWebSocket;
	bool m_bUsingQuandlWebSocket;

	// ChinaMarket
	int m_iSavingChinaMarketStockDayLineThread; // �洢������ʷ����ʱ�Ĳ����߳�����
	bool m_bFastInquiringRTData;

	// �������ݸ���Ƶ�ʣ��������ǣ�
	int m_iStockProfileUpdateRate; // Ĭ��45�����һ��
	int m_iInsideTransactionUpdateRate; // Ĭ��45�����һ��
	int m_iInsideSentimentUpdateRate; // Ĭ��45�����һ��
	int m_iStockBasicFinancialUpdateRate; // Ĭ��45�����һ��
	int m_iStockPeerUpdateRate; // Ĭ��90�����һ��

	bool m_fUpdate;
	bool m_fInitialized = false;

	// ����Ϊ����洢֮ϵͳ����
	//
	// ���幤��������Ĳ���
	RECT m_rSystemDisplay; // ��ʾ��λ�����
	RECT m_rCurrentWindow; // ��ǰ����λ�����

	// ���뽫json����������������nlohmann json��������ǰ�棬���¸�ֵ����Ŀǰԭ������VSϵͳbug����nholmann json��bug�����߱ؾ���һ����
	// ��VSϵͳ17.2.4��bug��17.2.5�Ѿ����������bug��
	json m_systemConfigeration;
};

extern	std::string gl_sSystemConfigeration; // ϵͳ������Ϣ

extern CSystemConfigeration gl_systemConfigeration;
