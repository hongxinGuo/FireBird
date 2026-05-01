#include "pch.h"

#include "ContainerTiingoStockDayLine.h"

#include "InfoReport.h"
#include"SetTiingoStockDayLine.h"

bool CContainerTiingoStockDayLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		UpdateDB(&setTiingoStockDayLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerTiingoStockDayLine::LoadDB(const string& strStockSymbol) {
	CSetTiingoStockDayLine setDayLineBasic;

	// 装入DayLine数据
	setDayLineBasic.m_strFilter = "[Symbol] = '";
	setDayLineBasic.m_strFilter += strStockSymbol.c_str();
	setDayLineBasic.m_strFilter += "'";
	setDayLineBasic.m_strSort = "[Date]";
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

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
//Note  更新完后，本container中的日线数据也自动更新为最新数据(以备以后的处理日线数据）
//
//////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStockDayLine::UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const {
	vector<CTiingoCandleLinePtr> vOldHistoryCandle;
	CTiingoCandleLinePtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;

	const size_t lSize = Size();
	long lLastDate = 0;
	pSetTiingoStockDayLine->m_strFilter = "[Symbol] = '";
	pSetTiingoStockDayLine->m_strFilter += strStockSymbol.c_str();
	pSetTiingoStockDayLine->m_strFilter += "'";
	pSetTiingoStockDayLine->m_strSort = "[Date]";

	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	while (!pSetTiingoStockDayLine->IsEOF()) {
		if (pSetTiingoStockDayLine->m_Date > lLastDate) {
			lLastDate = pSetTiingoStockDayLine->m_Date;
			pHistoryCandle = make_shared<CTiingoCandleLine>();
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
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetDate() < vOldHistoryCandle.at(0)->GetDate()) {	// 有更早的新数据？
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetDate() < pHistoryCandle->GetDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetDate() > pHistoryCandle->GetDate()) { // 前数据集中有遗漏的日期
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
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
		}
	}
	pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
	pSetTiingoStockDayLine->Close();
}

bool CContainerTiingoStockDayLine::UpdateDB2(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const {
	CTiingoCandleLinePtr pHistoryCandle = nullptr;
	bool fNeedUpdate = false;
	long lSizeOfOldDayLine = 0;
	vector<CTiingoCandleLinePtr> vOldHistoryCandle;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	if (!strStockSymbol.empty()) {
		pSetTiingoStockDayLine->m_strFilter = "[Symbol] = '";
		pSetTiingoStockDayLine->m_strFilter += strStockSymbol.c_str();
		pSetTiingoStockDayLine->m_strFilter += "'";
		pSetTiingoStockDayLine->m_strSort = "[Date]";

		pSetTiingoStockDayLine->Open();
		long lLastDate = 0;
		pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
		while (!pSetTiingoStockDayLine->IsEOF()) {
			if (pSetTiingoStockDayLine->m_Date > lLastDate) {
				lLastDate = pSetTiingoStockDayLine->m_Date;
				pHistoryCandle = make_shared<CTiingoCandleLine>();
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
	pSetTiingoStockDayLine->m_strFilter = "[ID] = 1";
	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetDate() < vOldHistoryCandle.at(0)->GetDate()) {	// 有更早的新数据？
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetDate() < pHistoryCandle->GetDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetDate() > pHistoryCandle->GetDate()) { // 前数据集中有遗漏的日期
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
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
		}
	}
	pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
	pSetTiingoStockDayLine->Close();

	return fNeedUpdate;
}

bool CContainerTiingoStockDayLine::LoadBasicDB(CSetTiingoStockDayLine* pSetHistoryCandle) {
	ASSERT(pSetHistoryCandle->IsOpen());

	Unload(); // 卸载之前的日线
	// 装入DayLine数据
	while (!pSetHistoryCandle->IsEOF()) {
		auto pHistoryCandle = make_shared<CTiingoCandleLine>();
		pHistoryCandle->LoadBasicData(pSetHistoryCandle);
		Add(pHistoryCandle);
		pSetHistoryCandle->MoveNext();
	}
	m_fDataLoaded = true;

	SplitAdjust();
	return true;
}

void CContainerTiingoStockDayLine::UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine) {
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

struct CSplitFactor {
	long date;
	double factor;
};

void CContainerTiingoStockDayLine::SplitAdjust() {
	ASSERT(IsDataLoaded());
	// 按拆分因子调整日线数据
	vector<shared_ptr<CSplitFactor>> vpSplitFactor;
	double dTotalFactor = 1.0;
	// 找出所有的拆分因子，并计算累计拆分因子。注意，拆分因子是从后向前计算的。
	for (long i = m_vHistoryData.size() - 1; i >= 0; i--) {
		auto data = m_vHistoryData.at(i);
		if (std::abs(data->GetSplitFactor() - 1.0) > EPSILON) {
			dTotalFactor *= data->GetSplitFactor();
			auto p = make_shared<CSplitFactor>();
			p->date = data->GetDate();
			p->factor = dTotalFactor;
			vpSplitFactor.push_back(p);
		}
	}
	if (vpSplitFactor.empty()) return; // 没有拆分因子，直接返回

	// 从后向前调整日线数据
	long j = 0;
	long prevDate;
	int i = m_vHistoryData.size() - 1;
	double currentFactor = 1.0;
	auto currentData = m_vHistoryData.at(i);
	auto currentSpiltDate = vpSplitFactor.at(j)->date;
	while (currentData->GetDate() > currentSpiltDate) currentData = m_vHistoryData.at(--i); // 找到拆分日的日线数据
	do {
		const double prevFactor = currentFactor;
		currentSpiltDate = vpSplitFactor.at(j)->date;
		currentFactor = vpSplitFactor.at(j)->factor;
		if (j < vpSplitFactor.size() - 1) prevDate = vpSplitFactor.at(j + 1)->date;
		else prevDate = 0;

		ASSERT(currentData->GetDate() == currentSpiltDate);
		currentData->SetLastClose(currentData->GetLastClose() * prevFactor / currentFactor); // 拆分日只有前收盘价需要调整，其他价格不调整。

		while (currentData->GetDate() > prevDate && i > 0) { // 调整拆分日之前的日线数据，直到下一个拆分日（如果有的话）
			currentData = m_vHistoryData.at(--i);
			currentData->AdjustByFactor(currentFactor);
		}
		j++;
	} while (i > 0);
}
