#include"stdafx.h"
#include"globedef.h"

#include "ChinaStake.h"

#include"WebInquirer.h"

#ifdef __GOOGLEMOCK__
#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
using namespace testing;

CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CMockSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry = nullptr;
CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry7 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry8 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry9 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry10 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry11 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry12 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry13 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry14 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry15 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry16 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry17 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry18 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry19 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry20 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry21 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry22 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry23 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry24 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry25 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry26 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry27 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry28 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry29 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry30 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry31 = nullptr; // 网易日线历史数据
CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry32 = nullptr; // 网易日线历史数据
#else
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry = nullptr;
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry7 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry8 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry9 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry10 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry11 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry12 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry13 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry14 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry15 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry16 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry17 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry18 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry19 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry20 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry21 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry22 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry23 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry24 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry25 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry26 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry27 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry28 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry29 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry30 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry31 = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry32 = nullptr; // 网易日线历史数据
#endif

CWebInquirer::CWebInquirer(void) {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
#ifdef __GOOGLEMOCK__
  gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
  gl_pSinaStakeRTWebInquiry = make_shared<CMockSinaStakeRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry3 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry4 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry5 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry6 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry7 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry8 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry9 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry10 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry11 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry12 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry13 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry14 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry15 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry16 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry17 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry18 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry19 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry20 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry21 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry22 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry23 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry24 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry25 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry26 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry27 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry28 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry29 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry30 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry31 = make_shared<CMockNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry32 = make_shared<CMockNeteaseDayLineWebInquiry>();
#else
  gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
  gl_pSinaStakeRTWebInquiry = make_shared<CSinaStakeRTWebInquiry>();
  gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
  gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
  gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
  gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
  gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry3 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry4 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry5 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry6 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry7 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry8 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry9 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry10 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry11 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry12 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry13 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry14 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry15 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry16 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry17 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry18 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry19 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry20 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry21 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry22 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry23 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry24 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry25 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry26 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry27 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry28 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry29 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry30 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry31 = make_shared<CNeteaseDayLineWebInquiry>();
  gl_pNeteaseDayLineWebInquiry32 = make_shared<CNeteaseDayLineWebInquiry>();
#endif
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
  // 抓取日线数据.
  // 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
  ASSERT((gl_cMaxSavingOneDayLineThreads <= 32) && (gl_cMaxSavingOneDayLineThreads > 0));
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 40:
  case 39:
  case 38:
  case 37:
  case 36:
  case 35:
  case 34:
  case 33:
  case 32:
  if (!gl_pNeteaseDayLineWebInquiry32->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry32->GetWebData();
  }
  case 31:
  if (!gl_pNeteaseDayLineWebInquiry31->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry31->GetWebData();
  }
  case 30:
  if (!gl_pNeteaseDayLineWebInquiry30->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry30->GetWebData();
  }
  case 29:
  if (!gl_pNeteaseDayLineWebInquiry29->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry29->GetWebData();
  }
  case 28:
  if (!gl_pNeteaseDayLineWebInquiry28->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry28->GetWebData();
  }
  case 27:
  if (!gl_pNeteaseDayLineWebInquiry27->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry27->GetWebData();
  }
  case 26:
  if (!gl_pNeteaseDayLineWebInquiry26->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry26->GetWebData();
  }
  case 25:
  if (!gl_pNeteaseDayLineWebInquiry25->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry25->GetWebData();
  }
  case 24:
  if (!gl_pNeteaseDayLineWebInquiry24->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry24->GetWebData();
  }
  case 23:
  if (!gl_pNeteaseDayLineWebInquiry23->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry23->GetWebData();
  }
  case 22:
  if (!gl_pNeteaseDayLineWebInquiry22->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry22->GetWebData();
  }
  case 21:
  if (!gl_pNeteaseDayLineWebInquiry21->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry21->GetWebData();
  }

  case 20:
  if (!gl_pNeteaseDayLineWebInquiry20->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry20->GetWebData();
  }
  case 19:
  if (!gl_pNeteaseDayLineWebInquiry19->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry19->GetWebData();
  }
  case 18:
  if (!gl_pNeteaseDayLineWebInquiry18->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry18->GetWebData();
  }
  case 17:
  if (!gl_pNeteaseDayLineWebInquiry17->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry17->GetWebData();
  }
  case 16:
  if (!gl_pNeteaseDayLineWebInquiry16->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry16->GetWebData();
  }
  case 15:
  if (!gl_pNeteaseDayLineWebInquiry15->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry15->GetWebData();
  }
  case 14:
  if (!gl_pNeteaseDayLineWebInquiry14->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry14->GetWebData();
  }
  case 13:
  if (!gl_pNeteaseDayLineWebInquiry13->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry13->GetWebData();
  }
  case 12:
  if (!gl_pNeteaseDayLineWebInquiry12->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry12->GetWebData();
  }
  case 11:
  if (!gl_pNeteaseDayLineWebInquiry11->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry11->GetWebData();
  }
  case 10:
  if (!gl_pNeteaseDayLineWebInquiry10->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry10->GetWebData();
  }
  case 9:
  if (!gl_pNeteaseDayLineWebInquiry9->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry9->GetWebData();
  }
  case 8:
  if (!gl_pNeteaseDayLineWebInquiry8->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry8->GetWebData();
  }
  case 7:
  if (!gl_pNeteaseDayLineWebInquiry7->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry7->GetWebData();
  }
  case 6:
  if (!gl_pNeteaseDayLineWebInquiry6->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry6->GetWebData();
  }
  case 5:
  if (!gl_pNeteaseDayLineWebInquiry5->IsReadingWebData()) {
    gl_pNeteaseDayLineWebInquiry5->GetWebData();
  }
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