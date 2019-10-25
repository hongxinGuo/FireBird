// SetDayLineToday.h : CSetDayLineToday 类的实现

// CSetDayLineToday 实现

// 代码生成在 2019年6月2日, 5:10

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
  m_RelativeStrong = _T("");	//相对于总市场的强度
  m_TransactionNumber = _T("");
  m_TransactionNumberBelow5000 = _T("");
  m_TransactionNumberBelow50000 = _T("");
  m_TransactionNumberBelow200000 = _T("");
  m_TransactionNumberAbove200000 = _T("");
  m_AttackBuyVolume = _T("");	//攻击性买盘数量
  m_AttackSellVolume = _T("");	//攻击性买盘数量
  m_StrongBuyVolume = _T("");	//强攻击性买盘数量
  m_StrongSellVolume = _T("");	//强攻击性买盘数量
  m_UnknownVolume = _T("");	//未知买卖盘数量
  m_CancelBuyVolume = _T("");	//买单撤单数量
  m_CancelSellVolume = _T("");	//卖单撤单数量
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
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
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
  RFX_Text(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//相对于总市场的强度
  RFX_Text(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Text(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Text(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Text(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Text(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Text(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);
  RFX_Text(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);
  RFX_Text(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//攻击性买盘数量
  RFX_Text(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//攻击性买盘数量
  RFX_Text(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//强攻击性买盘数量
  RFX_Text(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//强攻击性买盘数量
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
// CSetDayLineToday 诊断

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