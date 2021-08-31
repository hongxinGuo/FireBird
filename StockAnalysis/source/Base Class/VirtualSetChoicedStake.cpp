// VirtualSetChoicedStake.h : VirtualSetChoicedStake 类的实现

// VirtualSetChoicedStake 实现

#include"pch.h"
#include"globedef.h"
#include"Accessory.h"

#include "VirtualSetChoicedStake.h"

CString CVirtualSetChoicedStake::sm_TableName = _T("");

IMPLEMENT_DYNAMIC(CVirtualSetChoicedStake, CRecordset)

CVirtualSetChoicedStake::CVirtualSetChoicedStake(CDatabase* pdb) : CRecordset(pdb) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

CString CVirtualSetChoicedStake::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CVirtualSetChoicedStake::GetDefaultSQL() {
	return _T("[") + sm_TableName + _T("]");
}

void CVirtualSetChoicedStake::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CVirtualSetChoicedStake 诊断

#ifdef _DEBUG
void CVirtualSetChoicedStake::AssertValid() const {
	CRecordset::AssertValid();
}

void CVirtualSetChoicedStake::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG