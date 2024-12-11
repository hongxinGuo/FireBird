#include "pch.h"
module;
#include"SetTiingoStockDayLine.h"
module FireBird.Container.Tiingo.StockDayLine;

import FireBird.Accessory.InfoReport;

CContainerTiingoStockDayLine::CContainerTiingoStockDayLine() {}

bool CContainerTiingoStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		UpdateDB(&setTiingoStockDayLineBasic, strStockSymbol);
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerTiingoStockDayLine::LoadDB(const CString& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setDayLineBasic;

		// 装入DayLine数据
		setDayLineBasic.m_strFilter = _T("[Symbol] = '");
		setDayLineBasic.m_strFilter += strStockSymbol;
		setDayLineBasic.m_strFilter += _T("'");
		setDayLineBasic.m_strSort = _T("[Date]");
		setDayLineBasic.Open();
		LoadBasicDB(&setDayLineBasic);
		setDayLineBasic.Close();

		m_fDataLoaded = true;
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//  当存在旧日线历史数据时，本函数只是更新。
//
//  具体操作的数据表由第一个参数传入，
//  自动删除旧数据中的重复数据。
//
//Note  更新完后，本container中的日西安数据也自动更新为最新数据(以备以后的处理日线数据）
//
//////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStockDayLine::UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const CString& strStockSymbol) {
	vector<CTiingoDayLinePtr> vOldHistoryCandle;
	CTiingoDayLinePtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	long lLastDate = 0;
	pSetTiingoStockDayLine->m_strFilter = _T("[Symbol] = '");
	pSetTiingoStockDayLine->m_strFilter += strStockSymbol + _T("'");
	pSetTiingoStockDayLine->m_strSort = _T("[Date]");

	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	while (!pSetTiingoStockDayLine->IsEOF()) {
		if (pSetTiingoStockDayLine->m_Date > lLastDate) {
			lLastDate = pSetTiingoStockDayLine->m_Date;
			pHistoryCandle = make_shared<CTiingoDayLine>();
			pHistoryCandle->LoadBasicData(pSetTiingoStockDayLine);

			vOldHistoryCandle.push_back(pHistoryCandle);
			lSizeOfOldDayLine++;
		}
		else {
			pSetTiingoStockDayLine->Delete(); //删除日期重复的数据
		}
		pSetTiingoStockDayLine->MoveNext();
	}

	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vOldHistoryCandle.at(0)->GetMarketDate()) {	// 有更早的新数据？
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) { // 前数据集中有遗漏的日期
						pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
					}
				}
				else {
					pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
				}
			}
		}
	}
	else {// 没有旧数据
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
		}
	}
	pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
	pSetTiingoStockDayLine->Close();
}

bool CContainerTiingoStockDayLine::UpdateDB2(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const CString& strStockSymbol) const {
	vector<CTiingoDayLinePtr> vOldHistoryCandle;
	CTiingoDayLinePtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	if (strStockSymbol.GetLength() > 0) {
		long lLastDate = 0;
		pSetTiingoStockDayLine->m_strFilter = _T("[Symbol] = '");
		pSetTiingoStockDayLine->m_strFilter += strStockSymbol + _T("'");
		pSetTiingoStockDayLine->m_strSort = _T("[Date]");

		pSetTiingoStockDayLine->Open();
		pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
		while (!pSetTiingoStockDayLine->IsEOF()) {
			if (pSetTiingoStockDayLine->m_Date > lLastDate) {
				lLastDate = pSetTiingoStockDayLine->m_Date;
				pHistoryCandle = make_shared<CTiingoDayLine>();
				pHistoryCandle->LoadBasicData(pSetTiingoStockDayLine);

				vOldHistoryCandle.push_back(pHistoryCandle);
				lSizeOfOldDayLine++;
			}
			else {
				pSetTiingoStockDayLine->Delete(); //删除日期重复的数据
			}
			pSetTiingoStockDayLine->MoveNext();
		}
		pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
		pSetTiingoStockDayLine->Close();
	}
	pSetTiingoStockDayLine->m_strFilter = _T("[ID] = 1");
	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vOldHistoryCandle.at(0)->GetMarketDate()) {	// 有更早的新数据？
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) { // 前数据集中有遗漏的日期
						pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
						fNeedUpdate = true;
					}
				}
				else {
					pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
					fNeedUpdate = true;
				}
			}
		}
	}
	else {// 没有旧数据
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
		}
	}
	pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
	pSetTiingoStockDayLine->Close();

	return fNeedUpdate;
}

bool CContainerTiingoStockDayLine::LoadBasicDB(CSetTiingoStockDayLine* pSetHistoryCandleBasic) {
	ASSERT(pSetHistoryCandleBasic->IsOpen());

	Unload(); // 卸载之前的日线
	// 装入DayLine数据
	while (!pSetHistoryCandleBasic->IsEOF()) {
		const auto pHistoryCandle = make_shared<CTiingoDayLine>();
		pHistoryCandle->LoadBasicData(pSetHistoryCandleBasic);
		Add(pHistoryCandle);
		pSetHistoryCandleBasic->MoveNext();
	}
	return true;
}

bool CContainerTiingoStockDayLine::GetStartEndDate(long& lStartDate, long& lEndDate) const {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0)->GetMarketDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1)->GetMarketDate();
	return true;
}

void CContainerTiingoStockDayLine::UpdateData(CTiingoDayLinesPtr pvTempDayLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pDayLine : *pvTempDayLine) {
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pDayLine);
		}
	}
	m_fDataLoaded = true;
}
