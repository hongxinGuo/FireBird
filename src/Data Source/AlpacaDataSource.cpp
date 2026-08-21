#include"pch.h"

#include "AlpacaDataSource.h"

#include "FinnhubInquiryType.h"
#include "WorldMarket.h"
#include"VirtualWebProduct.h"
#include"AlpacaFactory.h"
#include"TiingoStock.h"
#include "ContainerTiingoStock.h"
#include"ContainerAlpacaStockSymbol.h"

#include "SystemConfiguration.h"
#include "SystemMessage.h"

namespace {
	auto s_setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
}

CAlpacaDataSource::CAlpacaDataSource() {
	m_pAlpacaFactory = std::make_unique<CAlpacaFactory>();

	ABSL_DCHECK(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = ""; // Alpaca有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strHeaders = "APCA-API-KEY-ID:PK3J5QOOORALNDMELW2XS5RDZX\r\nAPCA-API-SECRET-KEY:DybHyD53p5KCGLaSPd6oa6dKwA1cvtgSM5UGvC73oAfk\r\nConnection:close\r\n\r\n";
	m_strParam = "";
	m_strSuffix = "";
	m_strInquiryToken = "";
	m_lInquiringNumber = 1; // Alpaca实时数据查询数量默认值

	CAlpacaDataSource::ConfigureInternetOption();
	CAlpacaDataSource::Reset();

	m_bUsingNewInterface = true; // Alpaca使用新接口
}

bool CAlpacaDataSource::Reset() {
	m_bUpdateTradingAsset = true;
	m_fUpdateStockDayLine = false;// 更新日线时间由任务决定
	m_fAlpacaDataInquiryFinished = false;
	return true;
}

bool CAlpacaDataSource::GenerateInquiryMessage(const local_seconds& lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请Alpha Vantage各数据。
	if (llTickCount <= (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketAlpacaInquiryTime())) return false;
	m_PrevInquireTimePoint = llTickCount;
	ABSL_DCHECK(!IsInquiring());
	if (GenerateTradingAsset()) return true;
	if (GenerateStockDayLine()) return true;

	ABSL_DCHECK(!IsInquiring());
	if (!m_fAlpacaDataInquiryFinished) {
		gl_systemMessage.PushInformationMessage("Alpaca data inquiry finished");
		gl_systemMessage.SetCurrentAlpacaFunction("idling");
		m_fAlpacaDataInquiryFinished = true;
	}
	return false;
}

bool CAlpacaDataSource::GenerateTradingAsset() {
	auto isUpdateNeeded = [this]() { return IsUpdateTradingAsset(); };
	auto createProduct = [this](int inquireType) {
		return m_pAlpacaFactory->CreateProduct(gl_pWorldMarket, inquireType);
	};
	return GenerateSimpleInquiry(
		ALPACA_TRADING_ASSET_,
		isUpdateNeeded,
		createProduct,
		[] {
			gl_systemMessage.SetCurrentAlpacaFunction("Trading asset");
			gl_systemMessage.PushInformationMessage("Alpaca Trading asset updated");
		}
	);
}

bool CAlpacaDataSource::GenerateStockDayLine() {
	const auto size = gl_dataContainerTiingoStock.Size();
	bool haveInquiry = false;
	CTiingoStockPtr item;
	if (IsUpdateStockDayLine()) {
		bool found = false;
		size_t pos;
		for (pos = 0; pos < size; ++pos) {
			item = gl_dataContainerTiingoStock.GetItem(pos);
			if (item->IsUpdateDayLine()) {
				// Alpaca活跃证券中有大量option、ETF、债券等其他品种，只能作为辅助信息来源。
				if (gl_dataContainerAlpacaStockSymbol.IsSymbol(item->GetSymbol())) { // 如果同时alpaca证券集中有才申请。
					found = true;
					break;
				}
			}
		}
		if (found) {
			auto product = m_pAlpacaFactory->CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_);
			product->SetIndex(pos);
			StoreInquiry(product);
			std::string str = "DayLine:";
			str += item->GetSymbol();
			gl_systemMessage.SetCurrentAlpacaFunction(str);
			haveInquiry = true;
		}
		else {
			SetUpdateStockDayLine(false);
			ReportFinishedMsg("Alpaca dayline updated");
			haveInquiry = false;
		}
	}
	return haveInquiry;
}

void CAlpacaDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 100000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}
