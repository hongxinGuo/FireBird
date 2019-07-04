// SetDayLineInfo.h : CSetDayLine 类的实现



// CSetDayLineInfo 实现

// 代码生成在 2019年6月2日, 5:10

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
  m_RelativeStrong = 0;	//相对于总市场的强度
  m_TransactionNumber = 0;
  m_TransactionNumberBelow5000 = 0; // 本日的盘口变化数
  m_TransactionNumberBelow50000 = 0; // 本日的盘口变化数
  m_TransactionNumberBelow200000 = 0; // 本日的盘口变化数
  m_TransactionNumberAbove200000 = 0; // 本日的盘口变化数
  m_AttackBuyVolume = 0;	//攻击性买盘数量
  m_AttackSellVolume = 0;	//攻击性买盘数量
  m_StrongBuyVolume = 0;	//强攻击性买盘数量
  m_StrongSellVolume = 0;	//强攻击性买盘数量
  m_OrdinaryBuyVolume = 0;
  m_OrdinarySellVolume = 0;
  m_UnknownVolume = 0;	//未知买卖盘数量
  m_CancelBuyVolume = 0;	//买单撤单数量
  m_CancelSellVolume = 0;	//卖单撤单数量
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
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Time]"), m_Time);
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Double(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//相对于总市场的强度
  RFX_Long(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Long(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Long(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Long(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Long(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Long(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//攻击性买盘数量
  RFX_Long(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//攻击性买盘数量
  RFX_Long(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//强攻击性买盘数量
  RFX_Long(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//强攻击性买盘数量
  RFX_Long(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);	//强攻击性买盘数量
  RFX_Long(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);	//强攻击性买盘数量
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
// CSetDayLine 诊断

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

