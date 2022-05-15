#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"

#include"ChinaMarket.h"
#include "DataChinaStock.h"
#include"CallableFunction.h"
#include"Thread.h"

#include"SetRSStrongStock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrong2Stock.h"

CDataChinaStock::CDataChinaStock() {
	Reset();
}

CDataChinaStock::~CDataChinaStock() {
}

void CDataChinaStock::Reset(void) {
	m_vStock.resize(0);
	m_mapStock.clear();

	m_lNeteaseDayLineDataInquiringIndex = 0;
	m_lNeteaseRTDataInquiringIndex = 0;
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

long CDataChinaStock::GetActiveStockSize(void) {
	long lTotalActiveStock = 0;
	for (auto& pStock : m_vStock) {
		if (pStock->IsActive()) lTotalActiveStock++;
	}
	return lTotalActiveStock;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�strStockCode�Ƿ�Ϊ����A������Ĺ�Ʊ���롣
//		����A�ɴ�����600��601��ͷ������A�ɴ�����000��001��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::IsAStock(CString strStockCode) const {
	CString strSymbol = GetStockSymbol(strStockCode);
	if (IsShanghaiExchange(strStockCode)) {
		if ((strSymbol[0] == '6') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '1')) {
				return true;
			}
		}
	}
	else if (IsShenzhenExchange(strStockCode)) {
		if ((strSymbol[0] == '0') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '2')) {
				return true;
			}
		}
	}
	return(false);
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��ָ�롣
//	���û�ҵ��Ļ����ؿ�ָ��
//
//
////////////////////////////////////////////////////////////////////////////////
CChinaStockPtr CDataChinaStock::GetStock(CString strStockCode) {
	CChinaStockPtr pStock;
	if (!IsStock(strStockCode)) {
		return nullptr;
	}
	else {
		return (m_vStock.at(m_mapStock.at(strStockCode)));
	}
}

CChinaStockPtr CDataChinaStock::GetStock(long lIndex) {
	try {
		return m_vStock.at(lIndex);
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStock "), e);
		return nullptr;
	}
}

bool CDataChinaStock::Delete(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsStock(pStock->GetSymbol())) return false;

	m_vStock.erase(m_vStock.begin() + m_mapStock.at(pStock->GetSymbol()));
	m_mapStock.erase(pStock->GetSymbol());

	return true;
}

bool CDataChinaStock::Add(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (IsStock(pStock->GetSymbol())) return false;

	m_mapStock[pStock->GetSymbol()] = m_vStock.size(); // ʹ���±������µ�ӳ��
	m_vStock.push_back(pStock);

	return true;
}

long CDataChinaStock::LoadStockCodeDB(void) {
	CSetChinaStockSymbol setChinaStockSymbol;
	char buffer[30]{ 0, 0, 0 };
	CString str;
	long lDayLineNeedCheck = 0;

	setChinaStockSymbol.m_strSort = _T("[Symbol]");
	setChinaStockSymbol.Open();
	setChinaStockSymbol.m_pDatabase->BeginTrans();
	// װ���Ʊ�������ݿ�
	while (!setChinaStockSymbol.IsEOF()) {
		CChinaStockPtr pStock = make_shared<CChinaStock>();
		if (!IsStock(setChinaStockSymbol.m_Symbol)) {
			pStock->LoadStockCodeDB(setChinaStockSymbol);
			Add(pStock);
		}
		else {
			str = _T("�����ظ����룺");
			str += setChinaStockSymbol.m_Symbol;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setChinaStockSymbol.Delete(); // ɾ�����ظ�����
		}
		setChinaStockSymbol.MoveNext();
	}
	if (IsDayLineNeedUpdate()) {
		lDayLineNeedCheck = GetDayLineNeedUpdateNumber();
		if (gl_pChinaMarket->GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage(_T("ÿ����һ�������й�Ʊ����"));
		TRACE("����%d����Ʊ��Ҫ�����������\n", lDayLineNeedCheck);
		_itoa_s(lDayLineNeedCheck, buffer, 10);
		str = buffer;
		str += _T("����Ʊ��Ҫ�����������");
		gl_systemMessage.PushInformationMessage(str);
	}
	setChinaStockSymbol.m_pDatabase->CommitTrans();
	setChinaStockSymbol.Close();
	m_lLoadedStock = m_vStock.size();
	SortStockVector();

	return lDayLineNeedCheck;
}

bool CDataChinaStock::UpdateStockCodeDB(void) {
	CChinaStockPtr pStock = nullptr;
	CSetChinaStockSymbol setChinaStockSymbol;
	int iStockCodeNeedUpdate = 0;
	int iCount = 0;

	//����ԭ�еĴ��뼯״̬
	if (IsUpdateStockCodeDB()) {
		for (auto& pStock2 : m_vStock) {
			if (pStock2->IsUpdateProfileDB()) iStockCodeNeedUpdate++;
		}
		setChinaStockSymbol.m_strSort = _T("[Symbol]");
		setChinaStockSymbol.Open();
		setChinaStockSymbol.m_pDatabase->BeginTrans();
		while (iCount < iStockCodeNeedUpdate) {
			if (setChinaStockSymbol.IsEOF()) break;
			pStock = m_vStock.at(m_mapStock.at(setChinaStockSymbol.m_Symbol));
			if (pStock->IsUpdateProfileDBAndClearFlag()) {
				//ASSERT(!pStock3->IsTodayNewStock());
				iCount++;
				pStock->UpdateSymbol(setChinaStockSymbol);
			}
			setChinaStockSymbol.MoveNext();
		}
		if (iCount < iStockCodeNeedUpdate) {
			for (auto& pStock3 : m_vStock) {
				if (pStock3->IsUpdateProfileDBAndClearFlag()) {
					ASSERT(pStock3->IsTodayNewStock());
					iCount++;
					pStock3->AppendSymbol(setChinaStockSymbol);
					pStock3->SetTodayNewStock(false);
				}
				if (iCount >= iStockCodeNeedUpdate) break;
			}
		}
		setChinaStockSymbol.m_pDatabase->CommitTrans();
		setChinaStockSymbol.Close();
		m_lLoadedStock = m_vStock.size();
	}
	ASSERT(iCount == iStockCodeNeedUpdate);
	return true;
}

bool CDataChinaStock::IsDayLineDBUpdated(void) noexcept {
	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineDBUpdated()) return true;
	}
	return false;
}

void CDataChinaStock::ClearDayLineDBUpdatedFlag(void) noexcept {
	for (auto& pStock : m_vStock) {
		pStock->SetDayLineDBUpdated(false);
	}
}

INT64 CDataChinaStock::GetTotalAttackBuyAmount(void) {
	INT64 lAmount = 0;
	for (auto& pStock : m_vStock) {
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackBuyAmount();
		}
	}
	return(lAmount);
}

INT64 CDataChinaStock::GetTotalAttackSellAmount(void) {
	INT64 lAmount = 0;
	for (auto& pStock : m_vStock) {
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return(lAmount);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������߹�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
//  �˺����Ǽ��m_vStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::CreateNeteaseDayLineInquiringStr(CString& strReturn) {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	strReturn = _T("");

	while (!fFound && (iCount++ < GetStockSize())) {
		CChinaStockPtr pStock = m_vStock.at(m_lNeteaseDayLineDataInquiringIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
			// TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			GetNextIndex(m_lNeteaseDayLineDataInquiringIndex);
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) { //��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
			pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
			// TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			GetNextIndex(m_lNeteaseDayLineDataInquiringIndex);
		}
		else {
			fFound = true;
		}
	}

	if (iCount >= GetStockSize()) { //  û���ҵ���Ҫ�������ߵ�֤ȯ
		TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
		return false;
	}

	// �ҵ���������������ʷ���ݵĹ�Ʊ��siCounterΪ������
	CChinaStockPtr pStock = m_vStock.at(m_lNeteaseDayLineDataInquiringIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandredToNetease(pStock->GetSymbol());
	GetNextIndex(m_lNeteaseDayLineDataInquiringIndex);
	return true;
}

CString CDataChinaStock::GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber) {
	CString strReturn = _T("");
	CString strStockCode, strStockExchange, strStockSymbol;

	if (m_vStock.size() == 0) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vStock.at(iStockIndex)->GetSymbol());  // �õ���һ����Ʊ����
	GetNextIndex(iStockIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while (iCount < lTotalNumber) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		if (m_vStock.at(iStockIndex)->IsActive() || m_vStock.at(iStockIndex)->IsIPOed()) {
			iCount++;
			strReturn += strPostfix;
			strReturn += XferStandredToSina(m_vStock.at(iStockIndex)->GetSymbol());  // �õ���һ����Ʊ����
		}
		GetNextIndex(iStockIndex);
	}

	return strReturn;
}

