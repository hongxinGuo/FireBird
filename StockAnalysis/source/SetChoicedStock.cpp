// SetChoicedStock.h : CSetChoicedStock ���ʵ��



// CSetChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include "stdafx.h"
#include"globedef.h"

#include "SetChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetChoicedStock, CRecordset)

CSetChoicedStock::CSetChoicedStock(CDatabase* pdb) : CRecordset(pdb)
{
	m_Market = 0;
	m_StockCode = L"";
	m_StockName = L"";
	m_nFields = 3;
	m_nDefaultType = dynaset;
}

CString CSetChoicedStock::GetDefaultConnect()
{
  /*
  CString str;
  str = _T("DSN=MS Access Database;DBQ=");
  str += gl_sDataBaseDir;
  str += _T("StockCode.mdb");
  str += _T(";DefaultDir=");
  str += gl_sDataBaseDir;
  str += _T("; DriverId = 25; FIL = MS Access; MaxBufferSize = 4096; PageTimeout = 5; UID = admin; ");
  return str;
  */
  return _T("DSN=mysql;MaxBufferSize=4096;PageTimeout=5;UID=guo;PASSWORD=guo1426hx;charset=utf8");

  //return _T("DSN=MS Access Database;DBQ=E:\\SmartStockDataBase\\Database\\StockCode.mdb;DefaultDir=E:\\SmartStockDataBase\\Database;DriverId=25;FIL=MS Access;MaxBufferSize=4096;PageTimeout=5;UID=admin;");
}

CString CSetChoicedStock::GetDefaultSQL()
{
	return _T("[ChoicedStocks]");
}

void CSetChoicedStock::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Market]"), m_Market);
	RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
	RFX_Text(pFX, _T("[StockName]"), m_StockName);

}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock ���

#ifdef _DEBUG
void CSetChoicedStock::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetChoicedStock::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


