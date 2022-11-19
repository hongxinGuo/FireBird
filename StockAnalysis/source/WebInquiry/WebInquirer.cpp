#include"pch.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDaylineDataSource.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

CWebInquirer::CWebInquirer(void) : CObject() {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
	gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
	gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
	gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
	gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
	gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
	gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();

	// 查询器和数据源要一一对应、互相包含
	gl_pSinaRTDataSource->SetWebInquiringPtr(gl_pSinaRTWebInquiry);
	gl_pTengxunRTDataSource->SetWebInquiringPtr(gl_pTengxunRTWebInquiry);
	gl_pNeteaseRTDataSource->SetWebInquiringPtr(gl_pNeteaseRTWebInquiry);
	gl_pNeteaseDaylineDataSource->SetWebInquiringPtr(gl_pNeteaseDayLineWebInquiry);

	gl_pChinaMarket->StoreDataSource(gl_pSinaRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pTengxunRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseDaylineDataSource);

	if (gl_systemConfigeration.GetChinaMarketRealtimeServer() == 0) { // 使用新浪实时数据服务器
		gl_pSinaRTDataSource->SetEanble(true);
		gl_pNeteaseRTDataSource->SetEanble(false);
	}
	else {
		gl_pSinaRTDataSource->SetEanble(false);
		gl_pNeteaseRTDataSource->SetEanble(true);
	}

	// 查询器和数据源要一一对应、互相包含
	gl_pDataSourceFinnhub->SetWebInquiringPtr(gl_pFinnhubWebInquiry);
	gl_pDataSourceTiingo->SetWebInquiringPtr(gl_pTiingoWebInquiry);
	gl_pDataSourceQuandl->SetWebInquiringPtr(gl_pQuandlWebInquiry);

	gl_pWorldMarket->StoreDataSource(gl_pDataSourceFinnhub);
	gl_pWorldMarket->StoreDataSource(gl_pDataSourceTiingo);
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
	// 抓取日线数据.
	// 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。目前使用两个。
	ASSERT((gl_systemConfigeration.GetSavingChinaMarketStockDayLineThread() <= 32) && (gl_systemConfigeration.GetSavingChinaMarketStockDayLineThread() > 0));
	switch (gl_systemConfigeration.GetSavingChinaMarketStockDayLineThread()) {
	case 6:
	case 5:
	case 4:
	case 3:
	case 2:
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