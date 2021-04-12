#pragma once
#include"globedef.h"
#include"Accessory.h"
#include"Semaphore.h"

#include"VirtualMarket.h"

#include"WebRTDataContainer.h"
#include"StockSection.h"

#include "ChinaStock.h"

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
  virtual bool SchedulingTask(void) override final; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  bool SchedulingTaskPerSecond(long lSecondNumber, long lCurrentTime); // ÿ�����һ��
  bool SchedulingTaskPer10Seconds(long lCurrentTime); // ÿʮ�����һ��
  bool SchedulingTaskPerMinute(long lCurrentTime); // ÿһ���ӵ���һ��
  bool SchedulingTaskPer5Minutes(long lCurrentTime); // ÿ����ӵ���һ��
  bool SchedulingTaskPerHour(long lCurrentTime); // ÿСʱ����һ��

  // ��������
  bool TaskGetRTDataFromWeb(void);
  bool TaskGetNeteaseDayLineFromWeb(void);

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

  bool TaskSaveStockSection(void); //

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
  bool TaskProcessRTData(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
  bool TaskProcessDayLineGetFromNeeteaseServer(void);

  // װ�ص�ǰ��Ʊ��������
  bool TaskLoadCurrentStockHistoryData(void);

  // �������̵߳��ð�������
  virtual bool RunningThreadAppendChoicedStockDB(void);
  virtual bool RunningThreadBuildDayLineRS(long lStartCalculatingDay);
  virtual bool RunningThreadBuildDayLineRSOfDate(long lThisDay);
  virtual bool RunningThreadBuildWeekLine(long lStartDate);
  virtual bool RunningThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate);
  virtual bool RunningThreadBuildWeekLineRS(void);
  virtual bool RunningThreadBuildWeekLineRSOfDate(long lThisDay);
  virtual bool RunningThreadBuildWeekLineOfCurrentWeek(void);
  virtual bool RunningThreadBuildCurrentWeekWeekLineTable(void);
  virtual bool RunningThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock);
  virtual bool RunningThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock);
  virtual bool RunningThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock);
  virtual bool RunningThreadChoice10RSStrong1StockSet(void);
  virtual bool RunningThreadChoice10RSStrongStockSet(void);
  virtual bool RunningThreadChoice10RSStrong2StockSet(void);
  virtual bool RunningThreadLoadDayLine(CChinaStock* pCurrentStock);
  virtual bool RunningThreadLoadWeekLine(CChinaStock* pCurrentStock);
  virtual bool RunningThreadProcessTodayStock(void);
  virtual bool RunningThreadSaveChoicedRTData(void);
  virtual bool RunningThreadSaveTempRTData(void);
  virtual bool RunningThreadSaveDayLineBasicInfoOfStock(CChinaStock* pStock);
  virtual bool RunningThreadSaveStockSection(void);
  virtual bool RunningThreadUpdateStockCodeDB(void);
  virtual bool RunningThreadUpdateOptionDB(void);
  // interface function

