#include"globedef.h"

#include"WebInquirer.h"
#include "PotenDailyBriefingMarket.h"

CPotenDailyBriefingMarket::CPotenDailyBriefingMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("Poten�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }

  m_strMarketId = _T("Poten.com");
  m_lTimeZoneOffset = 4 * 3600; // poten.comʹ������������׼ʱ��
  CalculateTime();
  m_fTodayDataUpdated = false;

  Reset();
}

void CPotenDailyBriefingMarket::Reset(void) {
  m_vPotenDailyBriefing.clear();
  m_fDataBaseLoaded = false;
  m_lCurrentInquiringDay = 20180411; //
  m_lNewestDatabaseDay = 0;
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
  if (GetLocalTime() > (s_timeLast)) {
    SchedulingTaskPerSecond(GetLocalTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetLocalTime();
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
  TaskResetMarket(lCurrentTime);

  if ((!m_fTodayDataUpdated) && (!gl_WebInquirer.IsReadingPotenDailyBriefing())) {
    ProcessData();
    if (m_fDataBaseLoaded) {
      if ((m_lCurrentInquiringDay <= GetDay())) {
        gl_WebInquirer.GetPotenDailyBriefingData();
        SetNextInquiringDay();
      }
    }
    else {
      LoadDatabase();
      m_fDataBaseLoaded = true;
    }
  }

  return true;
}

bool CPotenDailyBriefingMarket::TaskResetMarket(long lCurrentTime) {
  // �ŵ�����ϵͳ
  if (IsPermitResetMarket() && (lCurrentTime >= 90000) && (lCurrentTime <= 93000)) { // �ŵ��������г�
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
    if (setPotenDailyBriefing.m_Day > m_lNewestDatabaseDay) {
      m_lNewestDatabaseDay = setPotenDailyBriefing.m_Day;
    }
    if (setPotenDailyBriefing.m_Day > m_lCurrentInquiringDay) {
      m_lCurrentInquiringDay = GetNextDay(setPotenDailyBriefing.m_Day);
    }
    setPotenDailyBriefing.MoveNext();
  }
  setPotenDailyBriefing.Close();

  return true;
}

bool CPotenDailyBriefingMarket::ProcessData(void) {
  long lTotal = gl_WebInquirer.GetPotenDailyBriefingDataSize();
  for (int i = 0; i < lTotal; i++) {
    CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
    if (pWebData->GetBufferLength() > 40 * 1024) { // ��poten.com��ȡ�����ݴ�С�������40KBʱ����û��ʵ�����ݣ����账��
      CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
      if (pPotenDailyBriefing->ReadData(pWebData)) {
        pPotenDailyBriefing->SetDay(pWebData->m_lTime / 1000000);
        if (pPotenDailyBriefing->GetDay() > m_lNewestDatabaseDay) {
          CSetPotenDailyBriefing setPotenDailyBriefing;
          setPotenDailyBriefing.Open();
          setPotenDailyBriefing.m_pDatabase->BeginTrans();
          pPotenDailyBriefing->AppendData(setPotenDailyBriefing);
          setPotenDailyBriefing.m_pDatabase->CommitTrans();
          setPotenDailyBriefing.Close();
          gl_systemMessage.PushInformationMessage(_T("Poten�����Ѹ���"));
          m_lNewestDatabaseDay = pPotenDailyBriefing->GetDay();
          m_fTodayDataUpdated = true;

          m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
        }
        TRACE(_T("����%d�յ�poten����\n"), pPotenDailyBriefing->GetDay());
        ASSERT(pPotenDailyBriefing->m_lDay >= m_lNewestDatabaseDay);
      }
      else {
        TRACE(_T("%d�յ�poten��������\n"), pPotenDailyBriefing->GetDay());
      }
    }
    else {
      TRACE(_T("û��%d�յ�poten����\n"), pWebData->m_lTime / 1000000);
    }
  }
  return true;
}

void CPotenDailyBriefingMarket::SetNextInquiringDay(void) {
  long lNextInquiringDay = 0;
  long year = m_lCurrentInquiringDay / 10000;
  long month = m_lCurrentInquiringDay / 100 - year * 100;
  long day = m_lCurrentInquiringDay - year * 10000 - month * 100;
  CTime today(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);
  do {
    today += oneDay;
    lNextInquiringDay = today.GetYear() * 10000 + today.GetMonth() * 100 + today.GetDay();
  } while ((lNextInquiringDay <= GetDay()) && m_mapDataLoadedDays.at(lNextInquiringDay));

  SetCurrentInquiringDay(lNextInquiringDay);
}