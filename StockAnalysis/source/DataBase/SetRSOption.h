#pragma once
#include"afxdb.h"

class CSetRSOption : public CRecordset
{
public:
  CSetRSOption(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetRSOption)

  long m_ID;
  long m_Index;
  BOOL m_Active;
  long m_DayLengthFirst;
  long m_DayLengthSecond;
  long m_DayLengthThird;
  long m_DayLengthFourth;
  long m_StrongDayLengthFirst;
  long m_StrongDayLengthSecond;
  long m_StrongDayLengthThird;
  long m_StrongDayLengthFourth;
  CString m_RSStrongFirst;
  CString m_RSStrongSecond;
  CString m_RSStrongThird;
  CString m_RSStrongFourth;
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
