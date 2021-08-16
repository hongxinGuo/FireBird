// CSetCryptoDayLine.cpp : CSetCryptoDayLine 类的实现

// CSetWorldStockDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetCryptoDayLine.h"

IMPLEMENT_DYNAMIC(CSetCryptoDayLine, CRecordset)

CSetCryptoDayLine::CSetCryptoDayLine(CDatabase* pdb)
	: CRecordset(pdb) {
	m_ID = 0;
	m_Date = 0;
	m_Exchange = _T("");
	m_Symbol = _T("");
	m_LastClose = _T("0");
	m_Open = _T("0");
	m_High = _T("0");
	m_Low = _T("0");
	m_Close = _T("0");
	m_Volume = _T("0");
	m_nFields = 10;
}

CString CSetCryptoDayLine::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetCryptoDayLine::GetDefaultSQL() {
	return _T("[crypto_dayline]");
}

void CSetCryptoDayLine::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
	RFX_Text(pFX, _T("[Open]"), m_Open);
	RFX_Text(pFX, _T("[High]"), m_High);
	RFX_Text(pFX, _T("[Low]"), m_Low);
	RFX_Text(pFX, _T("[Close]"), m_Close);
	RFX_Text(pFX, _T("[Volume]"), m_Volume);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCryptoDayLine 诊断

#ifdef _DEBUG
void CSetCryptoDayLine::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetCryptoDayLine::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG