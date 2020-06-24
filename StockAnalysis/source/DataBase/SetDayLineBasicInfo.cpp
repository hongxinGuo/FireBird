// SetDayLineBasicInfo.h : CSetDayLineBasicInfo 类的实现

// CSetDayLineBasicInfo 实现

// 代码生成在 2019年6月2日, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetDayLineBasicInfo.h"
IMPLEMENT_DYNAMIC(CSetDayLineBasicInfo, CRecordset)

CSetDayLineBasicInfo::CSetDayLineBasicInfo(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Day = 0;
  m_Market = 0;
  m_StockCode = "";
  m_StockName = "";
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
  m_RelativeStrong = _T("");	//相对于总市场的强度
  m_RelativeStrongIndex = _T("");	//相对于总市场的强度
  m_nFields = 20;
}

CString CSetDayLineBasicInfo::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetDayLineBasicInfo::GetDefaultSQL() {
  return _T("[DayLine]");
}

void CSetDayLineBasicInfo::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Day]"), m_Day);
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
  RFX_Text(pFX, _T("[RelativeStrongIndex]"), m_RelativeStrongIndex);	//相对于市场指数的强度
  RFX_Text(pFX, _T("[RelativeStrongBackup]"), m_RelativeStrongBackup);	//相对于市场指数的强度
}
/////////////////////////////////////////////////////////////////////////////
// CSetDayLineBasicInfo 诊断

#ifdef _DEBUG
void CSetDayLineBasicInfo::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetDayLineBasicInfo::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG