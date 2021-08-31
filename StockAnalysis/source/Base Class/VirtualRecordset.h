// VirtualRecordset.h : CVirtualRecordset 的声明
//
// 本系统使用的默认数据库类的基类
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"afxdb.h"

class CVirtualRecordset : public CRecordset
{
public:
	CVirtualRecordset(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr)
		: CRecordset(pDatabase) {
		m_Schema = strSchema;
		m_Table = strTable;
	}
	DECLARE_DYNAMIC(CVirtualRecordset)

	CString m_Schema; // 数据库名称
	CString m_Table; // 数据库中默认的表名称

public:
	virtual CString GetDefaultConnect();	// 默认连接字符串
	virtual CString GetDefaultSQL(); 	// 记录集的默认SQL

// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
