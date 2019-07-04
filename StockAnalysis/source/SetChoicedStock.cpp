// SetChoicedStock.h : CSetChoicedStock 类的实现



// CSetChoicedStock 实现

// 代码生成在 2019年5月26日, 8:15

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
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Market]"), m_Market);
	RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
	RFX_Text(pFX, _T("[StockName]"), m_StockName);

}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock 诊断

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


