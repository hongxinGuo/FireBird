#pragma once

#include"stdafx.h"
#include"globedef.h"
#include"Accessory.h"
#include"Semaphore.h"

#include"VirtualMarket.h"

#include"RTDataContainer.h"

#include "ChinaStock.h"

using namespace MyAccessory;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
extern Semaphore gl_SaveOneStockDayLine;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SemaphoreCalculateDayLineRS;
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

extern CRTDataContainer gl_RTDataContainer;

class CChinaMarket final : public CVirtualMarket
{
public:
  // ֻ����һ��ʵ��
  CChinaMarket(void);
  ~CChinaMarket(void);
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

  bool TaskProcessTengxunRTData(void);  // ������Ѷʵʱ����
  bool TaskSetCheckActiveStockFlag(long lCurrentTime);
  bool TaskProcessTodayStock(long lCurrentTime);
  bool TaskCheckDayLineDB(void);
  bool TaskCheckStartReceivingData(long lCurrentTime);
  bool TaskCheckMarketOpen(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  bool TaskResetMarketAgain(long lCurrentTime);

  bool TaskUpdateStockCodeDB(void);
  bool TaskUpdateOptionDB(void);

  bool TaskShowCurrentTransaction(void);

  bool TaskSaveChoicedRTData(void);
  bool TaskClearChoicedRTDataSet(long lCurrentTime);

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
  bool TaskProcessRTData(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
  bool TaskProcessDayLineGetFromNeeteaseServer(void);

  // װ�ص�ǰ��Ʊ��������
  bool TaskLoadCurrentStockDayLine(void);

  // interface function
public:
  // ϵͳ״̬��

  // ��ʼ���г�

  // ʵʱ���ݶ�ȡ
  CString GetSinaInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock = true);
  CString GetTengxunInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock = true);
  CString	GetNeteaseInquiringStockStr(long lTotalNumber = 700, bool fSkipUnactiveStock = true);
  bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
  CString GetNextInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock = true);
  bool StepToActiveStockIndex(long& lStockIndex);

  //������ʷ���ݶ�ȡ
  CString CreateNeteaseDayLineInquiringStr(void);

  long IncreaseStockInquiringIndex(long& lIndex);

  bool IsAStock(CChinaStockPtr pStock); // �Ƿ�Ϊ����A��
  bool IsAStock(CString strStockCode); // �Ƿ�Ϊ����A��
  bool IsStock(CString  strStockCode);	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

  CString GetStockName(CString strStockCode);

  // �õ���Ʊ����
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // �õ���Ʊָ��
  CChinaStockPtr GetStock(CString strStockCode);
  CChinaStockPtr GetStock(long lIndex);

  void IncreaseActiveStockNumber(void);

  // �õ���ǰ��ʾ��Ʊ
  CChinaStockPtr GetCurrentStock(void) noexcept { return m_pCurrentStock; }
  void SetCurrentStock(CString strStockCode);
  void SetCurrentStock(CChinaStockPtr pStock);
  bool IsCurrentStockChanged(void) noexcept { return m_fCurrentStockChanged; }
  void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

  long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }
  void SetTotalActiveStock(long lValue) noexcept { m_lTotalActiveStock = lValue; }

  long GetMinLineOffset(CChinaStock sID, time_t Time);

  bool IsSystemReady(void) noexcept { return m_fSystemReady; }
  void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

  bool IsTodayStockProcessed(void) noexcept { return m_fTodayStockProcessed; }
  void SetTodayStockProcessed(bool fFlag) noexcept { m_fTodayStockProcessed = fFlag; }

  // ���ݿ��ȡ�洢����
  bool SaveRTData(void);  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool SaveDayLineData(void);  // ������ʷ���ݴ�����������ȡ����������ʷ���ݴ������ݿ���
  bool UpdateStockCodeDB(void);
  void LoadStockCodeDB(void);
  bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  bool UpdateTempRTData(void);
  bool UpdateTodayTempDB(void);
  bool LoadTodayTempDB(void);

  bool ClearDayLineContainer(void);

  // ��Ʊ��ʷ���ݴ���
  bool IsDayLineNeedUpdate(void);
  bool IsDayLineNeedSaving(void);

  long ProcessCurrentTradeDayStock(long lCurrentTradeDay);
  bool CalculateRelativeStrong(long lStartCalculatingDay);
  bool CalculateOneDayRelativeStrong(long lDay);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckActiveStock(void) noexcept { return m_fCheckActiveStock; }
  void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsDayLineDBUpdated(void);
  void ClearDayLineDBUpdatedFlag(void);

  long GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }
  long GetLastLoginDay(void) noexcept { return m_lLastLoginDay; }
  void SetLastLoginDay(long lDay) noexcept { m_lLastLoginDay = lDay; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketAStock.size(); }
  long GetTotalStockIndex(CString str) { return m_mapChinaMarketAStock.at(str); }

  void SetStockCodeForInquiringSinaRTData(CString strStockCode) noexcept { m_strStockCodeForInquiringSinaRTData = strStockCode; }
  CString GetStockCodeForInquiringSinaRTData(void) noexcept { return m_strStockCodeForInquiringSinaRTData; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) noexcept { return m_ReadingTengxunRTDataTime; }
  void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) noexcept { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
  CString GetStockCodeForInquiringNeteaseDayLine(void) noexcept { return m_strStockCodeForInquiringNeteaseDayLine; }

  // ������������ȡ��ʵʱ��Ʊ����
  bool TaskProcessWebRTDataGetFromSinaServer(void);
  void StoreChoiceRTData(CRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromTengxunServer(void);
  bool IsInvalidTengxunRTData(CWebDataPtr pWebDataReceived);
  void CheckTengxunRTData(CRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromNeteaseServer(void);
  bool IsInvalidNeteaseRTData(CWebDataPtr pWebDataReceived);
  bool IsValidNeteaseRTDataPrefix(CWebDataPtr pWebDataReceived);
  void CheckNeteaseRTData(CRTDataPtr pRTData);
  bool TaskProcessNeteaseRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool TaskDistributeSinaRTDataToProperStock(void);

  void TaskSaveTempDataIntoDB(long lCurrentTime);

  // ״̬����
  bool IsUsingSinaRTDataReceiver(void) noexcept { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) noexcept { return m_fUsingNeteaseRTDataReceiver; }
  bool IsUsingTengxunRTDataReceiver(void) noexcept { return m_fUsingTengxunRTDataReceiver; }

  long GetTotalStock(void) noexcept { return m_lTotalStock; }
  void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
  time_t GetNewestTransactionTime(void) noexcept { return m_ttNewestTransactionTime; }
  bool IsMarketOpened(void) noexcept { return m_fMarketOpened; }
  bool IsStartReceivingData(void) noexcept { return m_fStartReceivingData; }
  bool IsGetRTData(void) noexcept { return m_fGetRTData; }
  bool IsSaveDayLine(void) noexcept { return m_fSaveDayLine; }
  void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
  bool IsRTDataSetCleared(void) noexcept { return m_fRTDataSetCleared; }

  int GetCountDownSlowReadingRTData(void) noexcept { return m_iCountDownSlowReadingRTData; }
  bool IsCurrentEditStockChanged(void) noexcept { return m_fCurrentEditStockChanged; }
  void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

  void StoreChoiceStock(CChinaStockPtr pStock) noexcept { m_vStockChoice.push_back(pStock); }
  long GetChoicedRTDataSize(void) noexcept { return m_qRTData.size(); }
  void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

  void ResetSinaRTDataInquiringIndex(void) noexcept { m_lSinaRTDataInquiringIndex = 0; }
  void ResetTengxunRTDataInquiringIndex(void) noexcept { m_lTengxunRTDataInquiringIndex = 0; }
  void ResetNeteaseRTDataInquiringIndex(void) noexcept { m_lNeteaseRTDataInquiringIndex = 0; }
  void ResetNeteaseDayLineDataInquiringIndex(void) noexcept { m_lNeteaseDayLineDataInquiringIndex = 0; }
  long GetSinaRTDataInquiringIndex(void) noexcept { return m_lSinaRTDataInquiringIndex; }
  long GetTengxunRTDataInquiringIndex(void) noexcept { return m_lTengxunRTDataInquiringIndex; }
  long GetNeteaseRTDataInquiringIndex(void) noexcept { return m_lNeteaseRTDataInquiringIndex; }
  long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_lNeteaseDayLineDataInquiringIndex; }

  int GetDayLineNeedUpdateNumber(void) { int i = m_iDayLineNeedUpdate; return i; }
  void SetDayLineNeedUpdateNumber(int i) { m_iDayLineNeedUpdate = i; }
  int GetDayLineNeedProcessNumber(void) { int i = m_iDayLineNeedProcess; return i; }
  void SetDayLineNeedProcessNumber(int i) { m_iDayLineNeedProcess = i; }
  int GetDayLineNeedSaveNumber(void) { int i = m_iDayLineNeedSave; return i; }
  void SetDayLineNeedSaveNumber(int i) { m_iDayLineNeedSave = i; }
  void IncreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1) { m_iDayLineNeedUpdate += iNumber; }
  void DecreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1) { m_iDayLineNeedUpdate -= iNumber; }
  void IncreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) { m_iDayLineNeedProcess += iNumber; }
  void DecreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) { m_iDayLineNeedProcess -= iNumber; }
  void IncreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) { m_iDayLineNeedSave += iNumber; }
  void DecreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) { m_iDayLineNeedSave -= iNumber; }

  void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
  bool IsRecordingRTData(void) noexcept { if (m_fSaveRTData) return true; else return false; }

  void SetUpdateStockCodeDB(bool fFlag) noexcept { m_fUpdateStockCodeDB = fFlag; }
  bool IsUpdateStockCodeDB(void) noexcept { bool fFlag = m_fUpdateStockCodeDB; return fFlag; }
  void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
  bool IsUpdateOptionDB(void) noexcept { bool fFlag = m_fUpdateOptionDB; return fFlag; }

  INT64 GetRTDataReceived(void) noexcept { return m_llRTDataReceived; }
  void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }

  bool CheckMarketReady(void) noexcept;

  bool ChangeCurrentStockToNextStock(void);
  bool ChangeCurrentStockToPrevStock(void);

private:
  // ��ʼ��
  bool CreateTotalStockContainer(void); // �˺����ǹ��캯����һ���֣������������á�

protected:
  vector<CChinaStockPtr> m_vChinaMarketAStock; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  map<CString, long> m_mapChinaMarketAStock; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
  long m_lTotalStock; // ��Ʊ��������
  long m_lTotalActiveStock;	// �����Ʊ����

  vector<CChinaStockPtr> m_vStockChoice; // ��ѡ��Ʊ��

  INT64 m_llRTDataReceived; // ���յ���ʵʱ��������

  queue<CRTDataPtr> m_qRTData;
  bool m_fSaveRTData;

  bool m_fCurrentEditStockChanged;
  int m_iCountDownSlowReadingRTData; // ���ٶ�ȡʵʱ���ݼ�����
  bool m_fMarketOpened; // �Ƿ���
  bool m_fStartReceivingData; // �Ƿ�ʼ����ʵʱ����
  bool m_fGetRTData; // ��ȡʵʱ���ݱ�ʶ
  bool m_fSaveDayLine; // ����ȡ�����ߴ������ݿ��ʶ
  bool m_fRTDataSetCleared; // ʵʱ���ݿ��������ʶ���ŵ���ʮ��֮ǰΪ�٣�֮������Ϊ�档
  CChinaStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ

  time_t m_ttNewestTransactionTime;

  bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��

  CString m_strSinaRTDataInquiringStr;
  CString m_strTengxunRTDataInquiringStr;
  CString m_strNeteaseRTDataInquiringStr;
  CString m_strNeteaseDayLineDataInquiringStr;

  long m_lSinaRTDataInquiringIndex;
  long m_lTengxunRTDataInquiringIndex;
  long m_lNeteaseRTDataInquiringIndex;
  long m_lNeteaseDayLineDataInquiringIndex;

  // Option��ѡ��
  long m_lRelativeStrongStartDay;
  long m_lRelativeStrongEndDay;
  long m_lLastLoginDay; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����

  vector<CChinaStockPtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
  INT64 m_lTotalMarketBuy; // �����г��е�A������������
  INT64 m_lTotalMarketSell; // �����г��е�A�������������

  // ϵͳ״̬��
  bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�
  bool m_fTodayStockProcessed; // �����Ƿ�ִ���˹�Ʊ����
  bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
  bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

  // ���̶߳�ȡ֮����
  CString m_strStockCodeForInquiringSinaRTData;
  clock_t m_ReadingTengxunRTDataTime; // ÿ�ζ�ȡ��Ѷʵʱ���ݵ�ʱ��
  CString m_strStockCodeForInquiringNeteaseDayLine;

  // ���¹�Ʊ�������ݿ��ʶ
  atomic_bool m_fUpdateStockCodeDB;
  atomic_bool m_fUpdateOptionDB;

  // ����������ʷ���ݶ�ȡ����ʹ洢��������
  atomic_int m_iDayLineNeedUpdate; // ������Ҫ���µĹ�Ʊ����
  atomic_int m_iDayLineNeedProcess; // ������Ҫ����Ĺ�Ʊ����
  atomic_int m_iDayLineNeedSave; // ������Ҫ�洢�Ĺ�Ʊ����

private:
};
