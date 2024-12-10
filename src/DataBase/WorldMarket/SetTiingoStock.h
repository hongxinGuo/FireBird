// CSetTiingoStock.h : CSetTiingoStock ������

#pragma once
import FireBird.Set.VirtualRecordset;
import FireBird.System.ConstantChinaMarket;

class CSetTiingoStock : public CVirtualRecordset {
public:
	CSetTiingoStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_fundamental"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	CString m_TiingoPermaTicker{ _T("") }; // Tiingo���ô����ʶ
	CString m_Ticker{ _T("") };
	CString m_Name{ _T("") };
	BOOL m_IsActive{ false }; //
	BOOL m_IsADR{ false };
	long m_SicCode{ 0 };
	CString m_SicIndustry{ _T("") };
	CString m_SicSector{ _T("") };
	CString m_TiingoIndustry{ _T("") };
	CString m_TiingoSector{ _T("") };
	CString m_ReportingCurrency{ _T("") };
	CString m_Location{ _T("") };
	CString m_CompanyWebSite{ _T("") };
	CString m_SECFilingWebSite{ _T("") };
	long m_IPOStatus{ _STOCK_NOT_CHECKED_ };

	CString m_UpdateDate{ _T("{}") };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
