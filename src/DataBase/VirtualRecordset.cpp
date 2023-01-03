// CVirtualRecordset.cpp : CVirtualRecordset 类的实现

#include"pch.h"

#include "VirtualRecordset.h"

IMPLEMENT_DYNAMIC(CVirtualRecordset, CRecordset)

CString  CVirtualRecordset::GetSchemaConnect() {
	if (gl_systemStatus.IsWorkingMode()) {
		return _T("DSN=") + m_Schema + _T(";UID=") + gl_systemConfiguration.GetDatabaseAccountName() + _T(";PASSWORD=") +
			gl_systemConfiguration.GetDatabaseAccountPassword() + _T(";charset=utf8mb4"); // 运行时的DSN使用原schema名称
	}
	else {
		return _T("DSN=") + m_Schema + _T("Test;UID=Test;PASSWORD=test;charset=utf8mb4"); // Test操作时DSN名称后要加上后缀Test
	}
}

CString CVirtualRecordset::GetDefaultConnect() {
	ASSERT(m_Schema.GetLength() > 0);
	return GetSchemaConnect();
}

CString CVirtualRecordset::GetDefaultSQL() {
	ASSERT(m_Table.GetLength() > 0);
	return _T("[") + m_Table + _T("]"); // SQL的制式为: [表名称]
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualRecordset 诊断

#ifdef _DEBUG
void CVirtualRecordset::AssertValid() const {
	CRecordset::AssertValid();
}

void CVirtualRecordset::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG