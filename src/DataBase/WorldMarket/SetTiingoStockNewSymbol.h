#pragma once
#include"VirtualRecordset.h"

class CSetTiingoStockNewSymbol : public CVirtualRecordset {
public:
	CSetTiingoStockNewSymbol(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_new_symbol"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	CString m_symbol{ _T("") };
	long m_date{ 0 };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
