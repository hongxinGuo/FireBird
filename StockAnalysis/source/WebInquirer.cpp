#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4;

CWebInquirer::CWebInquirer(void) {
  m_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  m_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
  m_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
  m_pNeteaseDayLineWebInquirySecond = make_shared<CNeteaseDayLineWebInquiry>();
  m_pNeteaseDayLineWebInquiryThird = make_shared<CNeteaseDayLineWebInquiry>();
  m_pNeteaseDayLineWebInquiryFourth = make_shared<CNeteaseDayLineWebInquiry>();
  m_pNeteaseDayLineWebInquiryFifth = make_shared<CNeteaseDayLineWebInquiry>();
  m_pNeteaseDayLineWebInquirySixth = make_shared<CNeteaseDayLineWebInquiry>();
  m_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
  m_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
}

CWebInquirer::~CWebInquirer(void) {
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  if (!m_pNeteaseDayLineWebInquirySixth->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquirySixth->GetWebData(); // ����������ʷ����
  }
  case 5:
  if (!m_pNeteaseDayLineWebInquiryFifth->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquiryFifth->GetWebData();
  }
  case 4:
  if (!m_pNeteaseDayLineWebInquiryFourth->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquiryFourth->GetWebData();
  }
  case 3:
  if (!m_pNeteaseDayLineWebInquiryThird->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquiryThird->GetWebData();
  }
  case 2:
  if (!m_pNeteaseDayLineWebInquirySecond->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquirySecond->GetWebData();
  }
  case 1: case 0:
  if (!m_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  default:
  if (!m_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    m_pNeteaseDayLineWebInquiry->GetWebData();
  }
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}