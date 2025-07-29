#include"pch.h"

#include"NeteaseDayLineDataSource.h"
#include"ProductNeteaseDayLine.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "WebData.h"

CNeteaseDayLineDataSource::CNeteaseDayLineDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");

	CNeteaseDayLineDataSource::ConfigureInternetOption();

	CNeteaseDayLineDataSource::Reset();
}

bool CNeteaseDayLineDataSource::Reset() {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
// 抓取日线数据.开始于11:45:01
//
// 由于可能会抓取全部5000个左右日线数据，所需时间超过10分钟，故而9:15:00第一次重置系统时不去更新，而在9:25:00第二次重置系统后才开始。
// 为了防止与重启系统发生冲突，实际执行时间延后至11:45:01,且不是下载实时数据的工作时间
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请网易日线数据。
	if (gl_pChinaMarket->IsSystemReady() && gl_dataContainerChinaStock.IsUpdateDayLine() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			CreateProduct();
			return true;
		}
	}
	return false;
}

void CNeteaseDayLineDataSource::CreateProduct() {
	// 准备网易日线数据申请格式
	const string strMessage = _T("http://quotes.money.163.com/service/chddata.html?code=");;
	const string strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	string strParam = gl_dataContainerChinaStock.CreateNeteaseDayLineInquiringStr();
	if (strParam.length() > 0) {
		char buffer2[200];
		const string strStockCode = XferNeteaseToStandard(strParam);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiryDayLine(strStockCode);
		strParam += _T("");
		string s = fmt::format("&start=19900101&end={:8Ld}", gl_pChinaMarket->GetMarketDate());
		strParam += s;
		m_strParam = strParam;
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strParam);
		const CProductNeteaseDayLinePtr product = make_shared<CProductNeteaseDayLine>();
		product->SetMarket(gl_pChinaMarket);
		product->SetInquiryFunction(strMessage + strParam + strSuffix);
		StoreInquiry(product);
		SetInquiring(true);
	}
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
void CNeteaseDayLineDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CNeteaseDayLineDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 5000;
	m_internetOption.option_receive_timeout = 5000;
	m_internetOption.option_data_receive_timeout = 5000;
	m_internetOption.option_send_timeout = 500;
	m_internetOption.option_connect_retries = 1;
}

void CNeteaseDayLineDataSource::UpdateStatus(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineDataSource::SetDownLoadingStockCode(const string& strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
