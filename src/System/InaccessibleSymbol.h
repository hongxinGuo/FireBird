//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的交易所名称，在运行中排除该结构中的交易所（不申请）。
//
// // 美国市场（交易所代码为US）永远申请，其他交易所根据反馈情况决定是否继续申请。
//
// tiingo网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的证券名称，在运行中排除该结构中的证券（不申请）。
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。
#include"FinnhubInquiryType.h"

#include "Inaccessible.h"

class CInaccessibleSymbol {
public:
	CInaccessibleSymbol(const string& strFileName);
	// 不允许赋值、拷贝
	CInaccessibleSymbol(const CInaccessibleSymbol&) = delete;
	CInaccessibleSymbol& operator=(const CInaccessibleSymbol&) = delete;
	CInaccessibleSymbol(const CInaccessibleSymbol&&) noexcept = delete;
	CInaccessibleSymbol& operator=(const CInaccessibleSymbol&&) noexcept = delete;
	virtual ~CInaccessibleSymbol();
	void UpdateDB();

	bool LoadDB();
	bool LoadDB(const string& strFileDirectory);
	void SaveDB() const;
	void Update();
	void UpdateJson();

	void Clear();

	void SetFileName(const string& fileName) noexcept { m_strFileName = fileName; }
	string GetFileName() { return m_strFileName; }

	void SetUpdateDate(const long lDate) noexcept { m_lUpdateDate = lDate; }
	long GetUpdateDate() const { return m_lUpdateDate; }

	static int GetInquiryIndex(const string& sString) { return gl_FinnhubInquiryType.GetInquiryType(sString); }
	CInaccessiblePtr GetInaccessible(int iInquireType) { return m_mapInaccessible.at(iInquireType); }
	void SetInaccessible(const int iInquireType, const CInaccessiblePtr& pInaccessible) { m_mapInaccessible[iInquireType] = pInaccessible; }
	void AddInaccessible(int iInquireType, const string& strSymbol);
	void DeleteInaccessible(int iInquireType, const string& strSymbol);
	bool IsInaccessible(int iInquireType, const string& strSymbol) const;
	size_t GetItemSize() const noexcept { return m_mapInaccessible.size(); }

	bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
	void SetUpdateDB(const bool fUpdate) noexcept { m_fUpdateDB = fUpdate; }

protected:
	string m_strFileName;// 配置文件名称

	long m_lUpdateDate{ 19800101 }; // 本文件更新日期
	map<int, CInaccessiblePtr> m_mapInaccessible; //

	bool m_fInitialized{ false };
	bool m_fUpdateDB{ false };

private:
	nlohmannJson m_jsonInaccessible;
};

using CInaccessibleSymbolPtr = shared_ptr<CInaccessibleSymbol>;

extern CInaccessibleSymbol gl_finnhubInaccessibleExchange;
extern CInaccessibleSymbol gl_tiingoInaccessibleStock;
