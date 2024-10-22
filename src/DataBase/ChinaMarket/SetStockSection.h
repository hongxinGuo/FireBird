// SetSymbol.h : CSetActiveStockCode ������

#pragma once
#include"VirtualRecordset.h"

class CSetStockSection : public CVirtualRecordset {
public:
	CSetStockSection(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("Stock_Code_Section"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	BOOL m_Active{ false };
	int m_Market{ 0 };
	long m_IndexNumber{ 0 };
	CString m_Comment{ _T("") };
	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
