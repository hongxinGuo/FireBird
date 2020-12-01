// SetStakeCode.cpp : CSetStakeCode ���ʵ��

// CSetStakeCode ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include "SetStakeCode.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetStakeCode, CRecordset)

CSetStakeCode::CSetStakeCode(CDatabase* pdb)
  : CRecordset(pdb) {
  m_StakeType = 0;
  m_StakeCode = _T("");
  m_StakeName = _T("");
  m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
  m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
  m_IPOStatus = 0;
  m_nFields = 7;
}

CString CSetStakeCode::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetStakeCode::GetDefaultSQL() {
  return _T("[StakeCode]");
}

void CSetStakeCode::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Int(pFX, _T("[StakeType]"), m_StakeType);
  RFX_Text(pFX, _T("[StakeCode]"), m_StakeCode);
  RFX_Text(pFX, _T("[StakeName]"), m_StakeName);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
  RFX_Long(pFX, _T("[IPOed]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStakeCode ���

#ifdef _DEBUG
void CSetStakeCode::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetStakeCode::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG