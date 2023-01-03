// CSetTiingoStock.h : CSetTiingoStock ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoCrypto : public CVirtualRecordset
{
public:
	CSetTiingoCrypto(CString strSchema = _T("WorldMarket"), CString strTable = _T("tiingo_crypto_symbol"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetTiingoCrypto)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_Ticker;
	CString m_Name;
	CString m_BaseCurrency;
	CString m_QuoteCurrency;
	CString m_Description;

public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
