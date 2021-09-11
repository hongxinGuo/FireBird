// CSetInsiderTransaction.h : CSetInsiderTransaction ������

#pragma once

#include"VirtualRecordset.h"

class CSetInsiderTransaction : public CVirtualRecordset
{
public:
	CSetInsiderTransaction(CString strSchema = _T("WorldMarket"), CString strTable = _T("insider_transaction"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetInsiderTransaction)

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
	CString m_PersonName;
	long m_Share;
	long m_Change;
	long m_FilingDate;
	long m_TransactionDate;
	CString m_TransactionCode;
	double m_TransactionPrice;

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
