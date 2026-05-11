#pragma once

#include "WebRTData.h"

//成交的具体情况，分为三种：买，进攻性买，强买，。买是价位为卖一位置；进攻性买价位是至少卖二，且成交价位高于卖一低于卖二；
//强买价位至少卖三，且成交价位至少高于卖二。判断卖与之相类似。
enum {
	ATTACK_BUY_ = 1,
	STRONG_BUY_ = 2,
	ORDINARY_BUY_ = 3,
	UNKNOWN_BUYSELL_ = 4,
	ORDINARY_SELL_ = 5,
	STRONG_SELL_ = 6,
	ATTACK_SELL_ = 7,
	NO_TRANSACTION_ = 8
};

#include"RSReference.h"

#include"VirtualStock.h"

#include"SetChinaMarketDayLineInfo.h"
#include"SetWeekLineInfo.h"
#include"SetChinaStockSymbol.h"

#include"DayLine.h"
#include"ContainerChinaDayLine.h"
#include"ContainerChinaWeekLine.h"
#include"ContainerChinaStockMonthLine.h"

#include<concurrentqueue/moodycamel/concurrentqueue.h>
using namespace moodycamel;

class CChinaStock;
using CChinaStockPtr = shared_ptr<CChinaStock>;

#include <queue>

bool IsShareA(const string& strStockCode);
bool IsChinaStock(const CVirtualStockPtr& pStock);

// 证券名称数据包
class CChinaStock : public CVirtualStock {
public:
	CChinaStock();
	// 不允许赋值。
	CChinaStock(const CChinaStock&) = delete;
	CChinaStock& operator=(const CChinaStock&) = delete;
	CChinaStock(const CChinaStock&&) noexcept = delete;
	CChinaStock& operator=(const CChinaStock&&) noexcept = delete;
	~CChinaStock() override = default;

	int GetRatio() const final { return 1000; }

public:
	void UpdateRTData(const CWebRTDataPtr& pRTData);
	void UpdateStatus(const CWebRTDataPtr& pRTData);

	// 本股票各变量状态
	long GetHighLimitFromTengxun() const noexcept { return m_lHighLimitFromTengxun; }
	void SetHighLimitFromTengxun(const long lValue) noexcept { m_lHighLimitFromTengxun = lValue; }
	long GetLowLimitFromTengxun() const noexcept { return m_lLowLimitFromTengxun; }
	void SetLowLimitFromTengxun(const long lValue) noexcept { m_lLowLimitFromTengxun = lValue; }
	long GetHighLimit() const noexcept { return m_lHighLimit; }
	void SetHighLimit(const long lValue) noexcept { m_lHighLimit = lValue; }
	long GetLowLimit() const noexcept { return m_lLowLimit; }
	void SetLowLimit(const long lValue) noexcept { m_lLowLimit = lValue; }
	long GetPBuy(const int iIndex) const { return m_lPBuy.at(iIndex); }
	long GetVBuy(const int iIndex) const { return m_lVBuy.at(iIndex); }
	long GetPSell(const int iIndex) const { return m_lPSell.at(iIndex); }
	long GetVSell(const int iIndex) const { return m_lVSell.at(iIndex); }
	void SetPBuy(const int iIndex, const long value) { m_lPBuy.at(iIndex) = value; }
	void SetVBuy(const int iIndex, const long value) { m_lVBuy.at(iIndex) = value; }
	void SetPSell(const int iIndex, const long value) { m_lPSell.at(iIndex) = value; }
	void SetVSell(const int iIndex, const long value) { m_lVSell.at(iIndex) = value; }
	double GetRS() const noexcept { return m_dRealtimeRS; }
	void SetRS(const double value) noexcept { m_dRealtimeRS = value; }
	double GetRSIndex() const noexcept { return m_dRealtimeRSIndex; }
	void SetRSIndex(const double value) noexcept { m_dRealtimeRSIndex = value; }

	// 更新当前各变量状态
	void SetLastSavedVolume(const INT64 llVolume) noexcept { m_llLastSavedVolume = llVolume; }
	INT64 GetLastSavedVolume() const noexcept { return m_llLastSavedVolume; }

	// 各种状态标识提取和设置
	bool IsChosen() const noexcept { return m_fChosen; }
	void SetChosen(const bool fChosen) noexcept { m_fChosen = fChosen; }
	bool IsUpdateChosenStockDB() const noexcept { return m_fUpdateChosenStockDB; }
	void SetUpdateChosenStockDB(const bool fSaved) noexcept { m_fUpdateChosenStockDB = fSaved; }

