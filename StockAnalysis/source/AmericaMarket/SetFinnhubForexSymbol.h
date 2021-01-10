// SetFinnhubForexSymbol.h : CSetFinnhubForexSymbol ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:14

class CSetFinnhubForexSymbol : public CRecordset
{
public:
  CSetFinnhubForexSymbol(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetFinnhubForexSymbol)

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
  CString m_Exchange;
  CString m_Symbol;
  long m_DayLineStartDate;
  long m_DayLineEndDate;
  long m_IPOStatus;

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
