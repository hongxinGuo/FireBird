#include"pch.h"

#include "AlpacaDataSource.h"

#include "ContainerTiingoStock.h"
#include "FinnhubInquiryType.h"
#include "WorldMarket.h"
#include"VirtualWebProduct.h"
#include"FinnhubStock.h"
#include"AlpacaFactory.h"

#include "ContainerFinnhubStock.h"
#include "TiingoStock.h"

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
	m_fAlpacaDataInquiryFinished = false;
	return true;
}

bool CAlpacaDataSource::GenerateInquiryMessage(const local_seconds& lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请Alpha Vantage各数据。
	if (llTickCount <= (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketAlpacaInquiryTime())) return false;
	m_PrevInquireTimePoint = llTickCount;
	ABSL_DCHECK(!IsInquiring());
	if (GenerateStockDayLine()) return true;

	ABSL_DCHECK(!IsInquiring());
	if (!m_fAlpacaDataInquiryFinished) {
		gl_systemMessage.PushInformationMessage("Alpha Vantage data inquiry finished");
		m_fAlpacaDataInquiryFinished = true;
	}
	return false;
}

bool CAlpacaDataSource::GenerateStockDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto createProduct = [this](int inquireType) { return m_pAlpacaFactory->CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = "DayLine:";
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockDayLine(flag); };
	const std::string finishedMsg = "Alpha Vantage dayline updated";

	return GenerateInquiryIterateWithoutAccessCheck(
		gl_dataContainerTiingoStock,
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

void CAlpacaDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 100000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}
