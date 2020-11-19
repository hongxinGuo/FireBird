// SetStakeCode.h : CSetStakeCode ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:14

class CSetStakeCode : public CRecordset
{
public:
  CSetStakeCode(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetStakeCode)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;
  int	m_StakeType;
  CString	m_StakeCode;
  CStringW m_StakeName;
  long m_DayLineStartDate;     // ��������ʷ���߷������ϵõ��ĳ�ʼ������
  long m_DayLineEndDate;     // ��������ʷ���߷������ϵõ���������
  long m_IPOStatus;
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
