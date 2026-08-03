////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 股票实例数据容器（日线容器、周线容器等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
module;
#include <afxwin.h>
#include <absl/log/absl_check.h>

export module FireBirdLib.Container.HistoryCandle;

import FireBirdLib.HistoryCandle.Virtual;
import FireBirdLib.HistoryCandle.DayLine;

import std;
using std::atomic_bool;
using std::shared_ptr;
using std::string;
using std::vector;
using std::chrono::local_days;


export {
	class CVirtualDataHistoryCandle {
	public:
		CVirtualDataHistoryCandle();
		virtual ~CVirtualDataHistoryCandle() = default;

		void Reset(); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。
		auto Size() const noexcept { return m_vHistoryData.size(); }
		bool Empty() const noexcept { return m_vHistoryData.empty(); }
		void Reserve(size_t size) {
			m_vHistoryData.reserve(size);
			m_vHistoryData.clear();
		}
		void Unload() noexcept { Reset(); }

		// 所有的派生类皆需要定义此两个存储和提取函数，不允许调用此基类函数
		virtual void SaveDB(const string&) { ABSL_CHECK(0); }
		virtual void LoadDB(const string&) { ABSL_CHECK(0); }

		virtual void SplitAdjust() { ABSL_CHECK(0); } // 拆分调整

		void UpdateData(const vector<CVirtualHistoryCandle>& vTempData);
		void UpdateData(const vector<CDayLine>& vTempData);
		void UpdateData(const shared_ptr<vector<CDayLine>>& pvTempDayLine);

		int GetRatio() const {
			if (m_ratio == 0)
				ABSL_CHECK(0);
			return m_ratio;
		}

		vector<CVirtualHistoryCandle>* GetContainer() noexcept { return &m_vHistoryData; }

		bool GetStartEndDate(local_days& lStartDate, local_days& lEndDate) const;

		CVirtualHistoryCandle* GetData(const size_t lIndex) { return &m_vHistoryData.at(lIndex); }
		vector<CVirtualHistoryCandle>& GetDataVector() { return m_vHistoryData; }
		CVirtualHistoryCandle* GetCandle(local_days date);
		CVirtualHistoryCandle* GetCandle2(local_days date);

		void Add(CVirtualHistoryCandle data) {
			data.SetRatio(m_ratio);
			m_vHistoryData.push_back(data);
		}
		void Add(const CDayLine& data);
		bool HaveDayLine(local_days date);

		bool IsDataLoaded() const noexcept { return m_fDataLoaded.load(); }
		void SetDataLoaded(const bool fFlag) noexcept { m_fDataLoaded = fFlag; }

		bool IsSplitAdjusted() const noexcept { return m_bSplitAdjusted; }
		void SetSplitAdjusted(const bool fFlag) noexcept { m_bSplitAdjusted = fFlag; }

		void CalculateMA(size_t length);

		void CreateWeekLine(CVirtualDataHistoryCandle& dataDayLine);
		void CreateMonthLine(CVirtualDataHistoryCandle& dataDayLine);

		// 显示此容器
		std::pair<long, long> GetHighLow(int iCandleNumber);
		void ToShow(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow);

		void ShowLine(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, const vector<double>& vData);

	protected:
		int m_ratio{ 0 };
		vector<CVirtualHistoryCandle> m_vHistoryData;
		atomic_bool m_fDataLoaded{ false }; // 数据装载与否标识
		bool m_bSplitAdjusted{ false }; // 是否进行了拆分调整
	};

	using CVirtualDataHistoryCandlePtr = shared_ptr<CVirtualDataHistoryCandle>;
}