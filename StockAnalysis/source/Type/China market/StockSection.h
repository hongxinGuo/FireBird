#pragma once

#include<memory>

class CStockSection {
public:
	CStockSection();
	virtual ~CStockSection(void) = default;

	bool IsActive(void) const noexcept { return m_fActive; }
	void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }
	long GetIndexNumber(void) const noexcept { return m_lIndexNumber; }
	void SetIndexNumber(const long lNumber) noexcept { m_lIndexNumber = lNumber; }
	WORD GetMarket(void) const noexcept { return m_wMarket; }
	void SetMarket(const WORD wMarket) noexcept { m_wMarket = wMarket; }
	CString GetComment(void) const { return m_strComment; }
	void SetComment(const CString strComment) { m_strComment = strComment; }
	bool IsBuildStockPtr(void) const noexcept { return m_fBuildStockPtr; }
	void SetBuildStockPtr(const bool fFlag) noexcept { m_fBuildStockPtr = fFlag; }

protected:
	// ��洢����
	bool m_fActive; // ���δ����Ƿ��Ѿ�ʹ��
	long m_lIndexNumber; // ���δ���3����λ������
	WORD m_wMarket; // �г�
	CString m_strComment; // ע��

	// ����洢����
	bool m_fBuildStockPtr; // ����֤ȯ�Ƿ��ѽ���֤ȯָ�루��m_vChinaMarketStock�У���
};

using CStockSectionPtr = shared_ptr<CStockSection>;
