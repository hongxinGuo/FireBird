// CSetTiingoStock52WeekHigh.h : CSetTiingoStock52WeekHigh ������
#pragma once

#include"VirtualRecordset.h"

class CSetTiingoStock52WeekHigh : public CVirtualRecordset {
public:
	CSetTiingoStock52WeekHigh(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_52week_High"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	CString m_Symbol{ _T("") };
	CString m_Exchange{ _T("") };
	long m_Date{ 0 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
