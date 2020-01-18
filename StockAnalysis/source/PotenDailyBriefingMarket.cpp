#include"globedef.h"

#include"WebDataInquirer.h"
#include "PotenDailyBriefingMarket.h"

CPotenDailyBriefingMarket::CPotenDailyBriefingMarket(void) : CVirtualMarket() {
  m_fDataBaseLoaded = false;
  m_lNewestUpdatedDay = 20180411; //
}

CPotenDailyBriefingMarket::~CPotenDailyBriefingMarket(void) {
}

bool CPotenDailyBriefingMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;

  //根据时间，调度各项定时任务.每秒调度一次
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }
  return false;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerSecond(long lSecond) {
  if (m_fDataBaseLoaded) {
    if (m_lNewestUpdatedDay < gl_systemTime.GetDay()) {
      gl_WebDataInquirer.GetPotenDailyBriefingData();
    }
  }
  else {
    LoadDatabase();
    m_fDataBaseLoaded = true;
  }

  ProcessData();

  return true;
}

bool CPotenDailyBriefingMarket::LoadDatabase(void) {
  CSetPotenDailyBriefing setPotenDailyBriefing;
  setPotenDailyBriefing.Open();
  while (!setPotenDailyBriefing.IsEOF()) {
    CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
    pPotenDailyBriefing->LoadData(setPotenDailyBriefing);
    m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
    if (setPotenDailyBriefing.m_Day > m_lNewestUpdatedDay) {
      m_lNewestUpdatedDay = setPotenDailyBriefing.m_Day;
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
    CPotenDailyBriefingPtr pPotenDailyBriefing = make_shared<CPotenDailyBriefing>();
    if (pPotenDailyBriefing->ReadData(pWebData)) {
      m_vPotenDailyBriefing.push_back(pPotenDailyBriefing);
    }
  }

  return true;
}