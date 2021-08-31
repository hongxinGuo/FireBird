// CVirtualRecordset.cpp : CVirtualRecordset 类的实现

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "VirtualRecordset.h"

IMPLEMENT_DYNAMIC(CVirtualRecordset, CRecordset)

CString CVirtualRecordset::GetDefaultConnect() {
	return GetSchemaConnect(m_Schema);
}

CString CVirtualRecordset::GetDefaultSQL() {
	return _T("[") + m_Table + _T("]");
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