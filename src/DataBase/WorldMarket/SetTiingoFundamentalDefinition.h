// CSetTiingoFundamentalDefinition.h : CSetTiingoFundamentalDefinition ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoFundamentalDefinition : public CVirtualRecordset {
public:
	CSetTiingoFundamentalDefinition(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_fundamental_definitions"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_dataCode;
	CString m_name;
	CString m_description;
	CString m_statementType;
	CString m_units;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
