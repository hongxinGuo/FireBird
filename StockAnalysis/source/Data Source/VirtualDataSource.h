#pragma once

#include"TemplateMutexAccessQueue.h"

#include"VirtualWebProduct.h"
#include"VirtualWebInquiry.h"

#include<semaphore>
#include<list>
using std::counting_semaphore;
using std::list;

// 此信号量用于解析WebSource中的数据。
// 将ParseAndStoreData线程限制至最多3个，这样既能保证足够的计算速度，也不会发生系统颠簸。当改为4个时，就能观察到系统颠簸。
extern counting_semaphore<3> gl_WebSourceParseAndStoreData;

class CVirtualDataSource {
public:
	CVirtualDataSource(void);
	// 只能有一个实例,不允许赋值。
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource(void) = default;

	virtual bool Reset(void);

	void Run(const long lCurrentTime);
	virtual bool Inquire(const long) { return true; } // 继承类实现各自的查询任务. 参数为当前市场时间（hhmmss）
	virtual bool ProcessInquiringMessage(void);
	virtual bool ProcessWebDataReceived(void);
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData);
	// 默认是在处理完本次数据后方才允许再次接收。
	virtual bool UpdateStatus(void) {
		ASSERT(0);
		return true;
	}

	void SetCurrentInquiryFunction(const CString& strFunctionName) const { m_pWebInquiry->SetInquiryFunction(strFunctionName); }
	void StartThreadGetWebData(void) const { m_pWebInquiry->GetWebData(); }

	void SetWebInquiringPtr(CVirtualWebInquiry* p) noexcept { m_pWebInquiry = p; }
	CVirtualWebInquiry* GetWebInquiryPtr(void) const noexcept { return m_pWebInquiry; }

	size_t GetInquiryQueueSize(void) const noexcept { return m_qProduct.size(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }

	CVirtualProductWebDataPtr GetInquiry(void) {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}

	bool HaveInquiry(void) const {
		if (m_qProduct.empty()) return false;
		else return true;
	}

	CVirtualProductWebDataPtr GetCurrentInquiry(void) const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	bool IsInquiring(void) const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }

	bool IsInquiringAndClearFlag(void) noexcept {
		const bool fInquiring = m_fInquiring.exchange(false);
		return fInquiring;
	}

	void SetWebInquiryFinished(const bool fFlag) noexcept { m_fWebInquiryFinished = fFlag; }

	bool IsWebInquiryFinished(void) const noexcept { return m_fWebInquiryFinished; }

	bool IsWebInquiryFinishedAndClearFlag(void) noexcept {
		const bool fWebInquiryFinished = m_fWebInquiryFinished.exchange(false);
		return fWebInquiryFinished;
	}

	void StoreReceivedData(const CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }

	bool HaveReceivedData(void) {
		if (m_qReceivedData.Empty()) return false;
		else return true;
	}

	bool IsEnable(void) const noexcept { return m_fEnable; }
	void Enable(const bool fFlag) noexcept { m_fEnable = fFlag; }

protected:
	CVirtualWebInquiry* m_pWebInquiry; // 网络数据查询器。一个Data source包含一个唯一的查询器。该查询器只为此DataSource服务，不得滥用。此处使用裸指针，防止不必要的解析动作。
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // 网络查询命令队列
	CVirtualProductWebDataPtr m_pCurrentProduct;
	atomic_bool m_fInquiring;
	atomic_bool m_fWebInquiryFinished; // WebInquiry已经执行完毕
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // 网络数据暂存队列

	bool m_fEnable; // 允许执行标识
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
