// CSetTiingoStock52WeekLow.h : CSetTiingoStock52WeekLow ������
#include"pch.h"
export module FireBird.Set.TiingoStock52WeekLow;

import FireBird.Set.VirtualRecordset;

export {
	class CSetTiingoStock52WeekLow : public CVirtualRecordset {
	public:
		CSetTiingoStock52WeekLow(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_52week_low"), CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };  // �����ã��Զ����ɡ�
		CString m_Symbol{ _T("") };
		CString m_Exchange{ _T("") };
		long m_Date{ 0 };

		// ��д
		// �����ɵ��麯����д
	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
