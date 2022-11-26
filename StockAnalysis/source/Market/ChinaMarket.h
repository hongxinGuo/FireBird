#pragma once

#include"VirtualMarket.h"

#include"StockSection.h"

#include"DataStockSymbol.h"
#include"DataChinaStock.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>
#include<set>
#include<semaphore>

extern counting_semaphore<8> gl_BackGroundTaskThread;// ��̨�����߳��������Ϊ8

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // ʮ�����ǿ�ȹ�Ʊ����ʼλ�ã�10-19Ϊʮ�����ǿ�Թ�Ʊ������ʮ����

class CChinaMarket : public CVirtualMarket {
public:
	DECLARE_DYNCREATE(CChinaMarket)
		CChinaMarket(void);
	// ֻ����һ��ʵ��,������ֵ��
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	virtual ~CChinaMarket(void);
	virtual void ResetMarket(void) override final;
	void Reset(void);

	virtual bool PreparingExitMarket(void) override final;

	virtual bool IsTimeToResetSystem(long lCurrentTime)  override final;

	virtual bool IsOrdinaryTradeTime(void) override final; // �ճ�����ʱ��
	virtual bool IsOrdinaryTradeTime(long) override final;
	virtual bool IsWorkingTime(void) override final;
	virtual bool IsWorkingTime(long lTime) override final;
	virtual bool IsDummyTime(void) override final;
	virtual bool IsDummyTime(long lTime) override final;

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
public:
	// ��ʱ���£���ɾ���������������߳�CMainFrame��OnTimer�������á�������������Ⱥ���
	virtual bool SchedulingTask(void) override final; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��

	bool SchedulingTaskPerSecond(long lSecondNumber, long lCurrentTime); // ÿ�����һ��
	bool SchedulingTaskPer10Seconds(long lCurrentTime); // ÿʮ�����һ��
	bool SchedulingTaskPerMinute(long lCurrentTime); // ÿһ���ӵ���һ��
	bool SchedulingTaskPer5Minutes(long lCurrentTime); // ÿ����ӵ���һ��
	bool SchedulingTaskPerHour(long lCurrentTime); // ÿСʱ����һ��

	// ��������
	bool TaskProcessTengxunRTData(void);  // ������Ѷʵʱ����
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
	bool TaskUpdateChoicedStockDB(void);

	bool TaskShowCurrentTransaction(void);

	bool TaskSaveChoicedRTData(void);
	bool TaskClearChoicedRTDataSet(long lCurrentTime);

	bool TaskSaveStockSection(void); //

	void TaskGetActiveStockSize(void);

	//�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
	bool TaskProcessRTData(void) { return m_dataChinaStock.TaskProcessRTData(); }

	// �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
	bool TaskProcessDayLineGetFromNeeteaseServer(void);

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
	virtual bool CreatingThreadUpdateStockCodeDB(void);
	// interface function

public:
	// ϵͳ״̬��
	bool IsFastReceivingRTData(void) noexcept { return m_fFastReceivingRTData; }

	// ��ʼ���г�

	// ʵʱ���ݶ�ȡ
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString	GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
	CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(long lTotalNumber) { return m_dataStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(long lTotalNumber) { return m_dataStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextTengxunStockInquiringMiddleStr(lTotalNumber); }
	//������ʷ���ݶ�ȡ
	bool CreateNeteaseDayLineInquiringStr(CString& strReturn) { return m_dataChinaStock.CreateNeteaseDayLineInquiringStr(strReturn); }
	long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

	bool IsAStock(not_null<CChinaStockPtr> pStock) { return m_dataChinaStock.IsAStock(pStock); } // �Ƿ�Ϊ����A��
	bool IsAStock(CString strStockCode) { return m_dataChinaStock.IsAStock(strStockCode); } // �Ƿ�Ϊ����A��
	bool IsStock(CString strStockCode) { return m_dataChinaStock.IsStock(strStockCode); }	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

	CString GetStockName(CString strStockCode) { return m_dataChinaStock.GetStockName(strStockCode); }

	// �õ���Ʊָ��
	CChinaStockPtr GetStock(CString strStockCode) { return m_dataChinaStock.GetStock(strStockCode); }
	CChinaStockPtr GetStock(long lIndex) { return m_dataChinaStock.GetStock(lIndex); }

	// �õ���ǰ��ʾ��Ʊ
	CChinaStockPtr GetCurrentStock(void) const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(CString strStockCode);
	void SetCurrentStock(CChinaStockPtr pStock);
	void ResetCurrentStock(void);
	bool IsCurrentStockChanged(void) const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC);

