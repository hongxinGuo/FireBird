#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"

CVirtualDataSource::CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;

	Reset();
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;

	return true;
}

void CVirtualDataSource::Run(long lCurrentTime) {
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
		if (IsDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
			GetInquiry();

			m_pWebInquiry->SetInquiryFunction(m_pCurrentProduct->CreatMessage()); // 设置功能字符串

			SetDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
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
// 目前只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	vector<CWebDataPtr> vWebData;
	vector<CVirtualProductWebDataPtr> vProductWebData;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // 如果网络数据接收完成
		if (m_qReceivedData.Size() > 0) {  // 处理当前网络数据
			ASSERT(IsInquiring());
			pWebData = m_qReceivedData.PopData();
			m_pCurrentProduct->CheckNoRightToAccess(pWebData);
			if (m_pCurrentProduct->IsNoRightToAccess()) { // 如果系统报告无权查询此类数据
				// 目前先在软件系统消息中报告
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
				m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // 检查是否无权查询
			}
			// 有些网络数据比较大，处理需要的时间超长（如美国市场的股票代码有5M，处理时间为。。。）
			// 故而需要考虑将下面这个函数线程化。
			m_pCurrentProduct->ParseAndStoreWebData(pWebData);
			SetInquiring(false);
			// 下面是线程实现方式
			// vWebData[0] = pWebData;
			// vProductWebData[0] = m_pCurrentProduct;
			// thread thread1(ThreadWebSourceParseAndStoreWebData, this, vProductWebData, vWebData);
			// thread1.detach();
			return true;
		}
	}
	return false;
}

UINT ThreadWebSourceParseAndStoreWebData(CVirtualDataSource* pDataSource, vector<CVirtualProductWebDataPtr> vProductWebData, vector<CWebDataPtr> vWebData) {
	CVirtualProductWebDataPtr pProductWebData = vProductWebData.at(0);
	CWebDataPtr pWebData = vWebData.at(0);

	pProductWebData->ParseAndStoreWebData(pWebData);
	pDataSource->SetInquiring(false);

	return 203;
}