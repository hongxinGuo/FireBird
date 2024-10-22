// VirtualSetHistoryCandleExtend.h : CVirtualSetHistoryCandleExtend ������
//
// ���е���ʷ������չ���ݿ⣬��׼��ʹ�ô���Ϊ���࣬�Լ򻯴洢���̡�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualRecordset.h"

class CVirtualSetHistoryCandleExtend : public CVirtualRecordset {
public:
	CVirtualSetHistoryCandleExtend(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	long m_Date{ 0 };	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
	CString m_Symbol{ _T("") };	//��λ��Ʊ����
	CString m_TransactionNumber{ _T("") }; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow5000{ _T("") }; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow50000{ _T("") }; // ���յ��̿ڱ仯��
	CString m_TransactionNumberBelow200000{ _T("") }; // ���յ��̿ڱ仯��
	CString m_TransactionNumberAbove200000{ _T("") }; // ���յ��̿ڱ仯��
	CString m_AttackBuyVolume{ _T("") };	//��������������
	CString m_AttackSellVolume{ _T("") };	//��������������
	CString m_StrongBuyVolume{ _T("") };	//ǿ��������������
	CString m_StrongSellVolume{ _T("") };	//ǿ��������������
	CString m_OrdinaryBuyVolume{ _T("") };
	CString m_OrdinarySellVolume{ _T("") };
	CString m_UnknownVolume{ _T("") };	//δ֪����������
	CString m_CanceledBuyVolume{ _T("") };	//�򵥳�������
	CString m_CanceledSellVolume{ _T("") };	//������������
	CString m_AttackBuyBelow50000{ _T("") };
	CString m_AttackBuyBelow200000{ _T("") };
	CString m_AttackBuyAbove200000{ _T("") };
	CString m_AttackSellBelow50000{ _T("") };
	CString m_AttackSellBelow200000{ _T("") };
	CString m_AttackSellAbove200000{ _T("") };

	CString m_OrdinaryBuyVolumeBelow5000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow10000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow20000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow50000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow100000{ _T("") };
	CString m_OrdinaryBuyVolumeBelow200000{ _T("") };
	CString m_OrdinaryBuyVolumeAbove200000{ _T("") };
	CString m_OrdinaryBuyNumberBelow5000{ _T("") };
	CString m_OrdinaryBuyNumberBelow10000{ _T("") };
	CString m_OrdinaryBuyNumberBelow20000{ _T("") };
	CString m_OrdinaryBuyNumberBelow50000{ _T("") };
	CString m_OrdinaryBuyNumberBelow100000{ _T("") };
	CString m_OrdinaryBuyNumberBelow200000{ _T("") };
	CString m_OrdinaryBuyNumberAbove200000{ _T("") };
	CString m_OrdinarySellVolumeBelow5000{ _T("") };
	CString m_OrdinarySellVolumeBelow10000{ _T("") };
	CString m_OrdinarySellVolumeBelow20000{ _T("") };
	CString m_OrdinarySellVolumeBelow50000{ _T("") };
	CString m_OrdinarySellVolumeBelow100000{ _T("") };
	CString m_OrdinarySellVolumeBelow200000{ _T("") };
	CString m_OrdinarySellVolumeAbove200000{ _T("") };
	CString m_OrdinarySellNumberBelow5000{ _T("") };
	CString m_OrdinarySellNumberBelow10000{ _T("") };
	CString m_OrdinarySellNumberBelow20000{ _T("") };
	CString m_OrdinarySellNumberBelow50000{ _T("") };
	CString m_OrdinarySellNumberBelow100000{ _T("") };
	CString m_OrdinarySellNumberBelow200000{ _T("") };
	CString m_OrdinarySellNumberAbove200000{ _T("") };

	CString m_CanceledBuyVolumeBelow5000{ _T("") };
	CString m_CanceledBuyVolumeBelow10000{ _T("") };
	CString m_CanceledBuyVolumeBelow20000{ _T("") };
	CString m_CanceledBuyVolumeBelow50000{ _T("") };
	CString m_CanceledBuyVolumeBelow100000{ _T("") };
	CString m_CanceledBuyVolumeBelow200000{ _T("") };
	CString m_CanceledBuyVolumeAbove200000{ _T("") };
	CString m_CanceledSellVolumeBelow5000{ _T("") };
	CString m_CanceledSellVolumeBelow10000{ _T("") };
	CString m_CanceledSellVolumeBelow20000{ _T("") };
	CString m_CanceledSellVolumeBelow50000{ _T("") };
	CString m_CanceledSellVolumeBelow100000{ _T("") };
	CString m_CanceledSellVolumeBelow200000{ _T("") };
	CString m_CanceledSellVolumeAbove200000{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
