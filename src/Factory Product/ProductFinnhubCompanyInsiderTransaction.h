#pragma once

#include"ProductFinnhub.h"

#include"InsiderTransaction.h";

using std::shared_ptr;
using std::vector;

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;
	CInsiderTransactionsPtr Parse(const string& text);

	shared_ptr<vector<string>> CreateMessage() override;
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
