#include"pch.h"
export module FireBird.Set.TiingoMarketNews;

import FireBird.Set.VirtualRecordset;

export {
	class CSetTiingoMarketNews : public CVirtualRecordset {
	public:
		CSetTiingoMarketNews(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_market_news"), CDatabase* pDatabase = nullptr);

		// �ֶ�/��������

		// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
		// �������͵� CStringW)��ʵ���������͡�
		//  ����Ϊ��ֹ ODBC ��������ִ�п���
		// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
		// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
		// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
		// ��ͬʱ֧�� Unicode ����Щת��)��

		long m_ID{ 0 };
		long m_ids{ 0 };
		CString m_tickers{ _T("") };
		CString m_title{ _T("") };
		CString m_tag{ _T("") };
		CString m_source{ _T("") };
		CString m_description{ _T("") };
		CString m_url{ _T("") };
		CString m_crawlDate{ _T("") };
		CString m_publishedDate{ _T("") };

	public:
		void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
	};
}
