#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductNeteaseDayLine final : public CVirtualWebProduct {
public:
	CProductNeteaseDayLine();

	~CProductNeteaseDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};