#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyPeer final : public CProductFinnhub {
public:
	CProductFinnhubCompanyPeer();
	~CProductFinnhubCompanyPeer() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	nlohmannJson Parse(const string& text);
};

using CFinnhubCompanyPeerPtr = shared_ptr<CProductFinnhubCompanyPeer>;
