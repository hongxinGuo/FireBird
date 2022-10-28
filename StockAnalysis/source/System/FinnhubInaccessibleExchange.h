//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub tiingo����վ���ݷ�Ϊ��Ѻ��շ����֣�����˻�ֻ����ȡ�������ݡ��ʶ���Ҫ��������ȷ���Ƿ��ܹ����յ���Ч���ݡ�
// �˽ṹ�д洢�����ܲ�������յĽ��������ƣ����������ų��ýṹ�еĽ������������룩��
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<string>
#include<set>
#include<memory>
#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

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
	string m_sFunction; // ��������
	int m_iFunction; // �������Ƶ�ֵ
	vector<string> m_vExchange; // ����ֹ�ĸ����������Ƶ�����
	set<string> m_setExchange; // ����ֹ�Ľ��������Ƶļ���
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

	void SetDefaultFileName(CString fileName) { m_strFileName = fileName; }
	CString GetDefaultFileName(void) { return m_strFileName; }

	void CreateDefaultVector(void);

	void CreateFinnhubInquiryIndexToStringMap();
	void CreateFinnhubInquiryStringToIndexMap();

	string GetFinnhubInquiryString(int iInquiryIndex) { return m_mapFinnhubInquiryIndexToString.at(iInquiryIndex); }
	int GetFinnhubInquiryIndex(string sString) { return m_mapFinnhubInquiryStringToIndex.at(sString); }

public:
	CString m_strFileName; // �����ļ�����

	vector<CInaccessibleExchangesPtr> m_vpInaccessibleExchange; //
	map<int, string> m_mapFinnhubInquiryIndexToString;
	map<string, int> m_mapFinnhubInquiryStringToIndex;

	bool m_fUpdate;
	bool m_fInitialized = false;

	json m_finnhubInaccessibleExange;
};

typedef shared_ptr<CFinnhubInaccessibleExchange> CFinnhubInaccessibleExchangePtr;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;

extern std::string gl_sFinnhubInaccessibleExchange; // finnhub inaccessible exchange
