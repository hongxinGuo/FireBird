// CSetEconomicCalendar.h : CSetEconomicCalendar ������

#pragma once

#include"VirtualRecordset.h"

// ���������� 2019��6��2��, 5:10

class CSetEconomicCalendar : public CVirtualRecordset
{
public:
	CSetEconomicCalendar(CString strSchema = _T("WorldMarket"), CString strTable = _T("economic_calendar"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetEconomicCalendar)

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
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};