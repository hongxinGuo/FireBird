#pragma once

#include"ProductTiingo.h"
#include"TiingoCryptoSymbol.h"

class CProductTiingoCryptoSymbol final : public CProductTiingo {
public:
	CProductTiingoCryptoSymbol();
	~CProductTiingoCryptoSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCryptosPtr ParseTiingoCryptoSymbol(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;

	// ������
	long long m_differ1, m_differ2;
	double m_ratio;
};

using CProductTiingoCryptoSymbolPtr = shared_ptr<CProductTiingoCryptoSymbol>;
