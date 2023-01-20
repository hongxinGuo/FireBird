#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"

#include"VirtualWebProduct.h"

class CProductNeteaseRT final : public CVirtualWebProduct {
public:
	CProductNeteaseRT();
	~CProductNeteaseRT() override = default;

	CString CreateMessage(void) override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	bool ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};
