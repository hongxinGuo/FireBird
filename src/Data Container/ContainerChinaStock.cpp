#include"pch.h"

#include"ConvertToString.h"
#include"ThreadStatus.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"
#include"RSReference.h"
#include"Thread.h"

#include"SetRSStrongStock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrong2Stock.h"

using namespace std;

CContainerChinaStock::CContainerChinaStock() {
	CContainerChinaStock::Reset();
}

void CContainerChinaStock::Reset() {
	CContainerVirtualStock::Reset();

	m_lLoadedStock = 0;

	m_lNeteaseRTDataInquiringIndex = 0;
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

long CContainerChinaStock::GetActiveStockSize() const {
	long lTotalActiveStock = 0;
	for (const auto& pStock : m_vStock) {
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
bool CContainerChinaStock::IsAStock(const CString& strStockCode) {
	const CString strSymbol = GetStockSymbol(strStockCode);
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
	return (false);
}

long CContainerChinaStock::LoadStockProfileDB() {
	CSetChinaStockSymbol setChinaStockSymbol;
	char buffer[30]{0, 0, 0};
	CString str;
	long lDayLineNeedCheck = 0;

	setChinaStockSymbol.m_strSort = _T("[Symbol]");
	setChinaStockSymbol.Open();
	setChinaStockSymbol.m_pDatabase->BeginTrans();
	// װ���Ʊ�������ݿ�
	while (!setChinaStockSymbol.IsEOF()) {
		const auto pStock = make_shared<CChinaStock>();
		if (!IsSymbol(setChinaStockSymbol.m_Symbol)) {
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
	Sort();

	return lDayLineNeedCheck;
}

bool CContainerChinaStock::UpdateStockProfileDB() {
	int iStockCodeNeedUpdate = 0;
	int iCount = 0;

	//����ԭ�еĴ��뼯״̬
	if (IsUpdateProfileDB()) {
		CSetChinaStockSymbol setChinaStockSymbol;
		for (const auto& pStock2 : m_vStock) {
			if (pStock2->IsUpdateProfileDB()) iStockCodeNeedUpdate++;
		}
		setChinaStockSymbol.m_strSort = _T("[Symbol]");
		setChinaStockSymbol.Open();
		setChinaStockSymbol.m_pDatabase->BeginTrans();
		while (iCount < iStockCodeNeedUpdate) {
			if (setChinaStockSymbol.IsEOF()) break;
			const CChinaStockPtr pStock = GetStock(setChinaStockSymbol.m_Symbol);
			if (pStock->IsUpdateProfileDBAndClearFlag()) {
				//ASSERT(!pStock3->IsTodayNewStock());
				iCount++;
				pStock->UpdateSymbol(setChinaStockSymbol);
			}
			setChinaStockSymbol.MoveNext();
		}
		if (iCount < iStockCodeNeedUpdate) {
			for (const auto& pStock3 : m_vStock) {
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

bool CContainerChinaStock::IsDayLineDBUpdated() noexcept {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CChinaStock>(pStock)->IsDayLineDBUpdated(); });
}

void CContainerChinaStock::ClearDayLineDBUpdatedFlag() noexcept {
	for (long l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->SetDayLineDBUpdated(false);
	}
}

INT64 CContainerChinaStock::GetTotalAttackBuyAmount() {
	INT64 lAmount = 0;
	for (long l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackBuyAmount();
		}
	}
	return (lAmount);
}

INT64 CContainerChinaStock::GetTotalAttackSellAmount() {
	INT64 lAmount = 0;
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return (lAmount);
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
CString CContainerChinaStock::CreateNeteaseDayLineInquiringStr() {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	CString strReturn = _T("");
	long lIndex = 0;

	while (!fFound && (iCount++ < Size())) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
			// TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
			pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
			// TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else { fFound = true; }
	}

	if (iCount >= Size()) {
		//  û���ҵ���Ҫ�������ߵ�֤ȯ
		TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
		return _T("");
	}

	// �ҵ���������������ʷ���ݵĹ�Ʊ
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandardToNetease(pStock->GetSymbol());
	return strReturn;
}

CString CContainerChinaStock::GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber) {
	if (m_vStock.empty()) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	CString strReturn = XferStandardToSina(GetStock(iStockIndex)->GetSymbol()); // �õ���һ����Ʊ����
	iStockIndex = GetNextIndex(iStockIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while (iCount < lTotalNumber) {
		// ÿ������ѯ��ΪlTotalNumber����Ʊ
		if (GetStock(iStockIndex)->IsActive() || GetStock(iStockIndex)->IsIPOed()) {
			iCount++;
			strReturn += strPostfix;
			strReturn += XferStandardToSina(GetStock(iStockIndex)->GetSymbol()); // �õ���һ����Ʊ����
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
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
CString CContainerChinaStock::CreateTengxunDayLineInquiringStr() {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	CString strReturn = _T("");
	long lIndex = 0;

	while (!fFound && (iCount++ < Size())) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
			// TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
			pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
			// TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else { fFound = true; }
	}

	if (iCount >= Size()) {
		//  û���ҵ���Ҫ�������ߵ�֤ȯ
		TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
		return _T("");
	}

	// �ҵ���������������ʷ���ݵĹ�Ʊ
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandardToTengxun(pStock->GetSymbol());
	return strReturn;
}

CString CContainerChinaStock::GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber) {
	CString strNeteaseRTDataInquiringStr = XferStandardToNetease(GetStock(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
	m_lNeteaseRTDataInquiringIndex = GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while (iCount < lTotalNumber) {	// ÿ������ѯ��ΪlTotalNumber����Ʊ
		if (GetStock(m_lNeteaseRTDataInquiringIndex)->IsActive() || GetStock(m_lNeteaseRTDataInquiringIndex)->IsIPOed()) {//if (GetStock(m_lNeteaseRTDataInquiringIndex)->IsActive()) {
			iCount++;
			strNeteaseRTDataInquiringStr += _T(",");
			strNeteaseRTDataInquiringStr += XferStandardToNetease(GetStock(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
		}
		m_lNeteaseRTDataInquiringIndex = GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	}

	return strNeteaseRTDataInquiringStr;
}

long CContainerChinaStock::GetNextIndex(long lIndex) const {
	if (++lIndex >= m_vStock.size()) { lIndex = 0; }
	return lIndex;
}

bool CContainerChinaStock::ProcessRTData() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive()) {
			pStock->ProcessRTData();
		}
	}
	return true;
}

void CContainerChinaStock::ClearDayLineNeedUpdateStatus() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetDayLineNeedUpdate(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CContainerChinaStock::GetStockName(const CString& strStockCode) {
	try { return (GetStock(m_mapSymbol.at(strStockCode))->GetDisplaySymbol()); }
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStockName ") + strStockCode + _T(" "), e);
		return _T("");
	}
}

void CContainerChinaStock::UnloadDayLine() noexcept {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->UnloadDayLine();
	}
}

void CContainerChinaStock::SetDayLineNeedMaintain() const {
	SetDayLineNeedUpdate();
	for (auto& pStock : m_vStock) {
		pStock->SetDayLineEndDate(19900101);
	}
}

void CContainerChinaStock::SetDayLineNeedUpdate() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetDayLineNeedUpdate(true);
	}
}

long CContainerChinaStock::GetDayLineNeedUpdateNumber() const {
	long l = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedUpdate()) l++;
	}
	return l;
}

long CContainerChinaStock::GetDayLineNeedSaveNumber() const {
	long l = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedSaving()) ++l;
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
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::SaveDayLineData() {
	CString str;
	bool fSave = false;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsDayLineNeedSavingAndClearFlag()) {
			// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pStock->GetDayLineSize() > 0) {
				if (pStock->HaveNewDayLineData()) {
					CreateThreadSaveDayLineBasicInfo(pStock);
					fSave = true;
				}
				else pStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else {
				// �������Ϊ�й�Ʊ���룬���˴�����δ����
				pStock->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				CString str1 = pStock->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return fSave;
}

void CContainerChinaStock::CreateThreadSaveDayLineBasicInfo(CChinaStockPtr pStock) {
	thread thread1(ThreadSaveDayLineBasicInfo, pStock.get());
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

bool CContainerChinaStock::BuildWeekLine(long lStartDate) {
	gl_systemMessage.PushInformationMessage(_T("��������������ʷ����"));
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadBuildWeekLineOfStock, pStock.get(), lStartDate);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) { Sleep(100); }
	gl_systemMessage.PushInformationMessage(_T("������ʷ�����������"));

	return true;
}

bool CContainerChinaStock::Choice10RSStrong2StockSet() {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrong2Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
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
	for (const auto& pStock : v10RSStrongStock) {
		setRSStrong2.AddNew();
		setRSStrong2.m_Symbol = pStock->GetSymbol();
		setRSStrong2.Update();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.Close();

	return true;
}

bool CContainerChinaStock::Choice10RSStrong1StockSet() {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrong1Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
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
	for (const auto& pStock : v10RSStrongStock) {
		setRSStrong1.AddNew();
		setRSStrong1.m_Symbol = pStock->GetSymbol();
		setRSStrong1.Update();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.Close();

	return true;
}

bool CContainerChinaStock::Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrongStock, &v10RSStrongStock, pRef, pStock);
		thread1.detach();
	}

	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
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
	for (const auto& pStock : v10RSStrongStock) {
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
long CContainerChinaStock::BuildDayLine(long lCurrentTradeDay) {
	char buffer[20]{0};
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;
	long iCount = 0;

	CString str = "��ʼ����";
	str += ConvertDateToChineseTimeStampString(lCurrentTradeDay);
	str += _T("��ʵʱ����");
	gl_systemMessage.PushInformationMessage(str);

	DeleteDayLineBasicInfo(lCurrentTradeDay);
	DeleteDayLineExtendInfo(lCurrentTradeDay);

	// �洢��ǰ�����յ�����
	_ltoa_s(lCurrentTradeDay, buffer, 10);
	CString strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[ID] = 1");
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		iCount++;
		pStock->SetDayLineEndDate(lCurrentTradeDay);
		pStock->SetIPOStatus(_STOCK_IPOED_); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
		pStock->SetUpdateProfileDB(true);
		pStock->AppendTodayBasicInfo(&setDayLineBasicInfo);
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	// �洢�������ɵ�������DayLineExtendInfo���С�
	setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		pStock->AppendTodayExtendInfo(&setDayLineExtendInfo);
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	str = ConvertDateToChineseTimeStampString(lCurrentTradeDay);
	str += _T("����������������");
	gl_systemMessage.PushInformationMessage(str);

	sprintf_s(buffer, _T("%d"), iCount);
	str = _T("���մ�����");
	str += buffer;
	str += _T("����Ʊ");
	gl_systemMessage.PushInformationMessage(str);

	return iCount;
}

bool CContainerChinaStock::DeleteDayLineBasicInfo(long lDate) {
	char buffer[20]{0x000};
	CSetDayLineBasicInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
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

bool CContainerChinaStock::DeleteDayLineExtendInfo(long lDate) {
	char buffer[20]{0x000};
	CSetDayLineExtendInfo setDayLineExtendInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
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
//
// ����ģʽ�£�ʹ��ԭʼ������ɾ��������ʱ3.5������,ʹ��ExecuteSQL(_T("TRUNCATE `chinamarket`.`today`;"))��ʱ3.1�롣û�������ĸ��ơ�
// ����ֻʹ��ԭʼ������ɾ��ģʽ��
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::SaveTempRTData() {
	CSetDayLineTodaySaved setDayLineTemp;
	CHighPerformanceCounter counter;
	counter.start();

	DeleteTempRTData();

	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	// �洢�������ɵ�������DayLineToday���С�
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
			CString str = pStock->GetSymbol();
			str += _T(" ��������ȷ");
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
		setDayLineTemp.AddNew();
		pStock->SaveTempInfo(&setDayLineTemp);
		setDayLineTemp.Update();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();
	counter.stop();
	char buffer[30];
	sprintf_s(buffer, _T("%lld"), counter.GetElapsedMilliSecond());
	CString str = "�洢ʵʱ������ʱ��";
	str += buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);

	return true;
}

void CContainerChinaStock::DeleteTempRTData() {
	CSetDayLineTodaySaved setDayLineTemp;

	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	while (!setDayLineTemp.IsEOF()) {
		setDayLineTemp.Delete();
		setDayLineTemp.MoveNext();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::BuildDayLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	char pch[30];
	CSetDayLineBasicInfo setDayLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dRSIndex;

	sprintf_s(pch, _T("%08d"), lDate);
	const CString strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) {
		// ���ݼ�Ϊ�գ���������û�н���
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
		gl_systemMessage.PushInformationMessage(str); // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	int iStockNumber = 0;
	while (!setDayLineBasicInfo.IsEOF()) {
		if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sh000001")) == 0) {
			// �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sz399001")) == 0) {
			// ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setDayLineBasicInfo.m_Symbol)) {
			const auto lIndex = m_mapSymbol.at(setDayLineBasicInfo.m_Symbol);
			vStock.push_back(GetStock(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setDayLineBasicInfo.MoveNext();
	}
	const double dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setDayLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) {
		// ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) {
			// ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.Edit();
		const double dLastClose = atof(setDayLineBasicInfo.m_LastClose);
		const double dLow = atof(setDayLineBasicInfo.m_Low);
		const double dHigh = atof(setDayLineBasicInfo.m_High);
		const double dClose = atof(setDayLineBasicInfo.m_Close);
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) {
			// �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			const double dUpDownRate = (dClose - dLastClose) / dLastClose;
			if ((dUpDownRate > 0.11) || (dUpDownRate < -0.11)) {
				// ��Ȩ�ȵ��¼۸�ͻ��
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
		else if (((dLow / dLastClose) < 0.88) || ((dHigh / dLastClose) > 1.12)) {
			// ��Ȩ���¹����е�
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) {
			// ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(100);
		}
		else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) {
			// ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(0);
		}
		else { setDayLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare); }
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

	const CString strDate2 = ConvertDateToChineseTimeStampString(lDate);
	const CString strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushDayLineInfoMessage(strTemp); // ����ͬ�����Ʊ�����Ϣ

	return (true);
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::BuildWeekLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	char pch[30];
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dRSIndex;

	ASSERT(GetCurrentMonday(lDate) == lDate); // ȷ��������Ϊ����һ

	sprintf_s(pch, _T("%08d"), lDate);
	const CString strDate = pch;
	setWeekLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	if (setWeekLineBasicInfo.IsEOF()) {
		// ���ݼ�Ϊ�գ���������û�н���
		setWeekLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ� û�м������ǿ��");
		gl_systemMessage.PushDayLineInfoMessage(str); // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	int iStockNumber = 0;
	while (!setWeekLineBasicInfo.IsEOF()) {
		if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sh000001")) == 0) {
			// �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sz399001")) == 0) {
			// ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setWeekLineBasicInfo.m_Symbol)) {
			const auto lIndex = m_mapSymbol.at(setWeekLineBasicInfo.m_Symbol);
			vStock.push_back(GetStock(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setWeekLineBasicInfo.MoveNext();
	}
	const double dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setWeekLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) {
		// ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) {
			// ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.Edit();
		const double dLastClose = atof(setWeekLineBasicInfo.m_LastClose);
		const double dClose = atof(setWeekLineBasicInfo.m_Close);
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) {
			// �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			const double dUpDownRate = (dClose - dLastClose) / dLastClose;
			dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
		}
		setWeekLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// �����ǵ��������ǿ��
		if (dLastClose < 0.001) {
			setWeekLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else { setWeekLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare); }
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

	const CString strDate2 = ConvertDateToChineseTimeStampString(lDate);
	const CString strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushInformationMessage(strTemp); // ����ͬ�����Ʊ�����Ϣ

	return (true);
}

double CContainerChinaStock::GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept {
	char* p;
	const double lastClose = strtod(strLastClose, &p);
	if (lastClose < 0.001) return 0;
	double result = (strtod(strClose, &p) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}