	// 第一个实时数据判断和设置
	bool HaveFirstRTData() const noexcept { return m_fHaveFirstRTData; }

	bool SetHavingFirstRTData(const bool fFlag) noexcept {
		if (m_fHaveFirstRTData || !fFlag) return false;
		m_fHaveFirstRTData = fFlag;
		return true;
	}

	void SetNeedProcessRTData(const bool fFlag) noexcept { m_fNeedProcessRTData = fFlag; }
	bool IsNeedProcessRTData() const noexcept { return m_fNeedProcessRTData; }
	void SetRTDataCalculated(const bool fFlag) noexcept { m_fRTDataCalculated = fFlag; }
	bool IsRTDataCalculated() const noexcept { return m_fRTDataCalculated; }

	bool IsTodayDataActive() const; //采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据。当然在m_fActive为真状态下。
	bool IsTodayDataChanged() const; // 如果最高价、最低价、成交量和成交额中有数据不为零，则返回真。

	// 数据库的提取和存储
	// 日线装载函数，由工作线程ThreadLoadDayLine调用
	bool LoadDayLineDB() override { return m_dataDayLine.LoadDB(GetSymbol()); }
	virtual bool SaveDayLineBasicInfo() { return m_dataDayLine.SaveDB(GetSymbol()); }

	bool LoadDayLineBasicInfo(CSetChinaMarketDayLineInfo* pSetDayLineBasicInfo) { return m_dataDayLine.LoadBasicDB(pSetDayLineBasicInfo); }

	void AppendTodayBasicInfo(CSetChinaMarketDayLineInfo* pSetDayLine) const; // 存储当日基本数据
	void UpdateCurrentHistoryCandle(const CVirtualHistoryCandlePtr& pBeUpdated) const; // 用当前状态更新历史数据
	void UpdateDayLineStartEndDate();
	bool LoadStockCodeDB(CSetChinaStockSymbol& setChinaStockSymbol);
	void CheckNeedProcessRTData();
	void CheckIPOStatus();
	bool CheckDayLineStatus();
	//周线历史数据存取
	bool LoadWeekLineDB() override { return m_dataWeekLine.LoadDB(GetSymbol()); }
	virtual bool SaveWeekLine() { return m_dataWeekLine.SaveDB(GetSymbol()); }
	bool LoadWeekLineBasicInfo(CSetWeekLineInfo* pSetWeekLineBasicInfo) { return m_dataWeekLine.LoadBasicDB(pSetWeekLineBasicInfo); }
	virtual bool BuildWeekLine(long lStartDate = 19900101);

	// 月线历史数据存取
	bool LoadMonthLineDB() override { return CreateMonthLine(); }

	bool CreateMonthLine();

	void PushRTData(const CWebRTDataPtr& pData) { m_qRTData.enqueue(pData); }
	CWebRTDataPtr PopRTData() {
		CWebRTDataPtr pData = nullptr;
		m_qRTData.try_dequeue(pData);
		return pData;
	}
	auto GetRTDataQueueSize() const { return m_qRTData.size_approx(); }
	// 清空存储实时数据的队列
	void ClearRTDataDeque();

