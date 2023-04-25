// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢����֤ȯ�������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CVirtualSetStockSymbol : public CVirtualRecordset {
public:
	CVirtualSetStockSymbol(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

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
	CString m_Exchange;
	CString m_Symbol;
	CStringW m_DisplaySymbol; // �������й���Ʊʱ���洢��ʱ��Ʊ���������ƣ��ʶ���Ҫʹ��CStringW��ʽ����ֹ���ַǷ��ַ���
	long m_DayLineStartDate;
	long m_DayLineEndDate;
	long m_IPOStatus;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
