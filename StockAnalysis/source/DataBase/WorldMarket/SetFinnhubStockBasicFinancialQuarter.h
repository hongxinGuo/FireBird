// SetFinnhubStockBasicFinancialQuarter.h : CSetCountry ������

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubStockBasicFinancialQuarter : public CVirtualRecordset
{
public:
	CSetFinnhubStockBasicFinancialQuarter(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_stock_basic_financials_quarterly"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetFinnhubStockBasicFinancialQuarter)

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
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};