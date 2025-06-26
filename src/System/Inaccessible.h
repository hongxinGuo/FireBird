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
	string m_sFunction; // ��������
	int m_iFunction; // �������Ƶ�ֵ
	vector<string> m_vSymbol; // ����ֹ�ĸ����������Ƶ�����
	set<string> m_setSymbol; // ����ֹ�Ľ��������Ƶļ���
};

using CInaccessibleSymbolsPtr = shared_ptr<CInaccessible>;
