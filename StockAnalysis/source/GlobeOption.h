//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的全局参数皆位于此文件中，用于系统配置。
//
// 存储方式使用json格式，存储与文件systemOption.json中。
//
// 使用boost中的JSON库来解析之，实现序列化和反序列化。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "nlohmann/json.hpp"
using nlohmann::json;

class CGlobeOption : CObject {
public:
	CGlobeOption();
	~CGlobeOption();

	bool LoadDB();
	bool SaveDB();
	void Update(json& jInput);
	void Update(void);

	void SetDefaultFileName(CString fileName) { m_strFileName = fileName; }

	// 全局参数
	int GetChinaMarketRealtimeServer(void) noexcept { return m_iChinaMarketRealtimeServer; }
	void SetChinaMarketRealtimeServer(int iChinaMarketRealtimeServer) noexcept { m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer; }
	int GetChinaMarketRealtimeInquiryTime(void) noexcept { return m_iChinaMarketRealtimeInquiryTime; }
	void SetChinaMarketRealtimeInquiryTime(int iChinaMarketRealtimeInquiryTime) noexcept { m_iChinaMarketRealtimeInquiryTime = iChinaMarketRealtimeInquiryTime; }

	CString GetFinnhubToken(void) noexcept { return m_strFinnhubToken; }
	CString GetTiingoToken(void) noexcept { return m_strTiingoToken; }
	CString GetQuandlToken(void) noexcept { return m_strQuandlToken; }
	int GetWorldMarketFinnhubInquiryTime(void) noexcept { return m_iWorldMarketFinnhubInquiryTime; }
	void SetWorldMarketFinnhubInquiryTime(int iWorldMarketFinnhubInquiryTime) noexcept { m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime; }
	int GetWorldMarketTiingoInquiryTime(void) noexcept { return m_iWorldMarketTiingoInquiryTime; }
	void SetWorldMarketTiingoInquiryTime(int iWorldMarketTiingoInquiryTime) noexcept { m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime; }
	int GetWorldMarketQuandlInquiryTime(void) noexcept { return m_iWorldMarketQuandlInquiryTime; }
	void SetWorldMarketQuandlInquiryTime(int iWorldMarketQuandlInquiryTime) noexcept { m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime; }

	bool IsUsingSinaRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 0) return true; else return false; }
	bool IsUsingNeteaseRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 1) return true; else return false; }

	bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	json m_systemOption;
	CString m_strFileName;

	// 系统参数
	int m_iChinaMarketRealtimeServer; // 中国市场实时数据服务器
	int m_iChinaMarketRealtimeInquiryTime; // 中国市场实时数据查询间隔时间

	int m_iWorldMarketFinnhubInquiryTime;
	int m_iWorldMarketTiingoInquiryTime;
	int m_iWorldMarketQuandlInquiryTime;

	CString m_strFinnhubToken;
	CString m_strTiingoToken;
	CString m_strQuandlToken;

	bool m_fUpdate;
	bool m_fInitialized = false;
};

extern CGlobeOption gl_GlobeOption;
