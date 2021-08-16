// CSetFinnhubCryptoExchange.cpp : CSetFinnhubCryptoExchange 类的实现

// 代码生成在 2019年5月26日, 8:14

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubCryptoExchange.h"

IMPLEMENT_DYNAMIC(CSetFinnhubCryptoExchange, CRecordset)

CSetFinnhubCryptoExchange::CSetFinnhubCryptoExchange(CDatabase* pdb)
	: CRecordset(pdb) {
	m_Exchange = _T(" ");

	m_nFields = 2;
}

CString CSetFinnhubCryptoExchange::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetFinnhubCryptoExchange::GetDefaultSQL() {
	return _T("[finnhub_crypto_exchange]");
}

void CSetFinnhubCryptoExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock 诊断

#ifdef _DEBUG
void CSetFinnhubCryptoExchange::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetFinnhubCryptoExchange::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG