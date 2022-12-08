#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualProductWebData.h"

class CProductNeteaseDayLine : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductNeteaseDayLine)
		CProductNeteaseDayLine();
	~CProductNeteaseDayLine() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};