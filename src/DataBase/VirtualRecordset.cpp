// CVirtualRecordset.cpp : CVirtualRecordset ���ʵ��

#include"pch.h"

#include "VirtualRecordset.h"

IMPLEMENT_DYNAMIC(CVirtualRecordset, CRecordset)

CString  CVirtualRecordset::GetSchemaConnect() {
	if (gl_systemStatus.IsWorkingMode()) {
		return _T("DSN=") + m_Schema + _T(";UID=") + gl_systemConfiguration.GetDatabaseAccountName() + _T(";PASSWORD=") +
			gl_systemConfiguration.GetDatabaseAccountPassword() + _T(";charset=utf8mb4"); // ����ʱ��DSNʹ��ԭschema����
	}
	else {
		return _T("DSN=") + m_Schema + _T("Test;UID=Test;PASSWORD=test;charset=utf8mb4"); // Test����ʱDSN���ƺ�Ҫ���Ϻ�׺Test
	}
}

CString CVirtualRecordset::GetDefaultConnect() {
	ASSERT(m_Schema.GetLength() > 0);
	return GetSchemaConnect();
}

CString CVirtualRecordset::GetDefaultSQL() {
	ASSERT(m_Table.GetLength() > 0);
	return _T("[") + m_Table + _T("]"); // SQL����ʽΪ: [������]
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualRecordset ���

#ifdef _DEBUG
void CVirtualRecordset::AssertValid() const {
	CRecordset::AssertValid();
}

void CVirtualRecordset::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG