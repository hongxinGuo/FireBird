// CVirtualRecordset.cpp : CVirtualRecordset ���ʵ��

#include"pch.h"
#include"accessory.h"

#include "VirtualRecordset.h"

IMPLEMENT_DYNAMIC(CVirtualRecordset, CRecordset)

CString CVirtualRecordset::GetDefaultConnect() {
	ASSERT(m_Schema.GetLength() > 0);
	return GetSchemaConnect(m_Schema);
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