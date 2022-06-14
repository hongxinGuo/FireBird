#include "pch.h"

#include "DataSource.h"
#include"SystemConfigeration.h"

CDataSource::CDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;

	Reset();
}

void CDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;
}

void CDataSource::Run(long lCurrentTime) {
	ProcessWebDataReceived();
	ProcessInquiringMessage();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub读取函数采用申请和接收轮换执行方式，故而至少调用两次才完成一个轮回。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CDataSource::ProcessInquiringMessage(void) {
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
bool CDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	bool fDone = false;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // 如果网络数据接收完成
		if (m_qReveivedData.Size() > 0) {  // 处理当前网络数据
			ASSERT(IsInquiring());
			pWebData = m_qReveivedData.PopData();

			m_pCurrentProduct->ParseAndStoreWebData(pWebData);

			m_pWebInquiry->SetInquiryingStringMiddle(_T("")); // 有些网络申请没有用到中间字符段，如果不清除之前的中间字符段（如果有的话），会造成申请字符串的错误。
			SetInquiring(false);
			fDone = true;
			m_pCurrentProduct = nullptr;
		}
	}

	return fDone;
}