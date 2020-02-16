#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4;

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  if (!m_NeteaseDayLineWebInquirySixth.IsReadingWebData()) {
    m_NeteaseDayLineWebInquirySixth.GetWebData(); // ����������ʷ����
  }
  case 5:
  if (!m_NeteaseDayLineWebInquiryFifth.IsReadingWebData()) {
    m_NeteaseDayLineWebInquiryFifth.GetWebData();
  }
  case 4:
  if (!m_NeteaseDayLineWebInquiryFourth.IsReadingWebData()) {
    m_NeteaseDayLineWebInquiryFourth.GetWebData();
  }
  case 3:
  if (!m_NeteaseDayLineWebInquiryThird.IsReadingWebData()) {
    m_NeteaseDayLineWebInquiryThird.GetWebData();
  }
  case 2:
  if (!m_NeteaseDayLineWebInquirySecond.IsReadingWebData()) {
    m_NeteaseDayLineWebInquirySecond.GetWebData();
  }
  case 1: case 0:
  if (!m_NeteaseDayLineWebInquiry.IsReadingWebData()) {
    m_NeteaseDayLineWebInquiry.GetWebData();
  }
  break;
  default:
  if (!m_NeteaseDayLineWebInquiry.IsReadingWebData()) {
    m_NeteaseDayLineWebInquiry.GetWebData();
  }
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}