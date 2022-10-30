//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub tiingo等网站数据分为免费和收费两种，免费账户只能收取部分内容。故而需要在运行中确定是否能够接收到有效数据。
// 此结构中存储各功能不允许接收的交易所名称，在运行中排除该结构中的交易所（不申请）。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<string>
#include<set>
#include<memory>
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

class CInaccessibleExchanges : CObject {
public:
	CInaccessibleExchanges();
	CInaccessibleExchanges(CString sFunction, int iFunction, vector<CString>& vExchange);
	~CInaccessibleExchanges();

	bool Assign(CString sFunction, int iFunction, vector<CString>& vExchange);

	int GetFunction(void) noexcept { return m_iFunction; }
	void SetFunction(int iFunction) noexcept { m_iFunction = iFunction; }
	CString GetFunctionString(void) noexcept { return m_sFunction; }
	void SetFunctionString(CString sFunction) noexcept { m_sFunction = sFunction; }

	bool AddExchange(CString sExchangeName);
	bool DeleteExchange(CString sExchangeName);

	bool HaveExchange(CString sExchange);
	bool HaveExchange(void);

	size_t ExchangeSize(void) noexcept { return m_vExchange.size(); }
	CString GetExchange(int iIndex) { return m_vExchange.at(iIndex); }

public:

protected:
	CString m_sFunction; // 功能名称
	int m_iFunction; // 功能名称的值
	vector<CString> m_vExchange; // 所禁止的各交易所名称的序列
	set<CString> m_setExchange; // 所禁止的交易所名称的集合
};

typedef shared_ptr<CInaccessibleExchanges> CInaccessibleExchangesPtr;

class CFinnhubInaccessibleExchange : CObject {
public:
	CFinnhubInaccessibleExchange();
	~CFinnhubInaccessibleExchange();

	bool LoadDB(void);
	void SaveDB(void);
	void Update(void);
	void UpdateJson(void);

	void Clear(void) noexcept { m_mapInaccessibleExchange.clear(); m_finnhubInaccessibleExange.clear(); }

	void SetDefaultFileName(CString fileName) { m_strFileName = fileName; }
	CString GetDefaultFileName(void) { return m_strFileName; }

	void CreateFinnhubInquiryIndexToStringMap();
	void CreateFinnhubInquiryStringToIndexMap();

	CString GetFinnhubInquiryString(int iInquiryIndex) { return m_mapFinnhubInquiryIndexToString.at(iInquiryIndex); }
	int GetFinnhubInquiryIndex(CString sString) { return m_mapFinnhubInquiryStringToIndex.at(sString); }
	CInaccessibleExchangesPtr GetInaccessibleExchange(int iInquiryType);
	void SetInaccessibleExchange(int iInquiryType, CInaccessibleExchangesPtr pExchange) { m_mapInaccessibleExchange[iInquiryType] = pExchange; }
	size_t GetInaccessibleExchangeSize(void) noexcept { return m_mapInaccessibleExchange.size(); }

	bool IsNeedUpdate(void) noexcept { return m_fUpdate; }
	void SetUpdate(bool fUpdate) noexcept { m_fUpdate = fUpdate; }

	bool IsInaccessible(int iInquiryType, CString strExchangeCode);

public:

protected:
	CString m_strFileName; // 配置文件名称

	map<int, CInaccessibleExchangesPtr> m_mapInaccessibleExchange; //
	map<int, CString> m_mapFinnhubInquiryIndexToString;
	map<CString, int> m_mapFinnhubInquiryStringToIndex;

	bool m_fInitialized = false;
	bool m_fUpdate;

	json m_finnhubInaccessibleExange;
};

typedef shared_ptr<CFinnhubInaccessibleExchange> CFinnhubInaccessibleExchangePtr;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;

extern std::string gl_sFinnhubInaccessibleExchange; // finnhub inaccessible exchange
