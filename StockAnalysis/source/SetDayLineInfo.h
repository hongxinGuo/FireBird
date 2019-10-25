#pragma once

// ���������� 2019��6��17��, 5:10

class CSetDayLineInfo : public CRecordset
{
public:
  CSetDayLineInfo(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetDayLineInfo)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;  // �����ã��Զ����ɡ�
  long	m_Day;	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
  long	      m_Market;	//�г����Ϻ������ڣ�
  CString 	  m_StockCode;	//��λ��Ʊ����
  CString  m_TransactionNumber; // ���յ��̿ڱ仯��
  CString  m_TransactionNumberBelow5000; // ���յ��̿ڱ仯��
  CString  m_TransactionNumberBelow50000; // ���յ��̿ڱ仯��
  CString  m_TransactionNumberBelow200000; // ���յ��̿ڱ仯��
  CString  m_TransactionNumberAbove200000; // ���յ��̿ڱ仯��
  CString	m_AttackBuyVolume;	//��������������
  CString	m_AttackSellVolume;	//��������������
  CString	m_StrongBuyVolume;	//ǿ��������������
  CString	m_StrongSellVolume;	//ǿ��������������
  CString  m_OrdinaryBuyVolume;
  CString  m_OrdinarySellVolume;
  CString	m_UnknownVolume;	//δ֪����������
  CString	m_CancelBuyVolume;	//�򵥳�������
  CString	m_CancelSellVolume;	//������������
  CString	m_AttackBuyBelow50000;
  CString	m_AttackBuyBelow200000;
  CString	m_AttackBuyAbove200000;
  CString	m_AttackSellBelow50000;
  CString	m_AttackSellBelow200000;
  CString	m_AttackSellAbove200000;

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
