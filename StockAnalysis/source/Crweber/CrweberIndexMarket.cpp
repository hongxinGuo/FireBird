#include"pch.h"
#include "CrweberIndexMarket.h"
#include"thread.h"

#include"WebInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket市场变量只允许存在一个实例\n");
  }

  m_strMarketId = _T("Crweber.com");
  m_lMarketTimeZone = 0; // crweber.com使用GMT。
  CalculateTime();

  SetReadyToRun(false);
  Reset();
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  //SetReadyToRun(false); // 目前外网不稳，无法正确读取网站信息。
  m_fDataBaseLoaded = false;
  m_fTodayDataUpdated = true;
  m_fMaintainDatabase = true;
  m_lNewestDatabaseDate = 0;
  m_lNewestUPdateDate = 0;
  LoadDatabase();
  SaveDatabase();

  // 重置此全局变量
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

void CCrweberIndexMarket::ResetMarket(void) {
  Reset();
  CString str = _T("重置Crweber.com于格林威治标准时间：");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CCrweberIndexMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  static int i1HourCounter = 3599;  // 一小时一次的计数器
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  i1HourCounter -= lSecond;
  i1MinuteCounter -= lSecond;
  if (i1HourCounter < 0) {
    i1HourCounter = 3599;
    SchedulingTaskPer1Hour(lCurrentTime);
  }
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    SchedulingTaskPer1Minute(lCurrentTime);
  }
  return true;
}

bool CCrweberIndexMarket::SchedulingTaskPer1Hour(long lCurrentTime) {
  TaskResetMarket(lCurrentTime);
  //TaskMaintainDatabase(lCurrentTime);

  if (!gl_WebInquirer.IsReadingCrweberIndex()) {
    TaskProcessWebRTDataGetFromCrweberdotcom();
    if (m_fDataBaseLoaded) {
      if (m_lNewestUPdateDate < GetFormatedMarketDate()) {
        gl_WebInquirer.GetCrweberIndexData();
      }
    }
    else {
      GetNewestDatabaseDateFromDB();
      m_fDataBaseLoaded = true;
    }
  }

  return true;
}

bool CCrweberIndexMarket::SchedulingTaskPer1Minute(long lCurrentTime) {
  return true;
}

bool CCrweberIndexMarket::TaskResetMarket(long lCurrentTime) {
  // 市场时间四点重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 70000) && (lCurrentTime < 71000)) { // 本市场时间的一点重启本市场 // 东八区本地时间为下午三时。
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

bool CCrweberIndexMarket::TaskProcessWebRTDataGetFromCrweberdotcom(void) {
  CWebDataPtr pWebData = nullptr;
  const long lTotalData = gl_WebInquirer.GetCrweberDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebData = gl_WebInquirer.PopCrweberData();
    pWebData->ResetCurrentPos();
    if (m_CrweberIndex.ReadData(pWebData)) {
      m_lNewestUPdateDate = m_CrweberIndex.m_lDate;
      if (m_lNewestDatabaseDate < m_CrweberIndex.m_lDate) {
        m_fTodayDataUpdated = true;
        m_lNewestDatabaseDate = m_CrweberIndex.m_lDate;
      }
      if (!m_fTodayDataUpdated || m_CrweberIndex.IsDataChanged(m_CrweberIndexLast)) {
        m_CrweberIndexLast = m_CrweberIndex;
        CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>(m_CrweberIndex);
        m_vCrweberIndex.push_back(pCrweberIndex);
        RunningThreadSaveCrweberDB(pCrweberIndex);
        gl_systemMessage.PushInformationMessage(_T("crweber油运指数已更新"));
        m_fTodayDataUpdated = true;
      }
    }
    else return false;  // 后面的数据出问题，抛掉不用。
  }
  return true;
}

bool CCrweberIndexMarket::RunningThreadMaintainDB(void) {
  thread thread1(ThreadMaintainCrweberDB, this);
  thread1.detach();

  return true;
}

bool CCrweberIndexMarket::RunningThreadSaveCrweberDB(CCrweberIndexPtr pCrweberIndex) {
  thread thread1(ThreadSaveCrweberDB, this, pCrweberIndex);
  thread1.detach();

  return true;
}

bool CCrweberIndexMarket::LoadDatabase(void) {
  CSetCrweberIndex setCrweberIndex;
  int i = 0;
  int iTotal = 1000;
  m_vCrweberIndex.resize(iTotal);
  for (int j = 0; j < iTotal; j++) m_vCrweberIndex.at(j) = nullptr;

  setCrweberIndex.m_strSort = _T("[Date]");
  setCrweberIndex.Open();
  while (!setCrweberIndex.IsEOF()) {
    CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>();
    pCrweberIndex->LoadData(setCrweberIndex);
    m_vCrweberIndex.at(i) = pCrweberIndex;
    if ((i + 1) >= iTotal) {
      iTotal += 1000;
      m_vCrweberIndex.resize(iTotal);
      for (int j = iTotal - 1; j > i; j--) m_vCrweberIndex.at(j) = nullptr;
    }
    if (m_lNewestDatabaseDate < pCrweberIndex->m_lDate) {
      i++;
      m_lNewestDatabaseDate = pCrweberIndex->m_lDate;
    }
    setCrweberIndex.MoveNext();
  }
  setCrweberIndex.Close();
  return true;
}

bool CCrweberIndexMarket::SaveDatabase(void) {
  CSetCrweberIndex setCrweberIndex;

  setCrweberIndex.Open();
  setCrweberIndex.m_pDatabase->BeginTrans();
  while (!setCrweberIndex.IsEOF()) {
    setCrweberIndex.Delete();
    setCrweberIndex.MoveNext();
  }
  for (auto pCrweberIndex : m_vCrweberIndex) {
    if (pCrweberIndex != nullptr) {
      pCrweberIndex->AppendData(setCrweberIndex);
    }
  }
  setCrweberIndex.m_pDatabase->CommitTrans();
  setCrweberIndex.Close();

  return true;
}

bool CCrweberIndexMarket::GetNewestDatabaseDateFromDB(void) {
  CSetCrweberIndex setCrweberIndex;

  setCrweberIndex.m_strSort = _T("[Date]");
  setCrweberIndex.Open();
  if (!setCrweberIndex.IsEOF()) {
    setCrweberIndex.MoveLast();
    if (m_lNewestDatabaseDate < setCrweberIndex.m_Date) {
      m_lNewestDatabaseDate = setCrweberIndex.m_Date;
    }
  }
  setCrweberIndex.Close();
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将crweber.com油运指数数据存入数据库。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CCrweberIndexMarket::SaveCrweberIndexData(CCrweberIndexPtr pCrweberIndex) {
  CSetCrweberIndex setIndex;

  setIndex.m_strFilter = _T("[ID] = 1");
  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  pCrweberIndex->AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}