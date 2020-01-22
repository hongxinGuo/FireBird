#include "CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebDataInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  // ���ô�ȫ�ֱ���
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // ���û�����Ⱥ�������ɹ�ͬ����

  static time_t s_timeLast = 0;
  const long lCurrentTime = gl_systemTime.GetTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (gl_systemTime.Gett_time() > (s_timeLast + 60 * 5)) {
    SchedulingTaskPer5Minute(gl_systemTime.Gett_time() - s_timeLast, lCurrentTime);
    s_timeLast = gl_systemTime.Gett_time();
  }

  return true;
}

bool CCrweberIndexMarket::SchedulingTaskPer5Minute(long lSecond, long lCurrentTime) {
  // �Զ���ѯcrweber.com
  if (!gl_WebDataInquirer.IsReadingCrweberIndex()) {
    gl_WebDataInquirer.GetCrweberIndexData();
    TaskProcessWebRTDataGetFromCrweberdotcom();
  }

  return true;
}

bool CCrweberIndexMarket::TaskProcessWebRTDataGetFromCrweberdotcom(void) {
  CWebDataReceivedPtr pWebData = nullptr;
  long lTotalData = gl_WebDataInquirer.GetCrweberDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebData = gl_WebDataInquirer.PopCrweberData();
    pWebData->m_pCurrentPos = pWebData->m_pDataBuffer;
    pWebData->SetCurrentPos(0);
    if (m_CrweberIndex.ReadData(pWebData)) {
      if (m_CrweberIndex.IsTodayUpdated() || m_CrweberIndex.IsDataChanged(m_CrweberIndexLast)) {
        m_CrweberIndexLast = m_CrweberIndex;
        SaveCrweberIndexData();
        gl_systemMessage.PushInformationMessage(_T("crweber����ָ���Ѹ���"));
        m_CrweberIndex.m_fTodayUpdated = false;
      }
    }
    else return false;  // ��������ݳ����⣬�׵����á�
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��crweber.com����ָ�����ݴ������ݿ⡣
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CCrweberIndexMarket::SaveCrweberIndexData(void) {
  CSetCrweberIndex setIndex;
  setIndex.m_strFilter = _T("[ID] = 1");

  // �洢�������ɵ�������CrweberIndex���С�
  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  m_CrweberIndex.AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}