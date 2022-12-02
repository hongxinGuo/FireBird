#include"pch.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDaylineDataSource.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

CWebInquirer::CWebInquirer(void) : CObject() {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
	ASSERT(gl_pChinaMarket != nullptr);
	ASSERT(gl_pWorldMarket != nullptr);
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

	gl_pSinaRTWebInquiry->SetDataSource(gl_pSinaRTDataSource.get());
	gl_pTengxunRTWebInquiry->SetDataSource(gl_pTengxunRTDataSource.get());
	gl_pNeteaseRTWebInquiry->SetDataSource(gl_pNeteaseRTDataSource.get());
	gl_pNeteaseDayLineWebInquiry->SetDataSource(gl_pNeteaseDaylineDataSource.get());

	gl_pChinaMarket->StoreDataSource(gl_pSinaRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pTengxunRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseDaylineDataSource);

	if (gl_systemConfigeration.GetChinaMarketRealtimeServer() == 0) { // 使用新浪实时数据服务器
		gl_pSinaRTDataSource->Enable(true);
		gl_pNeteaseRTDataSource->Enable(false);
	}
	else {
		gl_pSinaRTDataSource->Enable(false);
		gl_pNeteaseRTDataSource->Enable(true);
	}

	// 查询器和数据源要一一对应
	gl_pFinnhubDataSource->SetWebInquiringPtr(gl_pFinnhubWebInquiry);
	gl_pTiingoDataSource->SetWebInquiringPtr(gl_pTiingoWebInquiry);
	gl_pQuandlDataSource->SetWebInquiringPtr(gl_pQuandlWebInquiry);

	gl_pFinnhubWebInquiry->SetDataSource(gl_pFinnhubDataSource.get());
	gl_pTiingoWebInquiry->SetDataSource(gl_pTiingoDataSource.get());
	gl_pQuandlWebInquiry->SetDataSource(gl_pQuandlDataSource.get());

	gl_pWorldMarket->StoreDataSource(gl_pFinnhubDataSource);
	gl_pWorldMarket->StoreDataSource(gl_pTiingoDataSource);
}