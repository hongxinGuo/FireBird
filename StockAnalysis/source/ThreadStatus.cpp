#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStatus::CThreadStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
  }
  m_NumberOfBackGroundWorkingThreads = 0;
  m_NumberOfSavingThread = 0;
  m_NumberOfWebInquiringThread = 0;

  m_CalculatingDayLineRS = false;
  m_CalculatingWeekLineRS = false;
  m_RTDataNeedCalculate = false;
  m_CalculatingRTData = false;
  m_SavingTempData = false;
  m_CreatingWeekLine = false;

}

CThreadStatus::~CThreadStatus() {
}
