#pragma once

#include"StockSection.h"

class CDataStockSymbol final {
public:
	CDataStockSymbol();
	// ֻ����һ��ʵ��,������ֵ��
	CDataStockSymbol(const CDataStockSymbol&) = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&) = delete;
	CDataStockSymbol(const CDataStockSymbol&&) noexcept = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&&) noexcept = delete;
	~CDataStockSymbol() = default;
	void Reset(void);
	bool CreateTotalStockContainer(void);

	[[nodiscard]] long GetIndex(const CString& strSymbol) const { return m_mapStockSymbol.at(strSymbol); }
	[[nodiscard]] size_t GetStockSize(void) const noexcept { return m_vStockSymbol.size(); }

	[[nodiscard]] bool IsStockSymbol(const CString& strSymbol) const {
		if (m_mapStockSymbol.contains(strSymbol)) return true;
		return false;
	}

	bool Delete(const CString& strSymbol);
	void Add(const CString& strSymbol);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber);
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	void LoadStockSectionDB(void);
	bool UpdateStockSectionDB(void);

	void SetUpdateStockSection(const bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
	[[nodiscard]] bool IsUpdateStockSection(void) const noexcept { return m_fUpdateStockSection; }

	void SetStockSectionActiveFlag(const long lIndex, const bool fFlag) const noexcept { m_vStockSection.at(lIndex)->SetActive(fFlag); }

	[[nodiscard]] bool IsStockSectionActive(const long lIndex) const noexcept { return m_vStockSection.at(lIndex)->IsActive(); }

	void CreateStockSection(const CString& strFirstStockCode);

	[[nodiscard]] bool UpdateStockSection(const CString& strStockCode) const;
	[[nodiscard]] bool UpdateStockSection(long lIndex) const;

	long GetNextIndex(long lIndex);

private:

protected:
	vector<CString> m_vStockSymbol;
	map<CString, size_t> m_mapStockSymbol;
	vector<CString> m_vCurrentSectionStockCode; // ��ǰ��Ʊ���ĵ�һ�����롣�ַ����ĸ�ʽΪ600000.SS��sz000001
	vector<CStockSectionPtr> m_vStockSection; // ��2000�����Ϻ����ڸ�1000��֤ȯ��������λ�Ƿ��Ѿ���ʹ�á�
	bool m_fUpdateStockSection; // ����StockSection��ʶ

private:
	long m_lNextSinaStockInquiringMiddleStrIndex;
	long m_lNeteaseRTDataInquiryIndex;
};

using CDataStockSymbolPtr = shared_ptr<CDataStockSymbol>;
