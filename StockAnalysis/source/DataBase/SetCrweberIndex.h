#pragma once

#include"stdafx.h"

class CSetCrweberIndex : public CRecordset
{
public:
  CSetCrweberIndex(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetCrweberIndex)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��
  long m_ID;
  long m_Date;
  CString m_TD1;
  CString m_TD2;
  CString m_TD3C;
  CString m_TD5;
  CString m_TD6;
  CString m_TD7;
  CString m_TD8;
  CString m_TD9;
  CString m_TD12;
  CString m_TD15;
  CString m_TD19;
  CString m_TD20;
  CString m_TD21;
  CString m_VLCC_USGSPORE;
  CString m_SUEZMAX_CBSUSG;
  CString m_TC1;
  CString m_TC2;
  CString m_TC3;
  CString m_TC4;
  CString m_TC5;
  CString m_TC14;
  CString m_CPP_USGCBS;
  CString m_VLCC_TC_1YEAR;
  CString m_SUEZMAX_TC_1YEAR;
  CString m_AFRAMAX_TC_1YEAR;
  CString m_PANAMAX_TC_1YEAR;
  CString m_MR_TC_1YEAR;
  CString m_HANDY_TC_1YEAR;
  CString m_VLCC_TC_3YEAR;
  CString m_SUEZMAX_TC_3YEAR;
  CString m_AFRAMAX_TC_3YEAR;
  CString m_PANAMAX_TC_3YEAR;
  CString m_MR_TC_3YEAR;
  CString m_HANDY_TC_3YEAR;

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
