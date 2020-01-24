#include"globedef.h"

#include"WebDataInquirer.h"
#include "PotenDailyBriefingMarket.h"

CPotenDailyBriefingMarket::CPotenDailyBriefingMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("Poten市场变量只允许存在一个实例\n");
    ASSERT(0);
  }
  Reset();
}

void CPotenDailyBriefingMarket::Reset(void) {
  m_vPotenDailyBriefing.clear();
  m_fDataBaseLoaded = false;
  m_lNewestUpdatedDay = 20180411; //
  m_lNewestDatabaseDay = 0;
  m_fTodayDataUupdated = false;
  gl_systemTime.CalculateLocalTime();
  for (long l = 20180411; l <= gl_systemTime.GetDay(); l = gl_systemTime.GetNextDay(l)) {
    m_mapDataLoadedDays[l] = false;
    if (!gl_systemTime.IsWorkingDay(l)) {
      m_mapDataLoadedDays.at(l) = true; // 星期六和星期日poten.com都没有数据，故而无需查询。
    }
  }
}

CPotenDailyBriefingMarket::~CPotenDailyBriefingMarket(void) {
}

bool CPotenDailyBriefingMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // 调用基类调度函数，完成共同任务

  static time_t s_timeLast = 0;
  const long lCurrentTime = gl_systemTime.GetTime();

  //根据时间，调度各项定时任务.每秒调度一次
  if (gl_systemTime.Gett_time() > (s_timeLast)) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast, lCurrentTime);
    s_timeLast = gl_systemTime.Gett_time();
  }
  return true;
}

void CPotenDailyBriefingMarket::ResetMarket(void) {
  Reset();
  gl_systemMessage.PushInformationMessage(_T("重置poten.com"));
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  TaskResetMarket(lCurrentTime);

  if ((!m_fTodayDataUupdated) && (!gl_WebDataInquirer.IsReadingPotenDailyBriefing())) {
    ProcessData();
    if (m_fDataBaseLoaded) {
      if ((m_lNewestUpdatedDay <= gl_systemTime.GetDay())) {
        gl_WebDataInquirer.GetPotenDailyBriefingData();
        SetNextInquiringDay();
      }
      else {
        if ((m_lNewestUpdatedDay > m_lNewestDatabaseDay)) {
          SaveDatabase();
          UpdateStatus();
        }
        gl_systemMessage.PushInformationMessage(_T("Poten数据已更新"));
        m_fTodayDataUupdated = true;
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
  // 九点重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 90000) && (lCurrentTime <= 93000)) { // 九点重启本市场
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
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
    if (setPotenDailyBriefing.m_Day > m_lNewestUpdatedDay) {
      m_lNewestUpdatedDay = gl_systemTime.GetNextDay(setPotenDailyBriefing.m_Day);
    }
    setPotenDailyBriefing.MoveNext();
  }
  setPotenDailyBriefing.Close();

  return true;
}

bool CPotenDailyBriefingMarket::ProcessData(void) {
  long lTotal = gl_WebDataInquirer.GetPotenDailyBriefingDataSize();
  for (int i = 0; i < lTotal; i++) {
    CWebDataReceivedPtr pWebData = gl_WebDataInquirer.PopPotenDailyBriefingData();
    if (pWebData->GetBufferLength() > 40 * 1024) { // 从poten.com读取的数据大小如果低于40KB时，其没有实际内容，无需处理
      CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
      if (pPotenDailyBriefing->ReadData(pWebData)) {
        pPotenDailyBriefing->SetDay(pWebData->m_lTime / 1000000);
        TRACE(_T("处理%d日的poten数据\n"), pPotenDailyBriefing->GetDay());
        ASSERT(pPotenDailyBriefing->m_lDay >= m_lNewestDatabaseDay);
        m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
      }
      else {
        TRACE(_T("%d日的poten数据有误\n"), pPotenDailyBriefing->GetDay());
      }
    }
    else {
      TRACE(_T("没有%d日的poten数据\n"), pWebData->m_lTime / 1000000);
    }
  }
  return true;
}

bool CPotenDailyBriefingMarket::SaveDatabase(void) {
  CSetPotenDailyBriefing setPotenDailyBriefing;
  setPotenDailyBriefing.Open();
  setPotenDailyBriefing.m_pDatabase->BeginTrans();
  for (auto pPotenDailyBriefing : m_vPotenDailyBriefing) {
    if (pPotenDailyBriefing->m_lDay > m_lNewestDatabaseDay) {
      pPotenDailyBriefing->AppendData(setPotenDailyBriefing);
    }
  }
  setPotenDailyBriefing.m_pDatabase->CommitTrans();
  setPotenDailyBriefing.Close();
  return true;
}

bool CPotenDailyBriefingMarket::UpdateStatus(void) {
  for (auto pPotenDailyBriefing : m_vPotenDailyBriefing) {
    if (pPotenDailyBriefing->m_lDay > m_lNewestDatabaseDay) {
      m_lNewestDatabaseDay = pPotenDailyBriefing->m_lDay;
    }
  }

  return true;
}

void CPotenDailyBriefingMarket::SetNextInquiringDay(void) {
  long lNextInquiringDay = 0;
  long year = m_lNewestUpdatedDay / 10000;
  long month = m_lNewestUpdatedDay / 100 - year * 100;
  long day = m_lNewestUpdatedDay - year * 10000 - month * 100;
  CTime today(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);
  do {
    today += oneDay;
    lNextInquiringDay = today.GetYear() * 10000 + today.GetMonth() * 100 + today.GetDay();
  } while ((lNextInquiringDay <= gl_systemTime.GetDay()) && m_mapDataLoadedDays.at(lNextInquiringDay));

  SetNewestUpdateDay(lNextInquiringDay);
}