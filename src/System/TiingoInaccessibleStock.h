//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// tiingo网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的证券名称，在运行中排除该结构中的证券（不申请）。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

#include "Inaccessible.h"

class CTiingoInaccessibleStock {
public:
	CTiingoInaccessibleStock(const string& strFileName);
	// 只能有一个实例,不允许赋值、拷贝
	CTiingoInaccessibleStock(const CTiingoInaccessibleStock&) = delete;
	CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&) = delete;
	CTiingoInaccessibleStock(const CTiingoInaccessibleStock&&) noexcept = delete;
	CTiingoInaccessibleStock& operator=(const CTiingoInaccessibleStock&&) noexcept = delete;
	virtual ~CTiingoInaccessibleStock();
	void UpdateDB();

	bool LoadDB();
	bool LoadDB(const string& strFileDirectory);
	void SaveDB() const;
	void Update();
	void UpdateJson();

	void Clear() noexcept {
		for (const auto& val : m_mapInaccessible | views::values) {
			val->Clear();
		}
	}

	void SetDefaultFileName(const string& fileName) noexcept { m_strFileName = fileName; }
	string GetDefaultFileName() { return m_strFileName; }

	void SetUpdateDate(const long lDate) noexcept { m_lUpdateDate = lDate; }
	long GetUpdateDate() const { return m_lUpdateDate; }

	static int GetInquiryIndex(const string& sString) { return gl_FinnhubInquiryType.GetInquiryType(sString); }
	CInaccessiblePtr GetInaccessible(int iInquireType) { return m_mapInaccessible.at(iInquireType); }
	void SetInaccessible(const int iInquireType, const CInaccessiblePtr& pInaccessible) { m_mapInaccessible[iInquireType] = pInaccessible; }
	void DeleteInaccessible(int iInquireType, const string& strSymbol);
	bool IsInaccessible(int iInquireType, const string& strSymbol) const;
	size_t GetItemSize() const noexcept { return m_mapInaccessible.size(); }

	bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
	void SetUpdateDB(const bool fUpdate) noexcept { m_fUpdateDB = fUpdate; }

protected:
	string m_strFileName{}; // 配置文件名称

	long m_lUpdateDate{ 19800101 }; // 本文件更新日期
	map<int, CInaccessiblePtr> m_mapInaccessible; //

	bool m_fInitialized{ false };
	bool m_fUpdateDB{ false };

private :
	nlohmannJson m_jsonInaccessible;
};

using CTiingoInaccessibleStockPtr = shared_ptr<CTiingoInaccessibleStock>;

extern CTiingoInaccessibleStock gl_tiingoInaccessibleStock;

extern std::string Test_gl_sTiingoInaccessibleStock; // tiingo inaccessible stock test data
