// CSetTiingoFinancialState.h : CSetTiingoFinancialState ������

#pragma once
#include"VirtualRecordset.h"

class CSetTiingoFinancialState : public CVirtualRecordset {
public:
	CSetTiingoFinancialState(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_financial_states"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;
	CString m_Symbol;
	CString m_Exchange;
	CString m_accoci;
	CString m_acctPay;
	CString m_acctRec;
	CString m_assetCurrent;
	CString m_assetNotCurrent;
	CString assetTurnover;
	CString m_boolVal;
	CString m_businessAcqDisposals;
	CString m_bvps;
	CString m_capex;
	CString m_cashAndEq;
	CString m_consolidatedIncome;
	CString m_costRev;
	CString m_currentRatio;
	CString m_debt;
	CString m_debtCurrent;
	CString m_debtEquity;
	CString m_debtNonCurrent;
	CString m_deferredRev;
	CString m_depamor;
	CString m_deposits;
	CString m_ebit;
	CString m_ebitda;
	CString m_ebt;
	CString m_enterpriseVal;
	CString m_eps;
	CString m_epsDil;
	CString m_epsQoQ;
	CString m_equity;
	CString m_freeCashFlow;
	CString m_fxRate;
	CString m_grossMargin;
	CString m_grossProfit;
	CString m_intangibles;
	CString m_intexp;
	CString m_inventory;
	CString m_investments;
	CString m_investmentsAcqDisposals;
	CString m_investmentsCurrent;
	CString m_investmentsNonCurrent;
	CString m_issrepayDebt;
	CString m_issrepayEquity;
	CString m_liabilitiesCurrent;
	CString m_liabilitiesEquity;
	CString m_longTermDebtEquity;
	CString m_marketCap;
	CString m_ncf;
	CString m_ncff;
	CString m_ncfi;
	CString m_ncfo;
	CString m_ncfx;
	CString m_netinc;
	CString m_netincComStock;
	CString m_netIncDiscOps;
	CString m_netMargin;
	CString m_nonControllingInterests;
	CString m_opex;
	CString m_opinc;
	CString m_opMargin;
	CString m_payDiv;
	CString m_pbRatio;
	CString m_peRatio;
	CString m_piotroskiFScore;
	CString m_ppeq;
	CString m_prefDVDs;
	CString m_profitMargin;
	CString m_retainedEarnings;
	CString m_revenue;
	CString m_revenueQoQ;
	CString m_rnd;
	CString m_roa;
	CString m_roe;
	CString m_rps;
	CString m_sbcomp;
	CString m_sga;
	CString m_shareFactor;
	CString m_shareBasic;
	CString m_shareswa;
	CString m_sharewaDil;
	CString m_taxAssets;
	CString m_taxExp;
	CString m_taxLiabilities;
	CString m_totalAssets;
	CString m_totalLiabilities;
	CString m_railingPEG1Y;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
