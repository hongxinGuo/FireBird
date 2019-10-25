#pragma once

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
  long m_Day;
  long m_TD1;
  long m_TD2;
  long m_TD3C;
  long m_TD5;
  long m_TD6;
  long m_TD7;
  long m_TD8;
  long m_TD9;
  long m_TD12;
  long m_TD15;
  long m_TD19;
  long m_TD20;
  long m_TD21;
  long m_VLCC_USGSPORE;
  long m_SUEZMAX_CBSUSG;
  long m_TC1;
  long m_TC2;
  long m_TC3;
  long m_TC4;
  long m_TC5;
  long m_TC14;
  long m_CPP_USGCBS;
  long m_VLCC_TC_1YEAR;
  long m_SUEZMAX_TC_1YEAR;
  long m_AFRAMAX_TC_1YEAR;
  long m_PANAMAX_TC_1YEAR;
  long m_MR_TC_1YEAR;
  long m_HANDY_TC_1YEAR;
  long m_VLCC_TC_3YEAR;
  long m_SUEZMAX_TC_3YEAR;
  long m_AFRAMAX_TC_3YEAR;
  long m_PANAMAX_TC_3YEAR;
  long m_MR_TC_3YEAR;
  long m_HANDY_TC_3YEAR;

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
