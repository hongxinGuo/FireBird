#include"pch.h"
#include"accessory.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_RSStartDate = __CHINA_MARKET_BEGIN_DATE__;
  m_RSEndDate = __CHINA_MARKET_BEGIN_DATE__;
  m_LastLoginDate = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS1 = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS2 = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS = __CHINA_MARKET_BEGIN_DATE__;
  m_nFields = 6;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CSetOption::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetOption::GetDefaultSQL() {
  return _T("[options]");
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[RelativeStrongStartDate]"), m_RSStartDate);
  RFX_Long(pFX, _T("[RelativeStrongEndDate]"), m_RSEndDate);
  RFX_Long(pFX, _T("[LastLoginDate]"), m_LastLoginDate);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS1]"), m_UpdatedDateFor10DaysRS1);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS2]"), m_UpdatedDateFor10DaysRS2);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS]"), m_UpdatedDateFor10DaysRS);
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