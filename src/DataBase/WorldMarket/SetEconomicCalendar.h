// CSetEconomicCalendar.h : CSetEconomicCalendar ������

#pragma once

#include"VirtualRecordset.h"

// ���������� 2019��6��2��, 5:10

class CSetEconomicCalendar : public CVirtualRecordset {
public:
	CSetEconomicCalendar(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_economic_calendar"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	CString m_Time{ _T("") };
	CString m_Country{ _T("") };
	CString m_Event{ _T("") };
	CString m_Impact{ _T("") };
	CString m_Actual{ _T("") };
	CString m_Estimate{ _T("") };
	CString m_Prev{ _T("") };
	CString m_Unit{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
