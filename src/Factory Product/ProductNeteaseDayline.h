#pragma once

#include"ClassDeclaration.h"

#include"VirtualChinaMarketWebProduct.h"

class CProductNeteaseDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseDayLine();

	~CProductNeteaseDayLine() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};

using CProductNeteaseDayLinePtr = shared_ptr<CProductNeteaseDayLine>;
