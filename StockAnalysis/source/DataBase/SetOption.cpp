#include "stdafx.h"
#include"accessory.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_RalativeStrongStartDate = __CHINA_MARKET_BEGIN_DAY__;
  m_RelativeStrongEndDate = __CHINA_MARKET_BEGIN_DAY__;
  m_LastLoginDay = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS1 = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS2 = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS = __CHINA_MARKET_BEGIN_DAY__;
  m_nFields = 6;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CSetOption::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetOption::GetDefaultSQL() {
  return _T("[options]");
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[RelativeStrongStartDate]"), m_RalativeStrongStartDate);
  RFX_Long(pFX, _T("[RelativeStrongEndDate]"), m_RelativeStrongEndDate);
  RFX_Long(pFX, _T("[LastLoginDay]"), m_LastLoginDay);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS1]"), m_UpdatedDayFor10DayRS1);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS2]"), m_UpdatedDayFor10DayRS2);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS]"), m_UpdatedDayFor10DayRS);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetOption::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetOption::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG