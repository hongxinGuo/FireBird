#pragma once
#include"VirtualRecordset.h"

class CSetRSStrongStock : public CVirtualRecordset
{
public:
	CSetRSStrongStock(long lIndex, CString strSchema = _T("ChinaMarket"), CString strTable = _T("selected_rs_"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetRSStrongStock)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	CString	m_Symbol;

	// ��д
		// �����ɵ��麯����д
public:
	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
	long m_lIndex; // ���ݱ�����

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
