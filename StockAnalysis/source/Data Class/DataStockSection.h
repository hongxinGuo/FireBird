#pragma once

#include"StockSection.h"
#include"ChinaStock.h"
#include"SetStockSection.h"

using namespace std;
#include<vector>
#include<map>

class CDataStockSection : public CObject {
public:
	CDataStockSection();
	~CDataStockSection();
	void Reset(void);

	void LoadDB(void);
	void UpdateDB(void);

protected:
	vector<CString> m_vCurrentSectionStockCode; // ��ǰ��Ʊ���ĵ�һ�����롣�ַ����ĸ�ʽΪ600000.SS��sz000001
	vector<CStockSectionPtr> m_vStockSection; // ��2000�����Ϻ����ڸ�1000��֤ȯ��������λ�Ƿ��Ѿ���ʹ�á�
	bool m_fUpdateStockSection; // ����StockSection��ʶ
};
