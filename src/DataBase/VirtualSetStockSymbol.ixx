// VirtualSetExchange.h : CVirtualSetExchange ������
//
// �洢����֤ȯ�������ݵ�Ĭ����ʽ
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.Set.VirtualStockSymbol;

import FireBird.Set.VirtualRecordset;
export import FireBird.System.ConstantChinaMarket;

export {
	class CVirtualSetStockSymbol : public CVirtualRecordset {
	public:
		CVirtualSetStockSymbol(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };
		CString m_Description{ _T("") };
		CString m_Exchange{ _T("") };
		CString m_Symbol{ _T("") };
		CStringW m_DisplaySymbol{ _T("") }; // �������й���Ʊʱ���洢��ʱ��Ʊ���������ƣ��ʶ���Ҫʹ��CStringW��ʽ����ֹ���ַǷ��ַ���
		long m_IPOStatus{ _STOCK_NOT_CHECKED_ };
		CString m_UpdateDate{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
