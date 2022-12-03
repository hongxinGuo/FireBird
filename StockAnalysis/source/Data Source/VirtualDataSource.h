#pragma once

#include"TemplateMutexAccessQueue.h"

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
	virtual bool Inquire(long) { return true; } // �̳���ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual bool ProcessInquiringMessage(void);
	virtual bool ProcessWebDataReceived(void);
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData); // Ĭ�����ڴ����걾�����ݺ󷽲������ٴν��ա�
	virtual bool UpdateStatus(void) { ASSERT(0); return true; }

	void SetWebInquiringPtr(CVirtualWebInquiry* p) noexcept { m_pWebInquiry = p; }
	CVirtualWebInquiry* GetWebInquiryPtr(void) noexcept { return m_pWebInquiry; }

	size_t GetInquiryQueueSize(void) noexcept { return m_qProduct.size(); }
	void StoreInquiry(CVirtualProductWebDataPtr p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr GetInquiry(void) { m_pCurrentProduct = m_qProduct.front(); m_qProduct.pop(); return m_pCurrentProduct; }

	CVirtualProductWebDataPtr GetCurrentInquiry(void) noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(CVirtualProductWebDataPtr p) { m_pCurrentProduct = p; }

	bool IsInquiring(void) noexcept { return m_fInquiring; }
	void SetInquiring(bool fFlag) noexcept { m_fInquiring = fFlag; }
	bool IsInquiringAndClearFlag(void) noexcept { const bool fInquiring = m_fInquiring.exchange(false); return fInquiring; }
	void SetDataReceived(bool fFlag) noexcept { m_fDataReceived = fFlag; }
	bool IsDataReceived(void) noexcept { const bool f = m_fDataReceived; return f; }
	bool IsDataReceivedAndClearFlag(void) noexcept { const bool fDataReceived = m_fDataReceived.exchange(false); return fDataReceived; }

	void StoreReceivedData(CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }

	bool IsEnable(void) noexcept { return m_fEnable; }
	void Enable(bool fFlag) noexcept { m_fEnable = fFlag; }

protected:
	CVirtualWebInquiry* m_pWebInquiry; // �������ݲ�ѯ����һ��Data source����һ��Ψһ�Ĳ�ѯ�����ò�ѯ��ֻΪ��DataSource���񣬲������á��˴�ʹ����ָ�룬��ֹ������
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct;
	atomic_bool m_fInquiring;
	atomic_bool m_fDataReceived;
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // ���������ݴ����

	bool m_fEnable; // ����ִ�б�ʶ
};

typedef shared_ptr<CVirtualDataSource> CVirtualDataSourcePtr;
