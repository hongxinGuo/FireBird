#include"globedef.h"

#include"WebInquirer.h"
#include "PotenDailyBriefingMarket.h"

#include"Thread.h"

using namespace std;
#include<thread>

CPotenDailyBriefingMarket::CPotenDailyBriefingMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("Poten�г�����ֻ��������һ��ʵ��\n");
  }

  m_strMarketId = _T("Poten.com");
  m_lMarketTimeZone = 4 * 3600; // poten.comʹ������������׼ʱ��
  CalculateTime();

  SetReadyToRun(false);

  Reset();
}

void CPotenDailyBriefingMarket::Reset(void) {
  SetReadyToRun(false); // ��2020��3��27�տ�ʼ������ַֹͣ�����ˣ��ʶ�������ͣ��ȡ���ݡ�
  m_fTodayDataUpdated = false;
  m_pDataToSaved = nullptr;
  m_vPotenDailyBriefing.clear();
  m_fDataBaseLoaded = false;
  m_lCurrentInquiringDate = 20180411; //
  m_lToday = GetFormatedMarketDate();
  for (long l = 20180411; l <= m_lToday; l = GetNextDay(l)) {
    m_mapDataLoadedDays[l] = false;
    if (!IsWorkingDay(l)) {
      m_mapDataLoadedDays.at(l) = true; // ��������������poten.com��û�����ݣ��ʶ������ѯ��
    }
  }
}

CPotenDailyBriefingMarket::~CPotenDailyBriefingMarket(void) {
}

bool CPotenDailyBriefingMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > (s_timeLast)) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
    return true;
  }
  else return false;
}

