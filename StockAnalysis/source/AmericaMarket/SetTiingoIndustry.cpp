// CSetTiingoIndustry.cpp : CSetTiingoIndustry ���ʵ��

// CSetTiingoIndustry ʵ��

// ���������� 2021��3��19��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetTiingoIndustry.h"

IMPLEMENT_DYNAMIC(CSetTiingoIndustry, CRecordset)

CSetTiingoIndustry::CSetTiingoIndustry(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Industry = _T("");
  m_Sector = _T("");
  m_nFields = 3;
}

CString CSetTiingoIndustry::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetTiingoIndustry::GetDefaultSQL() {
  return _T("[tiingo_industry]");
}

void CSetTiingoIndustry::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Industry]"), m_Industry);
  RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetTiingoIndustry ���

#ifdef _DEBUG
void CSetTiingoIndustry::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetTiingoIndustry::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG