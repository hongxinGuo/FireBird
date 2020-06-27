#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4; // �˱���ȡֵ��ΧΪ1-4

#ifdef __GOOGLEMOCK__
#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
using namespace testing;

CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // ����������ʷ����
CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
#else
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // ����������ʷ����
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
#endif

CWebInquirer::CWebInquirer(void) {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
#ifdef __GOOGLEMOCK__
  gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySecond = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryThird = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFourth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
#else
  gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySecond = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryThird = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFourth = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
#endif
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  ASSERT((gl_cMaxSavingOneDayLineThreads < 5) && (gl_cMaxSavingOneDayLineThreads > 0));
  switch (gl_cMaxSavingOneDayLineThreads) {
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
  case 1:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  default:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  }
  return true;
}