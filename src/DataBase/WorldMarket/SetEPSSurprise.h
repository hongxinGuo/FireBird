// CSetEPSSurprise.h : CSetEPSSurprise ������

#pragma once

#include"VirtualRecordset.h"

class CSetEPSSurprise : public CVirtualRecordset {
public:
	CSetEPSSurprise(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_estimates_eps_surprise"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	CString m_Symbol{ _T("") };
	long m_Date{ 0 };
	double m_Actual{ 0 };
	double m_Estimate{ 0 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
