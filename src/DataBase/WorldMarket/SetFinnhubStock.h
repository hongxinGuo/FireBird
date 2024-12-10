// SetFinnhubStock.h : CSetFinnhubStock ������

#pragma once
import FireBird.Set.VirtualRecordset;
import FireBird.System.ConstantChinaMarket;

class CSetFinnhubStock : public CVirtualRecordset {
public:
	CSetFinnhubStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_profile"), CDatabase* pDatabase = nullptr);
	~CSetFinnhubStock() override = default;

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
	CString m_DisplaySymbol{ _T("") };
	CString m_Symbol{ _T("") };
	CString m_ExchangeCode{ _T("") };
	CString m_Type{ _T("") };
	CString m_Mic{ _T("") };
	CString m_Figi{ _T("") };
	CString m_Currency{ _T("") };
	CString m_Address{ _T("") };
	CString m_City{ _T("") };
	CString m_Country{ _T("") };
	CString m_Cusip{ _T("") };
	CString m_Sedol{ _T("") };
	long m_EmployeeTotal{ 0 };
	CString m_Ggroup{ _T("") };
	CString m_Gind{ _T("") };
	CString m_Gsector{ _T("") };
	CString m_Gsubind{ _T("") };
	CString m_IPODate{ _T("") };
	CString m_Isin{ _T("") };
	CString m_MarketCapitalization{ _T("") };
	CString m_Naics{ _T("") };
	CString m_NaicsNationalIndustry{ _T("") };
	CString m_NaicsSector{ _T("") };
	CString m_NaicsSubsector{ _T("") };
	CString m_Name{ _T("") };
	CString m_Phone{ _T("") };
	CString m_ShareOutstanding{ _T("") };
	CString m_State{ _T("") };
	CString m_Ticker{ _T("") };
	CString m_WebURL{ _T("") };
	CString m_Logo{ _T("") };
	CString m_FinnhubIndustry{ _T("") };
	CString m_Peer{ _T("{}") }; //json��ʽ
	long m_IPOStatus{ _STOCK_NOT_CHECKED_ };

	CString m_UpdateDate{ _T("{}") }; // ������ڴ洢���������ڣ�json��ʽ��

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
