// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢���������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CVirtualSetExchange : public CVirtualRecordset {
public:
	CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

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
	CString m_PreMarket;
	CString m_Hour;
	CString m_PostMarket;
	CString m_CloseDate;
	CString m_Country;
	CString m_CountryName;
	CString m_Source;
	CString m_MyUnknownColumn;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
