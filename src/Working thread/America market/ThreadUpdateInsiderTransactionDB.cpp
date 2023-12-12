//////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateInsiderTransactionDB(const CWorldMarketPtr& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateInsiderTransactionDB();
	gl_UpdateWorldMarketDB.release();

	return 48;
}
