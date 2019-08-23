// SetRealTimeData.h : CSetRealTimeData 类的实现



// CSetRealTimeData 实现

// 代码生成在 2019年5月26日, 8:09

#include "stdafx.h"
#include"globedef.h"

#include "SetRealTimeData.h"
IMPLEMENT_DYNAMIC(CSetRealTimeData, CRecordset)

CSetRealTimeData::CSetRealTimeData(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Time = 0;
  m_lMarket = 0;
	m_StockCode = "";
	m_LastClose = 0.0;
	m_Open = 0;
	m_High = 0;
	m_Low = 0;
	m_CurrentPrice = 0;
	m_Volume = 0;
	m_Amount = 0;
	m_Stroke = 0;
	m_PBuy5 = 0;
	m_VBuy5 = 0;
	m_PBuy4 = 0;
	m_VBuy4 = 0;
	m_PBuy3 = 0;
	m_VBuy3 = 0;
	m_PBuy2 = 0;
	m_VBuy2 = 0;
	m_PBuy1 = 0;
	m_VBuy1 = 0;
	m_PSell1 = 0;
	m_VSell1 = 0;
	m_PSell2 = 0;
	m_VSell2 = 0;
	m_PSell3 = 0;
	m_VSell3 = 0;
	m_PSell4 = 0;
	m_VSell4 = 0;
	m_PSell5 = 0;
	m_VSell5 = 0;
	m_nFields = 31;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CSetRealTimeData::GetDefaultConnect()
{
  if (!gl_fNormalMode) {
    gl_fInTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8");
  }
  else {
    gl_fInTestMode = false;
    return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
  }
}

CString CSetRealTimeData::GetDefaultSQL()
{
	return _T("[RealTimeData]");
}

void CSetRealTimeData::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[Time]"), m_Time);
  RFX_Long(pFX, _T("[Market]"), m_lMarket);
	RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Double(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Double(pFX, _T("[Open]"), m_Open);
  RFX_Double(pFX, _T("[High]"), m_High);
  RFX_Double(pFX, _T("[Low]"), m_Low);
  RFX_Double(pFX, _T("[CurrentPrice]"), m_CurrentPrice);
  RFX_Long(pFX, _T("[Volume]"), m_Volume);
  RFX_Double(pFX, _T("[Amount]"), m_Amount);
	RFX_Long(pFX, _T("[Stroke]"), m_Stroke);
	RFX_Double(pFX, _T("[PBuy5]"), m_PBuy5);
	RFX_Long(pFX, _T("[VBuy5]"), m_VBuy5);
	RFX_Double(pFX, _T("[PBuy4]"), m_PBuy4);
	RFX_Long(pFX, _T("[VBuy4]"), m_VBuy4);
	RFX_Double(pFX, _T("[PBuy3]"), m_PBuy3);
	RFX_Long(pFX, _T("[VBuy3]"), m_VBuy3);
	RFX_Double(pFX, _T("[PBuy2]"), m_PBuy2);
	RFX_Long(pFX, _T("[VBuy2]"), m_VBuy2);
	RFX_Double(pFX, _T("[PBuy1]"), m_PBuy1);
	RFX_Long(pFX, _T("[VBuy1]"), m_VBuy1);
	RFX_Double(pFX, _T("[PSell1]"), m_PSell1);
	RFX_Long(pFX, _T("[VSell1]"), m_VSell1);
	RFX_Double(pFX, _T("[PSell2]"), m_PSell2);
	RFX_Long(pFX, _T("[VSell2]"), m_VSell2);
	RFX_Double(pFX, _T("[PSell3]"), m_PSell3);
	RFX_Long(pFX, _T("[VSell3]"), m_VSell3);
	RFX_Double(pFX, _T("[PSell4]"), m_PSell4);
	RFX_Long(pFX, _T("[VSell4]"), m_VSell4);
	RFX_Double(pFX, _T("[PSell5]"), m_PSell5);
	RFX_Long(pFX, _T("[VSell5]"), m_VSell5);

}
/////////////////////////////////////////////////////////////////////////////
// CSetRealTimeData 诊断

#ifdef _DEBUG
void CSetRealTimeData::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetRealTimeData::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


