#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualProductWebData.h"

class CProductSinaRT : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductSinaRT)
		CProductSinaRT();
	~CProductSinaRT() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

public:
	bool ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
