// CSetTiingoFundamentalDefinition.h : CSetTiingoFundamentalDefinition ������

#include"pch.h"
export module FireBird.Set.TiingoFundamentalDefinition;
import FireBird.Set.VirtualRecordset;

export {
	class CSetTiingoFundamentalDefinition : public CVirtualRecordset {
	public:
		CSetTiingoFundamentalDefinition(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_fundamental_definitions"), CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };
		CString m_dataCode{ _T("") };
		CString m_name{ _T("") };
		CString m_description{ _T("") };
		CString m_statementType{ _T("") };
		CString m_units{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
