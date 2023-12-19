#pragma once

#include"VirtualMarket.h"

#include"DayLineWebData.h"

#include"ContainerStockSymbol.h"
#include"ContainerChinaStock.h"

#include<semaphore>
#include<set>
#include<atomic>
using std::binary_semaphore;
using std::set;
using std::atomic_int64_t;

extern binary_semaphore gl_ProcessChinaMarketRTData; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
extern binary_semaphore gl_UpdateChinaMarketDB; // ���ڸ���ChinaMarket���ݿ�

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // ʮ�����ǿ�ȹ�Ʊ����ʼλ�ã�10-19Ϊʮ�����ǿ�Թ�Ʊ������ʮ����

class CChinaMarket : public CVirtualMarket {
public:
	CChinaMarket();
	// ֻ����һ��ʵ��,������ֵ��
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	~CChinaMarket() override;

	void ResetMarket() final;
	void Reset();

	void PreparingExitMarket() final;

	bool IsTimeToResetSystem(long lCurrentTime) final;
	bool IsOrdinaryTradeTime() final { return IsOrdinaryTradeTime(GetMarketTime()); } // �ճ�����ʱ��
	bool IsOrdinaryTradeTime(long lTime) final;
	bool IsWorkingTime() final { return IsWorkingTime(GetMarketTime()); }
	bool IsWorkingTime(long lTime) final;
	bool IsDummyTime() final { return !IsWorkingTime(); }
	bool IsDummyTime(long lTime) final { return !IsWorkingTime(lTime); }

public:
	bool ProcessTask(long lCurrentTime) override; // ÿ�ն�ʱ�������,�ɻ����SchedulingTask����

	// ��������
	virtual bool TaskCreateTask(long lCurrentTime);
	virtual void TaskReloadSystem(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	virtual bool TaskResetMarket(long lCurrentTime);
	void TaskDistributeAndCalculateRTData(long lCurrentTime);
	void TaskProcessAndSaveDayLine(long lCurrentTime);
	void TaskSaveTempData(long lCurrentTime);
	void TaskLoadCurrentStockHistoryData();// װ�ص�ǰ��Ʊ����
	void TaskAccessoryTask(long lCurrentTime); // ������������
	void TaskPreparingMarketOpen(long lCurrentTime);
	void TaskChoiceRSSet(long lCurrentTime);

	bool SetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);
	void ProcessTodayStock();
	bool IsTaskOfSavingDayLineDBFinished();
	bool CheckFastReceivingData(long lCurrentTime);
	bool CheckMarketOpen(long lCurrentTime);

	bool TaskUpdateStockProfileDB(long lCurrentTime);
	bool TaskUpdateOptionDB(long lCurrentTime);
	bool TaskUpdateChosenStockDB();

	bool TaskShowCurrentTransaction() const;

	bool TaskUpdateStockSection(); //

	bool ProcessDayLine();

	// �������̵߳��ð�������
	virtual void CreateThreadBuildDayLineRS(long lStartCalculatingDate);
	virtual void CreateThreadBuildDayLineRSOfDate(long lThisDate);
	virtual void CreateThreadBuildWeekLine(long lStartDate);
	virtual void CreateThreadBuildWeekLineOfStock(CChinaStockPtr pStock, long lStartDate);
	virtual void CreateThreadBuildWeekLineRS();
	virtual void CreateThreadBuildWeekLineRSOfDate(long lThisDate);
	virtual void CreateThreadBuildWeekLineOfCurrentWeek();
	virtual void CreateThreadBuildCurrentWeekWeekLineTable();
	virtual void CreateThreadChoice10RSStrong1StockSet();
	virtual void CreateThreadChoice10RSStrongStockSet();
	virtual void CreateThreadChoice10RSStrong2StockSet();
	virtual void CreateThreadLoadDayLine(CChinaStockPtr pCurrentStock);
	virtual void CreateThreadLoadWeekLine(CChinaStockPtr pCurrentStock);
	virtual void CreateThreadProcessTodayStock();
	virtual void CreateThreadUpdateStockProfileDB();
	virtual void CreateThreadUpdateOptionDB();
	virtual void CreateThreadUpdateTempRTData();
	virtual void CreateThreadLoadTempRTData(long lTheDate);
	virtual void CreateThreadSaveStockSection();
	virtual void CreateThreadUpdateChoseStockDB();
	virtual void CreateThreadDistributeAndCalculateRTData();

