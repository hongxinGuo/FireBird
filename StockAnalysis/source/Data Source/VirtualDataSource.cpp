#include "pch.h"

#include "VirtualDataSource.h"

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
	Inquiry(lCurrentTime);
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
	bool fDone = false;

	if (m_qProduct.size() > 0) { // 有申请等待？
		ASSERT(IsInquiring());
		if (IsDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
			m_pCurrentProduct = m_qProduct.front();
			m_qProduct.pop();

			m_pWebInquiry->SetInquiryingStringPrefix(m_pCurrentProduct->CreatMessage()); // 设置前缀

			SetDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
			m_pWebInquiry->GetWebData();
			fDone = true;
		}
	}

	return fDone;
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
	bool fDone = false;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // 如果网络数据接收完成
		if (m_qReceivedData.Size() > 0) {  // 处理当前网络数据
			ASSERT(IsInquiring());
			pWebData = m_qReceivedData.PopData();

			m_pCurrentProduct->ParseAndStoreWebData(pWebData);
			if (m_pCurrentProduct->NoRightToAccess()) { // 如果系统报告无权查询此类数据
				// 目前先在软件系统消息中报告
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access ") + m_pCurrentProduct->GetInquiringStr());
				if (!m_pCurrentProduct->IsUSMarket()) { // 只有当不是美国市场时，才检查数据的可读取性。美国市场皆默认为可读取。
					m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // 检查是否无权查询
				}
			}
			m_pWebInquiry->SetInquiryingStringMiddle(_T("")); // 有些网络申请没有用到中间字符段，如果不清除之前的中间字符段（如果有的话），会造成申请字符串的错误。
			SetInquiring(false);
			fDone = true;
		}
	}

	return fDone;
}