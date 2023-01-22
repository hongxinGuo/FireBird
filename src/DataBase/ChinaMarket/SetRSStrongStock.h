#pragma once
#include"VirtualRecordset.h"

class CSetRSStrongStock : public CVirtualRecordset {
public:
	CSetRSStrongStock(long lIndex, CString strSchema = _T("ChinaMarket"), CString strTable = _T("selected_rs_"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	CString m_Symbol;

	// ��д
	// �����ɵ��麯����д
public:
	CString GetDefaultSQL() override; 	// ��¼����Ĭ�� SQL
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��

	// ʵ��
	long m_lIndex; // ���ݱ�����
};