	//日线相关函数
	// 日线历史数据
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	bool HaveNewDayLineData();
	void UnloadDayLine() noexcept { m_dataDayLine.Unload(); }
	void StoreDayLine(const CDayLine& dayLine) { m_dataDayLine.Add(dayLine); }
	CDayLine* GetDayLine(const size_t lIndex) { return m_dataDayLine.GetData(lIndex); }
	void Get1DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS1(vRS); }
	void GetRSIndex1Day(vector<double>& vRS) const { m_dataDayLine.GetRSIndex1(vRS); }
	void GetRSLogarithm1Day(vector<double>& vRS) const { m_dataDayLine.GetRSLogarithm1(vRS); }
	void Get3DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS3(vRS); }
	void Get5DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS5(vRS); }
	void Get10DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS10(vRS); }
	void Get30DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS30(vRS); }
	void Get60DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS60(vRS); }
	void Get120DaysRS(vector<double>& vRS) const { m_dataDayLine.GetRS120(vRS); }

	void ProcessRTData();
	// 日线相对强度计算
	bool CalculateDayLineRS() { return m_dataDayLine.CalculateRS0(); }
	bool CalculateDayLineRSIndex() { return m_dataDayLine.CalculateRSIndex0(); }
	bool CalculateDayLineRSLogarithm() { return m_dataDayLine.CalculateRSLogarithm0(); }

	bool IsDayLineDBUpdated() const noexcept { return (m_fDayLineDBUpdated); }
	void SetDayLineDBUpdated(const bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }
	bool IsDayLineLoaded() const noexcept override { return m_dataDayLine.IsDataLoaded(); }
	void SetDayLineLoaded(const bool fFlag) noexcept override { m_dataDayLine.SetDataLoaded(fFlag); }

	// 提取网易日线历史数据各函数
	void UpdateStatusByDownloadedDayLine();

	void UpdateDayLine(const vector<CDayLine>& vTempDayLine) { m_dataDayLine.UpdateData(vTempDayLine); }
	static void ReportDayLineDownLoaded();
	// 当前被处理历史数据容器
	CVirtualDataHistoryCandle* DayLine() noexcept final { return &m_dataDayLine; }
	CVirtualDataHistoryCandle* WeekLine() noexcept final { return &m_dataWeekLine; }
	CVirtualDataHistoryCandle* MonthLine() noexcept final { return &m_dataMonthLine; }

	// 周线相关函数
	size_t GetWeekLineSize() const noexcept { return m_dataWeekLine.Size(); }
	CWeekLine* GetWeekLine(const long lIndex) { return m_dataWeekLine.GetData(lIndex); }
	void UnloadWeekLine() noexcept { m_dataWeekLine.Unload(); }
	bool CreateWeekLine(long lStartDate);
	bool CreateWeekLine2(long lStartDate);
	void StoreWeekLine(const CWeekLine& weekLine) { m_dataWeekLine.Add(weekLine); }
	bool IsWeekLineLoaded() const noexcept override { return m_dataWeekLine.IsDataLoaded(); }
	void SetWeekLineLoaded(const bool fFlag) noexcept override { m_dataWeekLine.SetDataLoaded(fFlag); }
	// 周线相对强度计算
	bool CalculateWeekLineRS() { return m_dataWeekLine.CalculateRS0(); }
	bool CalculateWeekLineRSIndex() { return m_dataWeekLine.CalculateRSIndex0(); }
	bool CalculateWeekLineRSLogarithm() { return m_dataWeekLine.CalculateRSLogarithm0(); }

	bool IsShareA() const { return ::IsShareA(GetSymbol()); }

protected:
	short m_nHand{ 100 }; // 每手股数

	long m_lHighLimitFromTengxun{ 0 }; // 涨停价。（此数据目前只有腾讯实时数据能够提供）
	long m_lLowLimitFromTengxun{ 0 }; // 跌停价。（此数据目前只有腾讯实时数据能够提供）
	long m_lHighLimit{ 0 }; // 涨停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
	long m_lLowLimit{ 0 }; // 跌停价。（当股票出现涨跌停板时，此数据由系统计算出来，否则为零）
	array<long, 5> m_lPBuy; // 买盘价。单位：0.001元
	array<long, 5> m_lVBuy; // 买盘量。单位：股
	array<long, 5> m_lPSell; // 卖盘价。单位：0.001元
	array<long, 5> m_lVSell; // 卖盘量。单位：股
	double m_dRealtimeRS{ 0 }; // 当日实时相对强度 单位：1%
	double m_dRealtimeRSIndex{ 0 }; // 当日实时相对强度（相对于指数）单位：1%

	INT64 m_llLastSavedVolume{ 0 }; // 如果交易中途系统退出，则再次登入时上次的交易数量

	bool m_fHaveFirstRTData{ false }; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
	bool m_fNeedProcessRTData{ true }; //指数类股票无需计算交易和挂单情况
	bool m_fRTDataCalculated{ false }; // 实时数据显示需要更新

	bool m_fChosen{ false }; // 此股票是否是自选股票.
	bool m_fUpdateChosenStockDB{ false }; // 是否存储至自选股票池。
	bool m_fMinLineUpdated{ false }; // 今天的分钟资料是否更新过.

	// 挂单的具体情况。
	CWebRTDataPtr m_pLastRTData{ nullptr }; // 从m_qRTData读出的上一个实时数据。

	ConcurrentQueue<CWebRTDataPtr> m_qRTData{ 32 }; // 采用优先队列存储实时数据，这样可以保证多源。

	// 日线容器
	CContainerChinaDayLine m_dataDayLine;
	// 周线容器
	CContainerChinaWeekLine m_dataWeekLine;
	// 月线容器
	CContainerChinaStockMonthLine m_dataMonthLine;

	bool m_fDayLineDBUpdated{ false }; // 日线历史数据库更新标识
};
