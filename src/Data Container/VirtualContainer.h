#pragma once

#include "ChinaStockCodeConverter.h"

class CVirtualContainer {
public:
	CVirtualContainer();
	virtual ~CVirtualContainer() = default;
	virtual void Reset();

	virtual CString GetItemSymbol(long lIndex) = 0;
	virtual bool IsEmpty() = 0;
	virtual size_t Size() = 0;
	long GetNextIndex(long lIndex);

	// todo 以下函数只用于申请Sina、Netease、Tengxun实时数据，为了方便将其放在此基类中。感觉还是改为独立函数为好
	void SetNeteaseRTDataInquiringIndex(const long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetNeteaseRTDataInquiringIndex() const noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(const long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetSinaRTDataInquiringIndex() const noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetTengxunRTDataInquiringIndex() const noexcept { return m_lTengxunRTDataInquiringIndex; }

	// todo 以下函数只用于申请Sina、Netease、Tengxun实时数据，为了方便将其放在此基类中。感觉还是改为独立函数为好
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, const CString& strDelimiter, long lTotalNumber, CString (*StockCodeTransfer)(const CString& str));
	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToSina); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToSina); }
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lNeteaseRTDataInquiringIndex, _T(","), lTotalNumber, XferStandardToNetease); }

private:
	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
