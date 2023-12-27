#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateCountryListDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubCountry.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 40;
}
