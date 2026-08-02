module;
#define NOMINMAX
#include <afx.h>
#include"sqlpp23/sqlpp23.h"
#include"absl/log/absl_check.h"
module FireBirdLib.DataSource.AlphaVantage;

import FireBirdLib.Container.Stock.TiingoStock;
import FireBirdLib.FinnhubInquiryType;
import FireBirdLib.Market.WorldMarket;
import FireBirdLib.Product;
import FireBirdLib.Stock.FinnhubStock;
import FireBirdLib.Factory.AlphaVantage;
import FireBirdLib.Container.Stock.FinnhubStock;

import FireBirdLib.SystemConfiguration;
import FireBirdLib.SystemMessage;

using namespace sqlpp;

import std;
using std::make_unique;
using std::chrono::local_seconds;

namespace {
	auto s_setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
}

CAlphaVantageDataSource::CAlphaVantageDataSource() {
	m_pAlphaVantageFactory = std::make_unique<CAlphaVantageFactory>();

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

bool CAlphaVantageDataSource::GenerateInquiryMessage(const local_seconds& lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请Alpha Vantage各数据。
	if (llTickCount <= (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketAlphaVantageInquiryTime())) return false;
	m_PrevInquireTimePoint = llTickCount;
	ABSL_CHECK(!IsInquiring());
	if (GenerateStockSplit()) return true;
	if (GenerateStockDayLine()) return true;

	ABSL_CHECK(!IsInquiring());
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
	auto createProduct = [this](int inquireType) { return m_pAlphaVantageFactory->CreateProduct(gl_pWorldMarket, inquireType); };
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
	auto createProduct = [this](int inquireType) { return m_pAlphaVantageFactory->CreateProduct(gl_pWorldMarket, inquireType); };
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
