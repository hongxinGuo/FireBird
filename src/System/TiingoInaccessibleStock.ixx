//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// tiingo网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的证券名称，在运行中排除该结构中的证券（不申请）。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include"NlohmannJsonHeader.h"
export module FireBird.Tiingo.InaccessibleStock;

import FireBird.System.Inaccessible;
import FireBird.FinnhubInquiryType;

export  {
	using CInaccessibleStocksPtr = shared_ptr<CInaccessible>;

	class CTiingoInaccessibleStock {
	public:
		CTiingoInaccessibleStock();
		// 只能有一个实例,不允许赋值、拷贝
		CTiingoInaccessibleStock(const CTiingoInaccessibleStock&) = delete;
		CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&) = delete;
		CTiingoInaccessibleStock(const CTiingoInaccessibleStock&&) noexcept = delete;
		CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&&) noexcept = delete;
		virtual ~CTiingoInaccessibleStock();
		void UpdateDB();

		bool LoadDB();
		bool LoadDB(const CString& strFileDirectory);
		void SaveDB() const;
		void Update();
		void UpdateJson();

		void Clear() noexcept {
			m_mapStock.clear();
			m_finnhubInaccessibleStock.clear();
		}

		void SetDefaultFileName(const CString& fileName) noexcept { m_strFileName = fileName; }
		CString GetDefaultFileName() { return m_strFileName; }

		void SetUpdateDate(const long lDate) noexcept { m_lUpdateDate = lDate; }
		long GetUpdateDate() const { return m_lUpdateDate; }

		static int GetTiingoInquiryIndex(const CString& sString) { return gl_FinnhubInquiryType.GetInquiryType(sString); }
		CInaccessibleStocksPtr GetStock(int iInquireType) { return m_mapStock.at(iInquireType); }
		void SetStock(const int iInquireType, const CInaccessibleStocksPtr& pStock) { m_mapStock[iInquireType] = pStock; }
		void DeleteStock(int iInquireType, const CString& strStock);
		bool HaveStock(int iInquireType, const CString& strStockCode) const;
		size_t GetItemSize() const noexcept { return m_mapStock.size(); }

		bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
		void SetUpdateDB(const bool fUpdate) noexcept { m_fUpdateDB = fUpdate; }

	protected:
		CString m_strFileName{ _T("TiingoInaccessibleStock.json") }; // 配置文件名称

		long m_lUpdateDate{ 19800101 }; // 本文件更新日期
		map<int, CInaccessibleStocksPtr> m_mapStock; //

		bool m_fInitialized{ false };
		bool m_fUpdateDB{ false };

		nlohmann::ordered_json m_finnhubInaccessibleStock;
	};

	using CTiingoInaccessibleStockPtr = shared_ptr<CTiingoInaccessibleStock>;

	extern CTiingoInaccessibleStock gl_tiingoInaccessibleStock;

	extern std::string Test_gl_sTiingoInaccessibleStock; // finnhub inaccessible exchange test data
}
