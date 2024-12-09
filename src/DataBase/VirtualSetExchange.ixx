// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢���������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.Exchange;

import FireBird.Set.VirtualRecordset;

export {
	class CVirtualSetExchange : public CVirtualRecordset {
	public:
		CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };
		CString m_Code{ _T("") };
		CString m_Name{ _T("") };
		CString m_Mic{ _T("") };
		CString m_TimeZone{ _T("") };
		CString m_PreMarket{ _T("") };
		CString m_Hour{ _T("") };
		CString m_PostMarket{ _T("") };
		CString m_CloseDate{ _T("") };
		CString m_Country{ _T("") };
		CString m_CountryName{ _T("") };
		CString m_Source{ _T("") };
		CString m_MyUnknownColumn{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
