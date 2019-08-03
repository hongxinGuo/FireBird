#include "stdafx.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_RalativeStrongStartDay = 19900101;
  m_RelativeStrongEndDay = 19900101;
  m_LastLoginDay = 19900101;
  m_nFields = 3;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CSetOption::GetDefaultConnect()
{
  if (gl_fTestMode) {
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8");
}
  else {
    return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
  }
}

CString CSetOption::GetDefaultSQL()
{
  return _T("[options]");
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX)
{
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[RelativeStrongStartDay]"), m_RalativeStrongStartDay);
  RFX_Long(pFX, _T("[RelativeStrongEndDay]"), m_RelativeStrongEndDay);
  RFX_Long(pFX, _T("[LastLoginDay]"), m_LastLoginDay);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetOption::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetOption::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG

