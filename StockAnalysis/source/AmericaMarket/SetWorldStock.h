// SetStockCode.h : CSetStockCode ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:14

class CSetWorldStock : public CRecordset
{
public:
  CSetWorldStock(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetWorldStock)

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
  CString m_ListedExchange;
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
  long m_ProfileUpdateDate;
  long m_DayLineStartDate;
  long m_DayLineEndDate;
  long m_LastRTDataUpdateDate; // ����ʵʱ���ݸ�������
  long m_PeerUpdateDate; // ����������Ϣ����ʱ��
  long m_LastEPSSurpriseUpdateDate;
  long m_IPOStatus;

  // Tiingo Symbol��Ϣ
  CString m_TiingoPermaTicker; // Tiingo���ô����ʶ
  BOOL m_IsActive; //
  BOOL m_IsADR;
  long m_SICCode;
  CString m_SICIndustry;
  CString m_SICSector;
  CString m_TiingoIndustry;
  CString m_TiingoSector;
  CString m_CompanyWebSite;
  CString m_SECFilingWebSite;
  long m_StatementUpdateDate;
  long m_DailyDataUpdateDate;

public:
  virtual CString GetDefaultConnect();	// Ĭ�������ַ���

  virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
  virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
