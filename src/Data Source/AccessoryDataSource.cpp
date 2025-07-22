#include"pch.h"

#include "AccessoryDataSource.h"

#include "FinnhubInquiryType.h"
#include "WorldMarket.h"

CAccessoryDataSource::CAccessoryDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Accessory有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	//m_strHeaders = _T("User-Agent:PostmanRuntime/7.4.4.1\r\n");

	m_strParam = _T("");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Accessory实时数据查询数量默认值

	CAccessoryDataSource::ConfigureInternetOption();
	CAccessoryDataSource::Reset();
}

bool CAccessoryDataSource::Reset() {
	m_fAccessoryDataInquiryFinished = false;
	m_fUpdateIndexNasdaq100Stocks = true;
	return true;
}

bool CAccessoryDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();

	m_PrevInquireTimePoint = llTickCount;

	ASSERT(!IsInquiring());

	if (GenerateIndexNasdaq100Stocks()) return true;

	ASSERT(!IsInquiring());
	return false;
}

bool CAccessoryDataSource::GenerateIndexNasdaq100Stocks() {
	ASSERT(!IsInquiring());
	if (IsUpdateIndexNasdaq100Stocks()) {
		const CVirtualProductWebDataPtr p = m_AccessoryFactory.CreateProduct(gl_pWorldMarket, ACCESSORY_INDEX_NASDAQ100_STOCKS);
		StoreInquiry(p);
		SetInquiring(true);
		return true;
	}
	return false;
}

void CAccessoryDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 100000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}
