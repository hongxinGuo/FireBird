#pragma once

#include"stdafx.h"
#include"globedef.h"
#include"Accessory.h"
#include"Semaphore.h"

#include"VirtualMarket.h"

#include"WebRTDataContainer.h"
#include"StakeCode.h"
#include"StakeSection.h"

#include "ChinaStake.h"

#include<gsl/gsl>
using namespace gsl;

using namespace MyLib;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>
#include<set>

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
extern Semaphore gl_SaveOneStockDayLine;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SemaphoreBackGroundTaskThreads; // ��̨�����߳��������Ϊ8
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

extern CWebRTDataContainer gl_WebRTDataContainer;

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // ʮ�����ǿ�ȹ�Ʊ����ʼλ�ã�10-19Ϊʮ�����ǿ�Թ�Ʊ������ʮ����

class CChinaMarket : public CVirtualMarket
{
public:
  // ֻ����һ��ʵ��
  CChinaMarket(void);
  virtual ~CChinaMarket(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif
public:
  // ��ʱ���£���ɾ���������������߳�CMainFrame��OnTimer�������á�������������Ⱥ���
  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime); // ÿʮ�����һ��
  bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime); // ÿ����ӵ���һ��
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // ÿСʱ����һ��

  // ��������
  bool TaskGetRTDataFromWeb(void);
  bool TaskGetNeteaseDayLineFromWeb(void);
  bool TaskUpdateStakeCodeFromWeb(void); // ÿ�ո���֤ȯ�����

  bool TaskProcessTengxunRTData(void);  // ������Ѷʵʱ����
  bool TaskSetCheckActiveStockFlag(long lCurrentTime);
  bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
  bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
  bool TaskChoice10RSStrongStockSet(long lCurrentTime);
  bool TaskProcessTodayStock(long lCurrentTime);
  bool TaskCheckDayLineDB(void);
  bool TaskCheckStartReceivingData(long lCurrentTime);
  bool TaskCheckMarketOpen(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  bool TaskResetMarketAgain(long lCurrentTime);

  bool TaskUpdateStockCodeDB(void);
  bool TaskUpdateOptionDB(void);
  bool TaskUpdateChoicedStockDB(void);

  bool TaskShowCurrentTransaction(void);

  bool TaskSaveChoicedRTData(void);
  bool TaskClearChoicedRTDataSet(long lCurrentTime);

  bool TaskSaveActiveStakeCode(void);
  bool TaskSaveStakeSection(void); //

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
  bool TaskProcessRTData(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
  bool TaskProcessDayLineGetFromNeeteaseServer(void);

  // װ�ص�ǰ��Ʊ��������
  bool TaskLoadCurrentStockHistoryData(void);

  // ����stakeCode����
  void TaskSetUpdateStakeCodeDBFlag(void);

  // �������̵߳��ð�������
  virtual bool RunningThreadSaveChoicedRTData(void);
  virtual bool RunningThreadProcessTodayStock(void);
  virtual bool RunningThreadBuildDayLineRS(long lStartCalculatingDay);
  virtual bool RunningThreadBuildDayLineRSOfDate(long lThisDay);
  virtual bool RunningThreadSaveTempRTData(void);
  virtual bool RunningThreadSaveDayLineBasicInfoOfStock(CChinaStake* pStake);
  virtual bool RunningThreadLoadDayLine(CChinaStake* pCurrentStock);
  virtual bool RunningThreadLoadWeekLine(CChinaStake* pCurrentStock);
  virtual bool RunningThreadUpdateStakeCodeDB(void);
  virtual bool RunningThreadUpdateOptionDB(void);
  virtual bool RunningThreadAppendChoicedStockDB(void);
  virtual bool RunningThreadChoice10RSStrong2StockSet(void);
  virtual bool RunningThreadChoice10RSStrong1StockSet(void);
  virtual bool RunningThreadChoice10RSStrongStockSet(void);
  virtual bool RunningThreadCalculate10RSStrongStock(vector<CChinaStakePtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStakePtr pStake);
  virtual bool RunningThreadCalculate10RSStrong1Stock(vector<CChinaStakePtr>* pv10RSStrongStock, CChinaStakePtr pStake);
  virtual bool RunningThreadCalculate10RSStrong2Stock(vector<CChinaStakePtr>* pv10RSStrongStock, CChinaStakePtr pStake);
  virtual bool RunningThreadBuildWeekLine(long lStartDate);
  virtual bool RunningThreadBuildWeekLineOfStock(CChinaStake* pStake, long lStartDate);
  virtual bool RunningThreadBuildWeekLineRS(void);
  virtual bool RunningThreadBuildWeekLineRSOfDate(long lThisDay);
  virtual bool RunningThreadBuildWeekLineOfCurrentWeek(void);
  virtual bool RunningThreadBuildCurrentWeekWeekLineTable(void);
  virtual bool RunningThreadSaveActiveStakeCode(void);
  virtual bool RunningThreadSaveStakeSection(void);
  // interface function
public:
  // ϵͳ״̬��

  // ��ʼ���г�

  // ʵʱ���ݶ�ȡ
  CString GetSinaStockInquiringStr(long lTotalNumber, bool fSkipUnactiveStock);
  CString GetSinaStakeInquiringStr(long lTotalNumber, long lStartPosition, bool fSkipUnactiveStake);
  CString CreateSinaStakeInquiringStr(long lTotalNumber);
  CString GetTengxunInquiringStockStr(long lTotalNumber, long lStartPosition, long lEndPosition, bool fSkipUnactiveStock);
  CString	GetNeteaseStakeInquiringStr(long lTotalNumber, long lStartPosition, long lEndPosition, bool fSkipUnactiveStock);
  bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
  CString GetNextStakeInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, long lStartPosition, long lEndPosition, bool fSkipUnactiveStock);
  bool StepToActiveStake(long& lStockIndex, long lStartPosition, long lEndPostion);
  CString CreateNextStakeInquiringMiddleStr(long& iStakeIndex, CString strPostfix, long lTotalNumber);
  CString CreateStakeCode(bool fShanghaiMarket, long lStakeIndex);
  //������ʷ���ݶ�ȡ
  bool CreateNeteaseDayLineInquiringStr(CString& strReturn, long lStartPosition, long lEndPosition);
  long IncreaseStakeInquiringIndex(long& lIndex, long lStartPosition, long lEndPosition);

  bool IsAStock(CChinaStakePtr pStake); // �Ƿ�Ϊ����A��
  bool IsAStock(CString strStockCode); // �Ƿ�Ϊ����A��
  bool IsStock(CString  strStockCode);	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

  CString GetStakeName(CString strStockCode);

  // �õ���Ʊ����
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // �õ���Ʊָ��
  CChinaStakePtr GetStock(CString strStockCode);
  CChinaStakePtr GetStock(long lIndex);

  void IncreaseActiveStockNumber(void) noexcept { m_lTotalActiveStock++; }

  // �õ���ǰ��ʾ��Ʊ
  CChinaStakePtr GetCurrentStock(void) noexcept { return m_pCurrentStock; }
  void SetCurrentStock(CString strStockCode);
  void SetCurrentStock(CChinaStakePtr pStake);
  void ResetCurrentStock(void);
  bool IsCurrentStockChanged(void) noexcept { return m_fCurrentStockChanged; }
  void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

  long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }
  void SetTotalActiveStock(long lValue) noexcept { m_lTotalActiveStock = lValue; }

  long GetMinLineOffset(time_t tMarket);

  bool IsTodayStockNotProcessed(void) noexcept { if (m_iTodayStockProcessed == 0) return true; else return false; }
  bool IsProcessingTodayStock(void) noexcept { if (m_iTodayStockProcessed == 1) return true; else return false; }
  bool IsTodayStockProcessed(void) noexcept { if (m_iTodayStockProcessed == 0) return false; else return true; }
  void SetProcessingTodayStock(void) noexcept { m_iTodayStockProcessed = 1; }
  void SetTodayStockProcessed(bool fFlag) noexcept { if (fFlag) m_iTodayStockProcessed = 2; else m_iTodayStockProcessed = 0; }

  long GetCurrentSelectedPosition(void) noexcept { return m_lCurrentSelectedPosition; }
  void SetCurrentSelectedPosition(long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
  long GetCurrentSelectedStockSet(void) noexcept { return m_lCurrentSelectedStockSet; }
  void SetCurrentSelectedStockSet(long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
  CChinaStakePtr GetCurrentSelectedStock(void);

  bool IsChoiced10RSStrong1StockSet(void) noexcept { return m_fChoiced10RSStrong1StockSet; }
  void SetChoiced10RSStrong1StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong1StockSet = fFlag; }
  bool IsChoiced10RSStrong2StockSet(void) noexcept { return m_fChoiced10RSStrong2StockSet; }
  void SetChoiced10RSStrong2StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong2StockSet = fFlag; }
  bool IsChoiced10RSStrongStockSet(void) noexcept { return m_fChoiced10RSStrongStockSet; }
  void SetChoiced10RSStrongStockSet(bool fFlag) noexcept { m_fChoiced10RSStrongStockSet = fFlag; }
  bool IsCalculateChoiced10RS(void) noexcept { return m_fCalculateChoiced10RS; }
  void SetCalculateChoiced10RS(bool fFlag) noexcept { m_fCalculateChoiced10RS = fFlag; }

  // ���ݿ��ȡ�洢����
  virtual bool SaveRTData(void);  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool TaskSaveDayLineData(void);  // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
  virtual bool UpdateStakeCodeDB(void);
  virtual bool UpdateStakeCodeDB2(void);
  void LoadStakeCodeDB(void);
  void LoadStakeSection(void);
  void CreateStakeSet(void);
  void CreateStakeSection(CStakeSectionPtr pStakeSection);
  void LoadStockCodeDB(void);

  virtual bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  void LoadOptionChinaStockMarketDB(void);
  virtual bool UpdateOptionChinaStockMarketDB(void);
  bool UpdateChoicedStockDB(void);
  virtual bool AppendChoicedStockDB(void);
  void LoadChoicedStockDB(void);
  bool UpdateTempRTData(void);
  virtual bool UpdateTodayTempDB(void);
  bool DeleteTodayTempDB(void);
  bool LoadTodayTempDB(void);
  bool LoadDayLine(CDayLineContainer& dayLineContainer, long lDate);
  bool LoadWeekLineBasicInfo(CWeekLineContainer& weekLineContainer, long lMondayOfWeek);
  bool SaveWeekLine(CWeekLineContainer& weekLineContainer);
  virtual bool SaveActiveStakeCode(void);
  virtual bool SaveStakeSection(void);

  bool DeleteStakeCodeDB(void);
  bool DeleteWeekLine(void);
  bool DeleteWeekLineBasicInfo(void);
  bool DeleteWeekLineExtendInfo(void);
  bool DeleteWeekLine(long lMonday);
  bool DeleteWeekLineBasicInfo(long lMonday);
  bool DeleteWeekLineExtendInfo(long lMonday);

  bool DeleteDayLine(long lDate);
  bool DeleteDayLineBasicInfo(long lDate);
  bool DeleteDayLineExtendInfo(long lDate);

  bool SaveCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  bool LoadCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  bool DeleteCurrentWeekWeekLine(void);
  bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

  bool Load10DaysRSStrong1StockSet(void);
  bool Load10DaysRSStrong2StockSet(void);

  bool LoadCalculatingRSOption(void);
  void SaveCalculatingRSOption(void);

  bool Load10DaysRSStrongStockDB(void);
  bool LoadOne10DaysRSStrongStockDB(long lIndex);

  bool UnloadDayLine(void) noexcept;

  bool BuildWeekLine(long lStartDate);
  virtual bool BuildWeekLineOfCurrentWeek(void);
  bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CChinaStockHistoryDataPtr>*> pvData);
  virtual bool BuildCurrentWeekWeekLineTable(void); // ʹ�����߱�����ǰ�����߱�

