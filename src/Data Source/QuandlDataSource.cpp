#include"pch.h"

#include "QuandlDataSource.h"

CQuandlDataSource::CQuandlDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Quandl有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("&api_key=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Quandl实时数据查询数量默认值

	CQuandlDataSource::ConfigureSession();
	CQuandlDataSource::Reset();
}

bool CQuandlDataSource::Reset() {
	m_fQuandlDataInquiryFinished = false;
	return true;
}

bool CQuandlDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime())) {
		m_llLastTimeTickCount = llTickCount;

		if (!IsInquiring()) {
			InquireQuandl();
			if (IsInquiring()) {
				return true;
			}
		}
	}
	return false;
}

bool CQuandlDataSource::InquireQuandl() {
	// not implemented
	return true;
}

void CQuandlDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2次重试
}
