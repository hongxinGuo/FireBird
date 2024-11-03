#include"pch.h"

#include"TiingoStock.h"

#include "ConvertToString.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

CTiingoStock::CTiingoStock() {
	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
	SetExchangeCode(_T("US"));
	CTiingoStock::ResetAllUpdateDate();
}

void CTiingoStock::ResetAllUpdateDate() {
	SetStatementLastUpdatedDate(0);
	SetCompanyFinancialStatementUpdateDate(19800101);
	SetDayLineUpdateDate(19800101);
	SetDayLineStartDate(29900101);
	SetDayLineEndDate(19800101);
	SetDayLineProcessDate(19800101);
	Set52WeekLow();
	Set52WeekHigh();
}

void CTiingoStock::Load(CSetTiingoStock& setTiingoStock) {
	m_strTiingoPermaTicker = setTiingoStock.m_TiingoPermaTicker;
	m_strSymbol = setTiingoStock.m_Ticker;
	m_strName = setTiingoStock.m_Name;
	m_fIsActive = setTiingoStock.m_IsActive;
	m_fIsADR = setTiingoStock.m_IsADR;
	m_iSicCode = setTiingoStock.m_SicCode;
	m_strSicIndustry = setTiingoStock.m_SicIndustry;
	m_strSicSector = setTiingoStock.m_SicSector;
	m_strTiingoIndustry = setTiingoStock.m_TiingoIndustry;
	m_strTiingoSector = setTiingoStock.m_TiingoSector;
	m_strReportingCurrency = setTiingoStock.m_ReportingCurrency;
	m_strLocation = setTiingoStock.m_Location;
	m_strCompanyWebSite = setTiingoStock.m_CompanyWebSite;
	m_strSECFilingWebSite = setTiingoStock.m_SECFilingWebSite;
	m_lIPOStatus = setTiingoStock.m_IPOStatus;

	LoadUpdateDate(setTiingoStock.m_UpdateDate);
	Get52WeekHigh();
	Get52WeekLow();
}

void CTiingoStock::Append(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.AddNew();
	Save(setTiingoStock);
	ASSERT(setTiingoStock.Update());
}

