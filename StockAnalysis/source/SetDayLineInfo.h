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
  long	m_Time;	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
  long	m_Market;	//�г����Ϻ������ڣ�
  CStringW	m_StockCode;	//��λ��Ʊ����
  CStringW	m_StockName;	//��λ��Ʊ����
  double	m_RelativeStrong;	//��������г���ǿ��
  long  m_TransactionNumber; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow5000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow50000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow200000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberAbove200000; // ���յ��̿ڱ仯��
  long	m_AttackBuyVolume;	//��������������
  long	m_AttackSellVolume;	//��������������
  long	m_StrongBuyVolume;	//ǿ��������������
  long	m_StrongSellVolume;	//ǿ��������������
  long  m_OrdinaryBuyVolume;
  long  m_OrdinarySellVolume;
  long	m_UnknownVolume;	//δ֪����������
  long	m_CancelBuyVolume;	//�򵥳�������
  long	m_CancelSellVolume;	//������������
  long	m_AttackBuyBelow50000;
  long	m_AttackBuyBelow200000;
  long	m_AttackBuyAbove200000;
  long	m_AttackSellBelow50000;
  long	m_AttackSellBelow200000;
  long	m_AttackSellAbove200000;

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

