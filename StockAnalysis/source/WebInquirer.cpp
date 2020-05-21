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

CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth = nullptr; // 网易日线历史数据
CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
#else
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth = nullptr; // 网易日线历史数据
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
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
  // 抓取日线数据.
  // 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  if (!gl_pNeteaseDayLineWebInquirySixth->IsReadingWebData()) {
    TRACE("使用第六个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquirySixth->GetWebData(); // 网易日线历史数据
  }
  case 5:
  if (!gl_pNeteaseDayLineWebInquiryFifth->IsReadingWebData()) {
    TRACE("使用第五个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquiryFifth->GetWebData();
  }
  case 4:
  if (!gl_pNeteaseDayLineWebInquiryFourth->IsReadingWebData()) {
    TRACE("使用第四个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquiryFourth->GetWebData();
  }
  case 3:
  if (!gl_pNeteaseDayLineWebInquiryThird->IsReadingWebData()) {
    TRACE("使用第三个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquiryThird->GetWebData();
  }
  case 2:
  if (!gl_pNeteaseDayLineWebInquirySecond->IsReadingWebData()) {
    TRACE("使用第二个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquirySecond->GetWebData();
  }
  case 1: case 0:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    TRACE("使用第一个日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  break;
  default:
  if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
    TRACE("使用第一个默认日线历史数据查询器\n");
    gl_pNeteaseDayLineWebInquiry->GetWebData();
  }
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
  }
  return true;
}