// ��Ʊ��ʷ���ݴ���
  virtual bool Choice10RSStrong2StockSet(void); // ѡ��10��ǿ�ƹ�Ʊ�������η�ֵ��
  virtual bool Choice10RSStrong1StockSet(void); // ѡ��10��ǿ�ƹ�Ʊ����һ�η�ֵ��
  virtual bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

  bool IsDayLineNeedUpdate(void) noexcept;
  bool IsDayLineNeedSaving(void);

  virtual long BuildDayLineOfDate(long lCurrentTradeDay);
  virtual bool BuildDayLineRSOfDate(long lDate);
  virtual bool BuildWeekLineRSOfDate(long lDate);
  double GetUpDownRate(CString strClose, CString StrLastClose) noexcept;

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckActiveStock(void) noexcept { return m_fCheckActiveStock; }
  void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsUpdatedStakeCode(void) noexcept { return m_fUpdatedStakeCode; }
  void SetUpdatedStakeCode(bool fFlag) noexcept { m_fUpdatedStakeCode = fFlag; }

  bool IsDayLineDBUpdated(void) noexcept;
  void ClearDayLineDBUpdatedFlag(void) noexcept;

  long GetRSStartDate(void) noexcept { return m_lRSStartDate; }
  void SetRSStartDate(long lDate) noexcept { m_lRSStartDate = lDate; }
  long GetRSEndDate(void) noexcept { return m_lRSEndDate; }
  void SetRSEndDate(long lDate) noexcept { m_lRSEndDate = lDate; }
  long GetLastLoginDate(void) noexcept { return m_lLastLoginDay; }
  void SetLastLoginDate(long lDate) noexcept { m_lLastLoginDay = lDate; }
  long GetUpdatedDateFor10DaysRS1(void) noexcept { return m_lUpdatedDateFor10DaysRS1; }
  void SetUpdatedDateFor10DaysRS1(long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
  long GetUpdatedDateFor10DaysRS2(void) noexcept { return m_lUpdatedDateFor10DaysRS2; }
  void SetUpdatedDateFor10DaysRS2(long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
  long GetUpdatedDateFor10DaysRS(void) noexcept { return m_lUpdatedDateFor10DaysRS; }
  void SetUpdatedDateFor10DaysRS(long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketStake.size(); }
  long GetStockOffset(CString str) { return m_mapChinaMarketStake.at(str); }

  void SetStockCodeForInquiringRTData(CString strStockCode) { m_strStockCodeForInquiringRTData = strStockCode; }
  CString GetStockCodeForInquiringRTData(void) { return m_strStockCodeForInquiringRTData; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) noexcept { return m_ReadingTengxunRTDataTime; }
  void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
  CString GetStockCodeForInquiringNeteaseDayLine(void) { return m_strStockCodeForInquiringNeteaseDayLine; }

  bool InsertActiveStakeCode(CWebRTDataPtr pRTData);
  bool UpdateStakeContainer(CWebRTDataPtr pRTData);

  // ������������ȡ��ʵʱ��Ʊ����
  bool TaskProcessWebRTDataGetFromSinaServer(void);
  void StoreChoiceRTData(CWebRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromTengxunServer(void);
  bool IsInvalidTengxunRTData(CWebData& WebDataReceived);
  void CheckTengxunRTData(CWebRTData& RTData);
  bool TaskProcessWebRTDataGetFromNeteaseServer(void);
  bool IsInvalidNeteaseRTData(CWebData& WebDataReceived);
  bool IsValidNeteaseRTDataPrefix(CWebData& pWebDataReceived);
  bool ValidateNeteaseRTData(CWebRTData& RTData);

  bool TaskGetStakeCodeGetFromSinaServer(void);

  bool TaskDiscardNeteaseRTData(void);
  bool TaskDiscardSinaRTData(void);
  bool TaskDiscardTengxunRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool TaskDistributeSinaRTDataToProperStock(void);
  bool TaskDistributeNeteaseRTDataToProperStock(void);

  void TaskSaveTempDataIntoDB(long lCurrentTime);

  void SetUsingSinaRTDataServer(void) noexcept { m_iRTDataServer = 0; }
  void SetUsingNeteaseRTDataServer(void) noexcept { m_iRTDataServer = 1; }
  bool IsUsingSinaRTDataServer(void) noexcept { if (m_iRTDataServer == 0) return true; else return false; }
  bool IsUsingNeteaseRTDataServer(void) noexcept { if (m_iRTDataServer == 1) return true; else return false; }

  // ״̬����
  bool IsUsingSinaRTDataReceiver(void) noexcept { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) noexcept { return m_fUsingNeteaseRTDataReceiver; }
  void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
  bool IsUsingTengxunRTDataReceiver(void) noexcept { return m_fUsingTengxunRTDataReceiver; }
  void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
  int GetCountDownTengxunNumber(void) noexcept { return m_iCountDownTengxunNumber; }
  void SetCountDownTengxunNumber(int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

  long GetTotalStock(void) noexcept { return m_lTotalStock; }
  long GetTotalStake(void) noexcept { return m_lTotalStake; }
  void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
  time_t GetNewestTransactionTime(void) noexcept { return m_ttNewestTransactionTime; }
  bool IsMarketOpened(void) noexcept { return m_fMarketOpened; }
  void SetMarketOpened(bool fFlag) noexcept { m_fMarketOpened = fFlag; }
  bool IsStartReceivingData(void) noexcept { return m_fStartReceivingData; }
  bool IsGetRTData(void) noexcept { return m_fGetRTData; }
  bool IsSaveDayLine(void) noexcept { return m_fSaveDayLine; }
  void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
  bool IsRTDataSetCleared(void) noexcept { return m_fRTDataSetCleared; }
  void SetRTDataSetCleared(bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
  bool IsSavingTempData(void) noexcept { return m_fSaveTempData; }
  void SetSavingTempData(bool fFlag) noexcept { m_fSaveTempData = fFlag; }

  int GetCountDownSlowReadingRTData(void) noexcept { return m_iCountDownSlowReadingRTData; }
  bool IsCurrentEditStockChanged(void) noexcept { return m_fCurrentEditStockChanged; }
  void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

  bool AddChoicedStock(CChinaStakePtr pStake);
  bool DeleteChoicedStock(CChinaStakePtr pStake);
  size_t GetChoicedStockSize(void) { return m_avChoicedStock.at(0).size(); }
  size_t GetChoicedStockSize(long lIndex) { return m_avChoicedStock.at(lIndex).size(); }
  void ClearChoiceStockContainer(void) { m_avChoicedStock.at(0).clear(); }
  size_t GetChoicedRTDataSize(void) noexcept { return m_qRTData.size(); }
  void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

  void ResetSinaStockRTDAtaInquiringIndex(void) noexcept { m_lSinaStockRTDataInquiringIndex = 0; }
  void ResetSinaStakeRTDAtaInquiringIndex(void) noexcept { m_lSinaStakeRTDataInquiringIndex = 0; }
  void ResetTengxunRTDataInquiringIndex(void) noexcept { m_lTengxunRTDataInquiringIndex = 0; }
  void ResetNeteaseRTDataInquiringIndex(void) noexcept { m_lNeteaseRTDataInquiringIndex = 0; }
  void ResetNeteaseDayLineDataInquiringIndex(void) noexcept { m_lNeteaseDayLineDataInquiringIndex = 0; }
  long GetSinaStockRTDataInquiringIndex(void) noexcept { return m_lSinaStockRTDataInquiringIndex; }
  long GetSinaStakeRTDataInquiringIndex(void) noexcept { return m_lSinaStakeRTDataInquiringIndex; }
  long GetTengxunRTDataInquiringIndex(void) noexcept { return m_lTengxunRTDataInquiringIndex; }
  long GetNeteaseRTDataInquiringIndex(void) noexcept { return m_lNeteaseRTDataInquiringIndex; }
  long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_lNeteaseDayLineDataInquiringIndex; }

  int GetDayLineNeedUpdateNumber(void) noexcept { const int i = m_iDayLineNeedUpdate; return i; }
  void SetDayLineNeedUpdateNumber(int i) noexcept { m_iDayLineNeedUpdate = i; }
  void ClearDayLineNeedUpdaeStatus(void);
  int GetDayLineNeedProcessNumber(void) noexcept { const int i = m_iDayLineNeedProcess; return i; }
  void SetDayLineNeedProcessNumber(int i) noexcept { m_iDayLineNeedProcess = i; }
  int GetDayLineNeedSaveNumber(void) noexcept { const int i = m_iDayLineNeedSave; return i; }
  void SetDayLineNeedSaveNumber(int i) noexcept { m_iDayLineNeedSave = i; }
  void IncreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1);
  void DecreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1);
  void IncreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) noexcept { m_iDayLineNeedProcess += iNumber; }
  void DecreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) noexcept { if (m_iDayLineNeedProcess >= iNumber) m_iDayLineNeedProcess -= iNumber; }
  void IncreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) noexcept { m_iDayLineNeedSave += iNumber; }
  void DecreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) noexcept { if (m_iDayLineNeedSave >= iNumber) m_iDayLineNeedSave -= iNumber; }

  void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
  bool IsRecordingRTData(void) noexcept { if (m_fSaveRTData) return true; else return false; }

  void SetUpdateStakeCodeDB(bool fFlag) noexcept { m_fUpdateStockCodeDB = fFlag; }
  bool IsUpdateStakeCodeDB(void) noexcept { const bool fFlag = m_fUpdateStockCodeDB; return fFlag; }
  void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
  bool IsUpdateOptionDB(void) noexcept { const bool fFlag = m_fUpdateOptionDB; return fFlag; }
  void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
  bool IsUpdateChoicedStockDB(void) noexcept { const bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

  INT64 GetRTDataReceived(void) noexcept { return m_llRTDataReceived; }
  void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }

  bool CheckMarketReady(void);

  bool ChangeToNextStock(void);
  bool ChangeToPrevStock(void);
  bool ChangeToPrevStockSet(void);
  bool ChangeToNextStockSet(void);
  bool IsTotalStockSetSelected(void) noexcept { if (m_lCurrentSelectedStockSet == -1) return true; else return false; }
  size_t GetCurrentStockSetSize(void);

  void SetStockNeedUpdated(long lValue) noexcept { m_lStockNeedUpdated = lValue; }
  bool TooManyStocksNeedUpdated(void) noexcept { if (m_lStockNeedUpdated > 1000) return true; else return false; }

  void SetUpdateStakeSection(bool fFlag) noexcept { m_fUpdateStakeSection = fFlag; }
  bool IsUpdateStakeSection(void) noexcept { return m_fUpdateStakeSection; }

