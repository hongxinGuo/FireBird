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

	void CreateDefaultVector(void);

protected:
	vector<CInaccessibleExchangesPtr> vpInaccessibleExchange; //

	json m_finnhubInaccessibleExange;
};

typedef shared_ptr<CFinnhubInaccessibleExchange> CFinnhubInaccessibleExchangePtr;

extern CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange;
