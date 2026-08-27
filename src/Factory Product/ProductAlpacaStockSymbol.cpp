///////////////////////////////////////////////////////////////////////////
///
/// Alpaca股票日线申请接受处理
///
/// 当网络
///
///
///
///
//////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "ProductAlpacaStockSymbol.h"
#include "AlpacaDataSource.h"
#include"AlpacaStock.h"
#include "SystemData.h"
#include "SystemMessage.h"
#include "WebData.h"
#include"ContainerAlpacaStockSymbol.h"

#include<cpr/cpr.h>

using namespace std;

namespace {
	struct Symbol {
		string classes;
		string exchange;
		string symbol;
		string name;
		string status;
		bool tradable;
	};

	void from_json(const nlohmann::json& j, Symbol& b) {
		j.at("class").get_to(b.classes);
		j.at("exchange").get_to(b.exchange);
		j.at("symbol").get_to(b.symbol);
		j.at("name").get_to(b.name);
		j.at("status").get_to(b.status);
		j.at("tradable").get_to(b.tradable);
	}
}

CProductAlpacaStockSymbol::CProductAlpacaStockSymbol() {
	m_strInquiryFunction = "https://paper-api.alpaca.markets/v2/assets?";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>
/// 
/// </summary>
/// <param name="strHeaders"></param>
/// <param name="strParams"></param>
/// <param name="strSuffix"></param>
/// <param name="strInquiryToken"></param>
/// 
/// 使用cpr库，由本函数申请网络数据。注意：此函数只在使用cpr库时才会被调用。
/// 
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductAlpacaStockSymbol::InquireData(const std::stop_token& st) {
	shared_ptr<vector<CAlpacaStockPtr>> pvStock = make_shared<vector<CAlpacaStockPtr>>();
	shared_ptr<vector<string>> pInquiry = CreateMessage();

	auto inquireStrings = CreateMessage();
	ABSL_DCHECK(inquireStrings->size() == 1);
	cpr::Response r = cpr::Get(cpr::Url{ inquireStrings->at(0) }, gl_pAlpacaDataSource->GetHeader());

	long status = r.status_code;
	if (status != 200) {
		WebStatusCheck(r);
		return;
	}

	pvStock = Parse(r);

	std::ranges::sort(*pvStock, [](const CAlpacaStockPtr& a, const CAlpacaStockPtr& b) {
		return a->GetSymbol() < b->GetSymbol();
	});
	for (const auto& pStock : *pvStock) {
		if (!gl_dataContainerAlpacaStockSymbol.IsSymbol(pStock->GetSymbol())) { // 新代码
			pStock->SetUpdateProfileDB(true);
			pStock->SetNewStock(true);
			gl_dataContainerAlpacaStockSymbol.Add(pStock); // 存储新代码
		}
	}
}

void CProductAlpacaStockSymbol::WebStatusCheck(cpr::Response& r) {
	nlohmannJson j;
	string message;
	switch (r.status_code) {
	case 200: // good;
		break;
	case 400: // one of request parameters is invalid.See the return message for detail.
		j = nlohmann::json::parse(r.text, nullptr, false);
		message = "Alpaca stock assert: ";
		message += j.at("message");
		gl_systemMessage.PushErrorMessage(message);
		break;
	case 401: // Authentication headers are missing or invalid.
		break;
	case 403: // The requested resource is forbidden.
		break;
	case 429: // Too many requests.You hit the rate limit.
		break;
	case 500: // Internal server error.
		break;
	default: // unknown problem
		string number = std::format("Error: Alpaca stock assert {:d}", r.status_code);
		gl_systemMessage.PushErrorMessage(number);
		break;
	}
}

void CProductAlpacaStockSymbol::UpdateSystemStatus() {
	gl_pAlpacaDataSource->SetUpdateTradingAsset(false);
}
///////////////////////////////////////////////////////////////////////////////////////////
///
/// 
///
///////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<std::vector<std::string>> CProductAlpacaStockSymbol::CreateMessage() {
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	string sParam = "status=active";
	pInquiry->push_back(m_strInquiryFunction + sParam);
	return pInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 
/// [
/// {
///	"id": "631be973-b807-4344-a470-829a83f7ff2e",
///		"class" : "us_equity",
///		"exchange" : "OTC",
///		"symbol" : "ILLMF",
///		"name" : "Illumin Holdings Inc. Common Stock (Canada)",
///		"status" : "active",
///		"tradable" : false,
///		"marginable" : false,
///		"maintenance_margin_requirement" : 100,
///		"margin_requirement_long" : "100",
///		"margin_requirement_short" : "100",
///		"shortable" : false,
///		"easy_to_borrow" : false,
///		"borrow_status" : "hard_to_borrow",
///		"fractionable" : false,
///		"attributes" : []
/// },
///	{
///		"id": "d2e2e46f-5bd9-4551-9bb6-466320a3ca89",
///		"class" : "us_equity",
///		"exchange" : "OTC",
///		"symbol" : "NBSTW",
///		"name" : "NEWBURY STR ACQUISITION CORP Warrant   04/30/2028",
///		"status" : "active",
///		"tradable" : false,
///		"marginable" : false,
///		"maintenance_margin_requirement" : 100,
///		"margin_requirement_long" : "100",
///		"margin_requirement_short" : "100",
///		"shortable" : false,
///		"easy_to_borrow" : false,
///		"borrow_status" : "hard_to_borrow",
///		"fractionable" : false,
///		"attributes" : []
///	},
///	]
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CAlpacaStockPtr>> CProductAlpacaStockSymbol::Parse(const cpr::Response& r) {
	shared_ptr<vector<CAlpacaStockPtr>> pvStock = make_shared<vector<CAlpacaStockPtr>>();
	nlohmannJson j = nlohmann::json::parse(r.text, nullptr, false);
	if (j.is_discarded()) {
		gl_systemMessage.PushErrorMessage("Alpaca asset parse failed: invalid JSON");
		return pvStock;
	}
	if (!j.is_array()) {
		gl_systemMessage.PushErrorMessage("Alpaca asset parse failed: expected JSON array");
		return pvStock;
	}

	for (const auto& item : j) {
		try {
			Symbol s;
			item.get_to(s); // uses local from_json to populate Symbol

			auto pStock = std::make_shared<CAlpacaStock>();
			pStock->SetSymbol(s.symbol);
			pStock->SetDisplaySymbol(s.name);
			pStock->SetExchange(s.exchange);
			pvStock->push_back(pStock);
		} catch (const std::exception& ex) {
			std::string msg = std::format("Alpaca asset parse exception: {}", ex.what());
			gl_systemMessage.PushErrorMessage(msg);
		} catch (...) {
			gl_systemMessage.PushErrorMessage("Alpaca asset parse unknown exception");
		}
	}
	return pvStock;
}
