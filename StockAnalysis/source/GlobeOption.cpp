#include"pch.h"

#include"ChinaMarket.h"

#include"GlobeOption.h"

using namespace std;
#include<iostream>
#include<fstream>

CGlobeOption::CGlobeOption() {
	m_fUpdate = false; // update flag
	m_strFileName = "C:\\StockAnalysis\\systemOption.json"; // json file name

	Load();

	// ChinaMarket
	if (m_systemOption.find("ChinaMarketRealtimeDataServer") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeDataServer"] = 0; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	}

	if (m_systemOption.find("ChinaMarketRealtimeDataInquiryTime") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeDataInquiryTime"] = 400; // 实时数据查询时间间隔（单位：毫秒）
	}
}

CGlobeOption::~CGlobeOption() {
	Save();
}

bool CGlobeOption::Load() {
	fstream f(m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_systemOption;
	}
	return true;
}

bool CGlobeOption::Save() {
	fstream f(m_strFileName, ios::out);
	f << m_systemOption;
	f.close();

	return true;
}

bool CGlobeOption::AssignKey(const char* key, CString value) {
	ASSERT(m_systemOption.find(key) != m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::AssignKey(const char* key, int value) {
	ASSERT(m_systemOption.find(key) != m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::AssignKey(const char* key, bool value) {
	ASSERT(m_systemOption.find(key) != m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::AssignKey(const char* key, double value) {
	ASSERT(m_systemOption.find(key) != m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::NewKey(const char* key, CString value) {
	ASSERT(m_systemOption.find(key) == m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::NewKey(const char* key, int value) {
	ASSERT(m_systemOption.find(key) == m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::NewKey(const char* key, bool value) {
	ASSERT(m_systemOption.find(key) == m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}

bool CGlobeOption::NewKey(const char* key, double value) {
	ASSERT(m_systemOption.find(key) == m_systemOption.end());
	m_systemOption[key] = value;
	return true;
}