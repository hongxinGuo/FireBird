#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDLThreads = 4; // �˱���ȡֵ��ΧΪ1-4

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
CMockNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiry = nullptr; // ����������ʷ����
CMockNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquirySecond = nullptr; // ����������ʷ����
CMockNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiryThird = nullptr; // ����������ʷ����
CMockNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiryFourth = nullptr; // ����������ʷ����
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
CNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiry = nullptr; // ����������ʷ����
CNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquirySecond = nullptr; // ����������ʷ����
CNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiryThird = nullptr; // ����������ʷ����
CNeteaseDLWebInquiryPtr gl_pNeteaseDLWebInquiryFourth = nullptr; // ����������ʷ����
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
  gl_pNeteaseDLWebInquiry = make_shared<CMockNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquirySecond = make_shared<CMockNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquiryThird = make_shared<CMockNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquiryFourth = make_shared<CMockNeteaseDLWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
#else
  gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
  gl_pNeteaseDLWebInquiry = make_shared<CNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquirySecond = make_shared<CNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquiryThird = make_shared<CNeteaseDLWebInquiry>();
  gl_pNeteaseDLWebInquiryFourth = make_shared<CNeteaseDLWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
#endif
}

bool CWebInquirer::GetNeteaseDLData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  ASSERT((gl_cMaxSavingOneDLThreads < 5) && (gl_cMaxSavingOneDLThreads > 0));
  switch (gl_cMaxSavingOneDLThreads) {
  case 4:
  if (!gl_pNeteaseDLWebInquiryFourth->IsReadingWebData()) {
    gl_pNeteaseDLWebInquiryFourth->GetWebData();
  }
  case 3:
  if (!gl_pNeteaseDLWebInquiryThird->IsReadingWebData()) {
    gl_pNeteaseDLWebInquiryThird->GetWebData();
  }
  case 2:
  if (!gl_pNeteaseDLWebInquirySecond->IsReadingWebData()) {
    gl_pNeteaseDLWebInquirySecond->GetWebData();
  }
  case 1:
  if (!gl_pNeteaseDLWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDLWebInquiry->GetWebData();
  }
  break;
  default:
  if (!gl_pNeteaseDLWebInquiry->IsReadingWebData()) {
    gl_pNeteaseDLWebInquiry->GetWebData();
  }
  break;
  }
  return true;
}