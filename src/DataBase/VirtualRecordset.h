// VirtualRecordset.h : CVirtualRecordset 的声明
//
// 本系统使用的所有数据库类的默认基类，必须使用此基类。使用预置的schema和table,自动连接默认数据库。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"afxdb.h"

class CVirtualRecordset : public CRecordset
{
public:
	// 本系统使用的数据库类，必须提供Schema和Table两个参数。
	CVirtualRecordset(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr)
		: CRecordset(pDatabase) {
		ASSERT((strSchema.Compare(_T("WorldMarket")) == 0) || (strSchema.Compare(_T("ChinaMarket")) == 0)); // 目前使用的schema只有此两个
		ASSERT(strTable.GetLength() > 0); // 必须传入数据表名称。
		m_Schema = strSchema;
		m_Table = strTable;
	}
	DECLARE_DYNAMIC(CVirtualRecordset)

	CString GetSchemaConnect(void);// 用于数据集的GetDefaultConnect()，以保持一致性。参数strSchema为该市场的名称

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
