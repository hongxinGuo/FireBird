#pragma once

#include"stdafx.h"

class CSetPotenDailyBriefing : public CRecordset
{
public:
  CSetPotenDailyBriefing(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetPotenDailyBriefing)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��
  long m_ID;
  long m_Day;
  CString m_TD3C;
  CString m_TD9;
  CString m_TD20;
  CString m_TD21;
  CString m_TC1;
  CString m_TC2;
  CString m_TC5;
  CString m_TC14;
  CString m_VLCC_TC_1YEAR;
  CString m_SUEZMAX_TC_1YEAR;
  CString m_AFRAMAX_TC_1YEAR;
  CString m_PANAMAX_TC_1YEAR;
  CString m_MR_TC_1YEAR;
  CString m_HANDY_TC_1YEAR;
  CString m_VLCC_NEW_BUILD;
  CString m_SUEZMAX_NEW_BUILD;
  CString m_AFRAMAX_NEW_BUILD;
  CString m_LR2_NEW_BUILD;
  CString m_LR1_NEW_BUILD;
  CString m_MR_NEW_BUILD;
  CString m_HANDYMAX_NEW_BUILD;
  CString m_VLCC_5YEARS_OLD;
  CString m_SUEZMAX_5YEARS_OLD;
  CString m_AFRAMAX_5YEARS_OLD;
  CString m_LR2_5YEARS_OLD;
  CString m_LR1_5YEARS_OLD;
  CString m_MR_5YEARS_OLD;
  CString m_HANDYMAX_5YEARS_OLD;

  // ��д
    // �����ɵ��麯����д
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