CString CDataChinaStock::GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) {
	CString strStockCode, strRight6, strLeft2, strPrefix;
	CString strNeteaseRTDataInquiringStr = _T("");

	strNeteaseRTDataInquiringStr = XferStandredToNetease(m_vStock.at(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
	GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while (iCount < lTotalNumber) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		if (m_vStock.at(m_lNeteaseRTDataInquiringIndex)->IsActive() || m_vStock.at(m_lNeteaseRTDataInquiringIndex)->IsIPOed()) {
			iCount++;
			strNeteaseRTDataInquiringStr += _T(",");
			strNeteaseRTDataInquiringStr += XferStandredToNetease(m_vStock.at(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
		}
		GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	}

	return strNeteaseRTDataInquiringStr;
}

long CDataChinaStock::GetNextIndex(long& lIndex) {
	if (++lIndex >= m_vStock.size()) {
		lIndex = 0;
	}
	return lIndex;
}

bool CDataChinaStock::TaskProcessRTData(void) {
	ASSERT(gl_ThreadStatus.IsRTDataNeedCalculate() && !gl_ThreadStatus.IsSavingTempData());// �������̻߳���
	for (auto& pStock : m_vStock) {
		if (pStock->IsActive()) {
			pStock->ProcessRTData();
		}
	}
	return true;
}

void CDataChinaStock::ClearDayLineNeedUpdateStatus(void) {
	for (auto& pStock : m_vStock) {
		pStock->SetDayLineNeedUpdate(false);
	}
}

bool CDataChinaStock::IsUpdateStockCodeDB(void) {
	for (auto& pStock : m_vStock) {
		if (pStock->IsUpdateProfileDB()) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CDataChinaStock::GetStockName(CString strStockCode) {
	try {
		return (m_vStock.at(m_mapStock.at(strStockCode))->GetDisplaySymbol());
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStockName ") + strStockCode + _T(" "), e);
		return _T("");
	}
}

bool CDataChinaStock::UnloadDayLine(void) noexcept {
	for (auto& pStock : m_vStock) {
		pStock->UnloadDayLine();
	}

	return true;
}

bool CDataChinaStock::SortStockVector(void) {
	sort(m_vStock.begin(), m_vStock.end(), CompareChinaStock);
	m_mapStock.clear();
	int j = 0;
	for (auto& pStock : m_vStock) {
		pStock->SetOffset(j);
		ASSERT(!IsStock(pStock->GetSymbol()));
		m_mapStock[pStock->GetSymbol()] = j++;
	}
	ASSERT(m_vStock.size() == m_mapStock.size());
	return true;
}

bool CDataChinaStock::IsDayLineNeedUpdate(void) const noexcept {
	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedUpdate()) return true;
	}
	return false;
}

bool CDataChinaStock::IsDayLineNeedSaving(void) const {
	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedSaving()) {
			return true;
		}
	}
	return false;
}

long CDataChinaStock::GetDayLineNeedUpdateNumber(void) {
	long l = 0;
	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedUpdate()) l++;
	}
	return l;
}

long CDataChinaStock::GetDayLineNeedSaveNumber(void) {
	long l = 0;
	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedSaving()) l++;
	}
	return l;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//////////////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::TaskSaveDayLineData(void) {
	CString str;
	bool fSave = false;

	for (auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pStock->GetDayLineSize() > 0) {
				if (pStock->HaveNewDayLineData()) {
					thread thread1(ThreadSaveDayLineBasicInfoOfStock, pStock.get());
					thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
					fSave = true;
				}
				else pStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
				CString str1 = pStock->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return fSave;
}

bool CDataChinaStock::BuildWeekLine(long lStartDate) {
	gl_systemMessage.PushInformationMessage(_T("��������������ʷ����"));
	for (auto& pStock : m_vStock) {
		thread thread1(ThreadBuildWeekLineOfStock, pStock.get(), lStartDate);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		Sleep(1000);
	}
	gl_systemMessage.PushInformationMessage(_T("������ʷ�����������"));

	return true;
}

