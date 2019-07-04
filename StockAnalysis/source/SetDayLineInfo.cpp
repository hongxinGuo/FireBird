// SetDayLineInfo.h : CSetDayLine ���ʵ��



// CSetDayLineInfo ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"globedef.h"

#include "SetDayLineInfo.h"
IMPLEMENT_DYNAMIC(CSetDayLineInfo, CRecordset)

CSetDayLineInfo::CSetDayLineInfo(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_ID = 0;
  m_Time = 0;
  m_Market = 0;
  m_StockCode = L"";
  m_StockName = L"";
  m_RelativeStrong = 0;	//��������г���ǿ��
  m_TransactionNumber = 0;
  m_TransactionNumberBelow5000 = 0; // ���յ��̿ڱ仯��
  m_TransactionNumberBelow50000 = 0; // ���յ��̿ڱ仯��
  m_TransactionNumberBelow200000 = 0; // ���յ��̿ڱ仯��
  m_TransactionNumberAbove200000 = 0; // ���յ��̿ڱ仯��
  m_AttackBuyVolume = 0;	//��������������
  m_AttackSellVolume = 0;	//��������������
  m_StrongBuyVolume = 0;	//ǿ��������������
  m_StrongSellVolume = 0;	//ǿ��������������
  m_OrdinaryBuyVolume = 0;
  m_OrdinarySellVolume = 0;
  m_UnknownVolume = 0;	//δ֪����������
  m_CancelBuyVolume = 0;	//�򵥳�������
  m_CancelSellVolume = 0;	//������������
  m_AttackBuyBelow50000 = 0;
  m_AttackBuyBelow200000 = 0;
  m_AttackBuyAbove200000 = 0;
  m_AttackSellBelow50000 = 0;
  m_AttackSellBelow200000 = 0;
  m_AttackSellAbove200000 = 0;
  m_nFields = 26;
  //m_nDefaultType = dynaset;
}

CString CSetDayLineInfo::GetDefaultConnect()
{
  return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
}

CString CSetDayLineInfo::GetDefaultSQL()
{
  return _T("[DayLineInfo]");
}

void CSetDayLineInfo::DoFieldExchange(CFieldExchange* pFX)
{
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Time]"), m_Time);
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Double(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//��������г���ǿ��
  RFX_Long(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Long(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Long(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Long(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Long(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Long(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//��������������
  RFX_Long(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//��������������
  RFX_Long(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//ǿ��������������
  RFX_Long(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//ǿ��������������
  RFX_Long(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);	//ǿ��������������
  RFX_Long(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);	//ǿ��������������
  RFX_Long(pFX, _T("[UnknownVolume]"), m_UnknownVolume);
  RFX_Long(pFX, _T("[CancelBuyVolume]"), m_CancelBuyVolume);
  RFX_Long(pFX, _T("[CancelSellVolume]"), m_CancelSellVolume);
  RFX_Long(pFX, _T("[AttackBuyBelow50000]"), m_AttackBuyBelow50000);
  RFX_Long(pFX, _T("[AttackBuyBelow200000]"), m_AttackBuyBelow200000);
  RFX_Long(pFX, _T("[AttackBuyAbove200000]"), m_AttackBuyAbove200000);
  RFX_Long(pFX, _T("[AttackSellBelow50000]"), m_AttackSellBelow50000);
  RFX_Long(pFX, _T("[AttackSellBelow200000]"), m_AttackSellBelow200000);
  RFX_Long(pFX, _T("[AttackSellAbove200000]"), m_AttackSellAbove200000);

}
/////////////////////////////////////////////////////////////////////////////
// CSetDayLine ���

#ifdef _DEBUG
void CSetDayLineInfo::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetDayLineInfo::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG

