//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Ŀǰʹ��MFC��CRecordset��ִ�����ݿ����������CRecordset��CString�Ľ�����ϵ���ʶ����е����ݿ������ʹ��CString�������и���string��Ŭ����
///
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "VirtualRecordset.h"

CString CVirtualRecordset::GetSchemaConnect() const {
	if (gl_systemConfiguration.IsWorkingMode()) { // ����ʱʹ��ϵͳ�����ļ��е��û���������
		return _T("DSN=") + m_Schema + _T(";UID=") + gl_systemConfiguration.GetDatabaseAccountName() + _T(";PASSWORD=") +
		gl_systemConfiguration.GetDatabaseAccountPassword() + _T(";charset=utf8mb4"); // ����ʱ��DSNʹ��ԭschema����
	}
	// ����ʱʹ�ù̶����û���Test������test��
	return _T("DSN=") + m_Schema + _T("Test;UID=Test;PASSWORD=test;charset=utf8mb4"); // Test����ʱDSN���ƺ�Ҫ���Ϻ�׺Test
}

CString CVirtualRecordset::GetDefaultConnect() {
	ASSERT(m_Schema.GetLength() > 0);
	return GetSchemaConnect();
}

CString CVirtualRecordset::GetDefaultSQL() {
	ASSERT(m_Table.GetLength() > 0);
	return _T("[") + m_Table + _T("]"); // SQL����ʽΪ: [������]
}
