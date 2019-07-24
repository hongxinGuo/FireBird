// CSetDayLine.h : CSetDayLine ������

#pragma once

#include"afxdb.h"

// ���������� 2019��6��2��, 5:10

class CSetDayLineToday : public CRecordset
{
public:
  CSetDayLineToday(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetDayLineToday)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long        m_ID;  // �����ã��Զ����ɡ�
  long	      m_Time;	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
  long	      m_Market;	//�г����Ϻ������ڣ�
  CString 	  m_StockCode;	//��λ��Ʊ����
  CStringW 	  m_StockName;	//��λ��Ʊ����   ����������CStringW(Unicode)��ʽ��������Щ�����޷���ʾ���������ݿ��޷��洢
  double  	  m_LastClose;	//�������̼ۡ���λ��0.001Ԫ
  double  	  m_Open;	//���쿪�̼�
  double  	  m_High;	//������߼�
  double  	  m_Low;	//������ͼ�
  double  	  m_Close;	//�������̼�
  double      m_Volume;	//�����ܳɽ���������λ���ɡ�
  double      m_Amount;	//�����ܳɽ�����λ��Ԫ
  double      m_UpAndDown;	//�����ǵ�
  double      m_UpDownRate; // �����ǵ����ȣ���λ1%��
  double      m_ChangeHandRate; // ���ջ�����
  double      m_TotalValue;	//����ֵ����λ��Ԫ
  double      m_CurrentValue;	//��ͨ��ֵ����λ��Ԫ
  double	    m_RelativeStrong;	//��������г���ǿ��
  long        m_TransactionNumber; // ���յ��̿ڱ仯��
  long        m_TransactionNumberBelow5000;
  long        m_TransactionNumberBelow50000;
  long        m_TransactionNumberBelow200000;
  long        m_TransactionNumberAbove200000;
  double      m_OrdinaryBuyVolume; // һ���������������λ����
  double      m_OrdinarySellVolume; // һ������������
  double      m_AttackBuyVolume;	//��������������
  double      m_AttackSellVolume;	//��������������
  double      m_StrongBuyVolume;	//ǿ��������������
  double      m_StrongSellVolume;	//ǿ��������������
  double	    m_UnknownVolume;	//δ֪����������
  double	    m_CancelBuyVolume;	//�򵥳�������
  double	    m_CancelSellVolume;	//������������
  double	    m_AttackBuyBelow50000;
  double	    m_AttackBuyBelow200000;
  double      m_AttackBuyAbove200000;
  double	    m_AttackSellBelow50000;
  double      m_AttackSellBelow200000;
  double      m_AttackSellAbove200000;

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


