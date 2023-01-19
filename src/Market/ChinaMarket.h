#pragma once

#include"VirtualMarket.h"

#include"NeteaseDayLineWebData.h"

#include"DataStockSymbol.h"
#include"ContainerChinaStock.h"

#include<semaphore>
#include<set>
#include<atomic>
using std::binary_semaphore;
using std::set;
using std::atomic_int64_t;

extern counting_semaphore<8> gl_BackGroundTaskThread; // ��̨�����߳��������Ϊ8
extern binary_semaphore gl_ProcessChinaMarketRTData; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // ʮ�����ǿ�ȹ�Ʊ����ʼλ�ã�10-19Ϊʮ�����ǿ�Թ�Ʊ������ʮ����

class CChinaMarket : public CVirtualMarket {
public:
	CChinaMarket(void);
	// ֻ����һ��ʵ��,������ֵ��
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	~CChinaMarket(void) override;
	void ResetMarket(void) final;
	void Reset(void);

	bool PreparingExitMarket(void) final;

	bool IsTimeToResetSystem(long lCurrentTime) final;

	bool IsOrdinaryTradeTime(void) final; // �ճ�����ʱ��
	bool IsOrdinaryTradeTime(long) final;
	bool IsWorkingTime(void) final;
	bool IsWorkingTime(long lTime) final;
	bool IsDummyTime(void) final;
	bool IsDummyTime(long lTime) final;

public:
	// ��ʱ���£���ɾ���������������߳�CMainFrame��OnTimer�������á�������������Ⱥ���
	bool SchedulingTask(void) final; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��

	bool SchedulingTaskPerSecond(long lSecondNumber, long lCurrentTime); // ÿ�����һ��
	bool SchedulingTaskPer10Seconds(long lCurrentTime); // ÿʮ�����һ��
	bool SchedulingTaskPerMinute(long lCurrentTime); // ÿһ���ӵ���һ��
	bool SchedulingTaskPer5Minutes(long lCurrentTime); // ÿ����ӵ���һ��
	bool SchedulingTaskPerHour(long lCurrentTime); // ÿСʱ����һ��

	// ��������
	bool TaskProcessTengxunRTData(void); // ������Ѷʵʱ����
	bool TaskSetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);
	void ProcessTodayStock(void);
	bool TaskCheckDayLineDB(void);
	bool TaskCheckFastReceivingData(long lCurrentTime);
	bool TaskCheckMarketOpen(long lCurrentTime);
	bool TaskResetMarket(long lCurrentTime);
	bool TaskResetMarketAgain(long lCurrentTime);

	bool TaskUpdateStockCodeDB(void);
	bool TaskUpdateOptionDB(void);
	bool TaskUpdateChosenStockDB(void);

	bool TaskShowCurrentTransaction(void);

	bool TaskSaveChosenRTData(void);
	bool TaskClearChosenRTDataSet(long lCurrentTime);

	bool TaskSaveStockSection(void); //

	void TaskGetActiveStockSize(void);

	//�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ�
	bool TaskProcessRTData(void) { return m_dataChinaStock.TaskProcessRTData(); }

	// �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
	bool TaskProcessDayLineGetFromNeteaseServer(void);

	// װ�ص�ǰ��Ʊ��������
	bool TaskLoadCurrentStockHistoryData(void);

	// �������̵߳��ð�������
	virtual bool CreatingThreadBuildDayLineRS(long lStartCalculatingDay);
	virtual bool CreatingThreadBuildDayLineRSOfDate(long lThisDay);
	virtual bool CreatingThreadBuildWeekLine(long lStartDate);
	virtual bool CreatingThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate);
	virtual bool CreatingThreadBuildWeekLineRS(void);
	virtual bool CreatingThreadBuildWeekLineRSOfDate(long lThisDay);
	virtual bool CreatingThreadBuildWeekLineOfCurrentWeek(void);
	virtual bool CreatingThreadBuildCurrentWeekWeekLineTable(void);
	virtual bool CreatingThreadChoice10RSStrong1StockSet(void);
	virtual bool CreatingThreadChoice10RSStrongStockSet(void);
	virtual bool CreatingThreadChoice10RSStrong2StockSet(void);
	virtual bool CreatingThreadLoadDayLine(CChinaStock* pCurrentStock);
	virtual bool CreatingThreadLoadWeekLine(CChinaStock* pCurrentStock);
	virtual bool CreatingThreadProcessTodayStock(void);
	virtual bool CreatingThreadUpdateStockProfileDB(void);
	// interface function

