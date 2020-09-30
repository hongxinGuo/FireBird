// SetWeekLineInfo.h : CSetWeekLineInfo ���ʵ��

// CSetWeekLineInfo ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetWeekLineInfo.h"
IMPLEMENT_DYNAMIC(CSetWeekLineInfo, CRecordset)

CSetWeekLineInfo::CSetWeekLineInfo(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Date = 0;
  m_Market = 0;
  m_StockCode = _T("");
  m_StockName = _T("");
  m_LastClose = _T("0");
  m_Open = _T("0");
  m_High = _T("0");
  m_Low = _T("0");
  m_Close = _T("0");
  m_Volume = _T("0");
  m_Amount = _T("0");
  m_UpAndDown = _T("0");
  m_UpDownRate = _T("0");
  m_ChangeHandRate = _T("0");
  m_TotalValue = _T("0");
  m_CurrentValue = _T("0");
  m_RS = _T("");	//��������г���ǿ��
  m_RSIndex = _T("");	//��������г���ǿ��

  m_TransactionNumber = _T("0");
  m_TransactionNumberBelow5000 = _T("0"); // ���յ��̿ڱ仯��
  m_TransactionNumberBelow50000 = _T("0"); // ���յ��̿ڱ仯��
  m_TransactionNumberBelow200000 = _T("0"); // ���յ��̿ڱ仯��
  m_TransactionNumberAbove200000 = _T("0"); // ���յ��̿ڱ仯��
  m_AttackBuyVolume = _T("0");	//��������������
  m_AttackSellVolume = _T("0");	//��������������
  m_StrongBuyVolume = _T("0");	//ǿ��������������
  m_StrongSellVolume = _T("0");	//ǿ��������������
  m_OrdinaryBuyVolume = _T("0");
  m_OrdinarySellVolume = _T("0");
  m_UnknownVolume = _T("0");	//δ֪����������
  m_CanceledBuyVolume = _T("0");	//�򵥳�������
  m_CanceledSellVolume = _T("0");	//������������
  m_AttackBuyBelow50000 = _T("0");
  m_AttackBuyBelow200000 = _T("0");
  m_AttackBuyAbove200000 = _T("0");
  m_AttackSellBelow50000 = _T("0");
  m_AttackSellBelow200000 = _T("0");
  m_AttackSellAbove200000 = _T("0");
  m_OrdinaryBuyVolumeBelow5000 = _T("");
  m_OrdinaryBuyVolumeBelow10000 = _T("");
  m_OrdinaryBuyVolumeBelow20000 = _T("");
  m_OrdinaryBuyVolumeBelow50000 = _T("");
  m_OrdinaryBuyVolumeBelow100000 = _T("");
  m_OrdinaryBuyVolumeBelow200000 = _T("");
  m_OrdinaryBuyVolumeAbove200000 = _T("");
  m_OrdinaryBuyVolumeBelow5000 = _T("");
  m_OrdinaryBuyVolumeBelow10000 = _T("");
  m_OrdinaryBuyVolumeBelow20000 = _T("");
  m_OrdinaryBuyVolumeBelow50000 = _T("");
  m_OrdinaryBuyVolumeBelow100000 = _T("");
  m_OrdinaryBuyVolumeBelow200000 = _T("");
  m_OrdinaryBuyVolumeAbove200000 = _T("");
  m_OrdinarySellVolumeBelow5000 = _T("");
  m_OrdinarySellVolumeBelow10000 = _T("");
  m_OrdinarySellVolumeBelow20000 = _T("");
  m_OrdinarySellVolumeBelow50000 = _T("");
  m_OrdinarySellVolumeBelow100000 = _T("");
  m_OrdinarySellVolumeBelow200000 = _T("");
  m_OrdinarySellVolumeAbove200000 = _T("");
  m_OrdinaryBuyNumberBelow5000 = _T("");
  m_OrdinaryBuyNumberBelow10000 = _T("");
  m_OrdinaryBuyNumberBelow20000 = _T("");
  m_OrdinaryBuyNumberBelow50000 = _T("");
  m_OrdinaryBuyNumberBelow100000 = _T("");
  m_OrdinaryBuyNumberBelow200000 = _T("");
  m_OrdinaryBuyNumberAbove200000 = _T("");
  m_OrdinarySellNumberBelow5000 = _T("");
  m_OrdinarySellNumberBelow10000 = _T("");
  m_OrdinarySellNumberBelow20000 = _T("");
  m_OrdinarySellNumberBelow50000 = _T("");
  m_OrdinarySellNumberBelow100000 = _T("");
  m_OrdinarySellNumberBelow200000 = _T("");
  m_OrdinarySellNumberAbove200000 = _T("");

  m_CanceledBuyVolumeBelow5000 = _T("");
  m_CanceledBuyVolumeBelow10000 = _T("");
  m_CanceledBuyVolumeBelow20000 = _T("");
  m_CanceledBuyVolumeBelow50000 = _T("");
  m_CanceledBuyVolumeBelow100000 = _T("");
  m_CanceledBuyVolumeBelow200000 = _T("");
  m_CanceledBuyVolumeAbove200000 = _T("");
  m_CanceledSellVolumeBelow5000 = _T("");
  m_CanceledSellVolumeBelow10000 = _T("");
  m_CanceledSellVolumeBelow20000 = _T("");
  m_CanceledSellVolumeBelow50000 = _T("");
  m_CanceledSellVolumeBelow100000 = _T("");
  m_CanceledSellVolumeBelow200000 = _T("");
  m_CanceledSellVolumeAbove200000 = _T("");

  m_nFields = 82;
}

CString CSetWeekLineInfo::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetWeekLineInfo::GetDefaultSQL() {
  return _T("[CurrentWeekLine]");
}

