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
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
CGlobeOption::CGlobeOption() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOptionȫ�ֱ���ֻ�������һ��ʵ��\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = "C:\\StockAnalysis\\systemOption.json"; // json file name

	// China Market
	m_iChinaMarketRealtimeServer = 0; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	m_iChinaMarketRealtimeInquiryTime = 200; // Ĭ��ʵʱ���ݲ�ѯʱ����Ϊ200����

	// World Market
	m_strFinnhubToken = "&token=bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "&token=c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = "&api_key=zBMXMyoTyiy_N3pMb3ex"; // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 3600000 / 3000; // Ĭ��ÿСʱ����ѯ3000��
	m_iWorldMarketTiingoInquiryTime = 3600000 / 400; // Ĭ��ÿСʱ����ѯ400�Ρ� Ĭ������˻��Ĳ�ѯƵ��ΪÿСʱ500��(ÿ��7200���룩�������˻�ΪÿСʱ20000�Σ�ÿ��180���룩
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // Ĭ��ÿСʱ����ѯ100��

	LoadDB();
}

CGlobeOption::~CGlobeOption() {
	// China Market
	if (m_systemOption.find("ChinaMarketRealtimeServer") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeServer"] = m_iChinaMarketRealtimeServer; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	}
	if (m_systemOption.find("ChinaMarketRealtimeInquiryTime") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime; // ʵʱ���ݲ�ѯʱ��������λ�����룩
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
		m_systemOption["WorldMarketFinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime; // Ĭ��ÿСʱ����ѯ3000��
	}
	if (m_systemOption.find("WorldMarketTiingoInquiryTime") == m_systemOption.end()) {
		m_systemOption["WorldMarketTiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime; // Ĭ��ÿСʱ����ѯ400��
	}
	if (m_systemOption.find("WorldMarketQuandlInquiryTime") == m_systemOption.end()) {
		m_systemOption["WorldMarketQuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime; // Ĭ��ÿСʱ����ѯ100��
	}

	SaveDB();
}

void CGlobeOption::Update(json& jInput) {
	string sTemp;

	// ChinaMarket
	if (jInput.find("ChinaMarketRealtimeServer") != jInput.end()) {
		m_iChinaMarketRealtimeServer = jInput["ChinaMarketRealtimeServer"]; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	}

	if (jInput.find("ChinaMarketRealtimeInquiryTime") != jInput.end()) {
		m_iChinaMarketRealtimeInquiryTime = jInput["ChinaMarketRealtimeInquiryTime"]; // ʵʱ���ݲ�ѯʱ��������λ�����룩
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
		m_iWorldMarketFinnhubInquiryTime = jInput["WorldMarketFinnhubInquiryTime"]; // Ĭ��ÿСʱ����ѯ3000��
	}
	if (jInput.find("WorldMarketTiingoInquiryTime") != jInput.end()) {
		m_iWorldMarketTiingoInquiryTime = jInput["WorldMarketTiingoInquiryTime"]; // Ĭ��ÿСʱ����ѯ400��
	}
	if (jInput.find("WorldMarketQuandlInquiryTime") != jInput.end()) {
		m_iWorldMarketQuandlInquiryTime = jInput["WorldMarketQuandlInquiryTime"]; // Ĭ��ÿСʱ����ѯ100��
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