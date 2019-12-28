#ifndef __MARKET_H__
#define __MERKET_H__

#include"stdafx.h"
#include"globedef.h"

#include "Stock.h"
#include"SetStockCode.h"

#include"CrweberIndex.h"

using namespace std;
#include<vector>
#include<map>
#include<atomic>

class CMarket final : public CObject
{
public:
  // ֻ����һ��ʵ��
  CMarket(void);
  ~CMarket(void);
  void Reset(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif
public:
  // ��ʱ���£���ɾ���������������̵߳�OnTimer�������á�������������Ⱥ���
  bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime); // ÿʮ�����һ��
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime); // ÿ����ӵ���һ��
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // ÿСʱ����һ��

  // ��������
  bool TaskGetRTDataFromWeb(void);
  void TaskGetNeteaseDayLineFromWeb(void);

  bool GetNeteaseWebDayLineData(void);

  // ������Ѷʵʱ����
  bool TaskProcessTengxunRTData(void);

  void TaskSetCheckTodayActiveStockFlag(long lCurrentTime);
  bool TaskCompileTodayStock(long lCurrentTime);
  bool TaskUpdateStockCodeDB(void);
  bool TaskCheckMarketOpen(long lCurrentTime);
  bool TaskResetSystem(long lCurrentTime);
  bool TaskResetSystemAgain(long lCurrentTime);

  // interface function
public:
  // ϵͳ״̬��
  bool IsPermitResetSystem(void) noexcept { return m_fPermitResetSystem; }
  void SetPermitResetSystem(bool fFlag) noexcept { m_fPermitResetSystem = fFlag; }

  // ��ʼ���г�

  // ʵʱ���ݶ�ȡ
  int GetSinaInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock = true);
  int GetTengxunInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock = true);
  int	GetNeteaseInquiringStockStr(CString& str, long lTotalNumber = 700, bool fSkipUnactiveStock = true);
  int CMarket::GetInquiringStr(CString& str, int& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock);
  bool StepToActiveStockIndex(int& iStockIndex);

  //������ʷ���ݶ�ȡ
  bool CreateNeteaseDayLineInquiringStr(CString& str);

  int IncreaseStockInquiringIndex(int& iIndex);

  bool IsAStock(CStockPtr pStock); // �Ƿ�Ϊ����A��
  bool IsAStock(CString strStockCode); // �Ƿ�Ϊ����A��
  bool IsStock(CString  strStockCode, CStockPtr& pStock);	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

  CString GetStockName(CString strStockCode);

  // �õ���Ʊ����
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // �õ���Ʊָ��
  CStockPtr GetStockPtr(CString strStockCode);
  CStockPtr GetStockPtr(long lIndex);

  void IncreaseActiveStockNumber(void);

  // �õ���ǰ��ʾ��Ʊ
  CStockPtr GetShowStock(void) noexcept { return m_pCurrentStock; }
  void SetShowStock(CString strStockCode);
  void SetShowStock(CStockPtr pStock);
  bool IsCurrentStockChanged(void);

  long GetTotalActiveStock(void) { return m_lTotalActiveStock; }
  void SetTotalActiveStock(long lValue) { m_lTotalActiveStock = lValue; }

  long GetMinLineOffset(CStock sID, time_t Time);

  bool SystemReady(void) noexcept { return m_fSystemReady; }
  void SetSystemReady(bool fFlag) { m_fSystemReady = fFlag; }

  bool IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

  // ���ݿ��ȡ�洢����
  bool SaveRTData(void);  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool SaveDayLineData(void);  // �������ݴ�����������ȡ�����������ݴ������ݿ���
  bool SaveCrweberIndexData(void);  // crweber.com����ָ���洢����
  bool UpdateStockCodeDB(void);
  void LoadStockCodeDB(void);
  bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  bool UpdateTempRTData(void);
  bool UpdateTodayTempDB(void);
  bool LoadTodayTempDB(void);

  bool ClearAllDayLineVector(void);

  // ��Ʊ��ʷ���ݴ���
  bool IsDayLineNeedUpdate(void);
  bool IsDayLineNeedSaving(void);
  // �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
  bool ProcessDayLineGetFromNeeteaseServer(void);

  long CompileCurrentTradeDayStock(long lCurrentTradeDay);
  bool CalculateOneDayRelativeStrong(long lDay);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckTodayActiveStock(void) noexcept { return m_fCheckTodayActiveStock; }
  void SetCheckTodayActiveStock(bool fFlag) noexcept { m_fCheckTodayActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsCalculatingRS(void) noexcept { return m_fCalculatingRS; }
  void SetCalculatingRS(bool fFlag) noexcept { m_fCalculatingRS = fFlag; }

  bool IsUpdateStockCodeDB(void);
  void ClearUpdateStockCodeDBFlag(void);

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

  void SetReadingSinaRTDataTime(clock_t tt) noexcept { m_ReadingSinaRTDataTime = tt; }
  clock_t GetReadingSinaRTDataTime(void) noexcept { return m_ReadingSinaRTDataTime; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) noexcept { return m_ReadingTengxunRTDataTime; }
  void SetReadingNeteaseDayLineDataTime(clock_t tt) noexcept { m_ReadingNeteaseDayLineDataTime = tt; }
  clock_t GetReadingNeteaseDayLineDataTime(void) noexcept { return m_ReadingNeteaseDayLineDataTime; }

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
  bool ProcessRTData(void);

  // ������������ȡ��ʵʱ��Ʊ����
  bool TaskProcessWebRTDataGetFromSinaServer(void);
  bool TaskProcessWebRTDataGetFromTengxunServer(void);
  bool IsInvalidTengxunRTData(CWebDataReceivedPtr pWebDataReceived);
  void CheckTengxunRTData(CRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromNeteaseServer(void);
  bool IsInvalidNeteaseRTData(CWebDataReceivedPtr pWebDataReceived);
  bool IsValidNeteaseRTDataPrefix(CWebDataReceivedPtr pWebDataReceived);
  void CheckNeteaseRTData(CRTDataPtr pRTData);
  bool TaskProcessNeteaseRTData(void);
  bool TaskProcessWebRTDataGetFromCrweberdotcom(void);

  //����ʵʱ��Ʊ�仯��
  bool TaskDistributeSinaRTDataToProperStock(void);

  void ResetSystemFlagAtMidnight(long lCurrentTime);
  void SaveTempDataIntoDB(long lCurrentTime);

  // �����ú���
  bool __Test_IsPermitResetSystem(void) noexcept { return m_fPermitResetSystem; }
  void __Test_SetResetSystem(bool fFlag) noexcept { gl_fResetSystem = fFlag; }

  // ״̬����
  bool IsUsingSinaRTDataReceiver(void) { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) { return m_fUsingNeteaseRTDataReceiver; }
  bool IsUsingTengxunRTDataReceiver(void) { return m_fUsingTengxunRTDataReceiver; }

private:
  // ��ʼ��
  bool CreateTotalStockContainer(void); // �˺����ǹ��캯����һ���֣������������á�

public:

  CStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ
  char m_aStockCodeTemp[30];
  bool m_fCurrentEditStockChanged;
  bool m_fMarketOpened; // �Ƿ���
  bool m_fGetRTStockData; // ��ȡʵʱ���ݱ�ʶ
  bool m_fSaveDayLine; // ����ȡ�����ߴ������ݿ��ʶ
  int m_iCountDownDayLine; // �������ݶ�ȡ��ʱ������
  int m_iCountDownSlowReadingRTData; // ���ٶ�ȡʵʱ���ݼ�����

  bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
  bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��

  vector<CStockPtr> m_vChinaMarketAStock; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  long m_lTotalStock; // ��Ʊ��������

  vector<CStockPtr> gl_vStockChoice; // ��ѡ��Ʊ��

  vector<CrweberIndexPtr> gl_vCrweberIndex; // crweber.com��վ�ϵ�����ָ��

  time_t m_ttNewestTransactionTime;

protected:

  map<CString, long> m_mapChinaMarketAStock; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

  // Option��ѡ��
  long m_lRelativeStrongStartDay;
  long m_lRelativeStrongEndDay;
  long m_lLastLoginDay; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����

  long m_lTotalActiveStock;	// �����Ʊ����

  vector<CStockPtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
  INT64 m_lTotalMarketBuy; // �����г��е�A������������
  INT64 m_lTotalMarketSell; // �����г��е�A�������������

  bool m_fCalculatingRS;

  // ϵͳ״̬��
  bool m_fPermitResetSystem; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ
  bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�
  bool m_fTodayStockCompiled; // �����Ƿ�ִ���˹�Ʊ����
  bool m_fCheckTodayActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
  bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

  // ���̶߳�ȡ֮����
  atomic<clock_t> m_ReadingSinaRTDataTime; // ÿ�ζ�ȡ����ʵʱ���ݵ�ʱ��
  atomic<clock_t> m_ReadingTengxunRTDataTime; // ÿ�ζ�ȡ��Ѷʵʱ���ݵ�ʱ��
  atomic<clock_t> m_ReadingNeteaseDayLineDataTime;    // ÿ�ζ�ȡ����������ʷ���ݵ�ʱ��

public:
  // ����������ʷ���ݶ�ȡ����ʹ洢��������
  atomic_int m_iDayLineNeedUpdate; // ������Ҫ���µĹ�Ʊ����
  atomic_int m_iDayLineNeedProcess; // ������Ҫ����Ĺ�Ʊ����
  atomic_int m_iDayLineNeedSave; // ������Ҫ�洢�Ĺ�Ʊ����

private:
};

#endif
