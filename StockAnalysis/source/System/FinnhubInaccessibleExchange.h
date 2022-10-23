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
	CInaccessibleExchanges(string sFunction, int iFunction, vector<string>& vExchange);
	~CInaccessibleExchanges();

	bool Assign(string sFunction, int iFunction, vector<string>& vExchange);

	int GetFunction(void) noexcept { return m_iFunction; }
	void SetFucntion(int iFunction) noexcept { m_iFunction = iFunction; }

	bool AddExchange(string sExchangeName);
	bool DeleteExchange(string sExchangeName);

	bool HaveExchange(string sExchange);

public:
	string m_sFunction; // 功能名称
	int m_iFunction; // 功能名称的值
	vector<string> m_vExchange; // 所禁止的各交易所名称的序列
	set<string> m_setExchange; // 所禁止的交易所名称的集合
};

typedef shared_ptr<CInaccessibleExchanges> CInaccessibleExchangesPtr;

class CFinnhubInaccessibleExchange : CObject {
public:
	CFinnhubInaccessibleExchange();
	~CFinnhubInaccessibleExchange();

	bool LoadDB(void);
	void SaveDB(void);

	void CreateDefaultVector(void);

protected:
	vector<CInaccessibleExchangesPtr> vpInaccessibleExchange; //

	json m_finnhubInaccessibleExange;
};

typedef shared_ptr<CFinnhubInaccessibleExchange> CFinnhubInaccessibleExchangePtr;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;
