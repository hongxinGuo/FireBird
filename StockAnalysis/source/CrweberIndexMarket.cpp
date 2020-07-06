#include "CrweberIndexMarket.h"
#include"thread.h"

#include"WebInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket�г�����ֻ�������һ��ʵ��\n");
  }

  m_strMarketId = _T("Crweber.com");
  m_lTimeZoneOffset = 0; // crweber.comʹ��GMT��
  CalculateTime();

  Reset();
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  //SetReadyToRun(false); // Ŀǰ�������ȣ��޷���ȷ��ȡ��վ��Ϣ��
  m_fDataBaseLoaded = false;
  m_fTodayDataUpdated = true;
  m_fMaintainDatabase = true;
  m_lNewestDatabaseDay = 0;
  m_lNewestUpdatedDay = 0;
  LoadDatabase();
  SaveDatabase();

  // ���ô�ȫ�ֱ���
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

void CCrweberIndexMarket::ResetMarket(void) {
  Reset();
  CString str = _T("����Crweber.com�ڸ������α�׼ʱ�䣺");
  str += GetMarketTimeString();
  gl_systemMessage.PushInformationMessage(str);
}

bool CCrweberIndexMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  return true;
}

bool CCrweberIndexMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    TaskResetMarket(lCurrentTime);
    //TaskMaintainDatabase(lCurrentTime);

    if (!gl_WebInquirer.IsReadingCrweberIndex()) {
      TaskProcessWebRTDataGetFromCrweberdotcom();
      if (m_fDataBaseLoaded) {
        if (m_lNewestUpdatedDay < GetFormatedMarketDay()) {
          gl_WebInquirer.GetCrweberIndexData();
        }
      }
      else {
        GetNewestDatabaseDayFromDB();
        m_fDataBaseLoaded = true;
      }
    }

    return true;
  }
  else return false;
}

bool CCrweberIndexMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // һСʱһ�εļ�����

  // �Զ���ѯcrweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;
    return true;
  }
  else {
    return false;
  }
}

bool CCrweberIndexMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ���ĵ�����ϵͳ
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime >= 70000) && (lCurrentTime < 71000)) { // ���г�ʱ���һ���������г� // ����������ʱ��Ϊ������ʱ��
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
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
        RunningSaveCrweberDB(pCrweberIndex);
        gl_systemMessage.PushInformationMessage(_T("crweber����ָ���Ѹ���"));
        m_fTodayDataUpdated = true;
      }
    }
    else return false;  // ��������ݳ����⣬�׵����á�
  }
  return true;
}

bool CCrweberIndexMarket::RunningMaintainDB(void) {
  thread thread1(ThreadMaintainCrweberDB, this);
  thread1.detach();

  return true;
}

bool CCrweberIndexMarket::RunningSaveCrweberDB(CCrweberIndexPtr pCrweberIndex) {
  thread thread1(ThreadSaveCrweberDB, this, pCrweberIndex);
  thread1.detach();

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
  setCrweberIndex.m_pDatabase->BeginTrans();
  while (!setCrweberIndex.IsEOF()) {
    setCrweberIndex.Delete();
    setCrweberIndex.MoveNext();
  }
  setCrweberIndex.m_pDatabase->CommitTrans();
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

bool CCrweberIndexMarket::GetNewestDatabaseDayFromDB(void) {
  CSetCrweberIndex setCrweberIndex;

  setCrweberIndex.m_strSort = _T("[Day]");
  setCrweberIndex.Open();
  if (!setCrweberIndex.IsEOF()) {
    setCrweberIndex.MoveLast();
    if (m_lNewestDatabaseDay < setCrweberIndex.m_Day) {
      m_lNewestDatabaseDay = setCrweberIndex.m_Day;
    }
  }
  setCrweberIndex.Close();
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��crweber.com����ָ�����ݴ������ݿ⡣
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