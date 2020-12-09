// SetStockCode.h : CSetStockCode ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:14

class CSetAmericaStake : public CRecordset
{
public:
  CSetAmericaStake(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetAmericaStake)

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
  CString m_Type;
  CString m_Currency;
  CString m_Country;
  CString m_Exchange;
  CString m_Ticker;
  CString m_IPODate;
  CString m_MarketCapitalization;
  CString m_ShareOutstanding;
  CString m_Phone;
  CString m_Name;
  CString m_WebURL;
  CString m_FinnhubIndustry;  // �����ɵ��麯����д
  long m_AmericaStakeUpdateDate;
  long m_DayLineStartDate;
  long m_DayLineEndDate;

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
