#include"pch.h"

#include "QuandlDataSource.h"

CQuandlDataSource::CQuandlDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Quandl有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("&api_key=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Quandl实时数据查询数量默认值

	CQuandlDataSource::ConfigureInternetOption();
	CQuandlDataSource::Reset();
}

bool CQuandlDataSource::Reset() {
	m_fQuandlDataInquiryFinished = false;
	return true;
}

bool CQuandlDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (llTickCount > (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime())) {
		m_PrevInquireTimePoint = llTickCount;

		if (!IsInquiring()) {
			Inquire();
			if (IsInquiring()) {
				return true;
			}
		}
	}
	return false;
}

bool CQuandlDataSource::Inquire() {
	// not implemented
	return true;
}

void CQuandlDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 100000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}
