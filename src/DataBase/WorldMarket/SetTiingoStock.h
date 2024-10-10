// CSetTiingoStock.h : CSetTiingoStock ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoStock : public CVirtualRecordset {
public:
	CSetTiingoStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_fundamental"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_TiingoPermaTicker; // Tiingo���ô����ʶ
	CString m_Ticker;
	CString m_Name;
	BOOL m_IsActive; //
	BOOL m_IsADR;
	long m_SicCode;
	CString m_SicIndustry;
	CString m_SicSector;
	CString m_TiingoIndustry;
	CString m_TiingoSector;
	CString m_ReportingCurrency;
	CString m_Location;
	CString m_CompanyWebSite;
	CString m_SECFilingWebSite;

	CString m_UpdateDate;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
