// SetStockCode.h : CSetStockCode 类的实现



// CSetStockCode 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include "SetStockCode.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetStockCode, CRecordset)

CSetStockCode::CSetStockCode(CDatabase* pdb)
	: CRecordset(pdb)
{
  m_ID = 0;
	m_Counter = 0;
	m_StockType = 0;
	m_StockCode = "";
	m_StockName = "";
  m_DayLineStartDay = 19900101;
	m_DayLineEndDay = 19900101;
  m_NewestDayLineDay = 19900101;
	m_IPOed = 0;
	m_nFields = 9;
	//m_nDefaultType = dynaset;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CSetStockCode::GetDefaultConnect()
{
#ifdef __USING_UNICODE_CHARSET__
  return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
#else
  return _T("DSN=mysqlA;UID=guo;PASSWORD=guo1426hx;charset=gb2312");
#endif

  //MaxBufferSize=4096;PageTimeout=5;return _T("DSN=MS Access Database;DBQ=E:\\SmartStockDataBase\\Database\\StockCode.mdb;DefaultDir=E:\\SmartStockDataBase\\Database;DriverId=25;FIL=MS Access;MaxBufferSize=4096;PageTimeout=5;UID=admin;");

}

CString CSetStockCode::GetDefaultSQL()
{
	return _T("[StockCode]");
}

void CSetStockCode::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Counter]"), m_Counter);
	RFX_Int(pFX, _T("[StockType]"), m_StockType);
	RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
	RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Long(pFX, _T("[DayLineStartDay]"), m_DayLineStartDay);
	RFX_Long(pFX, _T("[DayLineEndDay]"), m_DayLineEndDay);
  RFX_Long(pFX, _T("[NewestDayLineDay]"), m_NewestDayLineDay);
	RFX_Long(pFX, _T("[IPOed]"), m_IPOed);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetStockCode::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetStockCode::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


