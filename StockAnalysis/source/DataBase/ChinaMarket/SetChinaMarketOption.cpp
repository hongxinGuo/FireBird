#include"pch.h"

#include "SetChinaMarketOption.h"

IMPLEMENT_DYNAMIC(CSetChinaMarketOption, CVirtualRecordset)

CSetChinaMarketOption::CSetChinaMarketOption(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_RTDataServerIndex = 0; // 默认使用新浪实时数据服务器
	m_RTDataInquiryTime = 400; // 默认查询时间为400毫秒
	m_nFields = 3;
}

void CSetChinaMarketOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[RealTimeDataServer]"), m_RTDataServerIndex);
	RFX_Long(pFX, _T("[RealTimeDataInquiryTime]"), m_RTDataInquiryTime);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChinaMarketOption 诊断

#ifdef _DEBUG
void CSetChinaMarketOption::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetChinaMarketOption::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG