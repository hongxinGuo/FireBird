#include "CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }

  m_lTimeZoneOffset = 0; // crweber.comʹ��GMT��
  CalculateTime();

  Reset();
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  m_fDataBaseLoaded = false;
  m_fTodayDataUupdated = true;
  m_lNewestDatabaseDay = 0;
  m_lNewestUpdatedDay = 0;
  // ���ô�ȫ�ֱ���
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // ���û�����Ⱥ�������ɹ�ͬ����

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetLocalTime() > (s_timeLast + 60 * 5)) {
    SchedulingTaskPer5Minute(GetLocalTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetLocalTime();
  }

  return true;
}

void CCrweberIndexMarket::ResetMarket(void) {
  Reset();
}

bool CCrweberIndexMarket::SchedulingTaskPer5Minute(long lSecond, long lCurrentTime) {
  // �Զ���ѯcrweber.com
  if (!gl_WebInquirer.IsReadingCrweberIndex()) {
    TaskProcessWebRTDataGetFromCrweberdotcom();
    if (m_fDataBaseLoaded) {
      gl_WebInquirer.GetCrweberIndexData();
    }
    else {
      LoadDatabase();
      m_lNewestDatabaseDay = m_vCrweberIndex.at(m_vCrweberIndex.size() - 1)->m_lDay;
      SaveDatabase();
      m_fDataBaseLoaded = true;
    }
  }
  return true;
}

bool CCrweberIndexMarket::TaskProcessWebRTDataGetFromCrweberdotcom(void) {
  CWebDataReceivedPtr pWebData = nullptr;
  long lTotalData = gl_WebInquirer.GetCrweberDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebData = gl_WebInquirer.PopCrweberData();
    pWebData->m_pCurrentPos = pWebData->m_pDataBuffer;
    pWebData->SetCurrentPos(0);
    if (m_CrweberIndex.ReadData(pWebData)) {
      m_lNewestUpdatedDay = m_CrweberIndex.m_lDay;
      if (m_lNewestDatabaseDay < m_CrweberIndex.m_lDay) {
        m_fTodayDataUupdated = true;
        m_lNewestDatabaseDay = m_CrweberIndex.m_lDay;
      }
      if (!m_fTodayDataUupdated || m_CrweberIndex.IsDataChanged(m_CrweberIndexLast)) {
        m_CrweberIndexLast = m_CrweberIndex;
        CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>(m_CrweberIndex);
        m_vCrweberIndex.push_back(pCrweberIndex);
        SaveCrweberIndexData(pCrweberIndex);
        gl_systemMessage.PushInformationMessage(_T("crweber����ָ���Ѹ���"));
        m_fTodayDataUupdated = false;
      }
    }
    else return false;  // ��������ݳ����⣬�׵����á�
  }
  return true;
}

bool CCrweberIndexMarket::LoadDatabase(void) {
  CSetCrweberIndex setCrweberIndex;
  int i = 0;

  setCrweberIndex.m_strSort = _T("[Day]");
  setCrweberIndex.Open();
  while (!setCrweberIndex.IsEOF()) {
    CCrweberIndexPtr pCrweberIndex = make_shared<CCrweberIndex>();
    pCrweberIndex->LoadData(setCrweberIndex);
    m_vCrweberIndex.resize(i + 1);
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
    pCrweberIndex->AppendData(setCrweberIndex);
  }
  setCrweberIndex.m_pDatabase->CommitTrans();
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
  setIndex.m_strFilter = _T("[ID] = 1");

  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  pCrweberIndex->AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}