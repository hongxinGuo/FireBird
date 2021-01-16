// CSetFinnhubExchange.h : CSetFinnhubExchange ������

#pragma once

#include"afxdb.h"

// ���������� 2019��6��2��, 5:10

class CSetFinnhubExchange : public CRecordset
{
public:
  CSetFinnhubExchange(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetFinnhubExchange)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;  // �����ã��Զ����ɡ�
  CString m_Code;
  CString m_Name;
  CString m_Mic;
  CString m_TimeZone;
  CString m_Hour;
  CString m_CloseDate;
  CString m_Country;
  CString m_Source;
  BOOL m_IsActive;

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
