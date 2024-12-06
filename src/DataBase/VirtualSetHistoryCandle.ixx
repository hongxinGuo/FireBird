// VirtualSetHistoryCandle.h : CVirtualSetHistoryCandle ������
//
// ���е�������ʷ���ݿ⣬��׼��ʹ�ô���Ϊ���࣬�Լ򻯴洢���̡�
// ĿǰCurrentWeekLine��today��ʱ���ݱ�ʹ�ô����ݼ���
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.VirtualHistoryCandle;

import FireBird.Set.VirtualRecordset;

export {
	class CVirtualSetHistoryCandle : public CVirtualRecordset {
	public:
		CVirtualSetHistoryCandle(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);
		~CVirtualSetHistoryCandle() override = default;
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
		CString m_Exchange{ _T("") }; // ���������ƣ���Ʊ�ǽ���������λ���룬Forex��Crypto���ǽ��������ƣ�
		CString m_Symbol{ _T("") };	//����
		CStringW m_DisplaySymbol{ _T("") };	//��Ʊ���ƣ��й���Ʊ�����������ơ�   ����������CStringW(Unicode)��ʽ��������Щ�����޷���ʾ���������ݿ��޷��洢
		CString m_LastClose{ _T("") };	//�������̼ۡ���λ��0.001Ԫ
		CString m_Open{ _T("") };	//���쿪�̼�
		CString m_High{ _T("") };	//������߼�
		CString m_Low{ _T("") };	//������ͼ�
		CString m_Close{ _T("") };	//�������̼�
		CString m_Volume{ _T("") };	//�����ܳɽ���������λ���ɡ�
		CString m_Amount{ _T("") };	//�����ܳɽ�����λ��Ԫ
		CString m_UpAndDown{ _T("") };	//�����ǵ�
		CString m_UpDownRate{ _T("") }; // �����ǵ����ȣ���λ1%��
		CString m_ChangeHandRate{ _T("") }; // ���ջ�����
		CString m_TotalValue{ _T("") };	//����ֵ����λ��Ԫ
		CString m_CurrentValue{ _T("") };	//��ͨ��ֵ����λ��Ԫ
		CString m_RS{ _T("") };	//��������г���ǿ��
		CString m_RSIndex{ _T("") };	//������г�ָ����ǿ��
		CString m_RSBackup{ _T("") };	//��������

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
}
