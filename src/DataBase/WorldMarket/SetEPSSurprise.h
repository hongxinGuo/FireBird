// CSetEPSSurprise.h : CSetEPSSurprise ������

#pragma once

#include"VirtualRecordset.h"

class CSetEPSSurprise : public CVirtualRecordset
{
public:
	CSetEPSSurprise(CString strSchema = _T("WorldMarket"), CString strTable = _T("eps_surprise"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetEPSSurprise)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	CString m_Symbol;
	long m_Date;
	double m_Actual;
	double m_Estimate;

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
