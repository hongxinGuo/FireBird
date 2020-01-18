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
  CString m_LR2_TC_1YEAR;
  CString m_LR1_TC_1YEAR;
  CString m_MR_TC_1YEAR;
  CString m_HANDYMAX_TC_1YEAR;
  CString m_VLCC_NewBuild;
  CString m_SUEZMAX_NewBuild;
  CString m_AFRAMAX_NewBuild;
  CString m_LR2_NewBuild;
  CString m_LR1_NewBuild;
  CString m_MR_NewBuild;
  CString m_HANDYMAX_NewBuild;
  CString m_VLCC_5YearOld;
  CString m_SUEZMAX_5YearOld;
  CString m_AFRAMAX_5YearOld;
  CString m_LR2_5YearOld;
  CString m_LR1_5YearOld;
  CString m_MR_5YearOld;
  CString m_HANDYMAX_5YearOld;

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
