// SetDayLine.h : CSetDayLine 类的实现



// CSetDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"

#include "SetDayLine.h"
IMPLEMENT_DYNAMIC(CSetDayLine, CRecordset)

CSetDayLine::CSetDayLine(CDatabase* pdb)
	: CRecordset(pdb)
{
  m_ID = 0;
	m_Time = 0;
	m_Market = 0;
	m_StockCode = "";
  m_StockName = "";
	m_LastClose = 0;
	m_Open = 0;
	m_High = 0;
	m_Low = 0;
	m_Close = 0;
	m_Volume = 0;
	m_Amount = 0;
	m_UpAndDown = 0;
  m_UpDownRate = 0;
  m_ChangeHandRate = 0;
	m_TotalValue = 0.0;
	m_CurrentValue = 0.0;
  m_RelativeStrong = 0;	//相对于总市场的强度
  m_TransactionNumber = 0;
  m_TransactionNumberBelow5000 = 0;
  m_TransactionNumberBelow50000 = 0;
  m_TransactionNumberBelow200000 = 0;
  m_TransactionNumberAbove200000 = 0;
  m_AttackBuyVolume = 0;	//攻击性买盘数量
  m_AttackSellVolume = 0;	//攻击性买盘数量
  m_StrongBuyVolume = 0;	//强攻击性买盘数量
  m_StrongSellVolume = 0;	//强攻击性买盘数量
  m_UnknownVolume = 0;	//未知买卖盘数量
  m_CancelBuyVolume = 0;	//买单撤单数量
  m_CancelSellVolume = 0;	//卖单撤单数量
  m_OrdinaryBuyVolume = 0;
  m_AttackBuyBelow50000 = 0;
  m_AttackBuyBelow200000 = 0;
  m_AttackBuyAbove200000 = 0;
  m_OrdinarySellVolume = 0;
  m_AttackSellBelow50000 = 0;
  m_AttackSellBelow200000 = 0;
  m_AttackSellAbove200000 = 0;
	m_nFields = 38;
}

CString CSetDayLine::GetDefaultConnect()
{
  if (gl_fTestMode) {
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8");
}
  else {
    return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
  }
}

CString CSetDayLine::GetDefaultSQL()
{
	return _T("[DayLine]");
}

void CSetDayLine::DoFieldExchange(CFieldExchange* pFX)
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
  RFX_Double(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Double(pFX, _T("[Open]"), m_Open);
  RFX_Double(pFX, _T("[High]"), m_High);
  RFX_Double(pFX, _T("[Low]"), m_Low);
  RFX_Double(pFX, _T("[Close]"), m_Close);
  RFX_Double(pFX, _T("[Volume]"), m_Volume);
  RFX_Double(pFX, _T("[Amount]"), m_Amount);
  RFX_Double(pFX, _T("[UpAndDown]"), m_UpAndDown);
  RFX_Double(pFX, _T("[UpDownRate]"), m_UpDownRate);
  RFX_Double(pFX, _T("[ChangeHandRate]"), m_ChangeHandRate);
  RFX_Double(pFX, _T("[TotalValue]"), m_TotalValue);
  RFX_Double(pFX, _T("[CurrentValue]"), m_CurrentValue);
  RFX_Double(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//相对于总市场的强度
  RFX_Long(pFX, _T("[TransactionNumber]"), m_TransactionNumber);
  RFX_Long(pFX, _T("[TransactionNumberBelow5000]"), m_TransactionNumberBelow5000);
  RFX_Long(pFX, _T("[TransactionNumberBelow50000]"), m_TransactionNumberBelow50000);
  RFX_Long(pFX, _T("[TransactionNumberBelow200000]"), m_TransactionNumberBelow200000);
  RFX_Long(pFX, _T("[TransactionNumberAbove200000]"), m_TransactionNumberAbove200000);
  RFX_Double(pFX, _T("[OrdinaryBuyVolume]"), m_OrdinaryBuyVolume);
  RFX_Double(pFX, _T("[OrdinarySellVolume]"), m_OrdinarySellVolume);
  RFX_Double(pFX, _T("[AttackBuyVolume]"), m_AttackBuyVolume);	//攻击性买盘数量
  RFX_Double(pFX, _T("[AttackSellVolume]"), m_AttackSellVolume);	//攻击性买盘数量
  RFX_Double(pFX, _T("[StrongBuyVolume]"), m_StrongBuyVolume);	//强攻击性买盘数量
  RFX_Double(pFX, _T("[StrongSellVolume]"), m_StrongSellVolume);	//强攻击性买盘数量
  RFX_Double(pFX, _T("[UnknownVolume]"), m_UnknownVolume);
  RFX_Double(pFX, _T("[CancelBuyVolume]"), m_CancelBuyVolume);
  RFX_Double(pFX, _T("[CancelSellVolume]"), m_CancelSellVolume);
  RFX_Double(pFX, _T("[AttackBuyBelow50000]"), m_AttackBuyBelow50000);
  RFX_Double(pFX, _T("[AttackBuyBelow200000]"), m_AttackBuyBelow200000);
  RFX_Double(pFX, _T("[AttackBuyAbove200000]"), m_AttackBuyAbove200000);
  RFX_Double(pFX, _T("[AttackSellBelow50000]"), m_AttackSellBelow50000);
  RFX_Double(pFX, _T("[AttackSellBelow200000]"), m_AttackSellBelow200000);
  RFX_Double(pFX, _T("[AttackSellAbove200000]"), m_AttackSellAbove200000);

}
/////////////////////////////////////////////////////////////////////////////
// CSetDayLine 诊断

#ifdef _DEBUG
void CSetDayLine::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetDayLine::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


