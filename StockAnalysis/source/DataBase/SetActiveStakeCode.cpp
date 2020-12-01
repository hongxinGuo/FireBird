// SetActiveStakeCode.cpp : CSetActiveStakeCode 类的实现

// CSetActiveStakeCode 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include "SetActiveStakeCode.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetActiveStakeCode, CRecordset)

CSetActiveStakeCode::CSetActiveStakeCode(CDatabase* pdb)
  : CRecordset(pdb) {
  m_StakeType = 0;
  m_StakeCode = _T("");
  m_StakeName = _T("");
  m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
  m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
  m_IPOStatus = 0;
  m_nFields = 7;
}

CString CSetActiveStakeCode::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetActiveStakeCode::GetDefaultSQL() {
  return _T("[StakeCode]");
}

void CSetActiveStakeCode::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Int(pFX, _T("[StakeType]"), m_StakeType);
  RFX_Text(pFX, _T("[StakeCode]"), m_StakeCode);
  RFX_Text(pFX, _T("[StakeName]"), m_StakeName);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
  RFX_Long(pFX, _T("[IPOed]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetActiveStakeCode 诊断

#ifdef _DEBUG
void CSetActiveStakeCode::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetActiveStakeCode::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG