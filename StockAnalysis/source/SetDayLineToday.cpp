// SetDayLineToday.h : CSetDayLineToday ���ʵ��

// CSetDayLineToday ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetDayLineToday.h"

IMPLEMENT_DYNAMIC(CSetDayLineToday, CRecordset)

CSetDayLineToday::CSetDayLineToday(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_ID = 0;
  m_Time = 0;
  m_Market = 0;
  m_StockCode = "";
  m_StockName = "";
  m_LastClose = _T("");
  m_Open = _T("");
  m_High = _T("");
  m_Low = _T("");
  m_Close = _T("");
  m_Volume = _T("");
  m_Amount = _T("");
  m_UpAndDown = _T("");
  m_UpDownRate = _T("");
  m_ChangeHandRate = _T("");
  m_TotalValue = _T("");
  m_CurrentValue = _T("");
  m_RelativeStrong = _T("");	//��������г���ǿ��
  m_TransactionNumber = _T("");
  m_TransactionNumberBelow5000 = _T("");
  m_TransactionNumberBelow50000 = _T("");
  m_TransactionNumberBelow200000 = _T("");
  m_TransactionNumberAbove200000 = _T("");
  m_AttackBuyVolume = _T("");	//��������������
  m_AttackSellVolume = _T("");	//��������������
  m_StrongBuyVolume = _T("");	//ǿ��������������
  m_StrongSellVolume = _T("");	//ǿ��������������
  m_UnknownVolume = _T("");	//δ֪����������
  m_CancelBuyVolume = _T("");	//�򵥳�������
  m_CancelSellVolume = _T("");	//������������
  m_OrdinaryBuyVolume = _T("");
  m_AttackBuyBelow50000 = _T("");
  m_AttackBuyBelow200000 = _T("");
  m_AttackBuyAbove200000 = _T("");
  m_OrdinarySellVolume = _T("");
  m_AttackSellBelow50000 = _T("");
  m_AttackSellBelow200000 = _T("");
  m_AttackSellAbove200000 = _T("");
  m_nFields = 38;
}

CString CSetDayLineToday::GetDefaultConnect()
{
  return GetDefaultSchemaConnect();
}

CString CSetDayLineToday::GetDefaultSQL()
{
  return _T("[today]");
}

void CSetDayLineToday::DoFieldExchange(CFieldExchange* pFX)
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
  RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Text(pFX, _T("[Open]"), m_Open);
  RFX_Text(pFX, _T("[High]"), m_High);
  RFX_Text(pFX, _T("[Low]"), m_Low);
  RFX_Text(pFX, _T("[Close]"), m_Close);
  RFX_Text(pFX, _T("[Volume]"), m_Volume);
  RFX_Text(pFX, _T("[Amount]"), m_Amount);
  RFX_Text(pFX, _T("[UpAndDown]"), m_UpAndDown);
  RFX_Text(pFX, _T("[UpDownRate]"), m_UpDownRate);
  RFX_Text(pFX, _T("[ChangeHandRate]"), m_ChangeHandRate);
  RFX_Text(pFX, _T("[TotalValue]"), m_TotalValue);
  RFX_Text(pFX, _T("[CurrentValue]"), m_CurrentValue);
  RFX_Text(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//��������г���ǿ��
  RFX_Text(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Text(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Text(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Text(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Text(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);
  RFX_Text(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);
  RFX_Text(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//��������������
  RFX_Text(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//��������������
  RFX_Text(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[UnknownVolume]"), m_UnknownVolume);
  RFX_Text(pFX, _T("[CancelBuyVolume]"), m_CancelBuyVolume);
  RFX_Text(pFX, _T("[CancelSellVolume]"), m_CancelSellVolume);
  RFX_Text(pFX, _T("[AttackBuyBelow50000]"), m_AttackBuyBelow50000);
  RFX_Text(pFX, _T("[AttackBuyBelow200000]"), m_AttackBuyBelow200000);
  RFX_Text(pFX, _T("[AttackBuyAbove200000]"), m_AttackBuyAbove200000);
  RFX_Text(pFX, _T("[AttackSellBelow50000]"), m_AttackSellBelow50000);
  RFX_Text(pFX, _T("[AttackSellBelow200000]"), m_AttackSellBelow200000);
  RFX_Text(pFX, _T("[AttackSellAbove200000]"), m_AttackSellAbove200000);
}
/////////////////////////////////////////////////////////////////////////////
// CSetDayLineToday ���

#ifdef _DEBUG
void CSetDayLineToday::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetDayLineToday::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG