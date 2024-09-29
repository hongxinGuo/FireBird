#include"pch.h"

#include"TiingoStock.h"

#include "JsonParse.h"

CTiingoStock::CTiingoStock() {
	Reset();
}

void CTiingoStock::Reset() {
	CVirtualStock::Reset();
	m_strTiingoPermaTicker = _T("");
	m_strName = _T("");
	m_fIsActive = false;
	m_fIsADR = false;
	m_strTiingoIndustry = _T("");
	m_strTiingoSector = _T("");
	m_iSICCode = 0;
	m_strSICIndustry = _T("");
	m_strSICSector = _T("");
	m_strReportingCurrency = _T("");
	m_strLocation = _T("");
	m_strCompanyWebSite = _T("");
	m_strSECFilingWebSite = _T("");
	SetDailyDataUpdateDate(19800101);
	//SetDayLineUpdateDate(19800101);
	SetCompanyFinancialStatementUpdateDate(19800101);
}

void CTiingoStock::ResetAllUpdateDate() {
	m_jsonUpdateDate["Tiingo"]["CompanyProfile"] = 19800101;
	m_jsonUpdateDate["Tiingo"]["DayLine"] = 19800101;
	m_jsonUpdateDate["Tiingo"]["CompanyFinancialStatement"] = 19800101;
	m_jsonUpdateDate["Tiingo"]["DailyData"] = 19800101;
}

void CTiingoStock::Load(CSetTiingoStock& setTiingoStock) {
	m_strTiingoPermaTicker = setTiingoStock.m_TiingoPermaTicker;
	m_strSymbol = setTiingoStock.m_Ticker;
	m_strName = setTiingoStock.m_Name;
	m_fIsActive = setTiingoStock.m_IsActive;
	m_fIsADR = setTiingoStock.m_IsADR;
	m_iSICCode = setTiingoStock.m_SICCode;
	m_strSICIndustry = setTiingoStock.m_SICIndustry;
	m_strSICSector = setTiingoStock.m_SICSector;
	m_strTiingoIndustry = setTiingoStock.m_TiingoIndustry;
	m_strTiingoSector = setTiingoStock.m_TiingoSector;
	m_strReportingCurrency = setTiingoStock.m_ReportingCurrency;
	m_strLocation = setTiingoStock.m_Location;
	m_strCompanyWebSite = setTiingoStock.m_CompanyWebSite;
	m_strSECFilingWebSite = setTiingoStock.m_SECFilingWebSite;
	if (setTiingoStock.m_UpdateDate.IsEmpty()) {
		ResetAllUpdateDate();
	}
	else {
		CreateJsonWithNlohmann(m_jsonUpdateDate, setTiingoStock.m_UpdateDate);
	}
}

void CTiingoStock::Append(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.AddNew();
	Save(setTiingoStock);
	setTiingoStock.Update();
}

void CTiingoStock::Save(CSetTiingoStock& setTiingoStock) {
	if ((m_strCompanyWebSite.GetLength() > 100)
		|| (m_strName.GetLength() > 200)
		|| (m_strSECFilingWebSite.GetLength() > 150)
		|| (m_strSICIndustry.GetLength() > 100)
		|| (m_strSICSector.GetLength() > 100)
		|| (m_strTiingoIndustry.GetLength() > 100)
		|| (m_strTiingoSector.GetLength() > 100)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo stock ") + m_strName + _T(" 字符串太长"));
	}
	m_strName = m_strName.Left(200);
	m_strSICIndustry = m_strSICIndustry.Left(100);
	m_strSICSector = m_strSICSector.Left(100);
	m_strTiingoIndustry = m_strTiingoIndustry.Left(100);
	m_strTiingoSector = m_strTiingoSector.Left(100);
	m_strCompanyWebSite = m_strCompanyWebSite.Left(100);
	m_strSECFilingWebSite = m_strSECFilingWebSite.Left(150);

	setTiingoStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
	setTiingoStock.m_Ticker = m_strSymbol;
	setTiingoStock.m_Name = m_strName;
	setTiingoStock.m_IsActive = m_fIsActive;
	setTiingoStock.m_IsADR = m_fIsADR;
	setTiingoStock.m_SICCode = m_iSICCode;
	setTiingoStock.m_SICIndustry = m_strSICIndustry;
	setTiingoStock.m_SICSector = m_strSICSector;
	setTiingoStock.m_TiingoIndustry = m_strTiingoIndustry;
	setTiingoStock.m_TiingoSector = m_strTiingoSector;
	setTiingoStock.m_ReportingCurrency = m_strReportingCurrency;
	setTiingoStock.m_Location = m_strLocation;
	setTiingoStock.m_CompanyWebSite = m_strCompanyWebSite;
	setTiingoStock.m_SECFilingWebSite = m_strSECFilingWebSite;
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setTiingoStock.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CTiingoStock::UpdateFinancialStateDB() const {
	CSetTiingoFinancialState setFinancialState;
	vector<CTiingoFinancialStatePtr> vOldFinancialState;
	CTiingoFinancialStatePtr pTiingoFinancialState = nullptr;
	long lSizeOfOldDayLine = 0;
	const size_t lSize = m_pvFinancialState->size();
	long lLastDate = 0;

	setFinancialState.m_strFilter = _T("[symbol] = '");
	setFinancialState.m_strFilter += m_strSymbol;
	setFinancialState.m_strFilter += _T("'");
	setFinancialState.m_strSort = _T("[yearQuarter]");
	setFinancialState.Open();
	setFinancialState.m_pDatabase->BeginTrans();

	while (!setFinancialState.IsEOF()) {
		if (setFinancialState.m_yearQuarter > lLastDate) {
			lLastDate = setFinancialState.m_yearQuarter;
			pTiingoFinancialState = make_shared<CTiingoFinancialState>();
			pTiingoFinancialState->Load(setFinancialState);

			vOldFinancialState.push_back(pTiingoFinancialState);
			lSizeOfOldDayLine++;
		}
		else {
			setFinancialState.Delete(); //删除日期重复的数据
		}
		setFinancialState.MoveNext();
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();

	setFinancialState.m_strFilter = _T("[ID] = 1");
	setFinancialState.Open();
	setFinancialState.m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pTiingoFinancialState = m_pvFinancialState->at(i);
			if (pTiingoFinancialState->m_yearQuarter < vOldFinancialState.at(0)->m_yearQuarter) {	// 有更早的新数据？
				pTiingoFinancialState->Append(setFinancialState);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && vOldFinancialState.at(lCurrentPos)->m_yearQuarter < pTiingoFinancialState->m_yearQuarter) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldFinancialState.at(lCurrentPos)->m_yearQuarter > pTiingoFinancialState->m_yearQuarter) { // 前数据集中有遗漏的日期
						pTiingoFinancialState->Append(setFinancialState);
					}
				}
				else {
					pTiingoFinancialState->Append(setFinancialState);
				}
			}
		}
	}
	else {// 没有旧数据
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pTiingoFinancialState = m_pvFinancialState->at(i);
			pTiingoFinancialState->Append(setFinancialState);
		}
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();
}
