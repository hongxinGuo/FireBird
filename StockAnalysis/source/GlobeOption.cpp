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
		m_systemOption["ChinaMarketRealtimeDataServer"] = 0; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	}

	if (m_systemOption.find("ChinaMarketRealtimeDataInquiryTime") == m_systemOption.end()) {
		m_systemOption["ChinaMarketRealtimeDataInquiryTime"] = 400; // ʵʱ���ݲ�ѯʱ��������λ�����룩
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