//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///Note 目前使用MFC的CRecordset来执行数据库操作。由于CRecordset与CString的紧密联系，故而所有的数据库操作皆使用CString，不进行更换string的努力。
///
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "VirtualRecordset.h"

CVirtualRecordset::~CVirtualRecordset() {
}

CString CVirtualRecordset::GetSchemaConnect() const {
	if (gl_systemConfiguration.IsWorkingMode()) { // 工作时使用系统配置文件中的用户名和密码
		return "DSN=" + m_Schema + ";UID=" + gl_systemConfiguration.GetDatabaseAccountName().c_str() + ";PASSWORD=" +
		gl_systemConfiguration.GetDatabaseAccountPassword().c_str() + ";charset=utf8mb4"; // 运行时的DSN使用原schema名称
	}
	// 测试时使用固定的用户名Test和密码test。
	return "DSN=" + m_Schema + "Test;UID=Test;PASSWORD=test;charset=utf8mb4"; // Test操作时DSN名称后要加上后缀Test
}

CString CVirtualRecordset::GetDefaultConnect() {
	ASSERT(m_Schema.GetLength() > 0);
	return GetSchemaConnect();
}

CString CVirtualRecordset::GetDefaultSQL() {
	ASSERT(m_Table.GetLength() > 0);
	return "[" + m_Table + "]"; // SQL的制式为: [表名称]
}
