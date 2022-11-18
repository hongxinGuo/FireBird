#include"pch.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
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
	gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
	gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
	gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();

	// 查询器和数据源要一一对应、互相包含
	gl_pSinaRTDataSource->SetWebInquiringPtr(gl_pSinaRTWebInquiry);
	gl_pSinaRTWebInquiry->SetDataSource(gl_pSinaRTDataSource);
	gl_pTengxunRTDataSource->SetWebInquiringPtr(gl_pTengxunRTWebInquiry);
	gl_pTengxunRTWebInquiry->SetDataSource(gl_pTengxunRTDataSource);

	gl_pChinaMarket->StoreDataSource(gl_pSinaRTDataSource);

	// 查询器和数据源要一一对应、互相包含
	gl_pDataSourceFinnhub->SetWebInquiringPtr(gl_pFinnhubWebInquiry);
	gl_pFinnhubWebInquiry->SetDataSource(gl_pDataSourceFinnhub);
	gl_pDataSourceTiingo->SetWebInquiringPtr(gl_pTiingoWebInquiry);
	gl_pTiingoWebInquiry->SetDataSource(gl_pDataSourceTiingo);
	gl_pDataSourceQuandl->SetWebInquiringPtr(gl_pQuandlWebInquiry);
	gl_pQuandlWebInquiry->SetDataSource(gl_pDataSourceQuandl);

	gl_pWorldMarket->StoreDataSource(gl_pDataSourceFinnhub);
	gl_pWorldMarket->StoreDataSource(gl_pDataSourceTiingo);

	ASSERT(gl_pSinaRTDataSource->GetWebInquiryPtr()->GetDataSource()->IsKindOf(RUNTIME_CLASS(CSinaRTDataSource)));
	ASSERT(gl_pTengxunRTDataSource->GetWebInquiryPtr()->GetDataSource()->IsKindOf(RUNTIME_CLASS(CTengxunRTDataSource)));

	ASSERT(gl_pDataSourceFinnhub->GetWebInquiryPtr()->GetDataSource()->IsKindOf(RUNTIME_CLASS(CFinnhubDataSource)));
	ASSERT(gl_pDataSourceTiingo->GetWebInquiryPtr()->GetDataSource()->IsKindOf(RUNTIME_CLASS(CTiingoDataSource)));
	//ASSERT(gl_pDataSourceQuandl->GetWebInquiryPtr()->GetDataSource()->IsKindOf(RUNTIME_CLASS(CQuandlDataSource)));
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