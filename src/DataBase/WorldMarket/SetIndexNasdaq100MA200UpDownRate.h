//
// Nasdaq100ָ�������ݱ��ʽ
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CSetIndexNasdaq100MA200UpDownRate : public CVirtualRecordset {
public:
	explicit CSetIndexNasdaq100MA200UpDownRate(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("index_nasdaq100_200ma_updown_rate"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_Date{};
	long m_Rate{};

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
