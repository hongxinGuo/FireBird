#include"stdafx.h"
#include"globedef.h"

#include "ChinaStake.h"

#include"WebInquirer.h"

int gl_cMaxSavingOneDayLineThreads = 4; // 此变量取值范围为1-4

#ifdef __GOOGLEMOCK__
#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
#include"MockFinnhubWebInquiry.h"
#include"MockTiingoWebInquiry.h"
#include"MockQuandlWebInquiry.h"
using namespace testing;

CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4 = nullptr; // 网易日线历史数据
CMockFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr;
CMockTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr;
CMockQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr;
#else
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4 = nullptr; // 网易日线历史数据
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
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
  gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry3 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry4 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pFinnhubWebInquiry = make_shared<CMockFinnhubWebInquiry>();
  gl_pTiingoWebInquiry = make_shared<CMockTiingoWebInquiry>();
  gl_pQuandlWebInquiry = make_shared<CMockQuandlWebInquiry>();

#else
  gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry3 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry4 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
  gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
  gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();
#endif
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // 抓取日线数据.
  // 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
  ASSERT((gl_cMaxSavingOneDayLineThreads <= 32) && (gl_cMaxSavingOneDayLineThreads > 0));
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 6:
  case 5:
  case 4:
  if (!gl_pNeteaseDayLineWebInquiry4->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry4->GetWebData();
  }
  case 3:
  if (!gl_pNeteaseDayLineWebInquiry3->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry3->GetWebData();
  }
  case 2:
  if (!gl_pNeteaseDayLineWebInquiry2->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry2->GetWebData();
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