void CTiingoStock::Save(CSetTiingoStock& setTiingoStock) {
	if ((m_strCompanyWebSite.GetLength() > 100)
		|| (m_strName.GetLength() > 200)
		|| (m_strSECFilingWebSite.GetLength() > 150)
		|| (m_strSicIndustry.GetLength() > 100)
		|| (m_strSicSector.GetLength() > 100)
		|| (m_strTiingoIndustry.GetLength() > 100)
		|| (m_strTiingoSector.GetLength() > 100)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo stock ") + m_strName + _T(" �ַ���̫��"));
	}
	m_strName = m_strName.Left(200);
	m_strSicIndustry = m_strSicIndustry.Left(100);
	m_strSicSector = m_strSicSector.Left(100);
	m_strTiingoIndustry = m_strTiingoIndustry.Left(100);
	m_strTiingoSector = m_strTiingoSector.Left(100);
	m_strCompanyWebSite = m_strCompanyWebSite.Left(100);
	m_strSECFilingWebSite = m_strSECFilingWebSite.Left(150);

	setTiingoStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
	setTiingoStock.m_Ticker = m_strSymbol;
	setTiingoStock.m_Name = m_strName;
	setTiingoStock.m_IsActive = m_fIsActive;
	setTiingoStock.m_IsADR = m_fIsADR;
	setTiingoStock.m_SicCode = m_iSicCode;
	setTiingoStock.m_SicIndustry = m_strSicIndustry;
	setTiingoStock.m_SicSector = m_strSicSector;
	setTiingoStock.m_TiingoIndustry = m_strTiingoIndustry;
	setTiingoStock.m_TiingoSector = m_strTiingoSector;
	setTiingoStock.m_ReportingCurrency = m_strReportingCurrency;
	setTiingoStock.m_Location = m_strLocation;
	setTiingoStock.m_CompanyWebSite = m_strCompanyWebSite;
	setTiingoStock.m_SECFilingWebSite = m_strSECFilingWebSite;
	setTiingoStock.m_IPOStatus = m_lIPOStatus;

	Set52WeekHigh();
	Set52WeekLow();
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setTiingoStock.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CTiingoStock::Update(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.Edit();
	Save(setTiingoStock);
	setTiingoStock.Update();
}

void CTiingoStock::UpdateRTData(const CTiingoIEXTopOfBookPtr& pIEXTopOfBook) {
	m_TransactionTime = pIEXTopOfBook->m_llTimestamp;
	m_lOpen = pIEXTopOfBook->m_lOpen;
	m_lHigh = pIEXTopOfBook->m_lHigh;
	m_lLow = pIEXTopOfBook->m_lLow;
	m_lLastClose = pIEXTopOfBook->m_lLastClose;
	m_lNew = pIEXTopOfBook->m_lNew;
	m_llVolume = pIEXTopOfBook->m_llVolume;
}

void CTiingoStock::UpdateFinancialStateDB() {
	ASSERT(m_pvFinancialState != nullptr);
	CSetTiingoCompanyFinancialState setFinancialState;
	vector<CTiingoCompanyFinancialStatePtr> vOldFinancialState;
	CTiingoCompanyFinancialStatePtr pTiingoFinancialState = nullptr;
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
			pTiingoFinancialState = make_shared<CTiingoCompanyFinancialState>();
			pTiingoFinancialState->Load(setFinancialState);

			vOldFinancialState.push_back(pTiingoFinancialState);
			lSizeOfOldDayLine++;
		}
		else {
			setFinancialState.Delete(); //ɾ�������ظ�������
		}
		setFinancialState.MoveNext();
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();

	setFinancialState.m_strFilter = _T("[ID] = 1");
	setFinancialState.Open();
	setFinancialState.m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// �о�����
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pTiingoFinancialState = m_pvFinancialState->at(i);
			if (pTiingoFinancialState->m_yearQuarter < vOldFinancialState.at(0)->m_yearQuarter) {	// �и���������ݣ�
				pTiingoFinancialState->Append(setFinancialState);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && vOldFinancialState.at(lCurrentPos)->m_yearQuarter < pTiingoFinancialState->m_yearQuarter) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldFinancialState.at(lCurrentPos)->m_yearQuarter > pTiingoFinancialState->m_yearQuarter) { // ǰ���ݼ�������©������
						pTiingoFinancialState->Append(setFinancialState);
					}
				}
				else {
					pTiingoFinancialState->Append(setFinancialState);
				}
			}
		}
	}
	else {// û�о�����
		for (int i = 0; i < lSize; i++) {	// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pTiingoFinancialState = m_pvFinancialState->at(i);
			pTiingoFinancialState->Append(setFinancialState);
		}
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();

	SetCompanyFinancialStatementUpdateDate(gl_pWorldMarket->GetMarketDate());
	SetUpdateProfileDB(true);
}

bool CTiingoStock::UpdateDayLineDB() {
	if (IsUpdateDayLineDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
		if (GetDayLineSize() > 0) {
			if (HaveNewDayLineData()) {
				SaveDayLine();
				UpdateDayLineStartEndDate();
				SetUpdateProfileDB(true);
				const CString str = GetSymbol() + _T("�������ϴ洢���");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("����%s��������\n", GetSymbol().GetBuffer());
				UnloadDayLine();
				return true;
			}
			UnloadDayLine();
		}
	}
	return false;
}

void CTiingoStock::SaveCurrentDataToDayLineDB(CSetTiingoStockDayLine& setDayLine, long lTradeDay) const {
	setDayLine.AddNew();
	setDayLine.m_Date = lTradeDay;
	setDayLine.m_Symbol = m_strSymbol;
	setDayLine.m_Exchange = m_strExchangeCode;
	setDayLine.m_DisplaySymbol = _T("");
	setDayLine.m_Open = ConvertValueToString(m_lOpen, GetRatio());
	setDayLine.m_High = ConvertValueToString(m_lHigh, GetRatio());
	setDayLine.m_Low = ConvertValueToString(m_lLow, GetRatio());
	setDayLine.m_Close = ConvertValueToString(m_lNew, GetRatio());
	setDayLine.m_LastClose = ConvertValueToString(m_lLastClose, GetRatio());
	setDayLine.m_Volume = ConvertValueToString(m_llVolume);
	setDayLine.Update();
}

