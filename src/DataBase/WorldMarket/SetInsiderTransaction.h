// CSetInsiderTransaction.h : CSetInsiderTransaction ������

#pragma once

#include"VirtualRecordset.h"

class CSetInsiderTransaction : public CVirtualRecordset {
public:
	CSetInsiderTransaction(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_insider_transaction"), CDatabase* pDatabase = nullptr);

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
	CString m_PersonName{ _T("") };
	CString m_Share{ _T("") };
	CString m_Change{ _T("") };
	long m_FilingDate{ 0 };
	long m_TransactionDate{ 0 };
	CString m_TransactionCode{ _T("") };
	double m_TransactionPrice{ 0 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
