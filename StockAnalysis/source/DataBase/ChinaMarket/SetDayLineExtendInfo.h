#pragma once

#include"VirtualRecordset.h"

class CSetDayLineExtendInfo : public CVirtualRecordset
{
public:
	CSetDayLineExtendInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("daylineinfo"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetDayLineExtendInfo)

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
	CString m_Symbol;	//��λ��Ʊ����
	CString m_TransactionNumber; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow5000; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow50000; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow200000; // ���յ��̿ڱ仯��
	CString m_TransactionNumberAbove200000; // ���յ��̿ڱ仯��
	CString	m_AttackBuyVolume;	//��������������
	CString	m_AttackSellVolume;	//��������������
	CString	m_StrongBuyVolume;	//ǿ��������������
	CString	m_StrongSellVolume;	//ǿ��������������
	CString m_OrdinaryBuyVolume;
	CString m_OrdinarySellVolume;
	CString	m_UnknownVolume;	//δ֪����������
	CString	m_CanceledBuyVolume;	//�򵥳�������
	CString	m_CanceledSellVolume;	//������������
	CString	m_AttackBuyBelow50000;
	CString	m_AttackBuyBelow200000;
	CString	m_AttackBuyAbove200000;
	CString	m_AttackSellBelow50000;
	CString	m_AttackSellBelow200000;
	CString	m_AttackSellAbove200000;

	CString m_OrdinaryBuyVolumeBelow5000;
	CString m_OrdinaryBuyVolumeBelow10000;
	CString m_OrdinaryBuyVolumeBelow20000;
	CString m_OrdinaryBuyVolumeBelow50000;
	CString m_OrdinaryBuyVolumeBelow100000;
	CString m_OrdinaryBuyVolumeBelow200000;
	CString m_OrdinaryBuyVolumeAbove200000;
	CString m_OrdinaryBuyNumberBelow5000;
	CString m_OrdinaryBuyNumberBelow10000;
	CString m_OrdinaryBuyNumberBelow20000;
	CString m_OrdinaryBuyNumberBelow50000;
	CString m_OrdinaryBuyNumberBelow100000;
	CString m_OrdinaryBuyNumberBelow200000;
	CString m_OrdinaryBuyNumberAbove200000;
	CString m_OrdinarySellVolumeBelow5000;
	CString m_OrdinarySellVolumeBelow10000;
	CString m_OrdinarySellVolumeBelow20000;
	CString m_OrdinarySellVolumeBelow50000;
	CString m_OrdinarySellVolumeBelow100000;
	CString m_OrdinarySellVolumeBelow200000;
	CString m_OrdinarySellVolumeAbove200000;
	CString m_OrdinarySellNumberBelow5000;
	CString m_OrdinarySellNumberBelow10000;
	CString m_OrdinarySellNumberBelow20000;
	CString m_OrdinarySellNumberBelow50000;
	CString m_OrdinarySellNumberBelow100000;
	CString m_OrdinarySellNumberBelow200000;
	CString m_OrdinarySellNumberAbove200000;

	CString m_CanceledBuyVolumeBelow5000;
	CString m_CanceledBuyVolumeBelow10000;
	CString m_CanceledBuyVolumeBelow20000;
	CString m_CanceledBuyVolumeBelow50000;
	CString m_CanceledBuyVolumeBelow100000;
	CString m_CanceledBuyVolumeBelow200000;
	CString m_CanceledBuyVolumeAbove200000;
	CString m_CanceledSellVolumeBelow5000;
	CString m_CanceledSellVolumeBelow10000;
	CString m_CanceledSellVolumeBelow20000;
	CString m_CanceledSellVolumeBelow50000;
	CString m_CanceledSellVolumeBelow100000;
	CString m_CanceledSellVolumeBelow200000;
	CString m_CanceledSellVolumeAbove200000;

	// ��д
		// �����ɵ��麯����д
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