public:
	// ϵͳ״̬��
	bool IsFastReceivingRTData(void) noexcept { return m_fFastReceivingRTData; }

	// ��ʼ���г�

	// ʵʱ���ݶ�ȡ
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber) { return m_dataChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	bool CheckValidOfNeteaseDayLineInquiringStr(const CString& str);
	CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_dataStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_dataStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return m_dataChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return m_dataChinaStock.GetNextTengxunStockInquiringMiddleStr(lTotalNumber); }

	//������ʷ���ݶ�ȡ
	CString CreateNeteaseDayLineInquiringStr() { return m_dataChinaStock.CreateNeteaseDayLineInquiringStr(); }

	long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

	bool IsAStock(const not_null<CChinaStockPtr> pStock) { return m_dataChinaStock.IsAStock(pStock->GetSymbol()); } // �Ƿ�Ϊ����A��
	bool IsAStock(const CString& strStockCode) { return m_dataChinaStock.IsAStock(strStockCode); } // �Ƿ�Ϊ����A��
	bool IsStock(const CString& strStockCode) const { return m_dataChinaStock.IsSymbol(strStockCode); } // �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

	CString GetStockName(const CString& strStockCode) { return m_dataChinaStock.GetStockName(strStockCode); }

	// �õ���Ʊָ��
	CChinaStockPtr GetStock(const CString& strStockCode) { return m_dataChinaStock.GetStock(strStockCode); }
	CChinaStockPtr GetStock(const long lIndex) { return m_dataChinaStock.GetStock(lIndex); }
	size_t GetStockIndex(const CChinaStockPtr pStock) const { return m_dataChinaStock.GetOffset(pStock->GetSymbol()); }

	// �õ���ǰ��ʾ��Ʊ
	CChinaStockPtr GetCurrentStock(void) const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CString& strStockCode);
	void SetCurrentStock(CChinaStockPtr pStock);
	void ResetCurrentStock(void);
	bool IsCurrentStockChanged(void) const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(const bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC);

	bool IsTodayStockNotProcessed(void) const noexcept {
		if (m_iTodayStockProcessed == 0) return true;
		return false;
	}

	bool IsProcessingTodayStock(void) const noexcept {
		if (m_iTodayStockProcessed == 1) return true;
		return false;
	}

	bool IsTodayStockProcessed(void) const noexcept {
		if (m_iTodayStockProcessed == 0) return false;
		return true;
	}

	void SetProcessingTodayStock(void) noexcept { m_iTodayStockProcessed = 1; }

	void SetTodayStockProcessed(const bool fFlag) noexcept {
		if (fFlag) m_iTodayStockProcessed = 2;
		else m_iTodayStockProcessed = 0;
	}

	long GetCurrentSelectedPosition(void) const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	long GetCurrentSelectedStockSet(void) const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(const long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock(void);

	bool IsChosen10RSStrong1StockSet(void) const noexcept { return m_fChosen10RSStrong1StockSet; }
	void SetChosen10RSStrong1StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong1StockSet = fFlag; }
	bool IsChosen10RSStrong2StockSet(void) const noexcept { return m_fChosen10RSStrong2StockSet; }
	void SetChosen10RSStrong2StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong2StockSet = fFlag; }
	bool IsChosen10RSStrongStockSet(void) const noexcept { return m_fChosen10RSStrongStockSet; }
	void SetChosen10RSStrongStockSet(const bool fFlag) noexcept { m_fChosen10RSStrongStockSet = fFlag; }
	bool IsCalculateChosen10RS(void) const noexcept { return m_fCalculateChosen10RS; }
	void SetCalculateChosen10RS(const bool fFlag) noexcept { m_fCalculateChosen10RS = fFlag; }

	// ���ݿ��ȡ�洢����
	virtual bool SaveRTData(void); // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
	bool TaskSaveDayLineData(void) { return m_dataChinaStock.TaskSaveDayLineData(); } // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
	virtual bool UpdateStockProfileDB(void) { return m_dataChinaStock.UpdateStockProfileDB(); }
	void LoadStockProfileDB(void) { m_lStockDayLineNeedUpdate = m_dataChinaStock.LoadStockProfileDB(); }

	virtual bool UpdateOptionDB(void);
	void LoadOptionDB(void);
	bool UpdateChosenStockDB(void);
	virtual bool AppendChosenStockDB(void);
	void LoadChosenStockDB(void);
	bool UpdateTempRTData(void);
	virtual bool UpdateTodayTempDB(void) { return m_dataChinaStock.UpdateTodayTempDB(); }
	bool LoadTodayTempDB(long lTheDay);
	bool LoadDayLine(CDataChinaDayLine& dataChinaDayLine, long lDate);
	virtual bool SaveStockSection(void) { return m_dataStockSymbol.UpdateStockSectionDB(); }

	bool ChangeDayLineStockCodeToStandard(void);

	virtual bool DeleteWeekLine(void);
	bool DeleteWeekLineBasicInfo(void);
	bool DeleteWeekLineExtendInfo(void);
	virtual bool DeleteWeekLine(long lMonday);
	bool DeleteWeekLineBasicInfo(long lMonday);
	bool DeleteWeekLineExtendInfo(long lMonday);

	void MaintainDayLine(void) { m_dataChinaStock.SetAllDayLineNeedMaintain(); }

	bool DeleteDayLine(long lDate);
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);

	virtual bool DeleteCurrentWeekWeekLine(void);
	bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet(void);
	bool Load10DaysRSStrong2StockSet(void);

	bool LoadCalculatingRSOption(void);
	void SaveCalculatingRSOption(void);

	bool Load10DaysRSStrongStockDB(void);
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLine(const long lStartDate) { return m_dataChinaStock.BuildWeekLine(lStartDate); }
	virtual bool BuildWeekLineOfCurrentWeek(void);
	bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData);
	virtual bool BuildCurrentWeekWeekLineTable(void); // ʹ�����߱�����ǰ�����߱�

	// ��Ʊ��ʷ���ݴ���
	virtual bool Choice10RSStrong2StockSet(void) { return m_dataChinaStock.Choice10RSStrong2StockSet(); }
	// ѡ��10��ǿ�ƹ�Ʊ�������η�ֵ��
	virtual bool Choice10RSStrong1StockSet(void) { return m_dataChinaStock.Choice10RSStrong1StockSet(); }
	// ѡ��10��ǿ�ƹ�Ʊ����һ�η�ֵ��
	virtual bool Choice10RSStrongStockSet(CRSReference* pRef, const int iIndex) { return m_dataChinaStock.Choice10RSStrongStockSet(pRef, iIndex); }

	bool IsDayLineNeedUpdate(void) noexcept { return m_dataChinaStock.IsDayLineNeedUpdate(); }
	bool IsDayLineNeedProcess(void);
	bool IsDayLineNeedSaving(void) { return m_dataChinaStock.IsDayLineNeedSaving(); }
	long GetDayLineNeedUpdateNumber(void) { return m_dataChinaStock.GetDayLineNeedUpdateNumber(); }
	long GetDayLineNeedSaveNumber(void) { return m_dataChinaStock.GetDayLineNeedSaveNumber(); }

	virtual long BuildDayLine(const long lCurrentTradeDay) { return m_dataChinaStock.BuildDayLine(lCurrentTradeDay); }
	virtual bool BuildDayLineRS(const long lDate) { return m_dataChinaStock.BuildDayLineRS(lDate); }
	virtual bool BuildWeekLineRS(const long lDate) { return m_dataChinaStock.BuildWeekLineRS(lDate); }

	bool IsLoadSelectedStock(void) const noexcept { return m_fLoadedSelectedStock; }
	void SetLoadSelectedStock(const bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

	bool IsCheckingActiveStock(void) const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded(void) const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	bool IsDayLineDBUpdated(void) { return m_dataChinaStock.IsDayLineDBUpdated(); }
	void ClearDayLineDBUpdatedFlag(void) noexcept { m_dataChinaStock.ClearDayLineDBUpdatedFlag(); }

	long GetRSStartDate(void) const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(const long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate(void) const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(const long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate(void) const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(const long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(const long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1(void) const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2(void) const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS(void) const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

	INT64 GetTotalAttackBuyAmount(void) { return m_dataChinaStock.GetTotalAttackBuyAmount(); }
	INT64 GetTotalAttackSellAmount(void) { return m_dataChinaStock.GetTotalAttackSellAmount(); }

	long GetStockOffset(const CString& str) const { return m_dataChinaStock.GetOffset(str); }

	void StoreChoiceRTData(const CWebRTDataPtr pRTData) { m_qRTData.push(pRTData); }

	//����ʵʱ��Ʊ�仯��
	bool TaskDistributeSinaRTDataToStock(void);
	bool TaskDistributeNeteaseRTDataToStock(void);
	bool DistributeRTDataToStock(CWebRTDataPtr pRTData);

	long GetRTDataReceivedInOrdinaryTradeTime(void) const noexcept { return m_lRTDataReceivedInOrdinaryTradeTime; }
	long GetNewRTDataReceivedInOrdinaryTradeTime(void) const noexcept { return m_lNewRTDataReceivedInOrdinaryTradeTime; }

	void TaskSaveTempDataIntoDB(long lCurrentTime);

	// ״̬����
	bool IsUsingSinaRTDataReceiver(void) const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver(void) const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(const bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver(void) const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber(void) const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	long GetTotalStock(void) const noexcept { return m_dataChinaStock.Size(); }
	long GetTotalActiveStock(void) const noexcept { return m_lTotalActiveStock; }
	long GetTotalLoadedStock(void) const noexcept { return m_dataChinaStock.GetLoadedStockSize(); }
	void SetNewestTransactionTime(const time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
	time_t GetNewestTransactionTime(void) const noexcept { return m_ttNewestTransactionTime; }
	bool IsMarketOpened(void) const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData(void) const noexcept { return m_fFastReceivingRTData; }
	bool IsSaveDayLine(void) const noexcept { return m_fSaveDayLine; }
	void SetSaveDayLine(const bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
	bool IsRTDataSetCleared(void) const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsSavingTempData(void) const noexcept { return m_fSaveTempData; }
	void SetSavingTempData(const bool fFlag) noexcept { m_fSaveTempData = fFlag; }

	bool IsCurrentEditStockChanged(void) const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(const bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	size_t SinaRTSize(void) noexcept { return m_qSinaRT.Size(); }
	void PushSinaRT(const CWebRTDataPtr pData) noexcept { m_qSinaRT.PushData(pData); }
	CWebRTDataPtr PopSinaRT(void) { return m_qSinaRT.PopData(); }
	size_t NeteaseRTSize(void) noexcept { return m_qNeteaseRT.Size(); }
	void PushNeteaseRT(const CWebRTDataPtr pData) noexcept { m_qNeteaseRT.PushData(pData); }
	CWebRTDataPtr PopNeteaseRT(void) { return m_qNeteaseRT.PopData(); }
	size_t TengxunRTSize(void) noexcept { return m_qTengxunRT.Size(); }
	void PushTengxunRT(const CWebRTDataPtr pData) noexcept { m_qTengxunRT.PushData(pData); }
	CWebRTDataPtr PopTengxunRT(void) { return m_qTengxunRT.PopData(); }
	size_t NeteaseDayLineSize(void) noexcept { return m_qNeteaseDayLine.Size(); }
	void PushNeteaseDayLine(const CNeteaseDayLineWebDataPtr pData) noexcept { m_qNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopNeteaseDayLine(void) { return m_qNeteaseDayLine.PopData(); }

	// ʵʱ������Ҫ������������
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate(void) const noexcept { return m_RTDataNeedCalculate; }
	// �����������������ǿ����������
	void SetCalculatingDayLineRS(const bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS(void) const noexcept { return m_CalculatingDayLineRS; }
	// �����������������ǿ����������
	void SetCalculatingWeekLineRS(const bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS(void) const noexcept { return m_CalculatingWeekLineRS; }

	bool AddChosenStock(CChinaStockPtr pStock);
	bool DeleteChosenStock(CChinaStockPtr pStock);
	size_t GetChosenStockSize(void) const { return m_avChosenStock.at(0).size(); }
	size_t GetChosenStockSize(const long lIndex) const { return m_avChosenStock.at(lIndex).size(); }
	void ClearChoiceStockContainer(void) { m_avChosenStock.at(0).clear(); }
	size_t GetChosenRTDataSize(void) const noexcept { return m_qRTData.size(); }
	void ClearChosenRTDataQueue(void) noexcept { while (!m_qRTData.empty()) m_qRTData.pop(); }

	void SetSinaStockRTDataInquiringIndex(const long lIndex) noexcept { m_dataChinaStock.SetSinaRTDataInquiringIndex(lIndex); }
	long GetSinaStockRTDataInquiringIndex(void) const noexcept { return m_dataChinaStock.GetSinaRTDataInquiringIndex(); }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_dataChinaStock.SetTengxunRTDataInquiringIndex(lIndex); }
	long GetTengxunRTDataInquiringIndex(void) const noexcept { return m_dataChinaStock.GetTengxunRTDataInquiringIndex(); }

	void SetNeteaseDayLineDataInquiringIndex(const long lIndex) noexcept { m_dataChinaStock.SetNeteaseDayLineDataInquiringIndex(lIndex); }
	long GetNeteaseDayLineDataInquiringIndex(void) const noexcept { return m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(); }

	void ClearDayLineNeedUpdateStatus(void) { m_dataChinaStock.ClearDayLineNeedUpdateStatus(); }

	void SetRecordRTData(const bool fFlag) noexcept { m_fSaveRTData = fFlag; }

	bool IsRecordingRTData(void) const noexcept {
		if (m_fSaveRTData) return true;
		return false;
	}

	bool IsUpdateStockCodeDB(void) { return m_dataChinaStock.IsUpdateProfileDB(); }
	void SetUpdateOptionDB(const bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }

	bool IsUpdateOptionDB(void) const noexcept {
		const bool fFlag = m_fUpdateOptionDB;
		return fFlag;
	}

	void SetUpdateChosenStockDB(const bool fFlag) noexcept { m_fUpdateChosenStockDB = fFlag; }

	bool IsUpdateChosenStockDB(void) const noexcept {
		const bool fFlag = m_fUpdateChosenStockDB;
		return fFlag;
	}

	INT64 GetRTDataReceived(void) const noexcept { return m_llRTDataReceived; }
	void SetRTDataReceived(const INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(const INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	bool CheckMarketReady(void);

	bool ChangeToNextStock(void);
	bool ChangeToPrevStock(void);
	bool ChangeToPrevStockSet(void);
	bool ChangeToNextStockSet(void);

	bool IsTotalStockSetSelected(void) const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize(void);

	void SetStockDayLineNeedUpdate(const long lValue) noexcept { m_lStockDayLineNeedUpdate = lValue; }

	bool TooManyStockDayLineNeedUpdate(void) const noexcept {
		if (m_lStockDayLineNeedUpdate > 1000) return true;
		return false;
	}

	void SetUpdateStockSection(const bool fFlag) noexcept { m_dataStockSymbol.SetUpdateStockSection(fFlag); }
	bool IsUpdateStockSection(void) const noexcept { return m_dataStockSymbol.IsUpdateStockSection(); }

	bool AddStock(CChinaStockPtr pStock) { return m_dataChinaStock.Add(pStock); }
	bool DeleteStock(CChinaStockPtr pStock) { return m_dataChinaStock.Delete(pStock); }
	bool CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

protected:
	// ��ʼ��

public:
	// ����ר�ú���

protected:
	// ���г���ѡ��

	// ������
protected:
	CContainerChinaStock m_dataChinaStock;
	CDataStockSymbol m_dataStockSymbol;

	vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CRSReference> m_aRSStrongOption; // ���ڼ���RS�Ĳ��������ʮ����
	vector<vector<CChinaStockPtr>> m_avChosenStock; // ����ѡ��Ĺ�Ʊ����0-9����ѡ��Ʊ����10-19��10��RS��Ʊ����20-29���ɼ۱仯��Ʊ��
	long m_lCurrentSelectedPosition; // ��ǰ��Ʊ����λ��
	long m_lCurrentRSStrongIndex; // �����ڴ��ݵ�ǰ��λ�ã�������ѡ����ȷ�����ݱ�
	long m_lCurrentSelectedStockSet; // ��ǰѡ��Ĺ�Ʊ����-1Ϊ�����Ʊ����1-10Ϊ10��RS���Թ�Ʊ�����Դ����ƣ���
	bool m_fChosen10RSStrong1StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChosen10RSStrong2StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChosen10RSStrongStockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fCalculateChosen10RS;

	atomic_int64_t m_llRTDataReceived; // ���յ���ʵʱ��������
	long m_lRTDataReceivedInOrdinaryTradeTime; // ������������ʱ���ڽ��յ���ʵʱ��������
	long m_lNewRTDataReceivedInOrdinaryTradeTime; // ������������ʱ���ڽ��յ�����ʵʱ��������

	long m_lStockDayLineNeedUpdate; // ��Ʊ��ʷ���߽�����Ҫ������

	// �����ĸ�������
	CPriorityQueueWebRTData m_qSinaRT; // �й��г�ʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qNeteaseRT; // �й��г�ʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qTengxunRT; // �й��г�ʵʱ���ݶ��С�
	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qNeteaseDayLine; // ������������

	bool m_RTDataNeedCalculate;
	bool m_CalculatingDayLineRS;
	bool m_CalculatingWeekLineRS;

	queue<CWebRTDataPtr> m_qRTData;
	bool m_fSaveRTData;

	int m_iMarketOpenOffset; // ���е�ƫ�������Է���Ϊ��λ��0930 = 0��1129 = 120�� 1300 = 121�� 1459 = 240��

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // �Ƿ���
	bool m_fFastReceivingRTData; // �Ƿ�ʼ����ʵʱ����
	bool m_fSaveDayLine; // ����ȡ�����ߴ������ݿ��ʶ
	bool m_fRTDataSetCleared; // ʵʱ���ݿ��������ʶ���ŵ���ʮ��֮ǰΪ�٣�֮������Ϊ�档
	bool m_fSaveTempData; // �洢��ʱʵʱ���ݱ�ʶ
	CChinaStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ

	time_t m_ttNewestTransactionTime;

	bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��
	int m_iCountDownTengxunNumber;

	CString m_strSinaRTDataInquiringStr;
	CString m_strTengxunRTDataInquiringStr;
	CString m_strNeteaseRTDataInquiringStr;
	CString m_strNeteaseDayLineDataInquiringStr;

	// Option��ѡ��
	long m_lRSStartDate;
	long m_lRSEndDate;
	long m_lLastLoginDate; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����
	long m_lLastLoginTime;
	long m_lUpdatedDateFor10DaysRS2;
	long m_lUpdatedDateFor10DaysRS1;
	long m_lUpdatedDateFor10DaysRS;

	vector<CChinaStockPtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
	bool m_fLoadedSelectedStock;

	bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64 m_lTotalMarketBuy; // �����г��е�A������������
	INT64 m_lTotalMarketSell; // �����г��е�A�������������

	// ϵͳ״̬��
	int m_iTodayStockProcessed; // �����Ƿ�ִ���˹�Ʊ����.0:��δִ�У�1������ִ���У�2����ִ���ꡣ
	bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
	bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��
	long m_lTotalActiveStock;

	// ���¹�Ʊ�������ݿ��ʶ
	atomic_bool m_fUpdateOptionDB;
	bool m_fUpdateChosenStockDB;

private:
};

using CChinaMarketPtr = shared_ptr<CChinaMarket>;

extern CChinaMarketPtr gl_pChinaMarket; // �й���Ʊ�г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
