#pragma once

#include"TemplateMutexAccessQueue.h"

#include"VirtualProductWebData.h"
#include"VirtualWebInquiry.h"

using namespace std;
#include<queue>

class CVirtualDataSource : public CObject {
public:
	CVirtualDataSource(void);
	virtual ~CVirtualDataSource(void) {}

	virtual bool Reset(void);

	void Run(long lCurrentTime);
	virtual bool Inquiry(long lCurrentTime) { return true; } // 继承类实现各自的查询任务
	virtual bool ProcessInquiringMessage(void);
	virtual bool ProcessWebDataReceived(void);
	virtual bool UpdateStatus(void) { ASSERT(0); return true; }

	void SetWebInquiringPtr(CVirtualWebInquiryPtr p) {
		m_pWebInquiry = p;
	}

	size_t GetInquiryQueueSize(void) noexcept { return m_qProduct.size(); }
	void StoreInquiry(CProductWebSourceDataPtr p) { m_qProduct.push(p); }
	CProductWebSourceDataPtr GetInquiry(void) { m_pCurrentProduct = m_qProduct.front(); m_qProduct.pop(); return m_pCurrentProduct; }

	CProductWebSourceDataPtr GetCurrentInquiry(void) noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(CProductWebSourceDataPtr p) { m_pCurrentProduct = p; }

	bool IsInquiring(void) noexcept { return m_fInquiring; }
	void SetInquiring(bool fFlag) noexcept { m_fInquiring = fFlag; }
	void SetDataReceived(bool fFlag) noexcept { m_fDataReceived = fFlag; }
	bool IsDataReceived(void) noexcept { const bool f = m_fDataReceived; return f; }

	void StoreReceivedData(CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }

	int GetNumberOfNoRightToAccess(void) noexcept { return m_iNumberOfNoRightToAccess; }
	void ClearNoRightToAccess(void) noexcept { m_iNumberOfNoRightToAccess = 0; }

protected:
	CVirtualWebInquiryPtr m_pWebInquiry; // 网络数据查询器
	queue<CProductWebSourceDataPtr, list<CProductWebSourceDataPtr>> m_qProduct; // 网络查询命令队列
	CProductWebSourceDataPtr m_pCurrentProduct;
	bool m_fInquiring;
	atomic_bool m_fDataReceived;
	int m_iNumberOfNoRightToAccess;

	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // 网络数据暂存队列
};

typedef shared_ptr<CVirtualDataSource> CDataSourcePtr;

extern CDataSourcePtr gl_pDataSourceQuandl;
