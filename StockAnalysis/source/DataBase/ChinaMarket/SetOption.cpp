#include"pch.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CVirtualRecordset)

CSetOption::CSetOption(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_RSStartDate = __CHINA_MARKET_BEGIN_DATE__;
	m_RSEndDate = __CHINA_MARKET_BEGIN_DATE__;
	m_LastLoginDate = __CHINA_MARKET_BEGIN_DATE__;
	m_LastLoginTime = 0;
	m_UpdatedDateFor10DaysRS1 = __CHINA_MARKET_BEGIN_DATE__;
	m_UpdatedDateFor10DaysRS2 = __CHINA_MARKET_BEGIN_DATE__;
	m_UpdatedDateFor10DaysRS = __CHINA_MARKET_BEGIN_DATE__;
	m_nFields = 7;
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[RelativeStrongStartDate]"), m_RSStartDate);
	RFX_Long(pFX, _T("[RelativeStrongEndDate]"), m_RSEndDate);
	RFX_Long(pFX, _T("[LastLoginDate]"), m_LastLoginDate);
	RFX_Long(pFX, _T("[LastLoginTime]"), m_LastLoginTime);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS1]"), m_UpdatedDateFor10DaysRS1);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS2]"), m_UpdatedDateFor10DaysRS2);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS]"), m_UpdatedDateFor10DaysRS);
}
/////////////////////////////////////////////////////////////////////////////
// CSetOption 诊断

#ifdef _DEBUG
void CSetOption::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetOption::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG