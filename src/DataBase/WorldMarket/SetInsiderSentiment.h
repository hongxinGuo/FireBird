// CSetInsiderSentiment.h : CSetInsiderSentiment ������

#pragma once

#include"VirtualRecordset.h"

class CSetInsiderSentiment : public CVirtualRecordset {
public:
	CSetInsiderSentiment(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("insider_sentiment"), CDatabase* pDatabase = nullptr);

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
	long m_Change;
	double m_mspr;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
