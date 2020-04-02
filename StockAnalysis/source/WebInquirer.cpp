#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4;

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�

#ifdef __GOOGLEMOCK__
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
using namespace Testing;

CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth = nullptr; // ����������ʷ����
CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
#else
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth = nullptr; // ����������ʷ����
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
#endif

CWebInquirer::CWebInquirer(void) {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
  gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
#ifdef __GOOGLEMOCK__
  gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySecond = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryThird = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFourth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFifth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySixth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
#else
  gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySecond = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryThird = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFourth = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFifth = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySixth = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
#endif
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  if (!gl_pNeteaseDayLineWebInquirySixth->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquirySixth->GetWebData(); // ����������ʷ����
  }
  case 5:
  if (!gl_pNeteaseDayLineWebInquiryFifth->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiryFifth->GetWebData();
  }
  case 4:
  if (!gl_pNeteaseDayLineWebInquiryFourth->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiryFourth->GetWebData();
  }
  case 3:
  if (!gl_pNeteaseDayLineWebInquiryThird->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiryThird->GetWebData();
  }
  case 2:
  if (!gl_pNeteaseDayLineWebInquirySecond->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquirySecond->GetWebData();
  }
  case 1: case 0:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  default:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}