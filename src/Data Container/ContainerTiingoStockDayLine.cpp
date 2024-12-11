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

		// װ��DayLine����
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
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//  �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//  ������������ݱ��ɵ�һ���������룬
//  �Զ�ɾ���������е��ظ����ݡ�
//
//Note  ������󣬱�container�е�����������Ҳ�Զ�����Ϊ��������(�Ա��Ժ�Ĵ����������ݣ�
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
			pSetTiingoStockDayLine->Delete(); //ɾ�������ظ�������
		}
		pSetTiingoStockDayLine->MoveNext();
	}

	if (lSizeOfOldDayLine > 0) {// �о�����
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vOldHistoryCandle.at(0)->GetMarketDate()) {	// �и���������ݣ�
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) { // ǰ���ݼ�������©������
						pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
					}
				}
				else {
					pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
				}
			}
		}
	}
	else {// û�о�����
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
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
				pSetTiingoStockDayLine->Delete(); //ɾ�������ظ�������
			}
			pSetTiingoStockDayLine->MoveNext();
		}
		pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
		pSetTiingoStockDayLine->Close();
	}
	pSetTiingoStockDayLine->m_strFilter = _T("[ID] = 1");
	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// �о�����
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vOldHistoryCandle.at(0)->GetMarketDate()) {	// �и���������ݣ�
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) { // ǰ���ݼ�������©������
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
	else {// û�о�����
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
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

	Unload(); // ж��֮ǰ������
	// װ��DayLine����
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
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (const auto& pDayLine : *pvTempDayLine) {
		if (pDayLine->IsActive()) {
			// ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
			Add(pDayLine);
		}
	}
	m_fDataLoaded = true;
}
