// SetSymbol.h : CSetActiveStockCode ������

#pragma once
#include"VirtualRecordset.h"

class CSetStockSection : public CVirtualRecordset
{
public:
	CSetStockSection(CString strSchema = _T("ChinaMarket"), CString strTable = _T("Stock_Code_Section"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetStockSection)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	BOOL m_Active;
	int m_Market;
	long m_IndexNumber;
	CString m_Comment;
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