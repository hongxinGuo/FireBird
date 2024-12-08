#pragma once

import FireBird.Accessory.ChinaStockCodeConverter;

class CVirtualContainer {
public:
	CVirtualContainer();
	virtual ~CVirtualContainer() = default;
	virtual void Reset();

	virtual CString GetItemSymbol(size_t lIndex) = 0;
	virtual bool IsEmpty() = 0;
	virtual size_t Size() = 0;
	size_t GetNextIndex(size_t lIndex);

	// todo 以下函数只用于申请Sina、Netease、Tengxun实时数据，为了方便将其放在此基类中。感觉还是改为独立函数为好
	void SetNeteaseRTDataInquiringIndex(const long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	[[nodiscard]] auto GetNeteaseRTDataInquiringIndex() const noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(const long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	[[nodiscard]] auto GetSinaRTDataInquiringIndex() const noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	[[nodiscard]] auto GetTengxunRTDataInquiringIndex() const noexcept { return m_lTengxunRTDataInquiringIndex; }

	// todo 以下函数只用于申请Sina、Netease、Tengxun实时数据，为了方便将其放在此基类中。感觉还是改为独立函数为好
	CString GetNextStockInquiringMiddleStr(size_t& iStockIndex, const CString& strDelimiter, size_t lTotalNumber, CString (*StockCodeTransfer)(const CString& str));
	CString GetNextSinaStockInquiringMiddleStr(const size_t lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToSina); }
	CString GetNextTengxunStockInquiringMiddleStr(const size_t lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToSina); }
	CString GetNextNeteaseStockInquiringMiddleStr(size_t lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lNeteaseRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToNetease); }

private:
	size_t m_lNeteaseRTDataInquiringIndex;
	size_t m_lSinaRTDataInquiringIndex;
	size_t m_lTengxunRTDataInquiringIndex;
};