void CTiingoStock::UpdateProfile(const CTiingoStockPtr& pStock) {
	ASSERT(gl_systemConfiguration.IsPaidTypeTiingoAccount()); // ���ô˺���ʱ�����뱣֤�Ǹ����˻���
	m_strTiingoSector = pStock->m_strTiingoSector;
	m_strTiingoIndustry = pStock->m_strTiingoIndustry;
	m_iSicCode = pStock->m_iSicCode;
	m_strSicSector = pStock->m_strSicSector;
	m_strSicIndustry = pStock->m_strSicIndustry;
	m_strLocation = pStock->m_strLocation;
	m_strCompanyWebSite = pStock->m_strCompanyWebSite;
	m_strSECFilingWebSite = pStock->m_strSECFilingWebSite;
	SetUpdateProfileDB(true);
}

void CTiingoStock::UpdateDayLineStartEndDate() {
	long lStartDate = 0, lEndDate = 0;
	if (m_dataDayLine.GetStartEndDate(lStartDate, lEndDate)) {
		if (lStartDate < GetDayLineStartDate()) {
			SetDayLineStartDate(lStartDate);
			m_fUpdateProfileDB = true;
		}
		if (lEndDate > GetDayLineEndDate()) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CTiingoStock::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate())
		|| (m_dataDayLine.GetData(0)->GetMarketDate() < GetDayLineStartDate()))
		return true;
	return false;
}

void CTiingoStock::CheckUpdateStatus(long lTodayDate) {
	CheckFinancialStateUpdateStatus(lTodayDate);
	CheckIPOStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate, gl_pWorldMarket->GetLastTradeDate(), gl_pWorldMarket->GetMarketTime(), gl_pWorldMarket->GetDayOfWeek());
}

void CTiingoStock::CheckFinancialStateUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetCompanyFinancialStatementUpdateDate(), lTodayDate, gl_systemConfiguration.GetTiingoCompanyFinancialStateUpdateRate())) {
		m_fUpdateFinancialState = true;
	}
	else {
		m_fUpdateFinancialState = false;
	}
}

void CTiingoStock::CheckIPOStatus(long lCurrentDate) {
	if (!IsDelisted()) {
		if (IsEarlyThen(GetDayLineEndDate(), lCurrentDate, 30)) {
			SetIPOStatus(_STOCK_DELISTED_);
			SetUpdateProfileDB(true);
		}
	}
}

bool CTiingoStock::CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek) {
	ASSERT(IsUpdateDayLine()); // Ĭ��״̬Ϊ����������Ҫ����
	if (IsNullStock()) {
		SetUpdateDayLine(false);
		return m_fUpdateDayLine;
	}
	if (IsDelisted() || IsNotYetList()) {	// ժ�ƹ�Ʊ?
		if (lDayOfWeek != 3) {
			// ÿ���������һ��
			SetUpdateDayLine(false);
			return m_fUpdateDayLine;
		}
	}
	else if ((!IsNotChecked()) && (IsEarlyThen(GetDayLineEndDate(), gl_pWorldMarket->GetMarketDate(), 100))) {
		SetUpdateDayLine(false);
		return m_fUpdateDayLine;
	}
	else {
		if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) { // ��һ������
			if (lTime > 170000) {
				if (lTodayDate <= GetDayLineEndDate()) { // ������������Ϊ���յ����ݣ�����ǰʱ��Ϊ������ʱ֮��
					SetUpdateDayLine(false); // �������ݲ���Ҫ����
					return m_fUpdateDayLine;
				}
			}
			else {
				if (lLastTradeDate <= GetDayLineEndDate()) {// ������������Ϊ��һ�������յ�����,����ǰʱ��Ϊ������ʱ֮ǰ��
					SetUpdateDayLine(false); // �������ݲ���Ҫ����
					return m_fUpdateDayLine;
				}
			}
		}
		else if (lLastTradeDate <= GetDayLineEndDate()) { // ��������ʱ�� ������������Ϊ��һ�������յ�����
			SetUpdateDayLine(false); // �������ݲ���Ҫ����
			return m_fUpdateDayLine;
		}
	}
	return m_fUpdateDayLine;
}

