// CSetCompanyNews.h : CSetCompanyNews ������

#pragma once

#include"VirtualRecordset.h"

class CSetCompanyNews : public CVirtualRecordset
{
public:
	CSetCompanyNews(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_company_news"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetCompanyNews)

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID;  // �����ã��Զ����ɡ�
	CString m_CompanySymbol;
	CString m_Category;
	CString m_DateTime;
	CString m_HeadLine;
	int m_NewsID;
	CString m_Image;
	CString m_RelatedSymbol;
	CString m_Source;
	CString m_Summary;
	CString m_URL;

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
