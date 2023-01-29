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

	if (!IsInquiring() && IsUpdateDayLine()) {
		ASSERT(!HaveInquiry());
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
			const vector<CVirtualProductWebDataPtr> vProduct = CreateProduct(pStock);
			ASSERT(!vProduct.empty());
			for (auto& product : vProduct) {
				StoreInquiry(product);
			}
			(dynamic_cast<CTengxunDayLineWebInquiry*>(m_pWebInquiry))->SetDownLoadingStockCode(pStock->GetSymbol());
			gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(pStock->GetSymbol());
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

vector<CVirtualWebProductPtr> CTengxunDayLineDataSource::CreateProduct(CChinaStockPtr pStock) {
	const CString strFunction = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
	const CString strSuffix = _T(",2000,,");
	long lStartDate = GetPrevDay(pStock->GetDayLineEndDate()); // 腾讯日线没有提供昨收盘信息，故而多申请一天数据来更新昨收盘。
	const long lCurrentDate = gl_pChinaMarket->GetMarketDate();
	const long yearDiffer = (lCurrentDate - lStartDate) / 10000;
	const long lStockIndex = gl_pChinaMarket->GetStockIndex(pStock);
	vector<CVirtualWebProductPtr> vProduct;
	long l = 0;
	int iCounter = 0;
	const CString strStockCode = XferStandardToTengxun(pStock->GetSymbol());
	shared_ptr<CProductTengxunDayLine> product = nullptr;
	do {
		product = make_shared<CProductTengxunDayLine>();
		CString strStartDate = ConvertDateToString(lStartDate, _T("%4d-%02d-%02d"));
		CString strEndDate;
		const long year = lStartDate / 10000;
		if ((l + 7) > yearDiffer) {
			strEndDate = ConvertDateToString(lCurrentDate, _T("%4d-%02d-%02d"));
		}
		else {
			strEndDate = ConvertDateToString((year + 6) * 10000 + 1231, _T("%4d-%02d-%02d"));
		}
		const CString strTotalMessage = strFunction + strStockCode + _T(",day,") + strStartDate + _T(",") + strEndDate + strSuffix;
		product->SetIndex(lStockIndex);
		product->SetInquiry(strTotalMessage);
		vProduct.push_back(product);
		l += 7;
		lStartDate = (year + 7) * 10000 + 101;
		iCounter++;
	} while (l < yearDiffer);
	product->SetInquiryNumber(iCounter);

	return vProduct;
}
