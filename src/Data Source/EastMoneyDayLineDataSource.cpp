////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 东方财富日线历史数据。
///
/// 东方财富日线服务器一次只能发送最多1000个数据，超过1000个数据的申请，需要拆分成多次方可。故而申请信息的处理只能放在DataSource中处理，
/// product中存储的是处理后的完整申请字符串。
/// 日线的申请格式为：https://push2his.eastmoney.com/api/qt/stock/kline/get?secid=1.601872&fields1=f1,f2,f3,f4,f5,f6&fields2=f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61&klt=101&fqt=1&end=20250101&lmt=1000
/// 
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include <random>

#include"ChinaStockCodeConverter.h"
#include "EastmoneyDayLineDataSource.h"
#include"ProductEastmoneyDayLine.h"

#include"ChinaMarket.h"
#include "spdlog_assert.h"
#include "TimeConvert.h"
#include "WebData.h"

CEastmoneyDayLineDataSource::CEastmoneyDayLineDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = "";
	m_strHeaders = "Referer:https://quote.eastmoney.com/\r\n";
	m_strParam = "";
	m_strSuffix = "";
	m_iMaxNormalInquireTime = 500;

	CEastmoneyDayLineDataSource::ConfigureInternetOption();
	CEastmoneyDayLineDataSource::Reset();
}

bool CEastmoneyDayLineDataSource::Reset() {
	m_fUpdateDayLine = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
// 抓取日线数据.开始于11:45:01
//
// 由于可能会抓取全部5000个左右日线数据，所需时间超过10分钟，故而9:15:00第一次重置系统时不去更新，而在9:25:00第二次重置系统后才开始。
// 为了防止与重启系统发生冲突，实际执行时间延后至11:45:01,且不是下载实时数据的工作时间
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEastmoneyDayLineDataSource::GenerateInquiryMessage(const chrono::local_seconds& currentTime) {
	static int s_iSleep = 0;
	static int s_number = 0;
	int startDuration = 3000;
	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请日线数据。
	std::random_device r;
	// Choose a random mean between 1 and 6
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 4000);
	int mean = uniform_dist(e1);
	if (s_iSleep > 10 + s_number) {
		s_iSleep = 0;
		s_number = mean / 200;
		m_PrevInquireTimePoint += chrono::milliseconds(300000 + mean * 1000);
	}
	const auto llTickCount = GetTickCount();
	int duration = startDuration + mean;
	if (llTickCount < m_PrevInquireTimePoint + chrono::milliseconds(duration)) return false;
	// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
	if (!IsInquiring()) {
		m_PrevInquireTimePoint = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
	}

	if (gl_pChinaMarket->IsSystemReady() && gl_dataContainerChinaStock.IsUpdateDayLine()/* && gl_pChinaMarket->GetMarketTimeHMS().to_duration() > 9h + 30min*/) {
		if (!IsInquiring()) {
			s_iSleep++;
			Inquire();
			return true;
		}
	}
	return false;
}

bool CEastmoneyDayLineDataSource::Inquire() {
	const auto lStockSetSize = gl_dataContainerChinaStock.Size();

	if (!IsInquiring() && IsUpdateDayLine()) {
		CChinaStockPtr pStock;
		bool fFound = false;
		for (size_t lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			pStock = gl_dataContainerChinaStock.GetStock(lCurrentUpdateDayLinePos);
			if (!pStock->IsUpdateDayLine()) { // 需要更新？
				continue;
			}
			if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
				pStock->SetUpdateDayLine(false);
				continue;
			}
			fFound = true;
			break;
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = CreateProduct(pStock);
			StoreInquiry(product);
			SetDownLoadingStockCode(pStock->GetSymbol());
			gl_systemMessage.SetStockCodeForInquiryDayLine(pStock->GetSymbol());
			pStock->SetUpdateDayLine(false);
			return true;
		}
		else {
			SetUpdateDayLine(false);
			const string str = "中国市场股票日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/// 东方财富日线服务器申请信息的处理只能放在DataSource中处理，
/// product中存储的是处理后的完整申请字符串。
/// 腾讯日线的申请格式为：https://push2his.eastmoney.com/api/qt/stock/kline/get?secid=market.stockCode&fields1=f1,f2,f3,f4,f5,f6
///     &fields2=f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61&klt=101&fqt=1&end=20250101&lmt=number
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CVirtualWebProductPtr CEastmoneyDayLineDataSource::CreateProduct(const CChinaStockPtr& pStock) const {
	//long lStartDate = 20100101; // 强迫生成多次申请（测试用）
	chrono::local_days lStartDate = GetPrevDay(pStock->GetDayLineEndDate()); // 东方财富日线没有提供昨收盘信息，故而多申请一天数据来更新昨收盘。
	const chrono::local_days lCurrentDate = gl_pChinaMarket->GetMarketDate();
	int differDays = (lCurrentDate - lStartDate).count();
	const auto lStockIndex = gl_dataContainerChinaStock.GetOffset(pStock);
	const string strStockCode = XferStandardToEastmoney(pStock->GetSymbol());
	shared_ptr<CProductEastmoneyDayLine> product = nullptr;
	string sLength;
	if (differDays < 365) {
		sLength = std::format("{:d}", differDays);
	}
	else {
		sLength = "20000";
	}
	string sEndDate = toFormattedDateString(lCurrentDate);
	const string strTotalMessage = "https://push2his.eastmoney.com/api/qt/stock/kline/get?secid=" + strStockCode
	+ "&fields1=f1,f2,f3,f4,f5,f6&fields2=f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61&klt=101&fqt=1&end=" + sEndDate + "&lmt=" + sLength;
	product = make_shared<CProductEastmoneyDayLine>();
	product->SetInquiringSymbol(pStock->GetSymbol());
	product->SetIndex(lStockIndex);
	product->SetInquiryFunction(strTotalMessage);

	dynamic_pointer_cast<CProductEastmoneyDayLine>(product)->SetInquiryNumber(1);

	return product;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
void CEastmoneyDayLineDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();// 腾讯日线的查询字符串，在生成product时即完成了
}

void CEastmoneyDayLineDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 10000;
	m_internetOption.option_receive_timeout = 30000;
	m_internetOption.option_data_receive_timeout = 30000;
	m_internetOption.option_send_timeout = 5000;
	m_internetOption.option_connect_retries = 1;
}

void CEastmoneyDayLineDataSource::CheckWebData(const CWebDataPtr& pWebData) {
	ASSERT(m_pCurrentProduct != nullptr);

	m_eErrorMessageData = ERROR_NO_ERROR_;
	// 第一次switch处理非json数据格式的错误
	switch (m_dwHTTPStatusCode) {
	case 501://请求功能尚未实现，实际是服务器正处于维护状态
		TRACE(_T("关闭东方财富日线服务\n"));
		Enable(false); // 先暂停
		break;
	case 200:
		// everything is OK
		break;
	default: // something wrong,
		TRACE(_T("关闭东方财富日线服务\n"));
		Enable(false); // 先暂停
		break;
	}
}

void CEastmoneyDayLineDataSource::UpdateStatus(const CWebDataPtr& pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CEastmoneyDayLineDataSource::SetDownLoadingStockCode(const string& strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
