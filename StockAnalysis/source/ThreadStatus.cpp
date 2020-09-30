#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStatus::CThreadStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统状态只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
  }
  m_NumberOfBackGroundWorkingThreads = 0;

  m_CalculatingDLRS = false;
  m_CalculatingWeekLineRS = false;
  m_RTDataNeedCalculate = false;
  m_CalculatingRTData = false;
  m_SavingTempData = false;
  m_CreatingWeekLine = false;

  m_SavingDL = 0;
  m_NumberOfRunningThread = 0;
}

CThreadStatus::~CThreadStatus() {
}

bool CThreadStatus::IsSavingDL(void) {
  if (m_SavingDL > 0) {
    return true;
  }
  else return false;
}