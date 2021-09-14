// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢����֤ȯ�������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CVirtualSetStockSymbol : public CVirtualRecordset
{
public:
	CVirtualSetStockSymbol(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CVirtualSetStockSymbol)

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
	CStringW m_DisplaySymbol; // ����ֶε������й���Ʊʱ���洢��ʱ��Ʊ���������ƣ��ʶ���Ҫʹ��CStringW��ʽ����ֹ���ַǷ��ַ���
	long m_DayLineStartDate;
	long m_DayLineEndDate;
	long m_IPOStatus;

public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
