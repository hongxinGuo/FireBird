#include"pch.h"

#include"VirtualDataHistoryCandleExtend.h"
#include"DayLine.h"

CVirtualDataHistoryCandleExtend::CVirtualDataHistoryCandleExtend() {
	Reset();
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
//////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandleExtend::UpdateBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic, const string& strStockSymbol) const {
	vector<CVirtualHistoryCandleExtendPtr> vOldHistoryCandle;
	CVirtualHistoryCandleExtendPtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	if (strStockSymbol.length() > 0) {
		pSetHistoryCandleBasic->m_strFilter = "[Symbol] = '";
		pSetHistoryCandleBasic->m_strFilter += strStockSymbol.c_str();
		pSetHistoryCandleBasic->m_strFilter += "'";
		pSetHistoryCandleBasic->m_strSort = "[Date]";

		if (pSetHistoryCandleBasic->Open()) {
			long lLastDate = 0;
			pSetHistoryCandleBasic->m_pDatabase->BeginTrans();
			while (!pSetHistoryCandleBasic->IsEOF()) {
				if (pSetHistoryCandleBasic->m_Date > lLastDate) {
					lLastDate = pSetHistoryCandleBasic->m_Date;
					pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
					pHistoryCandle->LoadBasicData(pSetHistoryCandleBasic);

					vOldHistoryCandle.push_back(pHistoryCandle);
					lSizeOfOldDayLine++;
				}
				else {
					pSetHistoryCandleBasic->Delete(); //删除日期重复的数据
				}
				pSetHistoryCandleBasic->MoveNext();
			}
			pSetHistoryCandleBasic->m_pDatabase->CommitTrans();
			pSetHistoryCandleBasic->Close();
		}
	}
	pSetHistoryCandleBasic->m_strFilter = "[ID] = 1";
	if (pSetHistoryCandleBasic->Open()) {
		pSetHistoryCandleBasic->m_pDatabase->BeginTrans();
		if (lSizeOfOldDayLine > 0) {// 有旧数据
			long lCurrentPos = 0;
			for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
				pHistoryCandle = GetData(i);
				if (pHistoryCandle->GetDate() < vOldHistoryCandle.at(0)->GetDate()) {	// 有更早的新数据？
					pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
				}
				else {
					while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetDate() < pHistoryCandle->GetDate())) lCurrentPos++;
					if (lCurrentPos < lSizeOfOldDayLine) {
						if (vOldHistoryCandle.at(lCurrentPos)->GetDate() > pHistoryCandle->GetDate()) { // 前数据集中有遗漏的日期
							pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
							fNeedUpdate = true;
						}
					}
					else {
						pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
						fNeedUpdate = true;
					}
				}
			}
		}
		else {// 没有旧数据
			for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
				pHistoryCandle = GetData(i);
				pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
			}
		}
		pSetHistoryCandleBasic->m_pDatabase->CommitTrans();
		pSetHistoryCandleBasic->Close();
	}

	return fNeedUpdate;
}

bool CVirtualDataHistoryCandleExtend::SaveExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend) const {
	ASSERT(!m_vHistoryData.empty());

	pSetHistoryCandleExtend->m_strFilter = "[ID] = 1";
	if (pSetHistoryCandleExtend->Open()) {
		pSetHistoryCandleExtend->m_pDatabase->BeginTrans();
		for (const auto& pData : m_vHistoryData) { pData->AppendExtendData(pSetHistoryCandleExtend); }
		pSetHistoryCandleExtend->m_pDatabase->CommitTrans();
		pSetHistoryCandleExtend->Close();
	}

	return true;
}

bool CVirtualDataHistoryCandleExtend::LoadBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic) {
	if (gl_systemConfiguration.IsWorkingMode())
		ASSERT(!m_fBasicDataLoaded);
	ASSERT(pSetHistoryCandleBasic->IsOpen());

	Unload(); // 卸载之前的日线
	// 装入DayLine数据
	while (!pSetHistoryCandleBasic->IsEOF()) {
		const auto pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryCandle->LoadBasicData(pSetHistoryCandleBasic);
		Add(pHistoryCandle);
		pSetHistoryCandleBasic->MoveNext();
	}
	m_fBasicDataLoaded = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// 装载ExtendData表必须在装载BasicData表之后。
//
//
////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandleExtend::LoadExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend) {
	int iPosition = 0;

	if (gl_systemConfiguration.IsWorkingMode())
		ASSERT(m_fBasicDataLoaded);
	ASSERT(pSetHistoryCandleExtend->IsOpen());

	while (!pSetHistoryCandleExtend->IsEOF()) {
		CVirtualHistoryCandleExtendPtr pHistoryCandle = GetData(iPosition);
		while ((pHistoryCandle->GetDate() < pSetHistoryCandleExtend->m_Date)
			&& (Size() > (iPosition + 1))) {
			iPosition++;
			pHistoryCandle = GetData(iPosition);
		}
		if (pHistoryCandle->GetDate() == pSetHistoryCandleExtend->m_Date) { pHistoryCandle->LoadExtendData(pSetHistoryCandleExtend); }
		if (Size() <= (iPosition + 1)) break;
		pSetHistoryCandleExtend->MoveNext();
	}
	m_fBasicDataLoaded = false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(const vector<CVirtualHistoryCandleExtendPtr>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	for (const auto& p : vTempData) {
		if (p->IsActive()) Add(p);
	}
	SetDataLoaded(true);
}
void CVirtualDataHistoryCandleExtend::UpdateData(const vector<CDayLinePtr>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	for (const auto& p : vTempData) {
		if (p->IsActive()) Add(p);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(CDayLinesPtr vTempDayLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pDayLine : *vTempDayLine) {
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pDayLine);
		}
	}
	SetDataLoaded(true);
}