protected:
  // ��ʼ��
  bool CreateTotalStockContainer(void); //�˺����ǹ��캯����һ���֣������������á�ʹ��Mock�����ʱ������Mock���н�CChinaStake��ΪCMockChinaStake��
  void CreateStakeSection(CString strFirstStockCode, bool fProcessRTData);
  bool UpdateStakeSection(CString strStakeCode);
  bool UpdateStakeSection(long lIndex);

public:

protected:
  // ���г���ѡ��
  int m_iRTDataServer; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
  int m_iRTDataInquiryTickNumber; // ����ʵʱ������ѯ��ѯʱ�䣬Ĭ��Ϊ400����(3tick),�������ܱ�֤3��������ѯȫ���Ծ��Ʊһ�Ρ�

// ������
protected:
  vector<CString> m_vCurrentStockSet; // ��ǰ��Ʊ�����ַ����ĸ�ʽΪsh600000��sz000001
  vector<CStakeSectionPtr> m_vStakeSection; // ��2000�����Ϻ����ڸ�1000��֤ȯ��������λ�Ƿ��Ѿ���ʹ�á�
  bool m_fUpdateStakeSection; // ����StakeSection��ʶ

  vector<CStakeCodePtr> m_vChinaMarketActiveStakeCode; // ��ϵͳ�����е�֤ȯ����⣨������Ʊ������ծȯ���ڻ�����Ȩ�ȡ�ÿ�ո��£���Ϊ��Ч֤ȯ���룩
  map<CString, long> m_mapChinaMarketActiveStakeCode; // �����в�ѯ����֤ȯ����ӳ��Ϊƫ������
  long m_lCurrentStakeCodeIndex;
  long m_lCurrentStakeInquiringIndex; // ��ǰ����֤ȯ���ݵ�λ��
  long m_lTotalStakeCode; // ��ǰ֤ȯ��������
  long m_lTotalStakeCodeLastTime; // �ϴΣ����ݿ��еģ�֤ȯ��������

  vector<CChinaStakePtr> m_vChinaMarketStake; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  map<CString, long> m_mapChinaMarketStake; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
  long m_lTotalStock; // ��Ʊ����������Ŀǰ����Ϊ�̶���12000����λ��֤ȯǰ������
  long m_lTotalStake; // ֤ȯ����������ǰ12000��Ϊ��Ʊ���룬���Ϊ����֤ȯ��
  long m_lTotalActiveStock;	// �����Ʊ����
  long m_lLoadedStake; // ����װ�صĹ�Ʊ����

  vector<CChinaStakePtr> m_v10RSStrong1Stock; // 10��ǿ�ƹ�Ʊ��
  vector<CChinaStakePtr> m_v10RSStrong2Stock; // 10��ǿ�ƹ�Ʊ��
  vector<CRSReference> m_aRSStrongOption; // ���ڼ���RS�Ĳ��������ʮ����
  vector<vector<CChinaStakePtr> > m_avChoicedStock; // ����ѡ��Ĺ�Ʊ����0-9����ѡ��Ʊ����10-19��10��RS��Ʊ����20-29���ɼ۱仯��Ʊ��
  long m_lCurrentSelectedPosition; // ��ǰ��Ʊ����λ��
  long m_lCurrentRSStrongIndex; // �����ڴ��ݵ�ǰ��λ�ã�������ѡ����ȷ�����ݱ�
  long m_lCurrentSelectedStockSet; // ��ǰѡ��Ĺ�Ʊ����-1Ϊ�����Ʊ����1-10Ϊ10��RS���Թ�Ʊ�����Դ����ƣ���
  bool m_fChoiced10RSStrong1StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
  bool m_fChoiced10RSStrong2StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
  bool m_fChoiced10RSStrongStockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
  bool m_fCalculateChoiced10RS;

  INT64 m_llRTDataReceived; // ���յ���ʵʱ��������

  long m_lStockNeedUpdated; // ��Ʊ��ʷ���߽�����Ҫ������

  queue<CWebRTDataPtr> m_qRTData;
  bool m_fSaveRTData;

  int m_iMarketOpenOffset; // ���е�ƫ�������Է���Ϊ��λ��0930 = 0��1129 = 120�� 1300 = 121�� 1459 = 240��

  bool m_fCurrentEditStockChanged;
  int m_iCountDownSlowReadingRTData; // ���ٶ�ȡʵʱ���ݼ�����
  bool m_fMarketOpened; // �Ƿ���
  bool m_fStartReceivingData; // �Ƿ�ʼ����ʵʱ����
  bool m_fGetRTData; // ��ȡʵʱ���ݱ�ʶ
  bool m_fSaveDayLine; // ����ȡ�����ߴ������ݿ��ʶ
  bool m_fRTDataSetCleared; // ʵʱ���ݿ��������ʶ���ŵ���ʮ��֮ǰΪ�٣�֮������Ϊ�档
  bool m_fSaveTempData; // �洢��ʱʵʱ���ݱ�ʶ
  CChinaStakePtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ

  time_t m_ttNewestTransactionTime;

  bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��
  int m_iCountDownTengxunNumber;

  CString m_strSinaRTDataInquiringStr;
  CString m_strTengxunRTDataInquiringStr;
  CString m_strNeteaseRTDataInquiringStr;
  CString m_strNeteaseDayLineDataInquiringStr;

  long m_lSinaStockRTDataInquiringIndex;
  long m_lSinaStakeRTDataInquiringIndex;
  long m_lTengxunRTDataInquiringIndex;
  long m_lNeteaseRTDataInquiringIndex;
  long m_lNeteaseDayLineDataInquiringIndex;

  // Option��ѡ��
  long m_lRSStartDate;
  long m_lRSEndDate;
  long m_lLastLoginDay; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����
  long m_lUpdatedDateFor10DaysRS2;
  long m_lUpdatedDateFor10DaysRS1;
  long m_lUpdatedDateFor10DaysRS;

  vector<CChinaStakePtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
  INT64 m_lTotalMarketBuy; // �����г��е�A������������
  INT64 m_lTotalMarketSell; // �����г��е�A�������������

  // ϵͳ״̬��
  int m_iTodayStockProcessed; // �����Ƿ�ִ���˹�Ʊ����.0:��δִ�У�1������ִ���У�2����ִ���ꡣ
  bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
  bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��
  bool m_fUpdatedStakeCode; // �����Ƿ�ִ���˸���֤ȯ����������

  // ���̶߳�ȡ֮����
  CString m_strStockCodeForInquiringRTData;
  clock_t m_ReadingTengxunRTDataTime; // ÿ�ζ�ȡ��Ѷʵʱ���ݵ�ʱ��
  CString m_strStockCodeForInquiringNeteaseDayLine;

  // ���¹�Ʊ�������ݿ��ʶ
  atomic_bool m_fUpdateStockCodeDB;
  atomic_bool m_fUpdateOptionDB;
  bool m_fUpdateChoicedStockDB;

  // ����������ʷ���ݶ�ȡ����ʹ洢��������
  atomic_int m_iDayLineNeedUpdate; // ������Ҫ���µĹ�Ʊ����
  atomic_int m_iDayLineNeedProcess; // ������Ҫ����Ĺ�Ʊ����
  atomic_int m_iDayLineNeedSave; // ������Ҫ�洢�Ĺ�Ʊ����

private:
};

typedef shared_ptr<CChinaMarket> CChinaMarketPtr;
