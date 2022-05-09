#include"pch.h"
#include"globedef.h"

#include"ChinaMarket.h"

#include"GlobeOption.h"

#include "nlohmann/json.hpp"
using nlohmann::json;

using namespace std;
#include<iostream>
#include<fstream>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
// 下面的是第二个,用于dell240工作机。
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
CGlobeOption::CGlobeOption() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = "C:\\StockAnalysis\\systemOption.json"; // json file name

	// China Market
	m_iChinaMarketRealtimeServer = 0; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	m_iChinaMarketRealtimeInquiryTime = 200; // 默认实时数据查询时间间隔为200毫秒

	// World Market
	m_strFinnhubToken = "&token=bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "&token=c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = "&api_key=zBMXMyoTyiy_N3pMb3ex"; // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 3600000 / 3000; // 默认每小时最多查询3000次
	m_iWorldMarketTiingoInquiryTime = 3600000 / 400; // 默认每小时最多查询400次。 默认免费账户的查询频率为每小时500次(每次7200毫秒）；付费账户为每小时20000次（每次180毫秒）
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // 默认每小时最多查询100次

	LoadDB();
}

CGlobeOption::~CGlobeOption() {
	// China Market
	if (m_systemOption.find("ChinaMarketRealtimeServer") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeServer"] = m_iChinaMarketRealtimeServer; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	}
	if (m_systemOption.find("ChinaMarketRealtimeInquiryTime") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime; // 实时数据查询时间间隔（单位：毫秒）
	}

	// World Market
	if (m_systemOption.find("FinnhubToken") == m_systemOption.end()) {
		m_systemOption["FinnhubToken"] = m_strFinnhubToken; // Finnhub token
	}
	if (m_systemOption.find("TiingoToken") == m_systemOption.end()) {
		m_systemOption["TiingoToken"] = m_strTiingoToken; // Tiingo token
	}
	if (m_systemOption.find("QuandlToken") == m_systemOption.end()) {
		m_systemOption["QuandlToken"] = m_strQuandlToken; // Quandl token
	}
	if (m_systemOption.find("WorldMarketFinnhubInquiryTime") == m_systemOption.end()) {
		m_systemOption["WorldMarketFinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime; // 默认每小时最多查询3000次
	}
	if (m_systemOption.find("WorldMarketTiingoInquiryTime") == m_systemOption.end()) {
		m_systemOption["WorldMarketTiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime; // 默认每小时最多查询400次
	}
	if (m_systemOption.find("WorldMarketQuandlInquiryTime") == m_systemOption.end()) {
		m_systemOption["WorldMarketQuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime; // 默认每小时最多查询100次
	}

	SaveDB();
}

void CGlobeOption::Update(json& jInput) {
	string sTemp;

	// ChinaMarket
	if (jInput.find("ChinaMarketRealtimeServer") != jInput.end()) {
		m_iChinaMarketRealtimeServer = jInput["ChinaMarketRealtimeServer"]; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	}

	if (jInput.find("ChinaMarketRealtimeInquiryTime") != jInput.end()) {
		m_iChinaMarketRealtimeInquiryTime = jInput["ChinaMarketRealtimeInquiryTime"]; // 实时数据查询时间间隔（单位：毫秒）
	}

	// WorldMarket
	if (jInput.find("FinnhubToken") != jInput.end()) {
		sTemp = jInput["FinnhubToken"]; // Finnhub token
		m_strFinnhubToken = sTemp.c_str();
	}
	if (jInput.find("TiingoToken") != jInput.end()) {
		sTemp = jInput["TiingoToken"]; // Tiingo token
		m_strTiingoToken = sTemp.c_str();
	}
	if (jInput.find("QuandlToken") != jInput.end()) {
		sTemp = jInput["QuandlToken"]; // Quandl token
		m_strQuandlToken = sTemp.c_str();
	}
	if (jInput.find("WorldMarketFinnhubInquiryTime") != jInput.end()) {
		m_iWorldMarketFinnhubInquiryTime = jInput["WorldMarketFinnhubInquiryTime"]; // 默认每小时最多查询3000次
	}
	if (jInput.find("WorldMarketTiingoInquiryTime") != jInput.end()) {
		m_iWorldMarketTiingoInquiryTime = jInput["WorldMarketTiingoInquiryTime"]; // 默认每小时最多查询400次
	}
	if (jInput.find("WorldMarketQuandlInquiryTime") != jInput.end()) {
		m_iWorldMarketQuandlInquiryTime = jInput["WorldMarketQuandlInquiryTime"]; // 默认每小时最多查询100次
	}
}

void CGlobeOption::Update(void) {
	m_systemOption["ChinaMarketRealtimeServer"] = m_iChinaMarketRealtimeServer;
	m_systemOption["ChinaMarketRealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime;

	m_systemOption["FinnhubToken"] = m_strFinnhubToken;
	m_systemOption["TiingoToken"] = m_strTiingoToken;
	m_systemOption["QuandlToken"] = m_strQuandlToken;
	m_systemOption["WorldMarketFinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	m_systemOption["WorldMarketTiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	m_systemOption["WorldMarketQuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;
}

bool CGlobeOption::LoadDB() {
	fstream f(m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_systemOption;
		Update(m_systemOption);
	}

	return true;
}

bool CGlobeOption::SaveDB() {
	Update();
	fstream f(m_strFileName, ios::out);
	f << m_systemOption;
	f.close();

	return true;
}