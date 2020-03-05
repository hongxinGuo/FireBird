#include"globedef.h"

#include"WebInquirer.h"
#include "PotenDailyBriefingMarket.h"

#include"Thread.h"

using namespace std;
#include<thread>

CPotenDailyBriefingMarket::CPotenDailyBriefingMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("Poten�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }

  m_strMarketId = _T("Poten.com");
  m_lTimeZoneOffset = 4 * 3600; // poten.comʹ������������׼ʱ��
  CalculateTime();

  Reset();
}

void CPotenDailyBriefingMarket::Reset(void) {
  m_fTodayDataUpdated = false;
  m_pDataToSaved = nullptr;
  m_vPotenDailyBriefing.clear();
  m_fDataBaseLoaded = false;
  m_lCurrentInquiringDay = 20180411; //
  for (long l = 20180411; l <= GetDay(); l = GetNextDay(l)) {
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
  const long lCurrentTime = GetTime();

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
  str += GetMarketTimeString();
  gl_systemMessage.PushInformationMessage(str);
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPerMinute(lSecond, lCurrentTime);
  SchedulingTaskPer10Second(lSecond, lCurrentTime);

  return true;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPer10Second(long lSecond, long lCurrentTime) {
  static int s_i10SeocndCounter = 9;

  s_i10SeocndCounter -= lSecond;
  if (s_i10SeocndCounter < 0) {
    s_i10SeocndCounter = 9;
    return true;
  }
  return false;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerMinute(long lSecond, long lCurrentTime) {
  static int s_i1MinuteCounter = 59;

  s_i1MinuteCounter -= lSecond;
  if (s_i1MinuteCounter < 0) {
    s_i1MinuteCounter = 59;
    TaskResetMarket(lCurrentTime);

    TaskLoadDataBase();

    TaskProcessData();
    TaskCheckTodayDataUpdated();
    TaskInquiringData();

    return true;
  }
  else return false;
}

bool CPotenDailyBriefingMarket::TaskProcessData(void) {
  bool fGetData = false;

  if ((!m_fTodayDataUpdated) && (!gl_WebInquirer.IsReadingPotenDailyBriefing()) && m_fDataBaseLoaded) {
    long lTotal = gl_WebInquirer.GetPotenDailyBriefingDataSize();
    for (int i = 0; i < lTotal; i++) {
      CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
      if (pWebData->GetBufferLength() > 40 * 1024) { // ��poten.com��ȡ�����ݴ�С�������40KBʱ����û��ʵ�����ݣ����账��
        CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
        if (pPotenDailyBriefing->ReadData(pWebData)) {
          pPotenDailyBriefing->SetDay(pWebData->m_lTime / 1000000);
          if (!m_mapDataLoadedDays.at(pPotenDailyBriefing->GetDay())) {
            ASSERT(m_pDataToSaved == nullptr);
            fGetData = true;
            m_pDataToSaved = pPotenDailyBriefing;
            thread thread1(ThreadSavePotenData);
            thread1.detach();
            TRACE(_T("����%d�յ�poten����\n"), pPotenDailyBriefing->GetDay());
            gl_systemMessage.PushInformationMessage(_T("Poten�����Ѹ���"));
            m_mapDataLoadedDays.at(pPotenDailyBriefing->GetDay()) = true;
            m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
          }
        }
        else {
          TRACE(_T("%d�յ�poten��������\n"), pPotenDailyBriefing->GetDay());
        }
      }
      else {
        TRACE(_T("û��%d�յ�poten����\n"), pWebData->m_lTime / 1000000);
      }
    }
  }

  if (m_lCurrentInquiringDay < GetDay()) {
    ChoiceNextInquiringDay();
  }
  else if (fGetData) {
    ChoiceNextInquiringDay();
  }

  return true;
}

bool CPotenDailyBriefingMarket::TaskCheckTodayDataUpdated(void) {
  if (!m_fTodayDataUpdated) {
    if (m_lCurrentInquiringDay > GetDay()) {
      m_fTodayDataUpdated = true;
    }
    else if ((m_lCurrentInquiringDay == GetDay()) && (!gl_PotenDailyBriefingMarket.IsWorkingDay())) {
      m_fTodayDataUpdated = true;
    }
    else m_fTodayDataUpdated = false;

    if (m_fTodayDataUpdated) gl_systemMessage.PushInformationMessage(_T("Poten���ݽ������"));
    return true;
  }
  else return false;
}

bool CPotenDailyBriefingMarket::TaskInquiringData(void) {
  if (!m_fTodayDataUpdated && !gl_WebInquirer.IsReadingPotenDailyBriefing() && m_fDataBaseLoaded) {
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
  setPotenDailyBriefing.m_strSort = _T("[Day]");
  setPotenDailyBriefing.Open();
  while (!setPotenDailyBriefing.IsEOF()) {
    CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
    pPotenDailyBriefing->LoadData(setPotenDailyBriefing);
    m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
    m_mapDataLoadedDays.at(pPotenDailyBriefing->GetDay()) = true;
    if (setPotenDailyBriefing.m_Day >= m_lCurrentInquiringDay) {
      m_lCurrentInquiringDay = GetNextDay(setPotenDailyBriefing.m_Day);
    }
    setPotenDailyBriefing.MoveNext();
  }
  setPotenDailyBriefing.Close();

  return true;
}

bool CPotenDailyBriefingMarket::SaveCurrentData(void) {
  ASSERT(m_pDataToSaved != nullptr);
  CSetPotenDailyBriefing setPotenDailyBriefing;
  setPotenDailyBriefing.Open();
  setPotenDailyBriefing.m_pDatabase->BeginTrans();
  m_pDataToSaved->AppendData(setPotenDailyBriefing);
  setPotenDailyBriefing.m_pDatabase->CommitTrans();
  setPotenDailyBriefing.Close();
  m_pDataToSaved = nullptr;

  return true;
}

void CPotenDailyBriefingMarket::ChoiceNextInquiringDay(void) {
  long lNextInquiringDay = m_lCurrentInquiringDay;

  do {
    lNextInquiringDay = GetNextDay(lNextInquiringDay);
  } while ((lNextInquiringDay <= GetDay()) && m_mapDataLoadedDays.at(lNextInquiringDay));

  SetCurrentInquiringDay(lNextInquiringDay);
}