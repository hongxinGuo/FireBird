////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 腾讯日线历史数据。
///
/// 腾讯日线服务器一次只能发送最多2000个数据，超过2000个数据的申请，需要拆分成多次方可。故而申请信息的处理只能放在DataSource中处理，
/// product中存储的是处理后的完整申请字符串。
/// 腾讯日线的申请格式为：https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sz000003,day,2002-12-01,2009-01-23,2000,,
///
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"ChinaStockCodeConverter.h"
#include "TengxunDayLineDataSource.h"
#include"ProductTengxunDayLine.h"

#include"ChinaMarket.h"
#include "TimeConvert.h"

CTengxunDayLineDataSource::CTengxunDayLineDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
	m_strParam = _T("");
	m_strSuffix = _T(",2000,,");
	m_strInquiryToken = _T("");

	CTengxunDayLineDataSource::ConfigureSession();
	CTengxunDayLineDataSource::Reset();
}

bool CTengxunDayLineDataSource::Reset() {
	m_fUpdateDayLine = true;
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
bool CTengxunDayLineDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请腾讯日线数据。
	if (gl_pChinaMarket->IsSystemReady() && gl_dataContainerChinaStock.IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime() && (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			InquireDayLine();
			return true;
		}
	}
	return false;
}

bool CTengxunDayLineDataSource::InquireDayLine() {
	const auto lStockSetSize = gl_dataContainerChinaStock.Size();

	if (!IsInquiring() && IsUpdateDayLine()) {
		ASSERT(!HaveInquiry());
		CChinaStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			pStock = gl_dataContainerChinaStock.GetStock(lCurrentUpdateDayLinePos);
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
			const vector<CVirtualProductWebDataPtr> vProduct = CreateProduct(pStock);
			ASSERT(!vProduct.empty());
			for (auto& product : vProduct) {
				StoreInquiry(product);
			}
			SetDownLoadingStockCode(pStock->GetSymbol());
			gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(pStock->GetSymbol());
			pStock->SetDayLineNeedUpdate(false);
			SetInquiring(true);
			return true;
		}
		else {
			SetUpdateDayLine(false);
			const CString str = _T("中国市场股票日线历史数据更新完毕");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return false;
}

vector<CVirtualWebProductPtr> CTengxunDayLineDataSource::CreateProduct(const CChinaStockPtr& pStock) const {
	//long lStartDate = 20100101; // 强迫生成多次申请（测试用）
	long lStartDate = GetPrevDay(pStock->GetDayLineEndDate()); // 腾讯日线没有提供昨收盘信息，故而多申请一天数据来更新昨收盘。
	const long lCurrentDate = gl_pChinaMarket->GetMarketDate();
	const long yearDiffer = (lCurrentDate - lStartDate) / 10000;
	const long lStockIndex = gl_dataContainerChinaStock.GetOffset(pStock);
	vector<CVirtualWebProductPtr> vProduct;
	long l = 0;
	int iCounter = 0;
	const CString strStockCode = XferStandardToTengxun(pStock->GetSymbol());
	shared_ptr<CProductTengxunDayLine> product = nullptr;
	do {
		CString strStartDate = ConvertDateToTimeStamp(lStartDate);
		CString strEndDate;
		const long year = lStartDate / 10000;
		if ((l + 7) > yearDiffer) {
			strEndDate = ConvertDateToTimeStamp(lCurrentDate);
		}
		else {
			strEndDate = ConvertDateToTimeStamp((year + 6) * 10000 + 1231); // 第七年的最后一天
		}
		const CString strTotalMessage = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=") + strStockCode + _T(",day,") + strStartDate + _T(",") + strEndDate + m_strSuffix;
		product = make_shared<CProductTengxunDayLine>();
		product->SetMarket(gl_pChinaMarket);
		product->SetIndex(lStockIndex);
		product->SetInquiryFunction(strTotalMessage);
		vProduct.push_back(product);
		l += 7;
		lStartDate = (year + 7) * 10000 + 101;
		iCounter++;
	} while (l < yearDiffer);

	if (iCounter > 0) product->SetInquiryNumber(iCounter);

	return vProduct;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
void CTengxunDayLineDataSource::GenerateCurrentInquiryMessage() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();// 腾讯日线的查询字符串，在生成product时即完成了
}

void CTengxunDayLineDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000); // 正常情况下Tengxun日线数据接收时间不超过1秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 5000); // 设置接收超时时间为5秒
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 5000); // 设置接收超时时间为5秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

void CTengxunDayLineDataSource::UpdateStatusAfterReading(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CTengxunDayLineDataSource::SetDownLoadingStockCode(const CString& strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
