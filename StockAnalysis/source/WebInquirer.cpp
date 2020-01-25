#include"stdafx.h"
#include"globedef.h"

#include "Stock.h"

#include"WebInquirer.h"

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  m_NeteaseDayLineWebInquirySixth.GetWebData(); // ����������ʷ����
  case 5:
  m_NeteaseDayLineWebInquiryFifth.GetWebData();
  case 4:
  m_NeteaseDayLineWebInquiryFourth.GetWebData();
  case 3:
  m_NeteaseDayLineWebInquiryThird.GetWebData();
  case 2:
  m_NeteaseDayLineWebInquirySecond.GetWebData();
  case 1: case 0:
  m_NeteaseDayLineWebInquiry.GetWebData();
  break;
  default:
  m_NeteaseDayLineWebInquiry.GetWebData();
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}