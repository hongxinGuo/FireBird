#include"pch.h"
module;
#include<set>
export module FireBird.System.Inaccessible;

using std::set;

export {
	class CInaccessible {
	public:
		CInaccessible();
		CInaccessible(const CString& sFunction, int iFunction, const vector<CString>& vSymbol);
		virtual ~CInaccessible() = default;

		bool Assign(const CString& sFunction, int iFunction, const vector<CString>& vSymbol);

		int GetFunction() const noexcept { return m_iFunction; }
		void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
		CString GetFunctionString() noexcept { return m_sFunction; }
		void SetFunctionString(const CString& sFunction) noexcept { m_sFunction = sFunction; }

		bool AddSymbol(const CString& sSymbol);
		bool DeleteSymbol(const CString& sSymbol);

		bool HaveSymbol(const CString& sSymbol) const;
		bool HaveSymbol() const;

		size_t SymbolSize() const noexcept { return m_vSymbol.size(); }
		CString GetSymbol(const int iIndex) { return m_vSymbol.at(iIndex); }

	public:

	protected:
		CString m_sFunction; // ��������
		int m_iFunction; // �������Ƶ�ֵ
		vector<CString> m_vSymbol; // ����ֹ�ĸ����������Ƶ�����
		set<CString> m_setSymbol; // ����ֹ�Ľ��������Ƶļ���
	};

	using CInaccessibleSymbolsPtr = shared_ptr<CInaccessible>;
}