module;
#include <absl/log/absl_check.h>

#include"nlohmann/json.hpp"
export module FireBirdLib.Stock;

import FireBirdLib.Accessory.NlohmannJsonDeclaration;
import FireBirdLib.StockSplit;
import FireBirdLib.HistoryCandle;
import FireBirdLib.Container.HistoryCandle;

import std;
using std::string;
using std::string_view;
using std::atomic_bool;
using std::vector;
using std::int64_t;
using std::time_t;
using std::shared_ptr;
using std::chrono::local_days;
using std::literals::chrono_literals::operator ""y;

export {
	class CVirtualStock {
	public:
		CVirtualStock();
		// 不允许赋值。
		CVirtualStock(const CVirtualStock&) = delete;
		CVirtualStock& operator=(const CVirtualStock&) = delete;
		CVirtualStock(const CVirtualStock&&) noexcept = delete;
		CVirtualStock& operator=(const CVirtualStock&&) noexcept = delete;
		virtual ~CVirtualStock() = default;

		virtual void ResetAllUpdateDate();
		virtual int GetRatio() const = 0;

		virtual void UpdateJsonUpdateDate();
		virtual void UpdateAllUpdateDate();
		void LoadUpdateDate(const string& strUpdateDate);

		string GetDescription() const noexcept { return m_strDescription; }
		void SetDescription(const string& strDescription) noexcept { m_strDescription = strDescription; }
		void SetDescription(string_view svDescription) noexcept { m_strDescription = string(svDescription); }
		string GetExchange() const noexcept { return m_strExchange; }
		void SetExchange(const string& strExchange) noexcept { m_strExchange = strExchange; }
		void SetExchange(string_view svExchange) noexcept { m_strExchange = string(svExchange); }
		string GetSymbol() const noexcept { return m_strSymbol; }
		void SetSymbol(const string& str) noexcept { m_strSymbol = str; }
		void SetSymbol(string_view svSymbol) noexcept { m_strSymbol = string(svSymbol); }
		string GetDisplaySymbol() const noexcept { return m_strDisplaySymbol; }
		void SetDisplaySymbol(const string& str) noexcept { m_strDisplaySymbol = str; }
		void SetDisplaySymbol(string_view svDisplaySymbol) noexcept { m_strDisplaySymbol = string(svDisplaySymbol); }
		nlohmannJson GetJsonUpdateDate() const noexcept { return m_jsonUpdateDate; }

		// 基本实时数据，需要更新
		std::chrono::sys_seconds GetTimePoint() const noexcept { return m_tpTime; }
		virtual void SetTimePoint(const std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
		time_t GetTransactionTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
		virtual void SetTransactionTime(const time_t time) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
		int GetLastClose() const noexcept { return m_lLastClose; }
		virtual void SetLastClose(const int lValue) noexcept { m_lLastClose = lValue; }
		int GetOpen() const noexcept { return m_lOpen; }
		virtual void SetOpen(const int lValue) noexcept { m_lOpen = lValue; }
		int GetHigh() const noexcept { return m_lHigh; }
		virtual void SetHigh(const int lValue) noexcept { m_lHigh = lValue; }
		int GetLow() const noexcept { return m_lLow; }
		virtual void SetLow(const int lValue) noexcept { m_lLow = lValue; }
		int GetNew() const noexcept { return m_lNew; }
		virtual void SetNew(const int lValue) noexcept { m_lNew = lValue; }
		int64_t GetAmount() const noexcept { return m_llAmount; }
		virtual void SetAmount(const int64_t llValue) noexcept { m_llAmount = llValue; }
		int64_t GetVolume() const noexcept { return m_llVolume; }
		virtual void SetVolume(const int64_t llValue) noexcept { m_llVolume = llValue; }

		int GetUpDown() const noexcept { return m_lUpDown; }
		virtual void SetUpDown(const int lValue) noexcept { m_lUpDown = lValue; }
		double GetUpDownRate() const noexcept { return m_dUpDownRate; }
		virtual void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
		double GetChangeHandRate() const noexcept { return m_dChangeHandRate; }
		virtual void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
		int64_t GetTotalValue() const noexcept { return m_llTotalValue; }
		virtual void SetTotalValue(const int64_t llValue) noexcept { m_llTotalValue = llValue; }
		int64_t GetCurrentValue() const noexcept { return m_llCurrentValue; }
		virtual void SetCurrentValue(const int64_t llValue) noexcept { m_llCurrentValue = llValue; }

		//
		bool IsNewStock() const noexcept { return m_fNewStock; }
		void SetNewStock(const bool fFlag) noexcept { m_fNewStock = fFlag; }

		bool IsActive() const noexcept { return m_fActive; }
		void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }

		virtual CVirtualDataHistoryCandle* DayLine() noexcept {
			ABSL_CHECK(0);
			return nullptr;
		}
		virtual CVirtualDataHistoryCandle* WeekLine() noexcept {
			ABSL_CHECK(0);
			return nullptr;
		}
		virtual CVirtualDataHistoryCandle* MonthLine() noexcept {
			ABSL_CHECK(0);
			return nullptr;
		}

		virtual void LoadDayLineDB() { ABSL_CHECK(0); }

		virtual bool IsDayLineDuplicated() noexcept {
			ABSL_CHECK(0);
			return false;
		}
		virtual void DeleteDuplicatedDayLine() noexcept { ABSL_CHECK(0); }

		auto GetDayLineStartDate() const noexcept { return m_dayLineStartDate; }
		void SetDayLineStartDate(const local_days& date) noexcept { m_dayLineStartDate = date; }
		auto GetDayLineEndDate() const noexcept { return m_dayLineEndDate; }
		void SetDayLineEndDate(const local_days& date) noexcept { m_dayLineEndDate = date; }
		double GetShareCount() const { return m_dShareCount; }
		void SetShareCount(double val) { m_dShareCount = val; }

		size_t GetStockSplitCount() const noexcept { return m_pvStockSplit->size(); }
		shared_ptr<CStockSplit> GetStockSplit(size_t index) const noexcept { return m_pvStockSplit->at(index); }
		void AddStockSplit(const shared_ptr<CStockSplit>& pStockSplit) const noexcept; // 按日期顺序添加拆股信息
		void AddStockSplits(const shared_ptr<vector<shared_ptr<CStockSplit>>>& pvStockSplit) noexcept; // 按日期顺序添加拆股信息
		void ClearStockSplit() const noexcept { m_pvStockSplit->clear(); }

		virtual void RebuildStockSplitDB() {} // 重建拆股数据库

		// 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
		bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
		void SetUpdateDayLine(const bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }
		bool IsUpdateSplit() const noexcept { return m_fUpdateSplit; }
		void SetUpdateSplit(const bool fFlag) noexcept { m_fUpdateSplit = fFlag; }

		bool IsUpdateDayLineDB() const noexcept { return m_fUpdateDayLineDB; }
		void SetUpdateDayLineDB(const bool fFlag) noexcept { m_fUpdateDayLineDB = fFlag; }
		bool IsUpdateSplitDB() const noexcept { return m_fUpdateSplitDB; }
		void SetUpdateSplitDB(const bool fFlag) noexcept { m_fUpdateSplitDB = fFlag; }
		bool IsUpdateProfileDB() const noexcept { return m_fUpdateProfileDB; }
		void SetUpdateProfileDB(const bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }

		bool IsUpdateCompanyNewsDB() const noexcept { return m_fUpdateCompanyNewsDB; }
		void SetUpdateCompanyNewsDB(const bool fFlag) noexcept { m_fUpdateCompanyNewsDB = fFlag; }

		bool IsSameStock(const shared_ptr<CVirtualStock>& pStock) const;

		virtual bool IsDayLineLoaded() const noexcept {
			ABSL_CHECK(0);
			return false;
		}
		virtual void SetDayLineLoaded(const bool) noexcept { ABSL_CHECK(0); }
		virtual bool IsWeekLineLoaded() const noexcept {
			ABSL_CHECK(0);
			return false;
		}
		virtual void SetWeekLineLoaded(const bool) noexcept { ABSL_CHECK(0); }
		virtual bool IsMonthLineLoaded() const noexcept {
			ABSL_CHECK(0);
			return false;
		}
		virtual void SetMonthLineLoaded(const bool) noexcept { ABSL_CHECK(0); }

		void SetSelected(bool flag) noexcept { m_bSelected = flag; }
		bool IsSelected() const noexcept { return m_bSelected; }

	protected:
		string m_strDescription{ " " }; // 该证券的描述
		string m_strExchange{ " " }; // 证券所属交易所。美国为US，上海为SS，深圳为SZ；外汇为forex等。
		string m_strSymbol{ " " }; // 股票代码。二十位以内，后两位为市场前缀。如600601.SS，000001.SZ, AAPL（美国股票没有后缀）
		string m_strDisplaySymbol{ " " };

		nlohmannJson m_jsonUpdateDate{ nlohmannJson({}) }; // 存储所有的更新日期（json格式）。使用这种方式存储后，当增加或减少更新日期时，无需修改相应数据表的结构。
		local_days m_dayLineStartDate{ 2990y / 01 / 01 }; // 日线历史数据的起始日期
		local_days m_dayLineEndDate{ 1980y / 01 / 01 }; // 日线历史数据的结束日期
		shared_ptr<vector<shared_ptr<CStockSplit>>> m_pvStockSplit{ nullptr };
		double m_dShareCount{ 0.0 }; // 股本数量, 单位：百万股。从Finnhub获取。

		// 实时数据区
		std::chrono::sys_seconds m_tpTime{};// 实时数据交易时间
		int m_lLastClose{ 0 }; // 以0.001元计的收盘价
		int m_lOpen{ 0 }; // 以0.001元计的开盘价
		int m_lHigh{ 0 }; // 以0.001元计的最高价
		int m_lLow{ 0 }; // 以0.001元计的最低价
		int m_lNew{ 0 }; // 以0.001元计的最新价
		int m_lUpDown{ 0 }; // 涨跌值
		double m_dUpDownRate{ 0 }; // 涨跌率
		int64_t m_llVolume{ 0 }; // 以1股计的成交量
		int64_t m_llAmount{ 0 }; // 以元计的成交金额
		double m_dChangeHandRate{ 0 }; // 换手率
		int64_t m_llTotalValue{ 0 }; // 总市值。单位：万元
		int64_t m_llCurrentValue{ 0 }; // 流通市值。单位：万元

		bool m_fNewStock{ false }; // 本日新发现的股票
		bool m_fActive{ false }; // 是否是活跃股票

		bool m_bSelected{ false }; // 在股票列表中被选中

		atomic_bool m_fUpdateDayLine{ true }; // 日线需要更新。默认为真
		atomic_bool m_fUpdateSplit{ true }; // 拆股信息需要更新。默认为真

		atomic_bool m_fUpdateProfileDB{ false }; // 更新股票简介
		atomic_bool m_fUpdateCompanyNewsDB{ false }; // 更新公司新闻
		atomic_bool m_fUpdateDayLineDB{ false }; // 日线历史数据已处理，等待存储。
		atomic_bool m_fUpdateSplitDB{ false }; // 拆股信息已处理，等待存储。
	};

	using CVirtualStockPtr = shared_ptr<CVirtualStock>;

	extern shared_ptr<CVirtualStock> gl_pCurrentStock;
}