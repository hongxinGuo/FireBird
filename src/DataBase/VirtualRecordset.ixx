// VirtualRecordset.h : CVirtualRecordset 的声明
//
// 本系统使用的所有数据库类的默认基类，必须使用此基类。使用预置的schema和table,自动连接默认数据库。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include"afxdb.h"
export module FireBird.Set.VirtualRecordset;

export {
	class CVirtualRecordset : public CRecordset {
	public:
		// 本系统使用的数据库类，必须提供Schema和Table两个参数。
		CVirtualRecordset(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr)
			: CRecordset(pDatabase) {
			ASSERT((strSchema.Compare(_T("WorldMarket")) == 0) || (strSchema.Compare(_T("ChinaMarket")) == 0)); // 目前使用的schema只有此两个
			ASSERT(strTable.GetLength() > 0); // 必须传入数据表名称。
			m_Schema = strSchema;
			m_Table = strTable;
		}

		~CVirtualRecordset() override = default;

		CString GetSchemaConnect() const;// 用于数据集的GetDefaultConnect()，以保持一致性。参数strSchema为该市场的名称

		CString m_Schema{ _T("") }; // 数据库名称
		CString m_Table{ _T("") }; // 数据库中默认的表名称

	public:
		CString GetDefaultConnect() override;	// 默认连接字符串
		CString GetDefaultSQL() override; 	// 记录集的默认SQL
	};
}
