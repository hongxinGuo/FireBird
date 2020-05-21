#include"stdafx.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4;

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
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth = nullptr; // ����������ʷ����
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth = nullptr; // ����������ʷ����
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
#ifdef __GOOGLEMOCK__
  gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySecond = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryThird = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFourth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiryFifth = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquirySixth = make_shared<CMockNeteaseDayLineWebInquiry>();
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
    TRACE("ʹ�õ�����������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquirySixth->GetWebData(); // ����������ʷ����
  }
  case 5:
  if (!gl_pNeteaseDayLineWebInquiryFifth->IsReadingWebData()) {
    TRACE("ʹ�õ����������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquiryFifth->GetWebData();
  }
  case 4:
  if (!gl_pNeteaseDayLineWebInquiryFourth->IsReadingWebData()) {
    TRACE("ʹ�õ��ĸ�������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquiryFourth->GetWebData();
  }
  case 3:
  if (!gl_pNeteaseDayLineWebInquiryThird->IsReadingWebData()) {
    TRACE("ʹ�õ�����������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquiryThird->GetWebData();
  }
  case 2:
  if (!gl_pNeteaseDayLineWebInquirySecond->IsReadingWebData()) {
    TRACE("ʹ�õڶ���������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquirySecond->GetWebData();
  }
  case 1: case 0:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    TRACE("ʹ�õ�һ��������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  default:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    TRACE("ʹ�õ�һ��Ĭ��������ʷ���ݲ�ѯ��\n");
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}