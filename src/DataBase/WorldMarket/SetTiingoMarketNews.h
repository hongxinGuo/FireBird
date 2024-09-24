// CSetTiingoMarketNews.h : CSetTiingoMarketNews ������

#pragma once
#include"VirtualRecordset.h"

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

	long m_ID;
	long m_ids;
	CString m_tickers;
	CString m_title;
	CString m_tag;
	CString m_source;
	CString m_description;
	CString m_url;
	CString m_crawlDate;
	CString m_publishedDate;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
