#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualProductWebData.h"

class CProductNeteaseRT : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductNeteaseRT)
		CProductNeteaseRT();
	~CProductNeteaseRT() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

public:
	bool ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};