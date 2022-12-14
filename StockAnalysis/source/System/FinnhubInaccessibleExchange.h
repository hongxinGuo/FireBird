//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub tiingo����վ���ݷ�Ϊ��Ѻ��շ����֣�����˻�ֻ����ȡ�������ݡ��ʶ���Ҫ��������ȷ���Ƿ��ܹ����յ���Ч���ݡ�
// �˽ṹ�д洢�����ܲ�������յĽ��������ƣ����������ų��ýṹ�еĽ������������룩��
//
// // �����г�������������ΪUS����Զ���룬�������������ݷ�����������Ƿ�������롣
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<string>
#include<set>
#include<memory>
#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

class CInaccessibleExchanges : CObject {
public:
	CInaccessibleExchanges();
	CInaccessibleExchanges(CString sFunction, int iFunction, vector<CString>& vExchange);
	~CInaccessibleExchanges() override = default;

	bool Assign(CString sFunction, int iFunction, vector<CString>& vExchange);

	int GetFunction(void) const noexcept { return m_iFunction; }
	void SetFunction(const int iFunction) noexcept { m_iFunction = iFunction; }
	CString GetFunctionString(void) noexcept { return m_sFunction; }
	void SetFunctionString(const CString sFunction) noexcept { m_sFunction = sFunction; }

	bool AddExchange(CString sExchangeName);
	bool DeleteExchange(CString sExchangeName);

	bool HaveExchange(CString sExchange);
	bool HaveExchange(void);

	size_t ExchangeSize(void) const noexcept { return m_vExchange.size(); }
	CString GetExchange(const int iIndex) { return m_vExchange.at(iIndex); }

public:

protected:
	CString m_sFunction; // ��������
	int m_iFunction; // �������Ƶ�ֵ
	vector<CString> m_vExchange; // ����ֹ�ĸ����������Ƶ�����
	set<CString> m_setExchange; // ����ֹ�Ľ��������Ƶļ���
};

typedef shared_ptr<CInaccessibleExchanges> CInaccessibleExchangesPtr;

class CFinnhubInaccessibleExchange : CObject {
public:
	CFinnhubInaccessibleExchange();
	~CFinnhubInaccessibleExchange() override;

	bool LoadDB(void);
	void SaveDB(void);
	void Update(void);
	void UpdateJson(void);

	void Clear(void) noexcept {
		m_mapInaccessibleExchange.clear();
		m_finnhubInaccessibleExchange.clear();
	}

	void SetDefaultFileName(const CString fileName) { m_strFileName = fileName; }
	CString GetDefaultFileName(void) const { return m_strFileName; }

	void CreateFinnhubInquiryIndexToStringMap();
	void CreateFinnhubInquiryStringToIndexMap();

	CString GetFinnhubInquiryString(int iInquiryIndex) { return m_mapFinnhubInquiryIndexToString.at(iInquiryIndex); }
	int GetFinnhubInquiryIndex(const CString sString) const { return m_mapFinnhubInquiryStringToIndex.at(sString); }
	CInaccessibleExchangesPtr GetInaccessibleExchange(int iInquiryType);
	void SetInaccessibleExchange(const int iInquiryType, CInaccessibleExchangesPtr pExchange) { m_mapInaccessibleExchange[iInquiryType] = pExchange; }
	size_t GetInaccessibleExchangeSize(void) const noexcept { return m_mapInaccessibleExchange.size(); }

	bool IsNeedUpdate(void) const noexcept { return m_fUpdate; }
	void SetUpdate(const bool fUpdate) noexcept { m_fUpdate = fUpdate; }

	bool IsInaccessible(int iInquiryType, CString strExchangeCode);

public:

protected:
	CString m_strFileName; // �����ļ�����

	map<int, CInaccessibleExchangesPtr> m_mapInaccessibleExchange; //
	map<int, CString> m_mapFinnhubInquiryIndexToString;
	map<CString, int> m_mapFinnhubInquiryStringToIndex;

	bool m_fInitialized = false;
	bool m_fUpdate;

	json m_finnhubInaccessibleExchange;
};

typedef shared_ptr<CFinnhubInaccessibleExchange> CFinnhubInaccessibleExchangePtr;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;

extern std::string gl_sFinnhubInaccessibleExchange; // finnhub inaccessible exchange
