#include "CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebDataInquirer.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket市场变量只允许存在一个实例\n");
    ASSERT(0);
  }
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

void CCrweberIndexMarket::Reset(void) {
  // 重置此全局变量
  m_CrweberIndex.Reset();
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // 调用基类调度函数，完成共同任务

  static time_t s_timeLast = 0;
  const long lCurrentTime = gl_systemTime.GetTime();

  //根据时间，调度各项定时任务.每秒调度一次
  if (gl_systemTime.Gett_time() > (s_timeLast + 60 * 5)) {
    SchedulingTaskPer5Minute(gl_systemTime.Gett_time() - s_timeLast, lCurrentTime);
    s_timeLast = gl_systemTime.Gett_time();
  }

  return true;
}

bool CCrweberIndexMarket::SchedulingTaskPer5Minute(long lSecond, long lCurrentTime) {
  // 自动查询crweber.com
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
        gl_systemMessage.PushInformationMessage(_T("crweber油运指数已更新"));
        m_CrweberIndex.m_fTodayUpdated = false;
      }
    }
    else return false;  // 后面的数据出问题，抛掉不用。
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将crweber.com油运指数数据存入数据库。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CCrweberIndexMarket::SaveCrweberIndexData(void) {
  CSetCrweberIndex setIndex;
  setIndex.m_strFilter = _T("[ID] = 1");

  // 存储今日生成的数据于CrweberIndex表中。
  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  m_CrweberIndex.AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}