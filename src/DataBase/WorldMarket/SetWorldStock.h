// SetWorldStock.h : CSetWorldStock ������

#pragma once
#include"VirtualRecordset.h"

class CSetWorldStock : public CVirtualRecordset {
public:
	CSetWorldStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_profile"), CDatabase* pDatabase = nullptr);
	~CSetWorldStock() override = default;

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_Description;
	CString m_DisplaySymbol;
	CString m_Symbol;
	CString m_ExchangeCode;
	CString m_Type;
	CString m_Mic;
	CString m_Figi;
	CString m_Currency;
	CString m_Address;
	CString m_City;
	CString m_Country;
	CString m_Cusip;
	CString m_Sedol;
	long m_EmployeeTotal;
	CString m_Ggroup;
	CString m_Gind;
	CString m_Gsector;
	CString m_Gsubind;
	CString m_IPODate;
	CString m_Isin;
	CString m_MarketCapitalization;
	CString m_Naics;
	CString m_NaicsNationalIndustry;
	CString m_NaicsSector;
	CString m_NaicsSubsector;
	CString m_Name;
	CString m_Phone;
	CString m_ShareOutstanding;
	CString m_State;
	CString m_Ticker;
	CString m_WebURL;
	CString m_Logo;
	CString m_FinnhubIndustry;
	CString m_Peer;
	long m_IPOStatus;

	CString m_UpdateDate;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
