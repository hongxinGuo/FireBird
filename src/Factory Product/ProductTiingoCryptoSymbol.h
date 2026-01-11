#pragma once

#include"ProductTiingo.h"
#include"TiingoCrypto.h"

class CProductTiingoCryptoSymbol final : public CProductTiingo {
public:
	CProductTiingoCryptoSymbol();
	// 不允许赋值。
	CProductTiingoCryptoSymbol(const CProductTiingoCryptoSymbol&) = delete;
	CProductTiingoCryptoSymbol& operator=(const CProductTiingoCryptoSymbol&) = delete;
	CProductTiingoCryptoSymbol(const CProductTiingoCryptoSymbol&&) noexcept = delete;
	CProductTiingoCryptoSymbol& operator=(const CProductTiingoCryptoSymbol&&) noexcept = delete;
	~CProductTiingoCryptoSymbol() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCryptosPtr ParseTiingoCryptoSymbol(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;

	// 测试用
	long long m_differ1, m_differ2;
	double m_ratio;
};

using CProductTiingoCryptoSymbolPtr = shared_ptr<CProductTiingoCryptoSymbol>;
