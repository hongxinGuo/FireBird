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

#include "nlohmann/json.hpp"
using json = nlohmann::ordered_json; // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

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

	// ��ȡϵͳ���ò���
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
	bool IsFastInquiringRTData(void) noexcept { return m_bFastInquiringRTData; }
	void SetFastInquiringRTData(bool fFlag) noexcept { m_fUpdate = true; m_bFastInquiringRTData = fFlag; }

	bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	json m_systemConfigeration;
	CString m_strFileName;

	// ϵͳ����
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
	bool m_bFastInquiringRTData;

	bool m_fUpdate;
	bool m_fInitialized = false;
};

extern	string gl_sSystemConfigeration; // ϵͳ������Ϣ

extern CSystemConfigeration gl_systemConfigeration;
