// CSetTiingoStockCurrentTrace.h : CSetTiingoStock ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoStockCurrentTrace : public CVirtualRecordset {
public:
	CSetTiingoStockCurrentTrace(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_current_trace"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	long m_Date{ 19800101 };
	CString m_Symbol{ _T("") }; // Tiingo���ô����ʶ

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
