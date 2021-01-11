// CSetFinnhubExchanges.cpp : CSetFinnhubExchanges 类的实现

// CSetAmericaStakeDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubExchanges.h"

IMPLEMENT_DYNAMIC(CSetFinnhubExchanges, CRecordset)

CSetFinnhubExchanges::CSetFinnhubExchanges(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Code = _T("");
  m_Name = _T("");
  m_Mic = _T("");
  m_TimeZone = _T("");
  m_Hour = _T("");
  m_CloseDate = _T("");
  m_Country = _T("");
  m_Source = _T("");
  m_nFields = 9;
}

CString CSetFinnhubExchanges::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetFinnhubExchanges::GetDefaultSQL() {
  return _T("[forex_dayline]");
}

void CSetFinnhubExchanges::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Code]"), m_Code);
  RFX_Text(pFX, _T("[Name]"), m_Name);
  RFX_Text(pFX, _T("[Mic]"), m_Mic);
  RFX_Text(pFX, _T("[TimeZone]"), m_TimeZone);
  RFX_Text(pFX, _T("[Hour]"), m_Hour);
  RFX_Text(pFX, _T("[CloseDate]"), m_CloseDate);
  RFX_Text(pFX, _T("[Country]"), m_Country);
  RFX_Text(pFX, _T("[Source]"), m_Source);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaForexDayLine 诊断

#ifdef _DEBUG
void CSetFinnhubExchanges::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetFinnhubExchanges::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG