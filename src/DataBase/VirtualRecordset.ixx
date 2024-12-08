// VirtualRecordset.h : CVirtualRecordset ������
//
// ��ϵͳʹ�õ��������ݿ����Ĭ�ϻ��࣬����ʹ�ô˻��ࡣʹ��Ԥ�õ�schema��table,�Զ�����Ĭ�����ݿ⡣
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include"afxdb.h"
export module FireBird.Set.VirtualRecordset;

export {
	class CVirtualRecordset : public CRecordset {
	public:
		// ��ϵͳʹ�õ����ݿ��࣬�����ṩSchema��Table����������
		CVirtualRecordset(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr)
			: CRecordset(pDatabase) {
			ASSERT((strSchema.Compare(_T("WorldMarket")) == 0) || (strSchema.Compare(_T("ChinaMarket")) == 0)); // Ŀǰʹ�õ�schemaֻ�д�����
			ASSERT(strTable.GetLength() > 0); // ���봫�����ݱ����ơ�
			m_Schema = strSchema;
			m_Table = strTable;
		}

		~CVirtualRecordset() override = default;

		CString GetSchemaConnect() const;// �������ݼ���GetDefaultConnect()���Ա���һ���ԡ�����strSchemaΪ���г�������

		CString m_Schema{ _T("") }; // ���ݿ�����
		CString m_Table{ _T("") }; // ���ݿ���Ĭ�ϵı�����

	public:
		CString GetDefaultConnect() override;	// Ĭ�������ַ���
		CString GetDefaultSQL() override; 	// ��¼����Ĭ��SQL
	};
}
