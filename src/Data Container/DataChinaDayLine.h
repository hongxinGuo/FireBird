#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CDataChinaDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataChinaDayLine();
	~CDataChinaDayLine() override = default;

public:
	bool SaveDB(const CString& strStockSymbol) final;
	bool LoadDB(const CString& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);

	// todo 使用如下方式，导致编译出错。不知何故
	//CDayLinePtr GetDayLine(size_t lIndex) const { return dynamic_pointer_cast<CDayLine>(GetData(lIndex)); }

public:
	// 特有函数
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};

using CDataChinaDayLinePtr = shared_ptr<CDataChinaDayLine>;
