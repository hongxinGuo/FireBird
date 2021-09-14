// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢���������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CVirtualSetExchange : public CVirtualRecordset
{
public:
	CVirtualSetExchange(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CVirtualSetExchange)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_Code;
	CString m_Name;
	CString m_Mic;
	CString m_TimeZone;
	CString m_Hour;
	CString m_CloseDate;
	CString m_Country;
	CString m_Source;

public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