void CSetWeekLineInfo::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Date]"), m_Date);
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
  RFX_Text(pFX, _T("[RelativeStrong]"), m_RS);	//��������г���ǿ��
  RFX_Text(pFX, _T("[RelativeStrongIndex]"), m_RSIndex);	//������г�ָ����ǿ��
  RFX_Text(pFX, _T("[RelativeStrongBackup]"), m_RSBackup);	//������г�ָ����ǿ��
  RFX_Text(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Text(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Text(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Text(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Text(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Text(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//��������������
  RFX_Text(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//��������������
  RFX_Text(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);	//ǿ��������������
  RFX_Text(pFX, _T("[UnknownVolume]"), m_UnknownVolume);
  RFX_Text(pFX, _T("[CancelBuyVolume]"), m_CanceledBuyVolume);
  RFX_Text(pFX, _T("[CancelSellVolume]"), m_CanceledSellVolume);
  RFX_Text(pFX, _T("[AttackBuyBelow50000]"), m_AttackBuyBelow50000);
  RFX_Text(pFX, _T("[AttackBuyBelow200000]"), m_AttackBuyBelow200000);
  RFX_Text(pFX, _T("[AttackBuyAbove200000]"), m_AttackBuyAbove200000);
  RFX_Text(pFX, _T("[AttackSellBelow50000]"), m_AttackSellBelow50000);
  RFX_Text(pFX, _T("[AttackSellBelow200000]"), m_AttackSellBelow200000);
  RFX_Text(pFX, _T("[AttackSellAbove200000]"), m_AttackSellAbove200000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow5000]"), m_OrdinaryBuyVolumeBelow5000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow10000]"), m_OrdinaryBuyVolumeBelow10000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow20000]"), m_OrdinaryBuyVolumeBelow20000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow50000]"), m_OrdinaryBuyVolumeBelow50000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow100000]"), m_OrdinaryBuyVolumeBelow100000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeBelow200000]"), m_OrdinaryBuyVolumeBelow200000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolumeAbove200000]"), m_OrdinaryBuyVolumeAbove200000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow5000]"), m_OrdinaryBuyNumberBelow5000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow10000]"), m_OrdinaryBuyNumberBelow10000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow20000]"), m_OrdinaryBuyNumberBelow20000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow50000]"), m_OrdinaryBuyNumberBelow50000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow100000]"), m_OrdinaryBuyNumberBelow100000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberBelow200000]"), m_OrdinaryBuyNumberBelow200000);
  RFX_Text(pFX, _T("[OrdinaryBuyNumberAbove200000]"), m_OrdinaryBuyNumberAbove200000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow5000]"), m_OrdinarySellVolumeBelow5000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow10000]"), m_OrdinarySellVolumeBelow10000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow20000]"), m_OrdinarySellVolumeBelow20000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow50000]"), m_OrdinarySellVolumeBelow50000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow100000]"), m_OrdinarySellVolumeBelow100000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeBelow200000]"), m_OrdinarySellVolumeBelow200000);
  RFX_Text(pFX, _T("[OrdinarySellVolumeAbove200000]"), m_OrdinarySellVolumeAbove200000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow5000]"), m_OrdinarySellNumberBelow5000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow10000]"), m_OrdinarySellNumberBelow10000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow20000]"), m_OrdinarySellNumberBelow20000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow50000]"), m_OrdinarySellNumberBelow50000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow100000]"), m_OrdinarySellNumberBelow100000);
  RFX_Text(pFX, _T("[OrdinarySellNumberBelow200000]"), m_OrdinarySellNumberBelow200000);
  RFX_Text(pFX, _T("[OrdinarySellNumberAbove200000]"), m_OrdinarySellNumberAbove200000);

  RFX_Text(pFX, _T("[CanceledBuyBelow5000]"), m_CanceledBuyVolumeBelow5000);
  RFX_Text(pFX, _T("[CanceledBuyBelow10000]"), m_CanceledBuyVolumeBelow10000);
  RFX_Text(pFX, _T("[CanceledBuyBelow20000]"), m_CanceledBuyVolumeBelow20000);
  RFX_Text(pFX, _T("[CanceledBuyBelow50000]"), m_CanceledBuyVolumeBelow50000);
  RFX_Text(pFX, _T("[CanceledBuyBelow100000]"), m_CanceledBuyVolumeBelow100000);
  RFX_Text(pFX, _T("[CanceledBuyBelow200000]"), m_CanceledBuyVolumeBelow200000);
  RFX_Text(pFX, _T("[CanceledBuyAbove200000]"), m_CanceledBuyVolumeAbove200000);
  RFX_Text(pFX, _T("[CanceledSellBelow5000]"), m_CanceledSellVolumeBelow5000);
  RFX_Text(pFX, _T("[CanceledSellBelow10000]"), m_CanceledSellVolumeBelow10000);
  RFX_Text(pFX, _T("[CanceledSellBelow20000]"), m_CanceledSellVolumeBelow20000);
  RFX_Text(pFX, _T("[CanceledSellBelow50000]"), m_CanceledSellVolumeBelow50000);
  RFX_Text(pFX, _T("[CanceledSellBelow100000]"), m_CanceledSellVolumeBelow100000);
  RFX_Text(pFX, _T("[CanceledSellBelow200000]"), m_CanceledSellVolumeBelow200000);
  RFX_Text(pFX, _T("[CanceledSellAbove200000]"), m_CanceledSellVolumeAbove200000);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWeekLineInfo ���

#ifdef _DEBUG
void CSetWeekLineInfo::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetWeekLineInfo::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG