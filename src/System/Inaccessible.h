#pragma once

#include<set>
using std::set;

class CInaccessible {
public:
	CInaccessible();
	CInaccessible(const string& sFunction, int iFunction, const vector<string>& vSymbol);
	virtual ~CInaccessible() = default;

	bool Assign(const string& sFunction, int iFunction, const vector<string>& vSymbol);

	int GetFunction() const noexcept { return m_iFunction; }
	void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
	string GetFunctionString() noexcept { return m_sFunction; }
	void SetFunctionString(const string& sFunction) noexcept { m_sFunction = sFunction; }

	bool AddSymbol(const string& sSymbol);
	bool DeleteSymbol(const string& sSymbol);

	bool HaveSymbol(const string& sSymbol) const;
	bool HaveSymbol() const;

	size_t SymbolSize() const noexcept { return m_vSymbol.size(); }
	string GetSymbol(const int iIndex) { return m_vSymbol.at(iIndex); }

public:

protected:
	string m_sFunction; // 功能名称
	int m_iFunction; // 功能名称的值
	vector<string> m_vSymbol; // 所禁止的各交易所名称的序列
	set<string> m_setSymbol; // 所禁止的交易所名称的集合
};

using CInaccessibleSymbolsPtr = shared_ptr<CInaccessible>;
