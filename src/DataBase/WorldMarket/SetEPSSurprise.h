// CSetEPSSurprise.h : CSetEPSSurprise ������

#pragma once

#include"VirtualRecordset.h"

class CSetEPSSurprise : public CVirtualRecordset {
public:
	CSetEPSSurprise(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_stock_estimates_eps_surprise"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	CString m_Symbol;
	long m_Date;
	double m_Actual;
	double m_Estimate;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