long CTiingoStock::GetDayLineProcessDate() {
	long l;
	try {
		l = m_jsonUpdateDate[_T("DayLineProcessDate")];
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineProcessDate")] = 19800101;
		l = 19800101;
	}
	return l;
}

void CTiingoStock::Get52WeekLow() {
	try {
		m_v52WeekLow.clear();
		json j = m_jsonUpdateDate["52WeekLow"];
		for (auto it = j.begin(); it != j.end(); ++it) {
			m_v52WeekLow.push_back(*it);
		}
	} catch (json::exception&) { // �����ڴ���
		// do nothing
	}
}

void CTiingoStock::Get52WeekHigh() {
	try {
		m_v52WeekHigh.clear();
		json j = m_jsonUpdateDate["52WeekHigh"];
		for (auto it = j.begin(); it != j.end(); ++it) {
			m_v52WeekHigh.push_back(*it);
		}
	} catch (json::exception&) { // �����ڴ���
		// do nothing
	}
}

void CTiingoStock::Delete52WeekLow(long lDate) {
	auto it = std::ranges::find(m_v52WeekLow, lDate);
	if (it != m_v52WeekLow.end()) {
		m_v52WeekLow.erase(it);
	}
}

void CTiingoStock::Delete52WeekHigh(long lDate) {
	auto it = std::ranges::find(m_v52WeekHigh, lDate);
	if (it != m_v52WeekHigh.end()) {
		m_v52WeekHigh.erase(it);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������
//todo Ŀǰֻ����52���µͺ�52���¸ߡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoStock::ProcessDayLine() {
	m_dataDayLine.LoadDB(GetSymbol());

	size_t iBeginPos = 0;
	if (GetDayLineProcessDate() == 19800101) { // ���û�д����
		iBeginPos = 250;
		// �����֮ǰ������
		m_v52WeekHigh.clear();
		m_v52WeekLow.clear();
	}
	else {
		while (m_dataDayLine.GetData(iBeginPos)->GetMarketDate() < GetDayLineProcessDate()) iBeginPos++;
	}
	auto dayLineSize = m_dataDayLine.Size();
	for (size_t index = iBeginPos; index < dayLineSize; index++) {
		long lClose = m_dataDayLine.GetData(index)->GetClose();
		switch (IsLowOrHigh(index, lClose)) {
		case -1: // new low
			Add52WeekLow(m_dataDayLine.GetData(index)->GetMarketDate());
			break;
		case 1: // new high
			Add52WeekHigh(m_dataDayLine.GetData(index)->GetMarketDate());
			break;
		default:
			break;
		}
	}
	m_dataDayLine.Unload();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// Note tiingo����������û�г�Ȩ��ֱ��ʹ�õ�ǰֵ�����³�Ȩ��һ���ڵ������޷���ȷ����52���µͺ��¸ߡ�
/// ĿǰTiingo.com�ĳ�Ȩ��ϢΪBeta״̬����Ҫ�������롣
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////
int CTiingoStock::IsLowOrHigh(size_t index, long lClose) const {
	bool fIsNewLow = true;
	bool fIsNewHigh = true;
	for (size_t i = index - 250; i < index; i++) {
		if (m_dataDayLine.GetData(i)->GetClose() <= lClose) {
			fIsNewLow = false;
			break;
		}
	}
	if (fIsNewLow) return -1; // 52���µͼ�
	for (size_t i = index - 250; i < index; i++) {
		if (m_dataDayLine.GetData(i)->GetClose() >= lClose) {
			fIsNewHigh = false;
			break;
		}
	}
	if (fIsNewHigh) return 1; // 52���¸߼�
	return 0; // �ȷ�52����߼����52����ͼ�
}
