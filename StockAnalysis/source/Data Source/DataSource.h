#pragma once

#include"ProductWebData.h"

using namespace std;
#include<queue>

class CDataSource : public CObject {
public:
	CDataSource(void);
	virtual ~CDataSource(void) {}

	size_t GetInquiryQueueSize(void) noexcept { return m_qProduct.size(); }
	void StoreInquiry(CProductWebSourceDataPtr p) { m_qProduct.push(p); }
	CProductWebSourceDataPtr GetInquiry(void) { CProductWebSourceDataPtr p = m_qProduct.front(); m_qProduct.pop(); return p; }

	bool IsInquiring(void) noexcept { return m_fInquiring; }
	void SetInquiring(bool fFlag) noexcept { m_fInquiring = fFlag; }
	void SetDataReceived(bool fFlag) noexcept { m_fDataReceived = fFlag; }
	bool IsDataReceived(void) noexcept { const bool f = m_fDataReceived; return f; }

protected:
	queue<CProductWebSourceDataPtr, list<CProductWebSourceDataPtr>> m_qProduct; // Õ¯¬Á≤È—Ø√¸¡Ó∂”¡–
	bool m_fInquiring;
	atomic_bool m_fDataReceived;
};
