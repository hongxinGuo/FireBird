#include "pch.h"

#include "TengxunDayLineDataSource.h"
#include"ProductTengxunDayLine.h"

#include"ChinaMarket.h"

CTengxunDayLineDataSource::CTengxunDayLineDataSource() {
}

bool CTengxunDayLineDataSource::Reset(void) {
	return true;
}

bool CTengxunDayLineDataSource::UpdateStatus(void) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
// 抓取日线数据.开始于11:45:01
//
// 由于可能会抓取全部5000个左右日线数据，所需时间超过10分钟，故而9:15:00第一次重置系统时不去更新，而在9:25:00第二次重置系统后才开始。
// 为了防止与重启系统发生冲突，实际执行时间延后至11:45:01,且不是下载实时数据的工作时间
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunDayLineDataSource::Inquire(const long lCurrentTime) {
	if (gl_systemStatus.IsWebBusy()) return false; // 网络出现问题时，不申请网易日线数据。
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			const auto product = make_shared<CProductTengxunDayLine>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}
