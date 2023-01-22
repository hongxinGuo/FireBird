// SetFinnhubStockBasicFinancialQuarter.h : CSetCountry ������

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubStockBasicFinancialQuarter : public CVirtualRecordset {
public:
	CSetFinnhubStockBasicFinancialQuarter(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_stock_basic_financials_quarterly"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	CString m_symbol;
	CString m_type;
	int m_date;
	double m_value;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
