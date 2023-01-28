#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"
#include"ThreadStatus.h"

#include<thread>
using std::thread;

CVirtualDataSource::CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;
	m_fEnable = true; // 默认为允许执行

	CVirtualDataSource::Reset();
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fWebInquiryFinished = true;

	return true;
}

void CVirtualDataSource::InquireAndProcess(const long lCurrentTime) {
	if (!m_fEnable) return; // 不允许执行的话，直接返回
	if (!HaveInquiry()) { // 目前允许一次申请生成多个查询，故而有可能需要多次查询后方允许再次申请。
		Inquire(lCurrentTime);
	}
	else {
		SetInquiring(true); // 队列中还有待查询的申请，设置此标识。
	}
	if (ProcessWebDataReceived()) {	// 先处理接收到的网络数据
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
	if (HaveInquiry()) {
		// 有申请等待？
		if (IsWebInquiryFinishedAndClearFlag()) {//已经发出了数据申请且数据已经接收到了？重置此标识需要放在启动工作线程（StartThreadGetWebData）之前，否则工作线程中的断言容易出错。
			GetInquiry();
			SetCurrentInquiryFunction(m_pCurrentProduct->CreateMessage()); // 设置功能字符串
			StartThreadGetWebData();
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
// 目前已将中国市场的SinaRT、NeteaseRT、TengxunRT和NeteaseDayLine的数据使用此数据源实现了。由于SinaRT和NeteaseRT要求的实时性比较强，
// 故而该两个数据源采用先返回后处理数据的模式。--20221120
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	if (HaveReceivedData()) {
		// 处理当前网络数据
		ASSERT(m_pCurrentProduct != nullptr);
		CWebDataPtr pWebData = GetReceivedData();
		if (pWebData->IsParsed()) {
			m_pCurrentProduct->CheckNoRightToAccess(pWebData);
			if (m_pCurrentProduct->IsNoRightToAccess()) {
				// 如果系统报告无权查询此类数据
				// 目前先在软件系统消息中报告
				gl_systemMessage.PushInnerSystemInformationMessage(
					_T("No right to access: ") + m_pCurrentProduct->GetTotalInquiryMessage() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
				m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // 检查是否无权查询
			}
		}
		ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
		if (!HaveInquiry()) { // 没有现存的申请时
			SetInquiring(false); // 此标识的重置需要位于位于最后一步
		}
		// 有些网络数据比较大，处理需要的时间超长（如美国市场的股票代码有5M，处理时间为。。。）， 故而需要将ProductWebData的函数ParseAndStoreWebData()线程化。
		// 本线程必须位于本函数的最后，因其调用SetInquiry(false)后，启动了下次申请，故而能防止发生重入问题。
		thread thread1(ThreadWebSourceParseAndStoreWebData, this, m_pCurrentProduct, pWebData);
		thread1.detach();
		return true;
	}
	return false;
}

UINT ThreadWebSourceParseAndStoreWebData(CVirtualDataSource* pDataSource, CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	gl_WebSourceParseAndStoreData.acquire();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	pDataSource->ParseAndStoreData(pProductWebData, pWebData);
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();
	gl_WebSourceParseAndStoreData.release();

	return 203;
}

/// <summary>
///
/// </summary>
/// <param name="pProductWebData"></param>
/// <param name="pWebData"></param>
void CVirtualDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	pProductWebData->ParseAndStoreWebData(pWebData);
}
