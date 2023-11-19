//////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateInsiderSentimentDB(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateInsiderSentimentDB();
	gl_UpdateWorldMarketDB.release();

	return 58;
}
