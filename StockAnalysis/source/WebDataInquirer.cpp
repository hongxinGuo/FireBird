#include"stdafx.h"
#include"globedef.h"

#include "Stock.h"

#include"WebDataInquirer.h"

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  m_NeteaseDayLineWebDataSixth.GetWebData(); // ����������ʷ����
  case 5:
  m_NeteaseDayLineWebDataFifth.GetWebData();
  case 4:
  m_NeteaseDayLineWebDataFourth.GetWebData();
  case 3:
  m_NeteaseDayLineWebDataThird.GetWebData();
  case 2:
  m_NeteaseDayLineWebDataSecond.GetWebData();
  case 1: case 0:
  m_NeteaseDayLineWebData.GetWebData();
  break;
  default:
  m_NeteaseDayLineWebData.GetWebData();
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}