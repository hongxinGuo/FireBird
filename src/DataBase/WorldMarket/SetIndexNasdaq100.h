//
// Nasdaq100ָ�������ݱ��ʽ
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CSetIndexNasdaq100 : public CVirtualRecordset {
public:
	explicit CSetIndexNasdaq100(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("index_nasdaq100"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	CString m_Symbol{ _T("") };
	long m_ExpiredDate{ 20990101 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
