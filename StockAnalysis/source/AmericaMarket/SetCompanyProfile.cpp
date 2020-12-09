// CSetCompanyProfile.cpp : CSetStockCode ���ʵ��

// CSetCompanyProfile ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetCompanyProfile.h"

IMPLEMENT_DYNAMIC(CSetCompanyProfile, CRecordset)

CSetCompanyProfile::CSetCompanyProfile(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T("");
  m_DisplaySymbol = _T("");
  m_Symbol = _T("");
  m_Type = _T("");
  m_Currency = _T("");
  m_CompanyProfileUpdateDate = 19900101;
  m_DayLineStartDate = 29900101;
  m_DayLineEndDate = 19900101;

  m_nFields = 19;
}

CString CSetCompanyProfile::GetDefaultConnect() {
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8mb4"); // mysqlTest��������TestData Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=AmericaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"); // mysql��������AmericaStakeMarket Schema
  }
}

CString CSetCompanyProfile::GetDefaultSQL() {
  return _T("[companyprofile]");
}

void CSetCompanyProfile::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Type]"), m_Type);
  RFX_Text(pFX, _T("[Currency]"), m_Currency);
  RFX_Text(pFX, _T("[Country]"), m_Country);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
  RFX_Text(pFX, _T("[IPODate]"), m_IPODate);
  RFX_Text(pFX, _T("[Phone]"), m_Phone);
  RFX_Text(pFX, _T("[ShareOutstanding]"), m_ShareOutstanding);
  RFX_Text(pFX, _T("[MarketCapitalization]"), m_MarketCapitalization);
  RFX_Text(pFX, _T("[Name]"), m_Name);
  RFX_Text(pFX, _T("[WebURL]"), m_WebURL);
  RFX_Text(pFX, _T("[FinnHubIndustry]"), m_FinnhubIndustry);
  RFX_Long(pFX, _T("[CompanyProfileUpdateDate]"), m_CompanyProfileUpdateDate);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCompanyProfile ���

#ifdef _DEBUG
void CSetCompanyProfile::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetCompanyProfile::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG