#include"pch.h"

#include "AlphaVantageDataSource.h"

#include "FinnhubInquiryType.h"
#include "WorldMarket.h"
#include"VirtualWebProduct.h"

#include "spdlog_assert.h"

namespace {
	auto s_setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
}

CAlphaVantageDataSource::CAlphaVantageDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = ""; // AlphaVantage有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = "";
	m_strSuffix = "&apikey=";
	m_strInquiryToken = "";
	m_lInquiringNumber = 1; // AlphaVantage实时数据查询数量默认值

	CAlphaVantageDataSource::ConfigureInternetOption();
	CAlphaVantageDataSource::Reset();
}

bool CAlphaVantageDataSource::Reset() {
	m_fAlphaVantageDataInquiryFinished = false;
	return true;
}

bool CAlphaVantageDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请Alpha Vantage各数据。
	if (llTickCount <= (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketAlphaVantageInquiryTime())) return false;
	m_PrevInquireTimePoint = llTickCount;
	SPDLOG_ASSERT(!IsInquiring());
	if (GenerateStockSplit()) return true;
	if (GenerateStockDayLine()) return true;

	SPDLOG_ASSERT(!IsInquiring());
	if (!m_fAlphaVantageDataInquiryFinished) {
		gl_systemMessage.PushInformationMessage("Alpha Vantage data inquiry finished");
		gl_systemMessage.SetCurrentAlphaVantageFunction("finished");
		m_fAlphaVantageDataInquiryFinished = true;
	}
	return false;
}

bool CAlphaVantageDataSource::GenerateStockSplit() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockSplit(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateSplit(); };
	auto createProduct = [this](int inquireType) { return m_AlphaVantageFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = "Split :";
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentAlphaVantageFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockSplit(flag); };
	const std::string finishedMsg = "Alpha Vantage stock split updated";

	return GenerateInquiryIterateWithoutAccessCheck(
		gl_dataContainerTiingoStock,
		STOCK_PRICE_SPLITS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CAlphaVantageDataSource::GenerateStockDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto createProduct = [this](int inquireType) { return m_AlphaVantageFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = "DayLine:";
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockDayLine(flag); };
	const std::string finishedMsg = "Alpha Vantage dayline updated";

	return GenerateInquiryIterateWithoutAccessCheck(
		gl_dataContainerFinnhubStock,
		STOCK_PRICE_CANDLES_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

void CAlphaVantageDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 100000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}
