#pragma once

#include"ProductTiingo.h"

class CTiingoCrypto;

class CProductTiingoCryptoSymbol final : public CProductTiingo {
public:
	CProductTiingoCryptoSymbol();
	// 不允许赋值。
	CProductTiingoCryptoSymbol(const CProductTiingoCryptoSymbol&) = delete;
	CProductTiingoCryptoSymbol& operator=(const CProductTiingoCryptoSymbol&) = delete;
	CProductTiingoCryptoSymbol(const CProductTiingoCryptoSymbol&&) noexcept = delete;
	CProductTiingoCryptoSymbol& operator=(const CProductTiingoCryptoSymbol&&) noexcept = delete;
	~CProductTiingoCryptoSymbol() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	shared_ptr<vector<shared_ptr<CTiingoCrypto>>> Parse(const string& text);

	void UpdateSystemStatus() override;
};

using CProductTiingoCryptoSymbolPtr = shared_ptr<CProductTiingoCryptoSymbol>;
