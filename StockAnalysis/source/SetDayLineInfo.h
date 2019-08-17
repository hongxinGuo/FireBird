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
  CString	m_StockCode;	//��λ��Ʊ����
  long  m_TransactionNumber; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow5000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow50000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberBelow200000; // ���յ��̿ڱ仯��
  long  m_TransactionNumberAbove200000; // ���յ��̿ڱ仯��
  double	m_AttackBuyVolume;	//��������������
  double	m_AttackSellVolume;	//��������������
  double	m_StrongBuyVolume;	//ǿ��������������
  double	m_StrongSellVolume;	//ǿ��������������
  double  m_OrdinaryBuyVolume;
  double  m_OrdinarySellVolume;
  double	m_UnknownVolume;	//δ֪����������
  double	m_CancelBuyVolume;	//�򵥳�������
  double	m_CancelSellVolume;	//������������
  double	m_AttackBuyBelow50000;
  double	m_AttackBuyBelow200000;
  double	m_AttackBuyAbove200000;
  double	m_AttackSellBelow50000;
  double	m_AttackSellBelow200000;
  double	m_AttackSellAbove200000;

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

