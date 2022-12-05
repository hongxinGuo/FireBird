#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"
#include"ThreadStatus.h"

CVirtualDataSource::CVirtualDataSource(void) {
	m_pCurrentProduct = nullptr;
	m_fEnable = true; // 默认为允许执行

	Reset();
}

CVirtualDataSource::~CVirtualDataSource(void) {
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;

	return true;
}

void CVirtualDataSource::Run(long lCurrentTime) {
	if (!m_fEnable) return; // 不允许执行的话，直接返回
	Inquire(lCurrentTime);
	if (ProcessWebDataReceived()) { // 先处理接收到的网络数据
		UpdateStatus();
	}
	ProcessInquiringMessage(); // 然后再申请下一个网络数据
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// DataSource读取函数采用申请和接收轮换执行方式，故而至少调用两次才完成一个轮回。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataSource::ProcessInquiringMessage(void) {
	if (m_qProduct.size() > 0) { // 有申请等待？
		ASSERT(IsInquiring());
		if (IsDataReceivedAndClearFlag()) { //已经发出了数据申请且Finnhub数据已经接收到了？重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
			GetInquiry();
			m_pWebInquiry->SetInquiryFunction(m_pCurrentProduct->CreatMessage()); // 设置功能字符串
			m_pWebInquiry->GetWebData();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////
//
// 处理工作线程接收到的网络信息。
//
// 只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
// 目前已将中国市场的SinaRT、NeteaseRT、TengxunRT和NeteaseDayline的数据使用此数据源实现了。由于SinaRT和NeteaseRT要求的实时性比较强，
// 故而该两个数据源采用先返回后处理数据的模式。--20221120
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // 如果网络数据接收完成
		if (m_qReceivedData.Size() > 0) {  // 处理当前网络数据
			pWebData = m_qReceivedData.PopData();
			if (pWebData->IsParsed()) {
				m_pCurrentProduct->CheckNoRightToAccess(pWebData);
				if (m_pCurrentProduct->IsNoRightToAccess()) { // 如果系统报告无权查询此类数据
					// 目前先在软件系统消息中报告
					gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
					m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // 检查是否无权查询
				}
			}
			ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
			// 有些网络数据比较大，处理需要的时间超长（如美国市场的股票代码有5M，处理时间为。。。）， 故而需要将ProductWebData的函数ParseAndStoreWebData()线程化。
			// 本线程必须位于本函数的最后，因其调用SetInquiry(false)后，启动了下次申请，故而能防止发生重入问题。
			thread thread1(ThreadWebSourceParseAndStoreWebData, this, m_pCurrentProduct, pWebData);
			thread1.detach();
			SetInquiring(false); // 此标识的重置需要位于位于最后一步
			return true;
		}
	}
	return false;
}

// 此信号量用于解析WebSource中的数据。
// 将ParseAndStoreData线程限制至最多3个，这样既能保证足够的计算速度，也不会发生系统颠簸。当改为4个时，就能观察到系统颠簸。
counting_semaphore<3> gl_WebSourceParseAndStoreData{ 3 };

UINT ThreadWebSourceParseAndStoreWebData(not_null<CVirtualDataSource*> pDataSource, not_null<CVirtualProductWebDataPtr> pProductWebData, not_null<CWebDataPtr> pWebData) {
	gl_WebSourceParseAndStoreData.acquire();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
	pDataSource->ParseAndStoreData(pProductWebData, pWebData);
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
	gl_WebSourceParseAndStoreData.release();

	return 203;
}

/// <summary>
/// 默认是在处理完本次数据后方允许接收下次数据，这样能够减少线程间的同步问题
/// </summary>
/// <param name="pProductWebData"></param>
/// <param name="pWebData"></param>
void CVirtualDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	pProductWebData->ParseAndStoreWebData(pWebData); // 在处理完数据后，方允许系统申请新的数据。
}