	bool IsTodayStockNotProcessed(void) const noexcept { if (m_iTodayStockProcessed == 0) return true; else return false; }
	bool IsProcessingTodayStock(void) const noexcept { if (m_iTodayStockProcessed == 1) return true; else return false; }
	bool IsTodayStockProcessed(void) const noexcept { if (m_iTodayStockProcessed == 0) return false; else return true; }
	void SetProcessingTodayStock(void) noexcept { m_iTodayStockProcessed = 1; }
	void SetTodayStockProcessed(bool fFlag) noexcept { if (fFlag) m_iTodayStockProcessed = 2; else m_iTodayStockProcessed = 0; }

	long GetCurrentSelectedPosition(void) const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	long GetCurrentSelectedStockSet(void) const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock(void);

	bool IsChoiced10RSStrong1StockSet(void) const noexcept { return m_fChoiced10RSStrong1StockSet; }
	void SetChoiced10RSStrong1StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong1StockSet = fFlag; }
	bool IsChoiced10RSStrong2StockSet(void) const noexcept { return m_fChoiced10RSStrong2StockSet; }
	void SetChoiced10RSStrong2StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong2StockSet = fFlag; }
	bool IsChoiced10RSStrongStockSet(void) const noexcept { return m_fChoiced10RSStrongStockSet; }
	void SetChoiced10RSStrongStockSet(bool fFlag) noexcept { m_fChoiced10RSStrongStockSet = fFlag; }
	bool IsCalculateChoiced10RS(void) const noexcept { return m_fCalculateChoiced10RS; }
	void SetCalculateChoiced10RS(bool fFlag) noexcept { m_fCalculateChoiced10RS = fFlag; }

	// ���ݿ��ȡ�洢����
	virtual bool SaveRTData(void);  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
	bool TaskSaveDayLineData(void) { return m_dataChinaStock.TaskSaveDayLineData(); }  // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
	virtual bool UpdateStockCodeDB(void) { return m_dataChinaStock.UpdateStockCodeDB(); }
	void LoadStockCodeDB(void) { m_lStockDayLineNeedUpdate = m_dataChinaStock.LoadStockCodeDB(); }

	virtual bool UpdateOptionDB(void);
	void LoadOptionDB(void);
	bool UpdateChoicedStockDB(void);
	virtual bool AppendChoicedStockDB(void);
	void LoadChoicedStockDB(void);
	bool UpdateTempRTData(void);
	virtual bool UpdateTodayTempDB(void) { return m_dataChinaStock.UpdateTodayTempDB(); }
	bool LoadTodayTempDB(long lTheDay);
	bool LoadDayLine(CDataChinaDayLine& dataChinaDayLine, long lDate);
	virtual bool SaveStockSection(void) { return m_dataStockSymbol.UpdateStockSectionDB(); }

	bool ChangeDayLineStockCodeToStandred(void);

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

	virtual bool BuildWeekLine(long lStartDate) { return m_dataChinaStock.BuildWeekLine(lStartDate); }
	virtual bool BuildWeekLineOfCurrentWeek(void);
	bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData);
	virtual bool BuildCurrentWeekWeekLineTable(void); // ʹ�����߱�����ǰ�����߱�

	// ��Ʊ��ʷ���ݴ���
	virtual bool Choice10RSStrong2StockSet(void) { return m_dataChinaStock.Choice10RSStrong2StockSet(); } // ѡ��10��ǿ�ƹ�Ʊ�������η�ֵ��
	virtual bool Choice10RSStrong1StockSet(void) { return m_dataChinaStock.Choice10RSStrong1StockSet(); } // ѡ��10��ǿ�ƹ�Ʊ����һ�η�ֵ��
	virtual bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) { return m_dataChinaStock.Choice10RSStrongStockSet(pRef, iIndex); }

	bool IsDayLineNeedUpdate(void) const noexcept { return m_dataChinaStock.IsDayLineNeedUpdate(); }
	bool IsDayLineNeedProcess(void) const noexcept;
	bool IsDayLineNeedSaving(void) const { return m_dataChinaStock.IsDayLineNeedSaving(); }
	long GetDayLineNeedUpdateNumber(void) { return m_dataChinaStock.GetDayLineNeedUpdateNumber(); }
	long GetDayLineNeedSaveNumber(void) { return m_dataChinaStock.GetDayLineNeedSaveNumber(); }

	virtual long BuildDayLine(long lCurrentTradeDay) { return m_dataChinaStock.BuildDayLine(lCurrentTradeDay); }
	virtual bool BuildDayLineRS(long lDate) { return m_dataChinaStock.BuildDayLineRS(lDate); }
	virtual bool BuildWeekLineRS(long lDate) { return m_dataChinaStock.BuildWeekLineRS(lDate); }

	bool IsLoadSelectedStock(void) const noexcept { return m_fLoadedSelectedStock; }
	void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

	bool IsCheckingActiveStock(void) const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	bool IsDayLineDBUpdated(void) noexcept { return m_dataChinaStock.IsDayLineDBUpdated(); }
	void ClearDayLineDBUpdatedFlag(void) noexcept { m_dataChinaStock.ClearDayLineDBUpdatedFlag(); }

	long GetRSStartDate(void) const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate(void) const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate(void) const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1(void) const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2(void) const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS(void) const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

	INT64 GetTotalAttackBuyAmount(void) { return m_dataChinaStock.GetTotalAttackBuyAmount(); }
	INT64 GetTotalAttackSellAmount(void) { return m_dataChinaStock.GetTotalAttackSellAmount(); }

	long GetStockOffset(CString str) { return m_dataChinaStock.GetIndex(str); }

	void StoreChoiceRTData(CWebRTDataPtr pRTData);

	bool TaskDiscardNeteaseRTData(void);
	bool TaskDiscardSinaRTData(void);
	bool TaskDiscardTengxunRTData(void);

	//����ʵʱ��Ʊ�仯��
	bool TaskDistributeSinaRTDataToStock(void);
	bool TaskDistributeNeteaseRTDataToStock(void);
	bool DistributeRTDataToStock(CWebRTDataPtr pData);

	long GetRTDataReceivedInOrdinaryTradeTime(void) noexcept { return m_lRTDataReceivedInOrdinaryTradeTime; }
	long GetNewRTDataReveivedInOrdinaryTradeTime(void) noexcept { return m_lNewRTDataReceivedInOrdinaryTradeTime; }

	void TaskSaveTempDataIntoDB(long lCurrentTime);

	// ״̬����
	bool IsUsingSinaRTDataReceiver(void) const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver(void) const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver(void) const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber(void) const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	long GetTotalStock(void) noexcept { return m_dataChinaStock.GetStockSize(); }
	long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }
	long GetTotalLoadedStock(void) noexcept { return m_dataChinaStock.GetLoadedStockSize(); }
	void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
	time_t GetNewestTransactionTime(void) const noexcept { return m_ttNewestTransactionTime; }
	bool IsMarketOpened(void) const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData(void) const noexcept { return m_fFastReceivingRTData; }
	bool IsSaveDayLine(void) const noexcept { return m_fSaveDayLine; }
	void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
	bool IsRTDataSetCleared(void) const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsSavingTempData(void) const noexcept { return m_fSaveTempData; }
	void SetSavingTempData(bool fFlag) noexcept { m_fSaveTempData = fFlag; }

	bool IsCurrentEditStockChanged(void) const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	bool AddChoicedStock(CChinaStockPtr pStock);
	bool DeleteChoicedStock(CChinaStockPtr pStock);
	size_t GetChoicedStockSize(void) const { return m_avChoicedStock.at(0).size(); }
	size_t GetChoicedStockSize(long lIndex) const { return m_avChoicedStock.at(lIndex).size(); }
	void ClearChoiceStockContainer(void) { m_avChoicedStock.at(0).clear(); }
	size_t GetChoicedRTDataSize(void) const noexcept { return m_qRTData.size(); }
	void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

	void SetSinaStockRTDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetSinaRTDataInquiringIndex(lIndex); }
	long GetSinaStockRTDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetSinaRTDataInquiringIndex(); }
	void SetTengxunRTDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetTengxunRTDataInquiringIndex(lIndex); }
	long GetTengxunRTDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetTengxunRTDataInquiringIndex(); }
	void SetNeteaseDayLineDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetNeteaseDayLineDataInquiringIndex(lIndex); }
	long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(); }

	void ClearDayLineNeedUpdateStatus(void) { m_dataChinaStock.ClearDayLineNeedUpdateStatus(); }

	void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
	bool IsRecordingRTData(void) const noexcept { if (m_fSaveRTData) return true; else return false; }

	bool IsUpdateStockCodeDB(void) { return m_dataChinaStock.IsUpdateStockCodeDB(); }
	void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
	bool IsUpdateOptionDB(void) const noexcept { const bool fFlag = m_fUpdateOptionDB; return fFlag; }
	void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
	bool IsUpdateChoicedStockDB(void) const noexcept { const bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

	INT64 GetRTDataReceived(void) const noexcept { return m_llRTDataReceived; }
	void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	bool CheckMarketReady(void);

	bool ChangeToNextStock(void);
	bool ChangeToPrevStock(void);
	bool ChangeToPrevStockSet(void);
	bool ChangeToNextStockSet(void);
	bool IsTotalStockSetSelected(void) const noexcept { if (m_lCurrentSelectedStockSet == -1) return true; else return false; }
	size_t GetCurrentStockSetSize(void);

	void SetStockDayLineNeedUpdate(long lValue) noexcept { m_lStockDayLineNeedUpdate = lValue; }
	bool TooManyStockDayLineNeedUpdate(void) const noexcept { if (m_lStockDayLineNeedUpdate > 1000) return true; else return false; }

	void SetUpdateStockSection(bool fFlag) noexcept { m_dataStockSymbol.SetUpdateStockSection(fFlag); }
	bool IsUpdateStockSection(void) noexcept { return m_dataStockSymbol.IsUpdateStockSection(); }

	bool AddStock(CChinaStockPtr pStock) { return m_dataChinaStock.Add(pStock); }
	bool DeleteStock(CChinaStockPtr pStock) { return m_dataChinaStock.Delete(pStock); }
	bool CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

protected:
	// ��ʼ��

public:
	// ����ר�ú���

protected:
	// ���г���ѡ��

// ������
protected:
	CDataChinaStock m_dataChinaStock;
	CDataStockSymbol m_dataStockSymbol;

	vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CRSReference> m_aRSStrongOption; // ���ڼ���RS�Ĳ��������ʮ����
	vector<vector<CChinaStockPtr> > m_avChoicedStock; // ����ѡ��Ĺ�Ʊ����0-9����ѡ��Ʊ����10-19��10��RS��Ʊ����20-29���ɼ۱仯��Ʊ��
	long m_lCurrentSelectedPosition; // ��ǰ��Ʊ����λ��
	long m_lCurrentRSStrongIndex; // �����ڴ��ݵ�ǰ��λ�ã�������ѡ����ȷ�����ݱ�
	long m_lCurrentSelectedStockSet; // ��ǰѡ��Ĺ�Ʊ����-1Ϊ�����Ʊ����1-10Ϊ10��RS���Թ�Ʊ�����Դ����ƣ���
	bool m_fChoiced10RSStrong1StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChoiced10RSStrong2StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChoiced10RSStrongStockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fCalculateChoiced10RS;

	atomic_uint64_t m_llRTDataReceived; // ���յ���ʵʱ��������
	long m_lRTDataReceivedInOrdinaryTradeTime; // ������������ʱ���ڽ��յ���ʵʱ��������
	long m_lNewRTDataReceivedInOrdinaryTradeTime; // ������������ʱ���ڽ��յ�����ʵʱ��������

	long m_lStockDayLineNeedUpdate; // ��Ʊ��ʷ���߽�����Ҫ������

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
	bool m_fUpdateChoicedStockDB;

private:
};

typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

extern CChinaMarketPtr gl_pChinaMarket; // �й���Ʊ�г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
