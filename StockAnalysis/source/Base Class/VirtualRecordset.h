// VirtualRecordset.h : CVirtualRecordset ������
//
// ��ϵͳʹ�õ�Ĭ�����ݿ���Ļ���
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"afxdb.h"

class CVirtualRecordset : public CRecordset
{
public:
	// ��ϵͳʹ�õ����ݿ��࣬�����ṩSchema��Table����������
	CVirtualRecordset(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr)
		: CRecordset(pDatabase) {
		ASSERT((strSchema.Compare(_T("WorldMarket")) == 0) || (strSchema.Compare(_T("ChinaMarket")) == 0));
		ASSERT(strTable.GetLength() > 0);
		m_Schema = strSchema;
		m_Table = strTable;
	}
	DECLARE_DYNAMIC(CVirtualRecordset)

	CString m_Schema; // ���ݿ�����
	CString m_Table; // ���ݿ���Ĭ�ϵı�����

public:
	virtual CString GetDefaultConnect();	// Ĭ�������ַ���
	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ��SQL

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