bool CVirtualDataHistoryCandleExtend::GetStartEndDate(long& lStartDate, long& lEndDate) const {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0)->GetDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1)->GetDate();
	return true;
}

bool CVirtualDataHistoryCandleExtend::HaveDayLine(long lDate) {
	auto it = ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                          [lDate](const CVirtualHistoryCandleExtendPtr& p) { return p->GetDate() == lDate; });
	if (it == m_vHistoryData.end()) {
		return false;
	}
	return true;
}

CVirtualHistoryCandleExtendPtr CVirtualDataHistoryCandleExtend::GetDayLine(long lDate) {
	auto it = std::ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                               [lDate](const CVirtualHistoryCandleExtendPtr& p) { return (p->GetDate() == lDate); });
	if (it != m_vHistoryData.end()) {
		return *it;
	}
	else return nullptr;
}

void CVirtualDataHistoryCandleExtend::Reset() {
	m_vHistoryData.clear();

	m_fDataLoaded = false;
	m_fDatabaseTodayUpdated = false;

	m_fBasicDataLoaded = false;
}

bool CVirtualDataHistoryCandleExtend::CalculateRS0() {
	CalculateRS1(3);
	CalculateRS1(5);
	CalculateRS1(10);
	CalculateRS1(30);
	CalculateRS1(60);
	CalculateRS1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSLogarithm0() {
	CalculateRSLogarithm1(3);
	CalculateRSLogarithm1(5);
	CalculateRSLogarithm1(10);
	CalculateRSLogarithm1(30);
	CalculateRSLogarithm1(60);
	CalculateRSLogarithm1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSIndex0() {
	CalculateRSIndex1(3);
	CalculateRSIndex1(5);
	CalculateRSIndex1(10);
	CalculateRSIndex1(30);
	CalculateRSIndex1(60);
	CalculateRSIndex1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSLogarithm1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRSLogarithm(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRS1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRS(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSIndex1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) {
			dTempRS += m_vHistoryData.at(j)->GetRSIndex();
		}
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

void CVirtualDataHistoryCandleExtend::GetRS1(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSIndex(); }
}

void CVirtualDataHistoryCandleExtend::GetRSIndex1(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSIndex(); }
}

void CVirtualDataHistoryCandleExtend::GetRSLogarithm1(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSLogarithm(); }
}

void CVirtualDataHistoryCandleExtend::GetRS3(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get3RS(); }
}

void CVirtualDataHistoryCandleExtend::GetRS5(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get5RS(); }
}

void CVirtualDataHistoryCandleExtend::GetRS10(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get10RS(); }
}

void CVirtualDataHistoryCandleExtend::GetRS30(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get30RS(); }
}

void CVirtualDataHistoryCandleExtend::GetRS60(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get60RS(); }
}

void CVirtualDataHistoryCandleExtend::GetRS120(vector<double>& vRS) const {
	for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get120RS(); }
}

void CVirtualDataHistoryCandleExtend::ToShow(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
	auto it = m_vHistoryData.end();
	--it;
	size_t i = 0;
	auto pOldPen = pDC->SelectObject(&pNewPen);
	for (; it != m_vHistoryData.begin(); --it) {
		const long x = rectClient.right - 2 - i * iStepWidth;
		int y = (1 - static_cast<double>((*it)->GetHigh() - lLow) / (lHigh - lLow)) * rectClient.Height();
		pDC->MoveTo(x, y);
		if ((*it)->GetHigh() == (*it)->GetLow()) {
			y = y - 1;
		}
		else {
			y = (1 - static_cast<double>((*it)->GetLow() - lLow) / (lHigh - lLow)) * rectClient.Height();
		}
		pDC->LineTo(x, y);
		i++;
		if (i >= m_vHistoryData.size()) break;
		if (rectClient.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pOldPen);
}

void CVirtualDataHistoryCandleExtend::ShowRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get3RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get5RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get10RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get30RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get60RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get120RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRSIndex() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRSLogarithm() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRS() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandleExtend::ShowLine(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, const vector<double>& vData) {
	auto it = vData.begin();
	int i = 1;

	int y = rectDrawArea.bottom - *it * rectDrawArea.Height() / 100;
	auto pLodPen = pDC->SelectObject(pNewPen);
	pDC->MoveTo(rectDrawArea.right - 1, y);
	for (; it != vData.end(); ++it, i++) {
		y = rectDrawArea.bottom - *it * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - 1 - iStepWidth * i, y);
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pLodPen);
}

std::pair<long, long> CVirtualDataHistoryCandleExtend::GetHighLow(int iCandleNumber) {
	long lHigh = 0;
	auto it = m_vHistoryData.end();
	--it;
	int i = 0;
	long lLow = (*it)->GetLow();
	for (; it != m_vHistoryData.begin(); --it) {
		if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
		if ((*it)->GetLow() > 0) { if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow(); }
		if (i > m_vHistoryData.size()) break;
		if (i >= iCandleNumber) break;
		i++;
	}
	return { lHigh, lLow };
}
