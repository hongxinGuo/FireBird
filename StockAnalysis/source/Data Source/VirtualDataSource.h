#pragma once

#include"TemplateMutexAccessQueue.h"

#include"VirtualWebProduct.h"
#include"VirtualWebInquiry.h"

#include<semaphore>
#include<list>
using std::counting_semaphore;
using std::list;

// ���ź������ڽ���WebSource�е����ݡ�
// ��ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������
extern counting_semaphore<3> gl_WebSourceParseAndStoreData;

class CVirtualDataSource {
public:
	CVirtualDataSource(void);
	// ֻ����һ��ʵ��,������ֵ��
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource(void) = default;

	virtual bool Reset(void);

	void Run(const long lCurrentTime);
	virtual bool Inquire(const long) { return true; } // �̳���ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual bool ProcessInquiringMessage(void);
	virtual bool ProcessWebDataReceived(void);
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData);
	// Ĭ�����ڴ����걾�����ݺ󷽲������ٴν��ա�
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
	CVirtualWebInquiry* m_pWebInquiry; // �������ݲ�ѯ����һ��Data source����һ��Ψһ�Ĳ�ѯ�����ò�ѯ��ֻΪ��DataSource���񣬲������á��˴�ʹ����ָ�룬��ֹ����Ҫ�Ľ���������
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct;
	atomic_bool m_fInquiring;
	atomic_bool m_fWebInquiryFinished; // WebInquiry�Ѿ�ִ�����
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // ���������ݴ����

	bool m_fEnable; // ����ִ�б�ʶ
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
