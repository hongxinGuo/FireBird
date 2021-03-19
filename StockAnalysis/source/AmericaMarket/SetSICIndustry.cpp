// CSetSICIndustry.cpp : CSetSICIndustry ���ʵ��

// CSetSICIndustry ʵ��

// ���������� 2021��3��19��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetSICIndustry.h"

IMPLEMENT_DYNAMIC(CSetSICIndustry, CRecordset)

CSetSICIndustry::CSetSICIndustry(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Code = 0;
  m_Industry = _T("");
  m_Sector = _T("");
  m_nFields = 4;
}

CString CSetSICIndustry::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetSICIndustry::GetDefaultSQL() {
  return _T("[sic_industry_sector]");
}

void CSetSICIndustry::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Code]"), m_Code);
  RFX_Text(pFX, _T("[Industry]"), m_Industry);
  RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetSICIndustry ���

#ifdef _DEBUG
void CSetSICIndustry::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetSICIndustry::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG