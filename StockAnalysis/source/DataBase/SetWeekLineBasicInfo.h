// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo ������

#pragma once

#include"afxdb.h"

// ���������� 2019��6��2��, 5:10

class CSetWeekLineBasicInfo : public CRecordset
{
public:
  CSetWeekLineBasicInfo(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetWeekLineBasicInfo)

  // �ֶ�/��������

  // �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
  // �������͵� CStringW)��ʵ���������͡�
  //  ����Ϊ��ֹ ODBC ��������ִ�п���
  // ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
  // CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
  // (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
  // ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;  // �����ã��Զ����ɡ�
  long m_Date;	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
  long m_Market;	//�г����Ϻ������ڣ�
  CString m_StockCode;	//��λ��Ʊ����
  CStringW m_StockName;	//��λ��Ʊ����   ����������CStringW(Unicode)��ʽ��������Щ�����޷���ʾ���������ݿ��޷��洢
  CString m_LastClose;	//�������̼ۡ���λ��0.001Ԫ
  CString m_Open;	//���쿪�̼�
  CString m_High;	//������߼�
  CString m_Low;	//������ͼ�
  CString m_Close;	//�������̼�
  CString m_Volume;	//�����ܳɽ���������λ���ɡ�
  CString m_Amount;	//�����ܳɽ�����λ��Ԫ
  CString m_UpAndDown;	//�����ǵ�
  CString m_UpDownRate; // �����ǵ����ȣ���λ1%��
  CString m_ChangeHandRate; // ���ջ�����
  CString m_TotalValue;	//����ֵ����λ��Ԫ
  CString m_CurrentValue;	//��ͨ��ֵ����λ��Ԫ
  CString m_RelativeStrong;	//��������г���ǿ��
  CString m_RelativeStrongIndex;	//������г�ָ����ǿ��
  CString m_RelativeStrongBackup;	//��������

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
