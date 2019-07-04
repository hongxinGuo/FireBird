#include "stdafx.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_nFields = 2;
  //m_nDefaultType = dynaset;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CSetOption::GetDefaultConnect()
{
  return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");

  //MaxBufferSize=4096;PageTimeout=5;return _T("DSN=MS Access Database;DBQ=E:\\SmartStockDataBase\\Database\\StockCode.mdb;DefaultDir=E:\\SmartStockDataBase\\Database;DriverId=25;FIL=MS Access;MaxBufferSize=4096;PageTimeout=5;UID=admin;");

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

