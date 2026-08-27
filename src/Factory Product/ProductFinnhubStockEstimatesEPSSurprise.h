#pragma once

#include "EPSSurprise.h"
#include"ProductFinnhub.h"

class CEPSSurprise;

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CEPSSurprisesPtr Parse(const string& text);
};

using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
