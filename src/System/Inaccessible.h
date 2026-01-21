#pragma once

using std::set;

class CInaccessible {
public:
	CInaccessible();
	CInaccessible(const string& sFunction, int iFunction, const vector<string>& vSymbol);
	// 不允许赋值、拷贝
	CInaccessible(const CInaccessible&) = delete;
	CInaccessible& operator=(const CInaccessible&) = delete;
	CInaccessible(const CInaccessible&&) noexcept = delete;
	CInaccessible& operator=(const CInaccessible&&) noexcept = delete;
	virtual ~CInaccessible() = default;

	bool Assign(const string& sFunction, int iFunction, const vector<string>& vSymbol);

	int GetFunction() const noexcept { return m_iFunction; }
	void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
	string GetFunctionString() noexcept { return m_sFunction; }
	void SetFunctionString(const string& sFunction) noexcept { m_sFunction = sFunction; }

	void AddSymbol(const string& sSymbol);
	void DeleteSymbol(const string& sSymbol);
	void Clear();

	bool HaveSymbol(const string& sSymbol) const;
	bool HaveSymbol() const { return !m_vSymbol.empty(); }

	size_t Size() const noexcept { return m_vSymbol.size(); }
	string GetSymbol(const size_t iIndex) { return m_vSymbol.at(iIndex); }

protected:
	string m_sFunction; // 功能名称
	int m_iFunction; // 功能名称的值
	vector<string> m_vSymbol; // 所禁止的各交易所名称的序列
	set<string> m_setSymbol; // 所禁止的交易所名称的集合
};

using CInaccessiblePtr = shared_ptr<CInaccessible>;
