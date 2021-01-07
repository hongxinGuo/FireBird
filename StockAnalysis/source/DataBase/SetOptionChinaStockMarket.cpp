#include "stdafx.h"
#include"accessory.h"

#include "SetOptionChinaStockMarket.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOptionChinaStockMarket, CRecordset)

CSetOptionChinaStockMarket::CSetOptionChinaStockMarket(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_RTDataServerIndex = 0; // 默认使用新浪实时数据服务器
  m_RTDataInquiryTime = 400; // 默认查询时间为400毫秒
  m_nFields = 2;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CSetOptionChinaStockMarket::GetDefaultConnect() {
  return GetChinaMarketSchemaVonnect();
}

CString CSetOptionChinaStockMarket::GetDefaultSQL() {
  return _T("[option_china_stockmarket]");
}

void CSetOptionChinaStockMarket::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[RealTimeDataServer]"), m_RTDataServerIndex);
  RFX_Long(pFX, _T("[RealTimeDataInquiryTime]"), m_RTDataInquiryTime);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetOptionChinaStockMarket::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetOptionChinaStockMarket::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG