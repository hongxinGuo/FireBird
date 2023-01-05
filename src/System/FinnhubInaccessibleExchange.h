//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub tiingo等网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的交易所名称，在运行中排除该结构中的交易所（不申请）。
//
// // 美国市场（交易所代码为US）永远申请，其他交易所根据反馈情况决定是否继续申请。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

#include<set>
using std::set;

class CInaccessibleExchanges {
public:
	CInaccessibleExchanges();
	CInaccessibleExchanges(const CString& sFunction, const int iFunction, const vector<CString>& vExchange);
	virtual ~CInaccessibleExchanges() = default;

	bool Assign(const CString& sFunction, const int iFunction, const vector<CString>& vExchange);

	int GetFunction(void) const noexcept { return m_iFunction; }
	void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
	CString GetFunctionString(void) noexcept { return m_sFunction; }
	void SetFunctionString(const CString& sFunction) noexcept { m_sFunction = sFunction; }

	bool AddExchange(const CString& sExchangeName);
	bool DeleteExchange(const CString& sExchangeName);

	bool HaveExchange(const CString& sExchange) const;
	bool HaveExchange(void) const;

	size_t ExchangeSize(void) const noexcept { return m_vExchange.size(); }
	CString GetExchange(const int iIndex) { return m_vExchange.at(iIndex); }

public:

protected:
	CString m_sFunction; // 功能名称
	int m_iFunction; // 功能名称的值
	vector<CString> m_vExchange; // 所禁止的各交易所名称的序列
	set<CString> m_setExchange; // 所禁止的交易所名称的集合
};

using CInaccessibleExchangesPtr = shared_ptr<CInaccessibleExchanges>;

class CFinnhubInaccessibleExchange {
public:
	CFinnhubInaccessibleExchange();
	virtual ~CFinnhubInaccessibleExchange();

	bool LoadDB(void);
	bool LoadDB(CString strFileDirectory);
	void SaveDB(void) const;
	void Update(void);
	void UpdateJson(void);

	void Clear(void) noexcept {
		m_mapInaccessibleExchange.clear();
		m_finnhubInaccessibleExchange.clear();
	}

	void SetDefaultFileName(const CString& fileName) noexcept { m_strFileName = fileName; }
	CString GetDefaultFileName(void) { return m_strFileName; }

	void SetUpdateDate(const long lDate) noexcept {	m_lUpdateDate = lDate; }
	long GetUpdateDate() { return m_lUpdateDate; }

	void CreateFinnhubInquiryIndexToStringMap();
	void CreateFinnhubInquiryStringToIndexMap();

	CString GetFinnhubInquiryString(int iInquiryIndex) { return m_mapFinnhubInquiryIndexToString.at(iInquiryIndex); }
	int GetFinnhubInquiryIndex(const CString& sString) const { return m_mapFinnhubInquiryStringToIndex.at(sString); }
	CInaccessibleExchangesPtr GetInaccessibleExchange(int iInquiryType);
	void SetInaccessibleExchange(const int iInquiryType, const CInaccessibleExchangesPtr pExchange) { m_mapInaccessibleExchange[iInquiryType] = pExchange; }
	size_t GetInaccessibleExchangeSize(void) const noexcept { return m_mapInaccessibleExchange.size(); }

	bool IsNeedUpdate(void) const noexcept { return m_fUpdate; }
	void SetUpdate(const bool fUpdate) noexcept { m_fUpdate = fUpdate; }

	bool IsInaccessible(const int iInquiryType, const CString& strExchangeCode);

public:

protected:
	CString m_strFileName; // 配置文件名称

	long m_lUpdateDate; // 本文件更新日期
	map<int, CInaccessibleExchangesPtr> m_mapInaccessibleExchange; //
	map<int, CString> m_mapFinnhubInquiryIndexToString;
	map<CString, int> m_mapFinnhubInquiryStringToIndex;

	bool m_fInitialized = false;
	bool m_fUpdate;


	json m_finnhubInaccessibleExchange;
};

using CFinnhubInaccessibleExchangePtr = shared_ptr<CFinnhubInaccessibleExchange>;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;

extern std::string gl_sFinnhubInaccessibleExchange; // finnhub inaccessible exchange
