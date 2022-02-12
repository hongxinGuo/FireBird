#pragma once

#include"VirtualMarket.h"

#include"PotenDailyBriefing.h"

using namespace std;
#include<vector>
#include<map>

class CPotenDailyBriefingMarket : public CVirtualMarket {
public:
	CPotenDailyBriefingMarket(void);
	virtual ~CPotenDailyBriefingMarket(void);

	virtual bool SchedulingTask(void) override final; // 由程序的定时器调度，大约每100毫秒一次
	virtual void ResetMarket(void) override;

	bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
	bool SchedulingTaskPerMinute(long lSecond, long lCurrentTime);
	bool SchedulingTaskPerHour(long lSecond, long lCurrentTime);
	bool TaskResetMarket(long lCurrentTime);
	void Reset(void);

	bool TaskProcessData(void);
	bool TaskCheckTodayDataUpdated(void);
	bool TaskInquiringData(void);
	bool TaskLoadDataBase(void);

	// 工作线程包裹函数
	bool CreatingThreadSavePotenData(void);

	bool LoadDatabase(void);
	virtual bool SaveCurrentData(void);

	void ChoiceNextInquiringDate(void);
	bool IsTodayDataUpdated(void) noexcept { return m_fTodayDataUpdated; }
	void SetTodayDataUpdated(bool fFlag) noexcept { m_fTodayDataUpdated = fFlag; }

	bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
	void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
	long GetCurrentInquiringDate(void) noexcept { return m_lCurrentInquiringDate; }
	void SetCurrentInquiringDate(long lDate) noexcept { m_lCurrentInquiringDate = lDate; }

	size_t GetDatabaseSize(void) noexcept { return m_vPotenDailyBriefing.size(); }
	void ClearDatabase(void) { m_vPotenDailyBriefing.resize(0); }

public:
	void __TEST_SetLoadedDate(long lDate, bool fFlag) { m_mapDataLoadedDays[lDate] = fFlag; }

protected:
	CPotenDailyBriefingPtr m_pDataToSaved;
	bool m_fDataBaseLoaded;
	bool m_fTodayDataUpdated;
	vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
	map<long, bool> m_mapDataLoadedDays;
	long m_lCurrentInquiringDate;
	long m_lToday;
};

typedef shared_ptr<CPotenDailyBriefingMarket> CPotenDailyBriefingMarketPtr;

extern CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket;
