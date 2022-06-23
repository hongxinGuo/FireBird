#include"pch.h"

#include "ChinaStock.h"

#include"WebInquirer.h"
#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

CWebInquirer::CWebInquirer(void) : CObject() {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
	gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
	gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
	gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
	gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
	gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
	gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();

	gl_pDataSourceFinnhub->SetWebInquiringPtr(gl_pFinnhubWebInquiry);
	gl_pDataSourceTiingo->SetWebInquiringPtr(gl_pTiingoWebInquiry);
	gl_pDataSourceQuandl->SetWebInquiringPtr(gl_pQuandlWebInquiry);

	gl_vDataSource.push_back(gl_pDataSourceFinnhub);
	gl_vDataSource.push_back(gl_pDataSourceTiingo);
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