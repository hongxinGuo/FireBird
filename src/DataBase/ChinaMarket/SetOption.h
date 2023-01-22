#pragma once
#include"VirtualRecordset.h"

class CSetOption : public CVirtualRecordset {
public:
	CSetOption(CString strSchema = _T("ChinaMarket"), CString strTable = _T("options"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��
	long m_ID;
	long m_RSEndDate;
	long m_RSStartDate;
	long m_LastLoginDate;
	long m_LastLoginTime;
	long m_UpdatedDateFor10DaysRS1;
	long m_UpdatedDateFor10DaysRS2;
	long m_UpdatedDateFor10DaysRS;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
