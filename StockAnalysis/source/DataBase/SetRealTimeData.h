// SetRealTimeData.h : CSetRealTimeData ������

#pragma once
#include"afxdb.h"

// ���������� 2019��5��26��, 8:09

class CSetRealTimeData : public CRecordset
{
public:
  CSetRealTimeData(CDatabase* pDatabase = nullptr);
  DECLARE_DYNAMIC(CSetRealTimeData)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;
  CString m_Time;	//�ɽ�ʱ�䣬������㣬
  long m_Market; // �Ϻ�Ϊ1������Ϊ2�� ������
  CStringW m_StockName;	//��λ��Ʊ���
  CString	m_StockCode;	//��λ��Ʊ����
  CString	m_LastClose;	//�������̼�
  CString	m_Open;	//���쿪�̼�
  CString	m_High;	//������߼�
  CString	m_Low;	//������ͼ�
  CString	m_New;	//���³ɽ��۸�
  CString	m_Volume;	//Ŀǰ�ܳɽ�����
  CString	m_Amount;	//��ǰ�ܳɽ����
  CString	m_Stroke;	//��ǰ�ܳɽ�����
  CString	m_PBuy5;	//���5
  CString	m_VBuy5;	//���5�ϵ�����
  CString	m_PBuy4;
  CString	m_VBuy4;
  CString	m_PBuy3;
  CString	m_VBuy3;
  CString	m_PBuy2;
  CString	m_VBuy2;
  CString	m_PBuy1;
  CString	m_VBuy1;
  CString	m_PSell1;
  CString	m_VSell1;
  CString	m_PSell2;
  CString	m_VSell2;
  CString	m_PSell3;
  CString	m_VSell3;
  CString	m_PSell4;
  CString	m_VSell4;
  CString	m_PSell5;	//����5
  CString	m_VSell5;	//����5�ϵ�����

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
