// SetFinnhubCryptoSymbol.h : CSetFinnhubCryptoSymbol ������

#pragma once
#include"VirtualRecordset.h"

class CSetFinnhubCryptoSymbol : public CVirtualRecordset
{
public:
	CSetFinnhubCryptoSymbol(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_crypto_symbol"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetFinnhubCryptoSymbol)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_Description;
	CString m_DisplaySymbol;
	CString m_Exchange;
	CString m_Symbol;
	long m_DayLineStartDate;
	long m_DayLineEndDate;
	long m_IPOStatus;

public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
