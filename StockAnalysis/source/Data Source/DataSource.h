#pragma once

#include"TemplateMutexAccessQueue.h"

#include"VirtualProductWebData.h"
#include"VirtualWebInquiry.h"

using namespace std;
#include<queue>

class CDataSource : public CObject {
public:
	CDataSource(void);
	virtual ~CDataSource(void) {}

	virtual bool Reset(void);

	void Run(long lCurrentTime);
	virtual bool Inquiry(long lCurrentTime) { return true; } // �̳���ʵ�ָ��ԵĲ�ѯ����
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

protected:
	CVirtualWebInquiryPtr m_pWebInquiry; // �������ݲ�ѯ��
	queue<CProductWebSourceDataPtr, list<CProductWebSourceDataPtr>> m_qProduct; // �����ѯ�������
	CProductWebSourceDataPtr m_pCurrentProduct;
	bool m_fInquiring;
	atomic_bool m_fDataReceived;

	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // ���������ݴ����
};

typedef shared_ptr<CDataSource> CDataSourcePtr;

extern CDataSourcePtr gl_pDataSourceQuandl;
