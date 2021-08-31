// CVirtualSetExchange.cpp : CVirtualSetExchange 类的实现

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "VirtualSetExchange.h"

CString CVirtualSetExchange::sm_TableName = _T("");

IMPLEMENT_DYNAMIC(CVirtualSetExchange, CRecordset)

CVirtualSetExchange::CVirtualSetExchange(CDatabase* pdb)
	: CRecordset(pdb) {
	m_Exchange = _T(" ");

	m_nFields = 2;
}

CString CVirtualSetExchange::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CVirtualSetExchange::GetDefaultSQL() {
	return sm_TableName;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock 诊断

#ifdef _DEBUG
void CVirtualSetExchange::AssertValid() const {
	CRecordset::AssertValid();
}

void CVirtualSetExchange::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG