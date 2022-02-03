// CSetWorldMarketOption.h : SetWorldMarketOption ������

#pragma once

#include"VirtualRecordset.h"

class CSetWorldMarketOption : public CVirtualRecordset
{
public:
	CSetWorldMarketOption(CString strSchema = _T("WorldMarket"), CString strTable = _T("world_market_option"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetWorldMarketOption)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	CString m_FinnhubToken;
	long m_FinnhubMaxPerHour; // Finnhub.ioÿСʱ����ѯ������ ����˻�3600��/Сʱ�������˻�54000��/Сʱ
	CString m_TiingoToken;
	long m_TiingoMaxPerHour; // tiingo.comÿСʱ����ѯ���� ����˻�500��/Сʱ�������˻�5000��/Сʱ
	CString m_QuandlToken;
	long m_QuandlMaxPerHour; // quandl.comÿСʱ����ѯ����

	// ��д
		// �����ɵ��麯����д
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
