// CSetSECFilings.h : CSetSECFilings 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetSECFilings : public CVirtualRecordset {
public:
	explicit CSetSECFilings(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_stock_sec_filings", CDatabase* pDatabase = nullptr);
	~CSetSECFilings() override = default;

	long m_ID{ 0 };
	CString m_Symbol{ "" };
	long m_CIK{ 0 };
	CString m_AccessNumber{ "" };
	CString m_Form{ "" };
	long m_FiledDate{ 0 };
	long m_AcceptedDate{ 0 };
	CString m_ReportURL{ "" };
	CString m_FilingURL{ "" };

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
