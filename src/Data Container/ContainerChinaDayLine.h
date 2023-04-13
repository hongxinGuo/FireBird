#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CContainerChinaDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaDayLine();
	~CContainerChinaDayLine() override = default;

public:
	bool SaveDB(const CString& strStockSymbol) final;
	bool LoadDB(const CString& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos) const;

	// todo ʹ�����·�ʽ�����±��������֪�ι�
	//CDayLinePtr GetDayLine(size_t lIndex) const { return dynamic_pointer_cast<CDayLine>(GetData(lIndex)); }

public:
	// ���к���
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const;

private:
};

using CDataChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
