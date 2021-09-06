// CSetNaicsIndustry.cpp : CSetNaicsIndustry ���ʵ��

// CSetWorldStockDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetNaicsIndustry.h"

IMPLEMENT_DYNAMIC(CSetNaicsIndustry, CRecordset)

CSetNaicsIndustry::CSetNaicsIndustry(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Naics = _T("");
  m_NationalIndustry = _T("");
  m_Sector = _T("");
  m_SubSector = _T("");
  m_nFields = 5;
}

CString CSetNaicsIndustry::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetNaicsIndustry::GetDefaultSQL() {
  return _T("[naics_industry]");
}

void CSetNaicsIndustry::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Naics]"), m_Naics);
  RFX_Text(pFX, _T("[NationalIndustry]"), m_NationalIndustry);
  RFX_Text(pFX, _T("[Sector]"), m_Sector);
  RFX_Text(pFX, _T("[SubSector]"), m_SubSector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetNaicsIndustry ���

#ifdef _DEBUG
void CSetNaicsIndustry::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetNaicsIndustry::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG