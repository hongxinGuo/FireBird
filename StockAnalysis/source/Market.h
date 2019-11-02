#ifndef __MARKET_H__
#define __MERKET_H__

#include"stdafx.h"
#include"globedef.h"
#include"CriticalSectionclock.h"

#include "Stock.h"
#include"SetStockCode.h"

#include"CrweberIndex.h"

using namespace std;
#include<vector>
#include<map>

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

  // interface function
public:
  // ϵͳ״̬��
  bool IsPermitResetSystem(void) noexcept { return m_fPermitResetSystem; }
  void SetPermitResetSystem(bool fFlag) noexcept { m_fPermitResetSystem = fFlag; }

  // ��ʼ���г�

  // ʵʱ���ݶ�ȡ
  bool CreateSinaRTDataInquiringStr(CString& str);
  bool CreateTengxunRTDataInquiringStr(CString& str);
  int GetSinaInquiringStockStr(CString& str);
  int	GetTengxunInquiringStockStr(CString& str);
  int GetInquiringStr(CString& str, vector<CStockPtr>::iterator& itStock, CString strPostfix, long lTotalNumber);
  bool GetTengxunStockRTData(void);
  void ResetIT(void); //���ø�������

  //������ʷ���ݶ�ȡ
  bool CreateNeteaseDayLineInquiringStr(CString& str, CString& strStartDay);

  bool IsAStock(CStockPtr pStock); // �Ƿ�Ϊ����A��
  bool IsAStock(CString strStockCode); // �Ƿ�Ϊ����A��
  bool IsStock(CString  strStockCode, CStockPtr& pStock);	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

  CString GetStockName(CString strStockCode);

  // �õ���Ʊ����
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // �õ���Ʊָ��
  CStockPtr GetStockPtr(CString strStockCode);
  CStockPtr GetStockPtr(long lIndex);

  // �洢�¹�Ʊָ�����Ծ��Ʊ��
  void AddStockToMarket(CStockPtr pStock);

  // ��ʼ��ʵʱ���ݼ�����
  bool CountLoopRTDataInquiring(void) { if (++m_lCountLoopRTDataInquiring >= 3) return true; else return false; }

  // �õ���ǰ��ʾ��Ʊ
  CStockPtr GetShowStock(void) noexcept { return m_pCurrentStock; }
  void SetShowStock(CString strStockCode);
  void SetShowStock(CStockPtr pStock);
  bool IsCurrentStockChanged(void);

  long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }

  long GetMinLineOffset(CStock sID, time_t Time);

  bool SystemReady(void) noexcept { return m_fSystemReady; }
  void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

  bool IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

  bool SaveDayLine(CStockPtr pStock);
  bool SaveOneRecord(CSetDayLine* psetDayLine, CDayLinePtr pDayLine);

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

  // �Ƿ��й�Ʊ����ʷ����������Ҫ���¡�
  bool IsDayLineNeedUpdate(void);

  bool IsDayLineNeedSaving(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ���ѯ��һ����
  bool IsDayLineDataInquiringOnce(void);

  long CompileCurrentTradeDayStock(long lCurrentTradeDay);

  bool CalculateOneDayRelativeStrong(long lDay);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckTodayActiveStock(void) noexcept { return m_fCheckTodayActiveStock; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsCalculatingRS(void) noexcept { return m_fCalculatingRS; }
  void SetCalculatingRS(bool fFlag) noexcept { m_fCalculatingRS = fFlag; }

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

  void SetReadingSinaRTDataTime(clock_t tt) { m_ReadingSinaRTDataTime.SetTime(tt); }
  clock_t GetReadingSinaRTDataTime(void) { return m_ReadingSinaRTDataTime.GetTime(); }
  void SetReadingTengxunRTDataTime(clock_t tt) { m_ReadingTengxunRTDataTime.SetTime(tt); }
  clock_t GetReadingTengxunRTDataTime(void) { return m_ReadingTengxunRTDataTime.GetTime(); }
  void SetReadingNeteaseDayDataTime(clock_t tt) { m_ReadingNeteaseDayDataTime.SetTime(tt); }
  clock_t GetReadingNeteaseDayDataTime(void) { return m_ReadingNeteaseDayDataTime.GetTime(); }

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ThreadCalculatingRTDataProc���á�
  bool ProcessRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool ProcessSinaRTDataReceivedFromWeb(void);
  // ����������ʷ����
  bool ProcessNeteaseDayLineData(CNeteaseDayLineWebData* pWebData);
  bool ProcessOneItemDayLineData(CString strStockCode, CDayLinePtr pDayLine, char*& pCurrentPos, long& lLength);
  // ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
  bool ReadOneValue(char*& pCurrentPos, char* buffer, long& iReadNumber);
  bool ReadOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter);

  // ��ʱ���£���ɾ���������������̵߳�OnTimer��������
  bool SchedulingTask(void);
  bool SchedulingTaskPerSecond(long lSecondNumber);
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime);
  void ResetSystemFlagAtMidnight(long lCurrentTime);
  void SaveTempDataIntoDB(long lCurrentTime);

private:
  // ��ʼ��
  bool CreateTotalStockContainer(void); // �˺����ǹ��캯����һ���֣������������á�

public:

  CStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ
  char m_aStockCodeTemp[30];
  bool m_fCurrentEditStockChanged;
  bool m_fMarketOpened; // �Ƿ���
  bool m_fGetRTStockData; // ��ȡʵʱ���ݱ�ʶ
  bool m_fReadingTengxunRTData; // ��ȡ��Ѷʵʱ����
  bool m_fGetDayLineData;  // ��ȡ������ʷ���ݱ�ʶ
  bool m_fSaveDayLine; // ����ȡ�����ߴ������ݿ��ʶ
  int m_iCountDownDayLine; // �������ݶ�ȡ��ʱ������
  int m_iCountDownSlowReadingRTData; // ���ٶ�ȡʵʱ���ݼ�����

  vector<CStockPtr> m_vChinaMarketAStock; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�

  vector<CStockPtr> gl_vStockChoice; // ��ѡ��Ʊ��

  vector<CrweberIndexPtr> gl_vCrweberIndex; // crweber.com��վ�ϵ�����ָ��

protected:

  map<CString, long> m_mapChinaMarketAStock; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

  // Option��ѡ��
  long m_lRelativeStrongStartDay;
  long m_lRelativeStrongEndDay;
  long m_lLastLoginDay; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����

  map<CString, long> m_mapActiveStockToIndex;	// ���г���Ծ�Ĺ�Ʊ����ӳ��Ϊƫ����
  vector<CStockPtr> m_vActiveStock;	//�����Ʊ����

  long m_lTotalActiveStock;	// �����Ʊ����

  vector<CStockPtr>::iterator m_itSinaStock; // ����ʵʱ��Ʊ�����ѯ������
  vector<CStockPtr>::iterator m_itTengxunStock; // ��Ѷʵʱ��Ʊ���������

  vector<CStockPtr> m_vpSelectedStock; // ��ǰѡ��Ĺ�Ʊ
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
  INT64 m_lTotalMarketBuy; // �����г��е�A������������
  INT64 m_lTotalMarketSell; // �����г��е�A�������������

  long m_lCountLoopRTDataInquiring; // ȫ���Ʊ�ر��������������ڳ�ʼ��ʱ

  bool m_fCalculatingRS;

  // ϵͳ״̬��
  bool m_fPermitResetSystem; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ
  bool m_fSystemReady; // �г���ʼ̬�Ѿ����ú�
  bool m_fTodayStockCompiled; // �����Ƿ�ִ���˹�Ʊ����
  bool m_fUpdatedStockCodeDataBase; //�Ƿ������������ʷ���ݿ��ʶ
  bool m_fCheckTodayActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
  bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

  // ���̶߳�ȡ֮��������Ҫ����ͬ������
  CCriticalSectionClock m_ReadingSinaRTDataTime; // ÿ�ζ�ȡ����ʵʱ���ݵ�ʱ��
  CCriticalSectionClock m_ReadingTengxunRTDataTime; // ÿ�ζ�ȡ��Ѷʵʱ���ݵ�ʱ��
  CCriticalSectionClock m_ReadingNeteaseDayDataTime;    // ÿ�ζ�ȡ����������ʷ���ݵ�ʱ��

private:
};

#endif
