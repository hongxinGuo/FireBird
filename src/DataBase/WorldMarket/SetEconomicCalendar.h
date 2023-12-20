// CSetEconomicCalendar.h : CSetEconomicCalendar ������

#pragma once

#include"VirtualRecordset.h"

// ���������� 2019��6��2��, 5:10

class CSetEconomicCalendar : public CVirtualRecordset {
public:
	CSetEconomicCalendar(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("economic_calendar"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	CString m_Time;
	CString m_Country;
	CString m_Event;
	CString m_Impact;
	CString m_Actual;
	CString m_Estimate;
	CString m_Prev;
	CString m_Unit;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