bool CDataChinaStock::Choice10RSStrong2StockSet(void) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vStock) {
		thread thread1(ThreadCalculate10RSStrong1Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	CSetRSStrong2Stock setRSStrong2;

	setRSStrong2.Open();
	setRSStrong2.m_pDatabase->BeginTrans();
	while (!setRSStrong2.IsEOF()) {
		setRSStrong2.Delete();
		setRSStrong2.MoveNext();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong2.AddNew();
		setRSStrong2.m_Symbol = pStock->GetSymbol();
		setRSStrong2.Update();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.Close();

	return true;
}

bool CDataChinaStock::Choice10RSStrong1StockSet(void) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vStock) {
		thread thread1(ThreadCalculate10RSStrong1Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	CSetRSStrong1Stock setRSStrong1;

	setRSStrong1.Open();
	setRSStrong1.m_pDatabase->BeginTrans();
	while (!setRSStrong1.IsEOF()) {
		setRSStrong1.Delete();
		setRSStrong1.MoveNext();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong1.AddNew();
		setRSStrong1.m_Symbol = pStock->GetSymbol();
		setRSStrong1.Update();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.Close();

	return true;
}

bool CDataChinaStock::Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vStock) {
		thread thread1(ThreadCalculate10RSStrongStock, &v10RSStrongStock, pRef, pStock);
		thread1.detach();
	}

	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	gl_pChinaMarket->SetCurrentRSStrongIndex(iIndex); // CSetRSStrongStock��Ҫ��m_lCurrentRSStrongIndex��ѡ����ȷ�����ݱ�
	CSetRSStrongStock setRSStrong(iIndex);

	setRSStrong.Open();
	setRSStrong.m_pDatabase->BeginTrans();
	while (!setRSStrong.IsEOF()) {
		setRSStrong.Delete();
		setRSStrong.MoveNext();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong.AddNew();
		setRSStrong.m_Symbol = pStock->GetSymbol();
		setRSStrong.Update();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.Close();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// ����ǰ�����յ�ʵʱ���ݣ��������߸��������ݣ����ǿ�ȡ������������̣���
//
// ֻ��������������ʷ���ݺ󣬷���ִ�д���ʵʱ���ݣ�����������й�Ʊ����������
//
// long lCurrentTradeDay ��ǰ�����ա����ڴ������������գ��ʶ������ڲ���һ�����ǵ�ǰ���ڣ�������ʱ����
//
//////////////////////////////////////////////////////////////////////////////////
long CDataChinaStock::BuildDayLine(long lCurrentTradeDay) {
	char buffer[20]{ 0 };
	CString strDate;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;
	long iCount = 0;

	CString str;
	str = _T("��ʼ����");
	str += ConvertDateToString(lCurrentTradeDay);
	str += _T("��ʵʱ����");
	gl_systemMessage.PushInformationMessage(str);

	DeleteDayLineBasicInfo(lCurrentTradeDay);
	DeleteDayLineExtendInfo(lCurrentTradeDay);

	// �洢��ǰ�����յ�����
	_ltoa_s(lCurrentTradeDay, buffer, 10);
	strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[ID] = 1");
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		iCount++;
		pStock->SetDayLineEndDate(lCurrentTradeDay);
		pStock->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
		pStock->SetUpdateProfileDB(true);
		pStock->AppendTodayBasicInfo(&setDayLineBasicInfo);
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	// �洢�������ɵ�������DayLineExtendInfo���С�
	setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		pStock->AppendTodayExtendInfo(&setDayLineExtendInfo);
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	str = ConvertDateToString(lCurrentTradeDay);
	str += _T("��ʵʱ���ݴ������");
	gl_systemMessage.PushInformationMessage(str);

	sprintf_s(buffer, _T("%d"), iCount);
	str = _T("���մ�����");
	str += buffer;
	str += _T("����Ʊ");
	gl_systemMessage.PushInformationMessage(str);

	return iCount;
}

bool CDataChinaStock::DeleteDayLineBasicInfo(long lDate) {
	char buffer[20]{ 0x000 };
	CString strDate;
	CSetDayLineBasicInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	return true;
}

bool CDataChinaStock::DeleteDayLineExtendInfo(long lDate) {
	char buffer[20]{ 0x000 };
	CString strDate;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	_ltoa_s(lDate, buffer, 10);
	strDate = buffer;
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Delete();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// �����մ���õ����ݴ��������ݿ��У��Ա���һϵͳ����ʱ����װ�롣
// �ƺ�Ӧ����һ����ʱ�����̵߳���ʽ���ڡ�
//
// �о�֮��
//
//////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::UpdateTodayTempDB(void) {
	CSetDayLineTodaySaved setDayLineTemp;
	CString str;

	DeleteTodayTempDB();

	// �洢�������ɵ�������DayLineToday���С�
	setDayLineTemp.m_strFilter = _T("[ID] = 1");
	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
			str = pStock->GetSymbol();
			str += _T(" ��������ȷ");
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
		setDayLineTemp.AddNew();
		pStock->SaveTempInfo(&setDayLineTemp);
		setDayLineTemp.Update();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();

	return true;
}

////////////////////////////////////////////////////////////////////
//
// �˺���ʹ��Ӳ���룬���������
//
////////////////////////////////////////////////////////////////////
bool CDataChinaStock::DeleteTodayTempDB(void) {
	CDatabase database;

	if (!gl_fNormalMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1); //�˳�ϵͳ
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`today`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::BuildDayLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	CString strDate;
	char  pch[30];
	int iStockNumber = 0;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dIndexUpDownRate{ 0.0 };
	double dRSIndex{ 0.0 };

	sprintf_s(pch, _T("%08d"), lDate);
	strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
		gl_systemMessage.PushInformationMessage(str);    // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	iStockNumber = 0;
	while (!setDayLineBasicInfo.IsEOF()) {
		if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sh000001")) == 0) { // �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sz399001")) == 0) { // ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setDayLineBasicInfo.m_Symbol)) {
			const long lIndex = m_mapStock.at(setDayLineBasicInfo.m_Symbol);
			vStock.push_back(m_vStock.at(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setDayLineBasicInfo.MoveNext();
	}
	dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setDayLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.Edit();
		const double dLastClose = atof(setDayLineBasicInfo.m_LastClose);
		const double dLow = atof(setDayLineBasicInfo.m_Low);
		const double dHigh = atof(setDayLineBasicInfo.m_High);
		const double dClose = atof(setDayLineBasicInfo.m_Close);
		double dUpDownRate = 0;
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) { // �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			dUpDownRate = (dClose - dLastClose) / dLastClose;
			if ((dUpDownRate > 0.11) || (dUpDownRate < -0.11)) { // ��Ȩ�ȵ��¼۸�ͻ��
				dRSIndex = 50;
			}
			else {
				dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
			}
		}
		setDayLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// �����ǵ��������ǿ��
		if (dLastClose < 0.001) {
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else if (((dLow / dLastClose) < 0.88) || ((dHigh / dLastClose) > 1.12)) { // ��Ȩ���¹����е�
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) { // ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(100);
		}
		else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) { // ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(0);
		}
		else {
			setDayLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
		}
		setDayLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setDayLineBasicInfo.MoveNext(); // �Ƶ���һ�����ݡ�
		iBefore++; // ������Ҳͬʱ��һ��
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	CString strDate2 = ConvertDateToString(lDate);
	CString strTemp;
	strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushDayLineInfoMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

	return(true);
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CDataChinaStock::BuildWeekLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	CString strDate;
	char  pch[30];
	int iStockNumber = 0;
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dIndexUpDownRate{ 0 };
	double dRSIndex{ 0 };

	ASSERT(GetCurrentMonday(lDate) == lDate); // ȷ��������Ϊ����һ

	sprintf_s(pch, _T("%08d"), lDate);
	strDate = pch;
	setWeekLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	if (setWeekLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
		setWeekLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
		gl_systemMessage.PushDayLineInfoMessage(str); // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	iStockNumber = 0;
	while (!setWeekLineBasicInfo.IsEOF()) {
		if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sh000001")) == 0) { // �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sz399001")) == 0) { // ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setWeekLineBasicInfo.m_Symbol)) {
			const long lIndex = m_mapStock.at(setWeekLineBasicInfo.m_Symbol);
			vStock.push_back(m_vStock.at(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setWeekLineBasicInfo.MoveNext();
	}
	dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setWeekLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.Edit();
		const double dLastClose = atof(setWeekLineBasicInfo.m_LastClose);
		const double dClose = atof(setWeekLineBasicInfo.m_Close);
		double dUpDownRate = 0;
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) { // �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			dUpDownRate = (dClose - dLastClose) / dLastClose;
			dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
		}
		setWeekLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// �����ǵ��������ǿ��
		if (dLastClose < 0.001) {
			setWeekLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else {
			setWeekLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
		}
		setWeekLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setWeekLineBasicInfo.MoveNext(); // �Ƶ���һ�����ݡ�
		iBefore++; // ������Ҳͬʱ��һ��
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	CString strDate2 = ConvertDateToString(lDate);
	CString strTemp;
	strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushInformationMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

	return(true);
}

double CDataChinaStock::GetUpDownRate(CString strClose, CString strLastClose) noexcept {
	const double lastClose = atof(strLastClose);
	if (lastClose < 0.001) return 0;
	double result = (atof(strClose) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}