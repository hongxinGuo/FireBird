// CSetTiingoStock.h : CSetTiingoStock ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoCrypto : public CVirtualRecordset {
public:
	CSetTiingoCrypto(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_crypto_symbol"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	CString m_Ticker{ _T("") };
	CString m_Name{ _T("") };
	CString m_BaseCurrency{ _T("") };
	CString m_QuoteCurrency{ _T("") };
	CString m_Description{ _T("") };
	CString m_UpdateDate{ _T("") };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
