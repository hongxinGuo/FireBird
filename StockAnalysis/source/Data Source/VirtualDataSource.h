#pragma once

#include"TemplateMutexAccessQueue.h"
#include"PriorityQueueWebRTData.h"

#include"VirtualProductWebData.h"
#include"VirtualWebInquiry.h"

using namespace std;
#include<queue>

class CVirtualDataSource : public CObject {
public:
	CVirtualDataSource(void);
	virtual ~CVirtualDataSource(void);

	virtual bool Reset(void);

	void Run(long lCurrentTime);
	virtual bool Inquire(long) { return true; } // 继承类实现各自的查询任务. 参数为当前市场时间（hhmmss）
	virtual bool ProcessInquiringMessage(void);
	virtual bool ProcessWebDataReceived(void);
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData); // 默认是在处理完本次数据后方才允许再次接收。
	virtual bool UpdateStatus(void) { ASSERT(0); return true; }

	void SetCurrentInquiryFunction(CString strFunctionName) { m_pWebInquiry->SetInquiryFunction(strFunctionName); }
	void StartThreadGetWebData(void) { m_pWebInquiry->GetWebData(); }

	void SetWebInquiringPtr(CVirtualWebInquiry* p) noexcept { m_pWebInquiry = p; }
	CVirtualWebInquiry* GetWebInquiryPtr(void) noexcept { return m_pWebInquiry; }

	size_t GetInquiryQueueSize(void) noexcept { return m_qProduct.size(); }
	void StoreInquiry(CVirtualProductWebDataPtr p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr GetInquiry(void) { m_pCurrentProduct = m_qProduct.front(); m_qProduct.pop(); return m_pCurrentProduct; }
	bool HaveInquiry(void) { if (m_qProduct.size() > 0) return true; else return false; }

	CVirtualProductWebDataPtr GetCurrentInquiry(void) noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(CVirtualProductWebDataPtr p) { m_pCurrentProduct = p; }

	bool IsInquiring(void) noexcept { return m_fInquiring; }
	void SetInquiring(bool fFlag) noexcept { m_fInquiring = fFlag; }
	bool IsInquiringAndClearFlag(void) noexcept { const bool fInquiring = m_fInquiring.exchange(false); return fInquiring; }

	void SetWebInquiryFinished(bool fFlag) noexcept { m_fWebInquiryFinished = fFlag; }
	bool IsWebInquiryFinished(void) noexcept { const bool f = m_fWebInquiryFinished; return f; }
	bool IsWebInquiryFinishedAndClearFlag(void) noexcept { const bool fWebInquiryFinished = m_fWebInquiryFinished.exchange(false); return fWebInquiryFinished; }

	void StoreReceivedData(CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }
	bool HaveReceiviedData(void) { if (m_qReceivedData.Size() > 0) return true; else return false; }

	bool IsEnable(void) noexcept { return m_fEnable; }
	void Enable(bool fFlag) noexcept { m_fEnable = fFlag; }

protected:
	CVirtualWebInquiry* m_pWebInquiry; // 网络数据查询器。一个Data source包含一个唯一的查询器。该查询器只为此DataSource服务，不得滥用。此处使用裸指针，防止不必要的解析动作。
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // 网络查询命令队列
	CVirtualProductWebDataPtr m_pCurrentProduct;
	atomic_bool m_fInquiring;
	atomic_bool m_fWebInquiryFinished; // WebInquiry已经执行完毕
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // 网络数据暂存队列

	bool m_fEnable; // 允许执行标识
};

typedef shared_ptr<CVirtualDataSource> CVirtualDataSourcePtr;