	//�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ�
	void DistributeRTData();
	void CalculateRTData();
	void ProcessRTData() { m_containerChinaStock.ProcessRTData(); }

	// ϵͳ״̬��
	bool IsFastReceivingRTData() noexcept { return m_fFastReceivingRTData; }

	// ʵʱ���ݶ�ȡ
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	bool CheckValidOfNeteaseDayLineInquiringStr(const CString& str) const;
	CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_containerStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_containerStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextTengxunStockInquiringMiddleStr(lTotalNumber); }

	//������ʷ���ݶ�ȡ
	CString CreateNeteaseDayLineInquiringStr() { return m_containerChinaStock.CreateNeteaseDayLineInquiringStr(); }
	CString CreateTengxunDayLineInquiringStr() { return m_containerChinaStock.CreateTengxunDayLineInquiringStr(); }

	static long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

	bool IsStock(const CString& strStockCode) const { return m_containerChinaStock.IsSymbol(strStockCode); } // �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

	CString GetStockName(const CString& strStockCode) { return m_containerChinaStock.GetStockName(strStockCode); }

	// �õ���Ʊָ��
	CChinaStockPtr GetStock(const CString& strStockCode) { return m_containerChinaStock.GetStock(strStockCode); }
	CChinaStockPtr GetStock(const size_t lIndex) { return m_containerChinaStock.GetStock(lIndex); }
	size_t GetStockIndex(const not_null<CChinaStockPtr>& pStock) const { return m_containerChinaStock.GetOffset(pStock->GetSymbol()); }

	// �õ���ǰ��ʾ��Ʊ
	CChinaStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CString& strStockCode);
	void SetCurrentStock(const CChinaStockPtr& pStock);
	void ResetCurrentStock();
	bool IsCurrentStockChanged() const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(const bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC) const;

	long GetCurrentSelectedPosition() const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	long GetCurrentSelectedStockSet() const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(const long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock();

	bool IsChosen10RSStrong1StockSet() const noexcept { return m_fChosen10RSStrong1StockSet; }
	void SetChosen10RSStrong1StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong1StockSet = fFlag; }
	bool IsChosen10RSStrong2StockSet() const noexcept { return m_fChosen10RSStrong2StockSet; }
	void SetChosen10RSStrong2StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong2StockSet = fFlag; }
	bool IsChosen10RSStrongStockSet() const noexcept { return m_fChosen10RSStrongStockSet; }
	void SetChosen10RSStrongStockSet(const bool fFlag) noexcept { m_fChosen10RSStrongStockSet = fFlag; }
	bool IsCalculateChosen10RS() const noexcept { return m_fCalculateChosen10RS; }
	void SetCalculateChosen10RS(const bool fFlag) noexcept { m_fCalculateChosen10RS = fFlag; }

	// ���ݿ��ȡ�洢����
	bool SaveDayLineData() { return m_containerChinaStock.SaveDayLineData(); } // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
	virtual void UpdateStockProfileDB() { m_containerChinaStock.UpdateStockProfileDB(); }
	void LoadStockProfileDB() { m_containerChinaStock.LoadStockProfileDB(); }

	virtual void UpdateOptionDB();
	void LoadOptionDB();
	void UpdateChosenStockDB() const;
	virtual void AppendChosenStockDB();
	void LoadChosenStockDB();
	virtual void SaveTempRTData() { m_containerChinaStock.SaveTempRTData(); }
	bool TaskLoadTempRTData(long lTheDate, long lCurrentTime);
	virtual void LoadTempRTData(long lTheDate);
	bool LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) const;
	virtual void SaveStockSection() { m_containerStockSymbol.UpdateStockSectionDB(); }

	static bool ChangeDayLineStockCodeTypeToStandard();

	virtual bool DeleteWeekLine();
	static void DeleteWeekLineBasicInfo();
	static void DeleteWeekLineExtendInfo();
	virtual void DeleteWeekLine(long lMonday);
	void DeleteWeekLineBasicInfo(long lMonday) const;
	void DeleteWeekLineExtendInfo(long lMonday) const;

	void MaintainDayLine() const { m_containerChinaStock.SetDayLineNeedMaintain(); }

	void DeleteDayLine(long lDate) const;
	void DeleteDayLineBasicInfo(long lDate) const;
	void DeleteDayLineExtendInfo(long lDate) const;

	virtual bool DeleteCurrentWeekWeekLine();
	static bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet();
	bool Load10DaysRSStrong2StockSet();

	bool LoadCalculatingRSOption();
	void SaveCalculatingRSOption() const;

	bool Load10DaysRSStrongStockDB();
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLine(const long lStartDate) { return m_containerChinaStock.BuildWeekLine(lStartDate); }
	virtual bool BuildWeekLineOfCurrentWeek();
	static bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData);
	virtual bool BuildCurrentWeekWeekLineTable(); // ʹ�����߱�����ǰ�����߱�

	// ��Ʊ��ʷ���ݴ���
	virtual bool Choice10RSStrong2StockSet() { return m_containerChinaStock.Choice10RSStrong2StockSet(); }
	// ѡ��10��ǿ�ƹ�Ʊ�������η�ֵ��
	virtual bool Choice10RSStrong1StockSet() { return m_containerChinaStock.Choice10RSStrong1StockSet(); }
	// ѡ��10��ǿ�ƹ�Ʊ����һ�η�ֵ��
	virtual bool Choice10RSStrongStockSet(CRSReference* pRef, const int iIndex) { return m_containerChinaStock.Choice10RSStrongStockSet(pRef, iIndex); }

	bool IsDayLineNeedUpdate() noexcept { return m_containerChinaStock.IsDayLineNeedUpdate(); }
	bool IsDayLineNeedProcess();
	bool IsDayLineNeedSaving() { return m_containerChinaStock.IsDayLineNeedSaving(); }
	long GetDayLineNeedUpdateNumber() const { return m_containerChinaStock.GetDayLineNeedUpdateNumber(); }
	long GetDayLineNeedSaveNumber() const { return m_containerChinaStock.GetDayLineNeedSaveNumber(); }

	virtual long BuildDayLine(const long lCurrentTradeDay) { return m_containerChinaStock.BuildDayLine(lCurrentTradeDay); }
	virtual bool BuildDayLineRS(const long lDate) { return m_containerChinaStock.BuildDayLineRS(lDate); }
	virtual bool BuildWeekLineRS(const long lDate) { return m_containerChinaStock.BuildWeekLineRS(lDate); }

	bool IsSelectedStockLoaded() const noexcept { return m_fSelectedStockLoaded; }
	void SetSelectedStockLoaded(const bool fLoad) noexcept { m_fSelectedStockLoaded = fLoad; }

	bool IsCheckingActiveStock() const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded() const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	bool IsDayLineDBUpdated() { return m_containerChinaStock.IsDayLineDBUpdated(); }
	void ClearDayLineDBUpdatedFlag() noexcept { m_containerChinaStock.ClearDayLineDBUpdatedFlag(); }

	long GetRSStartDate() const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(const long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate() const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(const long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate() const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(const long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(const long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1() const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2() const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS() const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

	INT64 GetTotalAttackBuyAmount() { return m_containerChinaStock.GetTotalAttackBuyAmount(); }
	INT64 GetTotalAttackSellAmount() { return m_containerChinaStock.GetTotalAttackSellAmount(); }

	size_t GetStockOffset(const CString& str) const { return m_containerChinaStock.GetOffset(str); }

	//����ʵʱ��Ʊ�仯��
	void DistributeSinaRTDataToStock();
	void DistributeTengxunRTDataToStock();
	void DistributeNeteaseRTDataToStock();
	bool DistributeRTDataToStock(const CWebRTDataPtr& pRTData);

	void ResetCurrentEffectiveRTDataRatio() noexcept {
		m_lRTDataReceivedInCurrentMinute = 0;
		m_lNewRTDataReceivedInCurrentMinute = 0;
	}
	double GetCurrentEffectiveRTDataRatio() const noexcept {
		if (m_lRTDataReceivedInCurrentMinute == 0) return 0.0;
		return static_cast<double>(m_lNewRTDataReceivedInCurrentMinute) / m_lRTDataReceivedInCurrentMinute;
	}

	// ״̬����
	bool IsUsingSinaRTDataReceiver() const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver() const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(const bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver() const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber() const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	size_t GetTotalStock() noexcept { return m_containerChinaStock.Size(); }
	long GetTotalActiveStock() const noexcept { return m_containerChinaStock.GetActiveStockSize(); }
	long GetTotalLoadedStock() const noexcept { return m_containerChinaStock.GetLoadedStockSize(); }
	void SetNewestTransactionTime(const time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
	time_t GetNewestTransactionTime() const noexcept { return m_ttNewestTransactionTime; }
	bool IsMarketOpened() const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData() const noexcept { return m_fFastReceivingRTData; }
	bool IsRTDataSetCleared() const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsSavingTempData() const noexcept { return m_fSaveTempData; }
	void SetSavingTempData(const bool fFlag) noexcept { m_fSaveTempData = fFlag; }

	bool IsCurrentEditStockChanged() const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(const bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	size_t SinaRTSize() noexcept { return m_qSinaRT.Size(); }
	void PushSinaRT(const not_null<CWebRTDataPtr>& pData) noexcept { m_qSinaRT.PushData(pData); }
	CWebRTDataPtr PopSinaRT() { return m_qSinaRT.PopData(); }
	size_t NeteaseRTSize() noexcept { return m_qNeteaseRT.Size(); }
	void PushNeteaseRT(const not_null<CWebRTDataPtr>& pData) noexcept { m_qNeteaseRT.PushData(pData); }
	CWebRTDataPtr PopNeteaseRT() { return m_qNeteaseRT.PopData(); }
	size_t TengxunRTSize() noexcept { return m_qTengxunRT.Size(); }
	void PushTengxunRT(const not_null<CWebRTDataPtr>& pData) noexcept { m_qTengxunRT.PushData(pData); }
	CWebRTDataPtr PopTengxunRT() { return m_qTengxunRT.PopData(); }
	size_t DayLineQueueSize() noexcept { return m_qDayLine.Size(); }
	void PushDayLine(const not_null<CDayLineWebDataPtr>& pData) noexcept { m_qDayLine.PushData(pData); }
	CDayLineWebDataPtr PopDayLine() { return m_qDayLine.PopData(); }

	// ʵʱ������Ҫ������������
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate() const noexcept { return m_RTDataNeedCalculate; }
	// �����������������ǿ����������
	void SetCalculatingDayLineRS(const bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS() const noexcept { return m_CalculatingDayLineRS; }
	// �����������������ǿ����������
	void SetCalculatingWeekLineRS(const bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS() const noexcept { return m_CalculatingWeekLineRS; }

	bool AddChosenStock(const CChinaStockPtr& pStock);
	bool DeleteChosenStock(const CChinaStockPtr& pStock);
	size_t GetChosenStockSize() const { return m_avChosenStock.at(0).size(); }
	size_t GetChosenStockSize(const long lIndex) const { return m_avChosenStock.at(lIndex).size(); }
	void ClearChoiceStockContainer() { m_avChosenStock.at(0).clear(); }

	void SetSinaStockRTDataInquiringIndex(const long lIndex) noexcept { m_containerChinaStock.SetSinaRTDataInquiringIndex(lIndex); }
	long GetSinaStockRTDataInquiringIndex() const noexcept { return m_containerChinaStock.GetSinaRTDataInquiringIndex(); }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_containerChinaStock.SetTengxunRTDataInquiringIndex(lIndex); }
	long GetTengxunRTDataInquiringIndex() const noexcept { return m_containerChinaStock.GetTengxunRTDataInquiringIndex(); }

	void ClearDayLineNeedUpdateStatus() const { m_containerChinaStock.ClearDayLineNeedUpdateStatus(); }

	bool IsUpdateStockProfileDB() { return m_containerChinaStock.IsUpdateProfileDB(); }
	void SetUpdateOptionDB(const bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }

	bool IsUpdateOptionDB() const noexcept { return m_fUpdateOptionDB; }

	void SetUpdateChosenStockDB(const bool fFlag) noexcept { m_fUpdateChosenStockDB = fFlag; }
	bool IsUpdateChosenStockDB() const noexcept { return m_fUpdateChosenStockDB; }

	INT64 GetRTDataReceived() const noexcept { return m_llRTDataReceived; }
	void SetRTDataReceived(const INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(const INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	bool ChangeToNextStock();
	bool ChangeToPrevStock();
	bool ChangeToPrevStockSet();
	bool ChangeToNextStockSet();

	bool IsTotalStockSetSelected() const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize();

	void SetUpdateStockSection(const bool fFlag) noexcept { m_containerStockSymbol.SetUpdateStockSection(fFlag); }
	bool IsUpdateStockSection() const noexcept { return m_containerStockSymbol.IsUpdateStockSection(); }

	void AddStock(const CChinaStockPtr& pStock) { m_containerChinaStock.Add(pStock); }
	void DeleteStock(const CChinaStockPtr& pStock) { m_containerChinaStock.Delete(pStock); }
	bool CreateStock(const CString& strStockCode, const CString& strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

	void ResetEffectiveRTDataRatio();

	void SetDistributeAndCalculateTime(time_t tt) { m_ttDistributeAndCalculateTime = tt; }

public:
	time_t m_ttDistributeAndCalculateTime; // ʵʱ���ݷ��估����ʱ��

protected:
	CContainerChinaStock m_containerChinaStock;
	CContainerStockSymbol m_containerStockSymbol;

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

	// �����ĸ�������
	CPriorityQueueWebRTData m_qSinaRT; // �й��г�ʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qNeteaseRT; // �й��г�ʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qTengxunRT; // �й��г�ʵʱ���ݶ��С�
	CTemplateMutexAccessQueue<CDayLineWebData> m_qDayLine; // ������������

	bool m_RTDataNeedCalculate;
	bool m_CalculatingDayLineRS;
	bool m_CalculatingWeekLineRS;

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // �Ƿ���
	bool m_fFastReceivingRTData; // �Ƿ�ʼ����ʵʱ����
	bool m_fRTDataSetCleared; // ʵʱ���ݿ��������ʶ���ŵ���ʮ��֮ǰΪ�٣�֮������Ϊ�档
	bool m_fSaveTempData; // �洢��ʱʵʱ���ݱ�ʶ
	CChinaStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ

	time_t m_ttNewestTransactionTime;

	bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��
	int m_iCountDownTengxunNumber;

	// Option��ѡ��
	long m_lRSStartDate;
	long m_lRSEndDate;
	long m_lLastLoginDate; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����
	long m_lLastLoginTime;
	long m_lUpdatedDateFor10DaysRS2;
	long m_lUpdatedDateFor10DaysRS1;
	long m_lUpdatedDateFor10DaysRS;

	bool m_fSelectedStockLoaded;

	bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64 m_lTotalMarketBuy; // �����г��е�A������������
	INT64 m_lTotalMarketSell; // �����г��е�A�������������

	// ϵͳ״̬��
	bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
	bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

	// ���¹�Ʊ�������ݿ��ʶ
	atomic_bool m_fUpdateOptionDB;
	bool m_fUpdateChosenStockDB;

private:
	long m_lRTDataReceivedInCurrentMinute; // ÿ���ӽ��յ���ʵʱ��������
	long m_lNewRTDataReceivedInCurrentMinute; // ÿ���ӽ��յ�����ʵʱ��������
};

using CChinaMarketPtr = shared_ptr<CChinaMarket>;

extern CChinaMarketPtr gl_pChinaMarket; // �й���Ʊ�г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
