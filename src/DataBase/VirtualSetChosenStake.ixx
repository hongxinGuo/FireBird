//
// ��ѡ֤ȯ�����ݱ��ʽ
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.VirtualChosenStake;

import FireBird.Set.VirtualRecordset;

export {
	class CVirtualSetChosenStake : public CVirtualRecordset {
	public:
		CVirtualSetChosenStake(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };
		CString m_Symbol{ _T("") };

		// ��д
		// �����ɵ��麯����д
	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
