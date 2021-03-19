// CSetTiingoStockProfile.h : CSetStockCode ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:14

class CSetTiingoStockProfile : public CRecordset
{
public:
  CSetTiingoStockProfile(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetTiingoStockProfile)

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
  long m_SICCode;
  CString m_SICIndustry;
  CString m_SICSector;
  CString m_TiingoIndustry;
  CString m_TiingoSector;
  CString m_ReportingCurrency;
  CString m_Location;
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
