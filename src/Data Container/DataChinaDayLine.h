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

	// todo ʹ�����·�ʽ�����±��������֪�ι�
	//CDayLinePtr GetDayLine(size_t lIndex) const { return dynamic_pointer_cast<CDayLine>(GetData(lIndex)); }

public:
	// ���к���
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};

using CDataChinaDayLinePtr = shared_ptr<CDataChinaDayLine>;
