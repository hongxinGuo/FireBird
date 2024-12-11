#include"pch.h"
module;
#include"NlohmannJsonHeader.h"
export module FireBird.Stock.Virtual;
using std::shared_ptr;
using std::atomic_bool;

export import FireBird.System.ConstantChinaMarket;
export import FireBird.System.Configuration;
export import FireBird.Set.VirtualStockSymbol;

export {
	class CVirtualStock {
	public:
		CVirtualStock();
		// ������ֵ��
		CVirtualStock(const CVirtualStock&) = delete;
		CVirtualStock& operator=(const CVirtualStock&) = delete;
		CVirtualStock(const CVirtualStock&&) noexcept = delete;
		CVirtualStock& operator=(const CVirtualStock&&) noexcept = delete;
		virtual ~CVirtualStock() = default;

		virtual void ResetAllUpdateDate();
		virtual int GetRatio() const = 0;

		void LoadUpdateDate(CString& strUpdateDate);
		virtual void LoadSymbol(CVirtualSetStockSymbol& setStockSymbol);
		virtual void AppendSymbol(CVirtualSetStockSymbol& setStockSymbol);
		virtual void UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol);
		virtual void SaveSymbol(CVirtualSetStockSymbol& setStockSymbol);

		CString GetDescription() const noexcept { return m_strDescription; }
		void SetDescription(const CString& strDescription) noexcept { m_strDescription = strDescription; }
		CString GetExchangeCode() const noexcept { return m_strExchangeCode; }
		void SetExchangeCode(const CString& strExchangeCode) noexcept { m_strExchangeCode = strExchangeCode; }
		CString GetSymbol() const noexcept { return m_strSymbol; }
		void SetSymbol(const CString& str) noexcept {
			m_strSymbol = str;
			const CString s = m_strSymbol.Left(6);
		}
		CString GetDisplaySymbol() const noexcept { return m_strDisplaySymbol; }
		void SetDisplaySymbol(const CString& str) noexcept { m_strDisplaySymbol = str; }

		// ����ʵʱ���ݣ���Ҫ����
		std::chrono::sys_seconds GetTimePoint() const noexcept { return m_tpTime; }
		virtual void SetTimePoint(const std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
		time_t GetTransactionTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
		virtual void SetTransactionTime(const time_t time) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
		long GetLastClose() const noexcept { return m_lLastClose; }
		virtual void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
		long GetOpen() const noexcept { return m_lOpen; }
		virtual void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
		long GetHigh() const noexcept { return m_lHigh; }
		virtual void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
		long GetLow() const noexcept { return m_lLow; }
		virtual void SetLow(const long lValue) noexcept { m_lLow = lValue; }
		long GetNew() const noexcept { return m_lNew; }
		virtual void SetNew(const long lValue) noexcept { m_lNew = lValue; }
		INT64 GetAmount() const noexcept { return m_llAmount; }
		virtual void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
		INT64 GetVolume() const noexcept { return m_llVolume; }
		virtual void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }

		long GetUpDown() const noexcept { return m_lUpDown; }
		virtual void SetUpDown(const long lValue) noexcept { m_lUpDown = lValue; }
		double GetUpDownRate() const noexcept { return m_dUpDownRate; }
		virtual void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
		double GetChangeHandRate() const noexcept { return m_dChangeHandRate; }
		virtual void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
		INT64 GetTotalValue() const noexcept { return m_llTotalValue; }
		virtual void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
		INT64 GetCurrentValue() const noexcept { return m_llCurrentValue; }
		virtual void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }

		//
		bool IsTodayNewStock() const noexcept { return m_fTodayNewStock; }
		void SetTodayNewStock(const bool fFlag) noexcept { m_fTodayNewStock = fFlag; }

		bool IsActive() const noexcept { return m_fActive; }
		void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }

		long GetDayLineStartDate();
		void SetDayLineStartDate(const long lDate) noexcept { m_jsonUpdateDate[_T("DayLineStartDate")] = lDate; }
		long GetDayLineEndDate();
		void SetDayLineEndDate(const long lDate) noexcept { m_jsonUpdateDate[_T("DayLineEndDate")] = lDate; }

		long GetIPOStatus() const noexcept { return m_lIPOStatus; }
		void SetIPOStatus(const long lValue) noexcept { m_lIPOStatus = lValue; }
		bool IsDelisted() const noexcept { return (m_lIPOStatus == _STOCK_DELISTED_); }
		bool IsNotYetList() const noexcept { return (m_lIPOStatus == _STOCK_NOT_YET_LIST_); }
		bool IsNullStock() const noexcept { return (m_lIPOStatus == _STOCK_NULL_); }
		bool IsIPOed() const noexcept { return (m_lIPOStatus == _STOCK_IPOED_); }
		bool IsNotChecked() const noexcept { return (m_lIPOStatus == _STOCK_NOT_CHECKED_); }

		// ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
		bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
		void SetUpdateDayLine(const bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

		bool IsUpdateDayLineDB() const noexcept { return m_fUpdateDayLineDB; }
		void SetUpdateDayLineDB(const bool fFlag) noexcept { m_fUpdateDayLineDB = fFlag; }
		bool IsUpdateDayLineDBAndClearFlag() noexcept { return m_fUpdateDayLineDB.exchange(false); }
		bool IsUpdateProfileDB() const noexcept { return m_fUpdateProfileDB; }
		void SetUpdateProfileDB(const bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }
		bool IsUpdateProfileDBAndClearFlag() noexcept { return m_fUpdateProfileDB.exchange(false); }

		bool IsUpdateCompanyNewsDB() const noexcept { return m_fUpdateCompanyNewsDB; }
		void SetUpdateCompanyNewsDB(const bool fFlag) noexcept { m_fUpdateCompanyNewsDB = fFlag; }
		bool IsUpdateCompanyNewsDBAndClearFlag() noexcept { return m_fUpdateCompanyNewsDB.exchange(false); }

	protected:
		CString m_strDescription{ _T("") }; // ��֤ȯ������
		CString m_strExchangeCode{ _T("") }; // ֤ȯ����������������ΪUS���Ϻ�ΪSS������ΪSZ�����Ϊforex�ȡ�
		CString m_strSymbol{ _T("") }; // ��Ʊ���롣��ʮλ���ڣ�����λΪ�г�ǰ׺����600601.SS��000001.SZ, AAPL��������Ʊû�к�׺��
		CString m_strDisplaySymbol{ _T("") };

		nlohmann::ordered_json m_jsonUpdateDate{ nlohmann::ordered_json({}) }; // �洢���еĸ������ڣ�json��ʽ����ʹ�����ַ�ʽ�洢�󣬵����ӻ���ٸ�������ʱ�������޸���Ӧ���ݱ�Ľṹ��

		// ʵʱ������
		std::chrono::sys_seconds m_tpTime{};// ʵʱ���ݽ���ʱ��
		long m_lLastClose{ 0 }; // ��0.001Ԫ�Ƶ����̼�
		long m_lOpen{ 0 }; // ��0.001Ԫ�ƵĿ��̼�
		long m_lHigh{ 0 }; // ��0.001Ԫ�Ƶ���߼�
		long m_lLow{ 0 }; // ��0.001Ԫ�Ƶ���ͼ�
		long m_lNew{ 0 }; // ��0.001Ԫ�Ƶ����¼�
		long m_lUpDown{ 0 }; // �ǵ�ֵ
		double m_dUpDownRate{ 0 }; // �ǵ���
		INT64 m_llVolume{ 0 }; // ��1�ɼƵĳɽ���
		INT64 m_llAmount{ 0 }; // ��Ԫ�Ƶĳɽ����
		double m_dChangeHandRate{ 0 }; // ������
		INT64 m_llTotalValue{ 0 }; // ����ֵ����λ����Ԫ
		INT64 m_llCurrentValue{ 0 }; // ��ͨ��ֵ����λ����Ԫ

		bool m_fTodayNewStock{ false }; // �����·��ֵĹ�Ʊ
		bool m_fActive{ false }; // �Ƿ��ǻ�Ծ��Ʊ
		long m_lIPOStatus{ _STOCK_NOT_CHECKED_ }; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
		// δ���У���Ч��Ʊ���룩Ϊ_STOCK_NULL_������Ϊ_STOCK_IPOED_����ͨ��IPO����δ���л�����Ϊ_STOCK_DELISTED�����������δ���֣������Ժ���

		atomic_bool m_fUpdateDayLine{ true }; // ������Ҫ���¡�Ĭ��Ϊ��

		atomic_bool m_fUpdateProfileDB{ false }; // ���¹�Ʊ���
		atomic_bool m_fUpdateCompanyNewsDB{ false }; // ���¹�˾����
		atomic_bool m_fUpdateDayLineDB{ false }; // ������ʷ�����Ѵ����ȴ��洢��
	};
	using CVirtualStockPtr = shared_ptr<CVirtualStock>;
}