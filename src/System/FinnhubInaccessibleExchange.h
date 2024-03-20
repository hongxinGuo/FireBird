//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub tiingo����վ���ݷ�Ϊ��Ѻ��շ����֣�����˻�ֻ����ȡ�������ݡ��ʶ���Ҫ��������ȷ���Ƿ��ܹ����յ���Ч���ݡ�
// �˽ṹ�д洢�����ܲ�������յĽ��������ƣ����������ų��ýṹ�еĽ������������룩��
//
// // �����г�������������ΪUS����Զ���룬�������������ݷ�����������Ƿ�������롣
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�
#include"FinnhubInquiryType.h"

#include<set>
using std::set;

class CInaccessibleExchanges {
public:
	CInaccessibleExchanges();
	CInaccessibleExchanges(const CString& sFunction, int iFunction, const vector<CString>& vExchange);
	virtual ~CInaccessibleExchanges() = default;

	bool Assign(const CString& sFunction, int iFunction, const vector<CString>& vExchange);

	int GetFunction() const noexcept { return m_iFunction; }
	void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
	CString GetFunctionString() noexcept { return m_sFunction; }
	void SetFunctionString(const CString& sFunction) noexcept { m_sFunction = sFunction; }

	bool AddExchange(const CString& sExchangeName);
	bool DeleteExchange(const CString& sExchangeName);

	bool HaveExchange(const CString& sExchange) const;
	bool HaveExchange() const;

	size_t ExchangeSize() const noexcept { return m_vExchange.size(); }
	CString GetExchange(const int iIndex) { return m_vExchange.at(iIndex); }

public:

protected:
	CString m_sFunction; // ��������
	int m_iFunction; // �������Ƶ�ֵ
	vector<CString> m_vExchange; // ����ֹ�ĸ����������Ƶ�����
	set<CString> m_setExchange; // ����ֹ�Ľ��������Ƶļ���
};

using CInaccessibleExchangesPtr = shared_ptr<CInaccessibleExchanges>;

class CFinnhubInaccessibleExchange {
public:
	CFinnhubInaccessibleExchange();
	// ֻ����һ��ʵ��,������ֵ������
	CFinnhubInaccessibleExchange(const CFinnhubInaccessibleExchange&) = delete;
	CFinnhubInaccessibleExchange& operator=(const CFinnhubInaccessibleExchange&) = delete;
	CFinnhubInaccessibleExchange(const CFinnhubInaccessibleExchange&&) noexcept = delete;
	CFinnhubInaccessibleExchange& operator=(const CFinnhubInaccessibleExchange&&) noexcept = delete;
	virtual ~CFinnhubInaccessibleExchange();
	void UpdateDB();

	bool LoadDB();
	bool LoadDB(const CString& strFileDirectory);
	void SaveDB() const;
	void Update();
	void UpdateJson();

	void Clear() noexcept {
		m_mapExchange.clear();
		m_finnhubInaccessibleExchange.clear();
	}

	void SetDefaultFileName(const CString& fileName) noexcept { m_strFileName = fileName; }
	CString GetDefaultFileName() { return m_strFileName; }

	void SetUpdateDate(const long lDate) noexcept { m_lUpdateDate = lDate; }
	long GetUpdateDate() const { return m_lUpdateDate; }

	static int GetFinnhubInquiryIndex(const CString& sString) { return gl_FinnhubInquiryType.GetInquiryType(sString); }
	CInaccessibleExchangesPtr GetExchange(int iInquireType) { return m_mapExchange.at(iInquireType); }
	void SetExchange(const int iInquireType, const CInaccessibleExchangesPtr& pExchange) { m_mapExchange[iInquireType] = pExchange; }
	void DeleteExchange(int iInquireType, const CString& strExchange);
	bool HaveExchange(int iInquireType, const CString& strExchangeCode) const;
	size_t GetItemSize() const noexcept { return m_mapExchange.size(); }

	bool IsNeedUpdate() const noexcept { return m_fUpdate; }
	void SetUpdate(const bool fUpdate) noexcept { m_fUpdate = fUpdate; }

protected:
	CString m_strFileName; // �����ļ�����

	long m_lUpdateDate; // ���ļ���������
	map<int, CInaccessibleExchangesPtr> m_mapExchange; //

	bool m_fInitialized = false;
	bool m_fUpdate;

	json m_finnhubInaccessibleExchange;
};

using CFinnhubInaccessibleExchangePtr = shared_ptr<CFinnhubInaccessibleExchange>;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;

extern std::string Test_gl_sFinnhubInaccessibleExchange; // finnhub inaccessible exchange test data
