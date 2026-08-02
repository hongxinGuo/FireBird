module;
#include <afx.h>

module FireBirdLib.DataSource.Accessory;
import FireBirdLib.Factory.Accessory;
import FireBirdLib.Product;
import FireBirdLib.FinnhubInquiryType;
import FireBirdLib.SystemConfiguration;
import FireBirdLib.Market.WorldMarket;

import std;
using std::chrono::local_seconds;

CAccessoryDataSource::CAccessoryDataSource() {
	m_pAccessoryFactory = std::make_unique<CAccessoryFactory>();

	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = ""; // Accessory有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	//m_strHeaders = "User-Agent:PostmanRuntime/7.4.4.1\r\n";

	m_strParam = "";
	m_strSuffix = "";
	m_strInquiryToken = "";
	m_lInquiringNumber = 1; // Accessory实时数据查询数量默认值

	CAccessoryDataSource::ConfigureInternetOption();
	CAccessoryDataSource::Reset();
}

bool CAccessoryDataSource::Reset() {
	m_fAccessoryDataInquiryFinished = false;
	m_fUpdateIndexNasdaq100Stocks = true;
	return true;
}

bool CAccessoryDataSource::GenerateInquiryMessage(const local_seconds& lCurrentTime) {
	const auto llTickCount = GetTickCount();

	m_PrevInquireTimePoint = llTickCount;


	if (GenerateIndexNasdaq100Stocks()) return true;

	return false;
}

bool CAccessoryDataSource::GenerateIndexNasdaq100Stocks() {
	if (IsUpdateIndexNasdaq100Stocks()) {
		const CVirtualWebProductPtr p = m_pAccessoryFactory->CreateProduct(gl_pWorldMarket, ACCESSORY_INDEX_NASDAQ100_STOCKS_);
		StoreInquiry(p);
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
