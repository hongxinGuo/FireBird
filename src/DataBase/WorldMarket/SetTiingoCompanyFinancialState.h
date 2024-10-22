// CSetTiingoCompanyFinancialState.h : CSetTiingoCompanyFinancialState ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoCompanyFinancialState : public CVirtualRecordset {
public:
	CSetTiingoCompanyFinancialState(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_company_financial_state"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };
	CString m_symbol{ _T("") };
	CString m_exchange{ _T("") };
	int m_yearQuarter{ 0 };
	CString m_accoci{ _T("") };
	CString m_acctPay{ _T("") };
	CString m_acctRec{ _T("") };
	CString m_assetsCurrent{ _T("") };
	CString m_assetsNonCurrent{ _T("") };
	CString m_assetTurnover{ _T("") };
	CString m_bookVal{ _T("") };
	CString m_businessAcqDisposals{ _T("") };
	CString m_bvps{ _T("") };
	CString m_capex{ _T("") };
	CString m_cashAndEq{ _T("") };
	CString m_consolidatedIncome{ _T("") };
	CString m_costRev{ _T("") };
	CString m_currentRatio{ _T("") };
	CString m_debt{ _T("") };
	CString m_debtCurrent{ _T("") };
	CString m_debtEquity{ _T("") };
	CString m_debtNonCurrent{ _T("") };
	CString m_deferredRev{ _T("") };
	CString m_depamor{ _T("") };
	CString m_deposits{ _T("") };
	CString m_ebit{ _T("") };
	CString m_ebitda{ _T("") };
	CString m_ebt{ _T("") };
	CString m_enterpriseVal{ _T("") };
	CString m_eps{ _T("") };
	CString m_epsDil{ _T("") };
	CString m_epsQoQ{ _T("") };
	CString m_equity{ _T("") };
	CString m_freeCashFlow{ _T("") };
	CString m_fxRate{ _T("") };
	CString m_grossMargin{ _T("") };
	CString m_grossProfit{ _T("") };
	CString m_intangibles{ _T("") };
	CString m_intexp{ _T("") };
	CString m_inventory{ _T("") };
	CString m_investments{ _T("") };
	CString m_investmentsAcqDisposals{ _T("") };
	CString m_investmentsCurrent{ _T("") };
	CString m_investmentsNonCurrent{ _T("") };
	CString m_issrepayDebt{ _T("") };
	CString m_issrepayEquity{ _T("") };
	CString m_liabilitiesCurrent{ _T("") };
	CString m_liabilitiesNonCurrent{ _T("") };
	CString m_longTermDebtEquity{ _T("") };
	CString m_marketCap{ _T("") };
	CString m_ncf{ _T("") };
	CString m_ncff{ _T("") };
	CString m_ncfi{ _T("") };
	CString m_ncfo{ _T("") };
	CString m_ncfx{ _T("") };
	CString m_netinc{ _T("") };
	CString m_netIncComStock{ _T("") };
	CString m_netIncDiscOps{ _T("") };
	CString m_netMargin{ _T("") };
	CString m_nonControllingInterests{ _T("") };
	CString m_opex{ _T("") };
	CString m_opinc{ _T("") };
	CString m_opMargin{ _T("") };
	CString m_payDiv{ _T("") };
	CString m_pbRatio{ _T("") };
	CString m_peRatio{ _T("") };
	CString m_piotroskiFScore{ _T("") };
	CString m_ppeq{ _T("") };
	CString m_prefDVDs{ _T("") };
	CString m_profitMargin{ _T("") };
	CString m_retainedEarnings{ _T("") };
	CString m_revenue{ _T("") };
	CString m_revenueQoQ{ _T("") };
	CString m_rnd{ _T("") };
	CString m_roa{ _T("") };
	CString m_roe{ _T("") };
	CString m_rps{ _T("") };
	CString m_sbcomp{ _T("") };
	CString m_sga{ _T("") };
	CString m_shareFactor{ _T("") };
	CString m_sharesBasic{ _T("") };
	CString m_shareswa{ _T("") };
	CString m_shareswaDil{ _T("") };
	CString m_taxAssets{ _T("") };
	CString m_taxExp{ _T("") };
	CString m_taxLiabilities{ _T("") };
	CString m_totalAssets{ _T("") };
	CString m_totalLiabilities{ _T("") };
	CString m_trailingPEG1Y{ _T("") };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
