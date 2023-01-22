// SetRealTimeData.h : CSetRealTimeData 类的实现

#include"pch.h"
#include "SetRealTimeData.h"

CSetRealTimeData::CSetRealTimeData(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Time = _T("0");
	m_Symbol = _T("");
	m_LastClose = _T("0");
	m_Open = _T("0");
	m_High = _T("0");
	m_Low = _T("0");
	m_New = _T("0");
	m_Volume = _T("0");
	m_Amount = _T("0");
	m_Stroke = _T("0");
	m_PBuy5 = _T("0");
	m_VBuy5 = _T("0");
	m_PBuy4 = _T("0");
	m_VBuy4 = _T("0");
	m_PBuy3 = _T("0");
	m_VBuy3 = _T("0");
	m_PBuy2 = _T("0");
	m_VBuy2 = _T("0");
	m_PBuy1 = _T("0");
	m_VBuy1 = _T("0");
	m_PSell1 = _T("0");
	m_VSell1 = _T("0");
	m_PSell2 = _T("0");
	m_VSell2 = _T("0");
	m_PSell3 = _T("0");
	m_VSell3 = _T("0");
	m_PSell4 = _T("0");
	m_VSell4 = _T("0");
	m_PSell5 = _T("0");
	m_VSell5 = _T("0");
	m_nFields = 31;
}

void CSetRealTimeData::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Time]"), m_Time);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
	RFX_Text(pFX, _T("[Open]"), m_Open);
	RFX_Text(pFX, _T("[High]"), m_High);
	RFX_Text(pFX, _T("[Low]"), m_Low);
	RFX_Text(pFX, _T("[New]"), m_New);
	RFX_Text(pFX, _T("[Volume]"), m_Volume);
	RFX_Text(pFX, _T("[Amount]"), m_Amount);
	RFX_Text(pFX, _T("[Stroke]"), m_Stroke);
	RFX_Text(pFX, _T("[PBuy5]"), m_PBuy5);
	RFX_Text(pFX, _T("[VBuy5]"), m_VBuy5);
	RFX_Text(pFX, _T("[PBuy4]"), m_PBuy4);
	RFX_Text(pFX, _T("[VBuy4]"), m_VBuy4);
	RFX_Text(pFX, _T("[PBuy3]"), m_PBuy3);
	RFX_Text(pFX, _T("[VBuy3]"), m_VBuy3);
	RFX_Text(pFX, _T("[PBuy2]"), m_PBuy2);
	RFX_Text(pFX, _T("[VBuy2]"), m_VBuy2);
	RFX_Text(pFX, _T("[PBuy1]"), m_PBuy1);
	RFX_Text(pFX, _T("[VBuy1]"), m_VBuy1);
	RFX_Text(pFX, _T("[PSell1]"), m_PSell1);
	RFX_Text(pFX, _T("[VSell1]"), m_VSell1);
	RFX_Text(pFX, _T("[PSell2]"), m_PSell2);
	RFX_Text(pFX, _T("[VSell2]"), m_VSell2);
	RFX_Text(pFX, _T("[PSell3]"), m_PSell3);
	RFX_Text(pFX, _T("[VSell3]"), m_VSell3);
	RFX_Text(pFX, _T("[PSell4]"), m_PSell4);
	RFX_Text(pFX, _T("[VSell4]"), m_VSell4);
	RFX_Text(pFX, _T("[PSell5]"), m_PSell5);
	RFX_Text(pFX, _T("[VSell5]"), m_VSell5);
}