void CPotenDailyBriefingMarket::ResetMarket(void) {
  Reset();
  CString str = _T("����poten.com��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPerHour(lSecond, lCurrentTime);
  SchedulingTaskPerMinute(lSecond, lCurrentTime);

  return true;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerMinute(long lSecond, long lCurrentTime) {
  static int s_i1MinuteCounter = 59;

  s_i1MinuteCounter -= lSecond;
  if (s_i1MinuteCounter < 0) {
    s_i1MinuteCounter = 59;
    TaskResetMarket(lCurrentTime);

    TaskLoadDataBase();

    if (!m_fTodayDataUpdated) {
      TaskProcessData();

      if (m_lCurrentInquiringDate < m_lToday) {
        ChoiceNextInquiringDate();
      }

      TaskCheckTodayDataUpdated();
      if (m_lCurrentInquiringDate < m_lToday) {
        TaskInquiringData();
      }
    }

    return true;
  }
  else return false;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerHour(long lSecond, long lCurrentTime) {
  static int s_i1MinuteCounter = 3599;

  s_i1MinuteCounter -= lSecond;
  if (s_i1MinuteCounter < 0) {
    s_i1MinuteCounter = 3599;
    if (!m_fTodayDataUpdated && (m_lCurrentInquiringDate == m_lToday)) {
      TaskInquiringData();
    }
    return true;
  }
  return false;
}

bool CPotenDailyBriefingMarket::TaskProcessData(void) {
  ASSERT(!m_fTodayDataUpdated);
  if ((!gl_WebInquirer.IsReadingPotenDailyBriefing()) && m_fDataBaseLoaded) {
    size_t lTotal = gl_WebInquirer.GetPotenDailyBriefingDataSize();
    for (int i = 0; i < lTotal; i++) {
      CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
      if (pWebData->GetBufferLength() > 40 * 1024) { // ��poten.com��ȡ�����ݴ�С�������40KBʱ����û��ʵ�����ݣ����账��
        CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
        if (pPotenDailyBriefing->ReadData(pWebData)) {
          pPotenDailyBriefing->SetDate(pWebData->GetTime() / 1000000);
          if (!m_mapDataLoadedDays.at(pPotenDailyBriefing->GetFormatedMarketDate())) {
            ASSERT(m_pDataToSaved == nullptr);
            if (m_lCurrentInquiringDate == m_lToday) m_fTodayDataUpdated = true;
            m_pDataToSaved = pPotenDailyBriefing;
            RunningThreadSavePotenData();
            TRACE(_T("����%d�յ�poten����\n"), pPotenDailyBriefing->GetFormatedMarketDate());
            gl_systemMessage.PushInformationMessage(_T("Poten�����Ѹ���"));
            m_mapDataLoadedDays.at(pPotenDailyBriefing->GetFormatedMarketDate()) = true;
            m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
          }
        }
        else {
          TRACE(_T("%d�յ�poten��������\n"), pPotenDailyBriefing->GetFormatedMarketDate());
        }
      }
      else {
        TRACE(_T("û��%d�յ�poten����\n"), pWebData->GetTime() / 1000000);
      }
    }
  }

  return true;
}

bool CPotenDailyBriefingMarket::RunningThreadSavePotenData(void) {
  thread threadSaveData(ThreadSavePotenData, this);
  threadSaveData.detach();
  return true;
}

bool CPotenDailyBriefingMarket::TaskCheckTodayDataUpdated(void) {
  ASSERT(!m_fTodayDataUpdated);
  if (m_lCurrentInquiringDate > m_lToday) {
    m_fTodayDataUpdated = true;
  }
  else if ((m_lCurrentInquiringDate == m_lToday) && (!gl_pPotenDailyBriefingMarket->IsWorkingDay())) {
    m_fTodayDataUpdated = true;
  }
  else m_fTodayDataUpdated = false;

  if (m_fTodayDataUpdated) gl_systemMessage.PushInformationMessage(_T("Poten���ݽ������"));
  return true;
}

bool CPotenDailyBriefingMarket::TaskInquiringData(void) {
  ASSERT(!m_fTodayDataUpdated);
  if (!gl_WebInquirer.IsReadingPotenDailyBriefing() && m_fDataBaseLoaded) {
    gl_WebInquirer.GetPotenDailyBriefingData();
    return true;
  }
  return false;
}

bool CPotenDailyBriefingMarket::TaskLoadDataBase(void) {
  if (!m_fDataBaseLoaded) {
    LoadDatabase();
    m_fDataBaseLoaded = true;
    return true;
  }
  return false;
}

bool CPotenDailyBriefingMarket::TaskResetMarket(long lCurrentTime) {
  // �ŵ�����ϵͳ
  if (IsPermitResetMarket() && (lCurrentTime >= 10000) && (lCurrentTime <= 13000)) { // �ŵ��������г�
    SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
    SetPermitResetMarket(false); // ���첻����������ϵͳ��
    return true;
  }
  return false;
}

bool CPotenDailyBriefingMarket::LoadDatabase(void) {
  CSetPotenDailyBriefing setPotenDailyBriefing;
  setPotenDailyBriefing.m_strSort = _T("[Date]");
  setPotenDailyBriefing.Open();
  while (!setPotenDailyBriefing.IsEOF()) {
    CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
    pPotenDailyBriefing->LoadData(setPotenDailyBriefing);
    m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
    m_mapDataLoadedDays.at(pPotenDailyBriefing->GetFormatedMarketDate()) = true;
    if (setPotenDailyBriefing.m_Date >= m_lCurrentInquiringDate) {
      m_lCurrentInquiringDate = setPotenDailyBriefing.m_Date;
    }
    setPotenDailyBriefing.MoveNext();
  }
  setPotenDailyBriefing.Close();

  return true;
}

bool CPotenDailyBriefingMarket::SaveCurrentData(void) {
  ASSERT(m_pDataToSaved != nullptr);
  CSetPotenDailyBriefing setPotenDailyBriefing;

  setPotenDailyBriefing.m_strFilter = _T("[ID] = 1");
  setPotenDailyBriefing.Open();
  setPotenDailyBriefing.m_pDatabase->BeginTrans();
  m_pDataToSaved->AppendData(setPotenDailyBriefing);
  setPotenDailyBriefing.m_pDatabase->CommitTrans();
  setPotenDailyBriefing.Close();
  m_pDataToSaved = nullptr;

  return true;
}

void CPotenDailyBriefingMarket::ChoiceNextInquiringDate(void) {
  long lNextInquiringDate = m_lCurrentInquiringDate;

  do {
    lNextInquiringDate = GetNextDay(lNextInquiringDate);
  } while ((lNextInquiringDate < m_lToday) && m_mapDataLoadedDays.at(lNextInquiringDate));

  SetCurrentInquiringDate(lNextInquiringDate);
}