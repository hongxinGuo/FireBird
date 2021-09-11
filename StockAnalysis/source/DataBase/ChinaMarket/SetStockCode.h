// SetSymbol.h : CSetStockCode ������

#pragma once
#include"VirtualRecordset.h"

class CSetStockCode : public CVirtualRecordset
{
public:
	CSetStockCode(CString strSchema = _T("ChinaMarket"), CString strTable = _T("stockcode"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetStockCode)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString	m_Symbol;
	CStringW m_StockName;
	long m_DayLineStartDate;     // ��������ʷ���߷������ϵõ��ĳ�ʼ������
	long m_DayLineEndDate;     // ��������ʷ���߷������ϵõ���������
	long m_IPOStatus;
	// ��д
		// �����ɵ��麯����д
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
