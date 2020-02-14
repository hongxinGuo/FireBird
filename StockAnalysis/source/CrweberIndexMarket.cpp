#include "CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket市场变量只允许存在一个实例\n");
    ASSERT(0);
  }

  m_strMarketId = _T("Crweber.com");
  m_lTimeZoneOffset = 0; // crweber.com使用GMT。
  CalculateTime();

  Reset();
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  m_fDataBaseLoaded = false;
  m_fTodayDataUpdated = true;
  m_lNewestDatabaseDay = 0;
  m_lNewestUpdatedDay = 0;
  // 重置此全局变量
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetTime();

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetLocalTime() > s_timeLast + 60) {
    SchedulingTaskPer1Minute(GetLocalTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetLocalTime();
  }

  return true;
}

void CCrweberIndexMarket::ResetMarket(void) {
  Reset();
  CString str = _T("重置Crweber.com于格林威治标准时间：");
  str += GetMarketTimeString();
  gl_systemMessage.PushInformationMessage(str);
}

bool CCrweberIndexMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  TaskResetMarket(lCurrentTime);

  // 自动查询crweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    if (!gl_WebInquirer.IsReadingCrweberIndex()) {
      TaskProcessWebRTDataGetFromCrweberdotcom();
      if (m_fDataBaseLoaded) {
        gl_WebInquirer.GetCrweberIndexData();
      }
      else {
        LoadDatabase();
        SaveDatabase();
        m_fDataBaseLoaded = true;
      }
    }
    return true;
  }
  else {
    return false;
  }
}

bool CCrweberIndexMarket::TaskResetMarket(long lCurrentTime) {
  // 九点重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if (lCurrentTime >= 10000) { // 本市场时间的一点重启本市场 // 东八区本地时间为上午九时。
      if (lCurrentTime <= 13000) {
        SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      }
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

bool CCrweberIndexMarket::TaskProcessWebRTDataGetFromCrweberdotcom(void) {
  CWebDataPtr pWebData = nullptr;
  long lTotalData = gl_WebInquirer.GetCrweberDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebData = gl_WebInquirer.PopCrweberData();
    pWebData->m_pCurrentPos = pWebData->m_pDataBuffer;
    pWebData->SetCurrentPos(0);
    if (m_CrweberIndex.ReadData(pWebData)) {
      m_lNewestUpdatedDay = m_CrweberIndex.m_lDay;
      if (m_lNewestDatabaseDay < m_CrweberIndex.m_lDay) {
        m_fTodayDataUpdated = true;
        m_lNewestDatabaseDay = m_CrweberIndex.m_lDay;
      }
      if (!m_fTodayDataUpdated || m_CrweberIndex.IsDataChanged(m_CrweberIndexLast)) {
        m_CrweberIndexLast = m_CrweberIndex;
        CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>(m_CrweberIndex);
        m_vCrweberIndex.push_back(pCrweberIndex);
        SaveCrweberIndexData(pCrweberIndex);
        gl_systemMessage.PushInformationMessage(_T("crweber油运指数已更新"));
        m_fTodayDataUpdated = true;
      }
    }
    else return false;  // 后面的数据出问题，抛掉不用。
  }
  return true;
}

bool CCrweberIndexMarket::LoadDatabase(void) {
  CSetCrweberIndex setCrweberIndex;
  int i = 0;
  m_vCrweberIndex.resize(10000);
  for (auto pCrweber : m_vCrweberIndex) pCrweber = nullptr;

  setCrweberIndex.m_strSort = _T("[Day]");
  setCrweberIndex.Open();
  while (!setCrweberIndex.IsEOF()) {
    CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>();
    pCrweberIndex->LoadData(setCrweberIndex);
    m_vCrweberIndex[i] = pCrweberIndex;
    if (m_lNewestDatabaseDay < pCrweberIndex->m_lDay) {
      i++;
      m_lNewestDatabaseDay = pCrweberIndex->m_lDay;
    }
    setCrweberIndex.MoveNext();
  }
  setCrweberIndex.Close();
  return true;
}

bool CCrweberIndexMarket::SaveDatabase(void) {
  CSetCrweberIndex setCrweberIndex;

  setCrweberIndex.Open();
  while (!setCrweberIndex.IsEOF()) {
    setCrweberIndex.Delete();
    setCrweberIndex.MoveNext();
  }
  setCrweberIndex.Close();

  setCrweberIndex.Open();
  setCrweberIndex.m_pDatabase->BeginTrans();
  for (auto pCrweberIndex : m_vCrweberIndex) {
    if (pCrweberIndex != nullptr) {
      pCrweberIndex->AppendData(setCrweberIndex);
    }
  }
  setCrweberIndex.m_pDatabase->CommitTrans();
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

  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  pCrweberIndex->AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}