public:
  // ϵͳ״̬��

  // ��ʼ���г�

  // ʵʱ���ݶ�ȡ
  CString GetSinaStockInquiringStr(long lTotalNumber, bool fCheckActiveStock);
  CString GetTengxunInquiringStockStr(long lTotalNumber, long lEndPosition);
  CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, long lEndPosition, bool fCheckActiveStock);
  CString	GetNextNeteaseStockInquiringStr(long lTotalNumber, long lEndPosition);
  bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
  CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(long& lStockIndex, CString strPostfix, long lTotalNumber);
  CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(CString strPostfix, long lTotalNumber);
  CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, long lEndPosition);
  //������ʷ���ݶ�ȡ
  bool CreateNeteaseDayLineInquiringStr(CString& strReturn, long lEndPosition);
  long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

  bool IsAStock(CChinaStockPtr pStock) const; // �Ƿ�Ϊ����A��
  bool IsAStock(CString strStockCode) const; // �Ƿ�Ϊ����A��
  bool IsStock(CString strStockCode) const;	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

  CString GetStockName(CString strStockCode);

  // �õ���Ʊָ��
  CChinaStockPtr GetStock(CString strStockCode);
  CChinaStockPtr GetStock(long lIndex);

  // �õ���ǰ��ʾ��Ʊ
  CChinaStockPtr GetCurrentStock(void) const noexcept { return m_pCurrentStock; }
  void SetCurrentStock(CString strStockCode);
  void SetCurrentStock(CChinaStockPtr pStock);
  void ResetCurrentStock(void);
  bool IsCurrentStockChanged(void) const noexcept { return m_fCurrentStockChanged; }
  void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

  long GetMinLineOffset(time_t tMarket);

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
  bool TaskSaveDayLineData(void);  // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
  virtual bool UpdateStockCodeDB(void);
  void LoadStockSection(void);
  void LoadStockCodeDB(void);

  virtual bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  void LoadOptionChinaStockMarketDB(void);
  virtual bool UpdateOptionChinaMarketDB(void);
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
  virtual bool SaveStockSection(void);

  bool ChangeDayLineStockCodeToStandred(void);

  virtual bool DeleteWeekLine(void);
  bool DeleteWeekLineBasicInfo(void);
  bool DeleteWeekLineExtendInfo(void);
  virtual bool DeleteWeekLine(long lMonday);
  bool DeleteWeekLineBasicInfo(long lMonday);
  bool DeleteWeekLineExtendInfo(long lMonday);

  bool DeleteDayLine(long lDate);
  bool DeleteDayLineBasicInfo(long lDate);
  bool DeleteDayLineExtendInfo(long lDate);

  bool SaveCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  bool LoadCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  virtual bool DeleteCurrentWeekWeekLine(void);
  bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

  bool Load10DaysRSStrong1StockSet(void);
  bool Load10DaysRSStrong2StockSet(void);

  bool LoadCalculatingRSOption(void);
  void SaveCalculatingRSOption(void);

  bool Load10DaysRSStrongStockDB(void);
  bool LoadOne10DaysRSStrongStockDB(long lIndex);

  bool UnloadDayLine(void) noexcept;

  virtual bool BuildWeekLine(long lStartDate);
  virtual bool BuildWeekLineOfCurrentWeek(void);
  bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CChinaStockHistoryDataPtr>*> pvData);
  virtual bool BuildCurrentWeekWeekLineTable(void); // ʹ�����߱�����ǰ�����߱�

  bool SortStockVector(void);

  // ��Ʊ��ʷ���ݴ���
  virtual bool Choice10RSStrong2StockSet(void); // ѡ��10��ǿ�ƹ�Ʊ�������η�ֵ��
  virtual bool Choice10RSStrong1StockSet(void); // ѡ��10��ǿ�ƹ�Ʊ����һ�η�ֵ��
  virtual bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

  bool IsDayLineNeedUpdate(void) const noexcept;
  bool IsDayLineNeedProcess(void) const noexcept;
  bool IsDayLineNeedSaving(void) const;
  long GetDayLineNeedUpdateNumber(void);
  long GetDayLineNeedProcessNumber(void);
  long GetDayLineNeedSaveNumber(void);

  virtual long BuildDayLineOfDate(long lCurrentTradeDay);
  virtual bool BuildDayLineRSOfDate(long lDate);
  virtual bool BuildWeekLineRSOfDate(long lDate);
  double GetUpDownRate(CString strClose, CString StrLastClose) noexcept;

  bool IsLoadSelectedStock(void) const noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckActiveStock(void) const noexcept { return m_fCheckActiveStock; }
  void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsDayLineDBUpdated(void) noexcept;
  void ClearDayLineDBUpdatedFlag(void) noexcept;

  long GetRSStartDate(void) const noexcept { return m_lRSStartDate; }
  void SetRSStartDate(long lDate) noexcept { m_lRSStartDate = lDate; }
  long GetRSEndDate(void) const noexcept { return m_lRSEndDate; }
  void SetRSEndDate(long lDate) noexcept { m_lRSEndDate = lDate; }
  long GetLastLoginDate(void) const noexcept { return m_lLastLoginDay; }
  void SetLastLoginDate(long lDate) noexcept { m_lLastLoginDay = lDate; }
  long GetUpdatedDateFor10DaysRS1(void) const noexcept { return m_lUpdatedDateFor10DaysRS1; }
  void SetUpdatedDateFor10DaysRS1(long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
  long GetUpdatedDateFor10DaysRS2(void) const noexcept { return m_lUpdatedDateFor10DaysRS2; }
  void SetUpdatedDateFor10DaysRS2(long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
  long GetUpdatedDateFor10DaysRS(void) const noexcept { return m_lUpdatedDateFor10DaysRS; }
  void SetUpdatedDateFor10DaysRS(long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapSize(void) const noexcept { return m_mapChinaMarketStock.size(); }
  long GetStockOffset(CString str) const { return m_mapChinaMarketStock.at(str); }

  void SetStockCodeForInquiringRTData(CString strStockCode) { m_strStockCodeForInquiringRTData = strStockCode; }
  CString GetStockCodeForInquiringRTData(void) const { return m_strStockCodeForInquiringRTData; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) const noexcept { return m_ReadingTengxunRTDataTime; }
  void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
  CString GetStockCodeForInquiringNeteaseDayLine(void) const { return m_strStockCodeForInquiringNeteaseDayLine; }

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

  bool TaskDiscardNeteaseRTData(void);
  bool TaskDiscardSinaRTData(void);
  bool TaskDiscardTengxunRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool TaskDistributeSinaRTDataToProperStock(void);
  bool TaskDistributeNeteaseRTDataToProperStock(void);

  void TaskSaveTempDataIntoDB(long lCurrentTime);

  void SetUsingSinaRTDataServer(void) noexcept { m_iRTDataServer = 0; }
  void SetUsingNeteaseRTDataServer(void) noexcept { m_iRTDataServer = 1; }
  bool IsUsingSinaRTDataServer(void) const noexcept { if (m_iRTDataServer == 0) return true; else return false; }
  bool IsUsingNeteaseRTDataServer(void) const noexcept { if (m_iRTDataServer == 1) return true; else return false; }

  // ״̬����
  bool IsUsingSinaRTDataReceiver(void) const noexcept { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) const noexcept { return m_fUsingNeteaseRTDataReceiver; }
  void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
  bool IsUsingTengxunRTDataReceiver(void) const noexcept { return m_fUsingTengxunRTDataReceiver; }
  void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
  int GetCountDownTengxunNumber(void) const noexcept { return m_iCountDownTengxunNumber; }
  void SetCountDownTengxunNumber(int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

  long GetTotalStock(void) const noexcept { return (long)m_vChinaMarketStock.size(); }
  long GetTotalLoadedStock(void) const noexcept { return m_lLoadedStock; }
  void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
  time_t GetNewestTransactionTime(void) const noexcept { return m_ttNewestTransactionTime; }
  bool IsMarketOpened(void) const noexcept { return m_fMarketOpened; }
  void SetMarketOpened(bool fFlag) noexcept { m_fMarketOpened = fFlag; }
  bool IsFastReceivingRTData(void) const noexcept { return m_fFastReceivingRTData; }
  bool IsGetRTData(void) const noexcept { return m_fGetRTData; }
  bool IsSaveDayLine(void) const noexcept { return m_fSaveDayLine; }
  void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
  bool IsRTDataSetCleared(void) const noexcept { return m_fRTDataSetCleared; }
  void SetRTDataSetCleared(bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
  bool IsSavingTempData(void) const noexcept { return m_fSaveTempData; }
  void SetSavingTempData(bool fFlag) noexcept { m_fSaveTempData = fFlag; }

  int GetCountDownSlowReadingRTData(void) const noexcept { return m_iCountDownSlowReadingRTData; }
  bool IsCurrentEditStockChanged(void) const noexcept { return m_fCurrentEditStockChanged; }
  void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

  bool AddChoicedStock(CChinaStockPtr pStock);
  bool DeleteChoicedStock(CChinaStockPtr pStock);
  size_t GetChoicedStockSize(void) const { return m_avChoicedStock.at(0).size(); }
  size_t GetChoicedStockSize(long lIndex) const { return m_avChoicedStock.at(lIndex).size(); }
  void ClearChoiceStockContainer(void) { m_avChoicedStock.at(0).clear(); }
  size_t GetChoicedRTDataSize(void) const noexcept { return m_qRTData.size(); }
  void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

  void SetSinaStockRTDataInquiringIndexFromTotalStockSet(long lIndex) noexcept { m_lSinaStockRTDataInquiringIndexFromTotalStockSet = lIndex; }
  long GetSinaStockRTDataInquiringIndexFromTotalStockSet(void) const noexcept { return m_lSinaStockRTDataInquiringIndexFromTotalStockSet; }
  void SetSinaStockRTDataInquiringIndex(long lIndex) noexcept { m_lSinaStockRTDataInquiringIndex = lIndex; }
  void SetTengxunRTDataInquiringIndex(long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
  void SetNeteaseRTDataInquiringIndex(long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
  void SetNeteaseDayLineDataInquiringIndex(long lIndex) noexcept { m_lNeteaseDayLineDataInquiringIndex = lIndex; }
  long GetSinaStockRTDataInquiringIndex(void) const noexcept { return m_lSinaStockRTDataInquiringIndex; }
  long GetTengxunRTDataInquiringIndex(void) const noexcept { return m_lTengxunRTDataInquiringIndex; }
  long GetNeteaseRTDataInquiringIndex(void) const noexcept { return m_lNeteaseRTDataInquiringIndex; }
  long GetNeteaseDayLineDataInquiringIndex(void) const noexcept { return m_lNeteaseDayLineDataInquiringIndex; }

  void ClearDayLineNeedUpdateStatus(void);

  void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
  bool IsRecordingRTData(void) const noexcept { if (m_fSaveRTData) return true; else return false; }

  bool IsUpdateStockCodeDB(void);
  void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
  bool IsUpdateOptionDB(void) const noexcept { const bool fFlag = m_fUpdateOptionDB; return fFlag; }
  void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
  bool IsUpdateChoicedStockDB(void) const noexcept { const bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

  INT64 GetRTDataReceived(void) const noexcept { return m_llRTDataReceived; }
  void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }

  bool CheckMarketReady(void);

  bool ChangeToNextStock(void);
  bool ChangeToPrevStock(void);
  bool ChangeToPrevStockSet(void);
  bool ChangeToNextStockSet(void);
  bool IsTotalStockSetSelected(void) const noexcept { if (m_lCurrentSelectedStockSet == -1) return true; else return false; }
  size_t GetCurrentStockSetSize(void);

  void SetStockDayLineNeedUpdate(long lValue) noexcept { m_lStockDayLineNeedUpdate = lValue; }
  bool TooManyStockDayLineNeedUpdate(void) const noexcept { if (m_lStockDayLineNeedUpdate > 1000) return true; else return false; }

  void SetUpdateStockSection(bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
  bool IsUpdateStockSection(void) const noexcept { return m_fUpdateStockSection; }

  bool AddStock(CChinaStockPtr pStock);
  bool DeleteStock(CChinaStockPtr pStock);
  bool CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData);

protected:
  // ��ʼ��
  bool CreateTotalStockContainer(void); //�˺����ǹ��캯����һ���֣������������á�ʹ��Mock�����ʱ������Mock���н�CChinaStock��ΪCMockChinaStock��
  void CreateStockSection(CString strFirstStockCode, bool fProcessRTData);
  bool UpdateStockSection(CString strStockCode);
  bool UpdateStockSection(long lIndex);

public:
  // ����ר�ú���

protected:
  // ���г���ѡ��
  int m_iRTDataServer; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
  int m_iRTDataInquiryTickNumber; // ����ʵʱ������ѯ��ѯʱ�䣬Ĭ��Ϊ400����(3tick),�������ܱ�֤3��������ѯȫ���Ծ��Ʊһ�Ρ�

// ������
protected:
  vector<CString> m_vCurrentSectionStockCode; // ��ǰ��Ʊ���ĵ�һ�����롣�ַ����ĸ�ʽΪ600000.SS��sz000001
  vector<CStockSectionPtr> m_vStockSection; // ��2000�����Ϻ����ڸ�1000��֤ȯ��������λ�Ƿ��Ѿ���ʹ�á�
  bool m_fUpdateStockSection; // ����StockSection��ʶ

  vector<CString> m_vTotalStockSet; // ��ǰ���еĹ�Ʊ����
  map<CString, long> m_mapTotalStockSet; // ��ǰ���й�Ʊ�����ӳ��

  vector<CChinaStockPtr> m_vChinaMarketStock; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  map<CString, long> m_mapChinaMarketStock; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
  long m_lLoadedStock; // ����װ�صĹ�Ʊ����

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

  INT64 m_llRTDataReceived; // ���յ���ʵʱ��������

  long m_lStockDayLineNeedUpdate; // ��Ʊ��ʷ���߽�����Ҫ������

  queue<CWebRTDataPtr> m_qRTData;
  bool m_fSaveRTData;

  int m_iMarketOpenOffset; // ���е�ƫ�������Է���Ϊ��λ��0930 = 0��1129 = 120�� 1300 = 121�� 1459 = 240��

  bool m_fCurrentEditStockChanged;
  int m_iCountDownSlowReadingRTData; // ���ٶ�ȡʵʱ���ݼ�����
  bool m_fMarketOpened; // �Ƿ���
  bool m_fFastReceivingRTData; // �Ƿ�ʼ����ʵʱ����
  bool m_fGetRTData; // ��ȡʵʱ���ݱ�ʶ
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

  long m_lSinaStockRTDataInquiringIndexFromTotalStockSet;
  long m_lSinaStockRTDataInquiringIndex;
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

  vector<CChinaStockPtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
  INT64 m_lTotalMarketBuy; // �����г��е�A������������
  INT64 m_lTotalMarketSell; // �����г��е�A�������������

  // ϵͳ״̬��
  int m_iTodayStockProcessed; // �����Ƿ�ִ���˹�Ʊ����.0:��δִ�У�1������ִ���У�2����ִ���ꡣ
  bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
  bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

  // ���̶߳�ȡ֮����
  CString m_strStockCodeForInquiringRTData;
  clock_t m_ReadingTengxunRTDataTime; // ÿ�ζ�ȡ��Ѷʵʱ���ݵ�ʱ��
  CString m_strStockCodeForInquiringNeteaseDayLine;

  // ���¹�Ʊ�������ݿ��ʶ
  atomic_bool m_fUpdateOptionDB;
  bool m_fUpdateChoicedStockDB;

private:
};

typedef shared_ptr<CChinaMarket> CChinaMarketPtr;
