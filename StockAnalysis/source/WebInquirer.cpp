#include"pch.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_iMaxSavingOneDayLineThreads = 4; // 此变量取值范围为1-4

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com上的每日油运指数
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com上的油运数据。
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

CWebInquirer::CWebInquirer(void) : CObject() {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
	gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
	gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
	gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
	gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
	gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
	gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
	gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
	gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
	// 抓取日线数据.
	// 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。目前使用两个。
	ASSERT((gl_iMaxSavingOneDayLineThreads <= 32) && (gl_iMaxSavingOneDayLineThreads > 0));
	switch (gl_iMaxSavingOneDayLineThreads) {
	case 6:
	case 5:
	case 4:
	case 3:
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