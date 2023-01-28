#include "pch.h"

#include"ChinaStockCodeConverter.h"
#include "TengxunDayLineDataSource.h"
#include"ProductTengxunDayLine.h"

#include"ChinaMarket.h"
#include "TengxunDayLineWebInquiry.h"
#include "TimeConvert.h"

CTengxunDayLineDataSource::CTengxunDayLineDataSource() {
	CTengxunDayLineDataSource::Reset();
}

bool CTengxunDayLineDataSource::Reset(void) {
	CVirtualDataSource::Reset();
	m_fUpdateDayLine = true;
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
// 腾讯日线数据每次最多提供2000个。当所需数据超过两千个时，需要分次提取。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunDayLineDataSource::Inquire(const long lCurrentTime) {
	if (gl_systemStatus.IsWebBusy()) return false; // 网络出现问题时，不申请网易日线数据。
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			InquireDayLine();
			return true;
		}
	}
	return true;
}

bool CTengxunDayLineDataSource::InquireDayLine(void) {
	const auto lStockSetSize = gl_pChinaMarket->GetTotalStock();
	bool fHaveInquiry = false;
	const CString strFunction = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
	const CString strSuffix = _T(",2000,,");

	if (!IsInquiring() && IsUpdateDayLine()) {
		CChinaStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			pStock = gl_pChinaMarket->GetStock(lCurrentUpdateDayLinePos);
			if (!pStock->IsDayLineNeedUpdate()) { // 无需更新？
			}
			else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
				pStock->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
			}
			else {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			long lStartDate = GetPrevDay(pStock->GetDayLineEndDate());
			const long lCurrentDate = gl_pChinaMarket->GetMarketDate();
			const long yearDiffer = (lCurrentDate - lStartDate) / 10000;
			const long lStockIndex = gl_pChinaMarket->GetStockIndex(pStock);
			long l = 0;
			int iCounter = 0;
			long yearEnd, monthEnd, dayEnd;
			char buffer[50], bufferEnd[50];
			const CString strStockCode = XferStandardToTengxun(pStock->GetSymbol());
			shared_ptr<CProductTengxunDayLine> product = nullptr;
			do {
				product = make_shared<CProductTengxunDayLine>();
				const long year = lStartDate / 10000;
				const long month = lStartDate / 100 - year * 100;
				const long day = lStartDate - year * 10000 - month * 100;
				sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
				CString strStartDate = buffer;
				if ((l + 7) > yearDiffer) {
					yearEnd = lCurrentDate / 10000;
					monthEnd = lCurrentDate / 100 - yearEnd * 100;
					dayEnd = lCurrentDate - yearEnd * 10000 - month * 100;
				}
				else {
					yearEnd = lStartDate / 10000 + 6;
					monthEnd = 12;
					dayEnd = 31;
				}
				sprintf_s(bufferEnd, _T("%4d-%02d-%02d"), yearEnd, monthEnd, dayEnd);
				CString strEndDate = bufferEnd;
				const CString strTotalMessage = strFunction + strStockCode + _T(",day,") + strStartDate + _T(",") + strEndDate + strSuffix;
				product->SetIndex(lStockIndex);
				product->SetInquiry(strTotalMessage);
				StoreInquiry(product);
				l += 7;
				lStartDate = (year + 7) * 10000 + 1 * 100 + 1;
				iCounter++;
			} while (l < yearDiffer);
			product->Reset();
			product->SetInquiryNumber(iCounter);
			(dynamic_cast<CTengxunDayLineWebInquiry*>(m_pWebInquiry))->SetDownLoadingStockCode(pStock->GetSymbol());
			gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
			pStock->SetDayLineNeedUpdate(false);
			SetInquiring(true);
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "中国市场股票日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
