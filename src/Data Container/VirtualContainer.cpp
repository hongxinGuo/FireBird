#include "pch.h"

#include "VirtualContainer.h"

CVirtualContainer::CVirtualContainer() {
	CVirtualContainer::Reset();
}

void CVirtualContainer::Reset() {
	m_lNeteaseRTDataInquiringIndex = 0;
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

size_t CVirtualContainer::GetNextIndex(size_t lIndex) {
	if (++lIndex >= Size()) { lIndex = 0; }
	return lIndex;
}

string CVirtualContainer::GetNextStockInquiringMiddleStr(size_t& iStockIndex, const string& strDelimiter, size_t lTotalNumber, string (*StockCodeTransfer)(const string& str)) {
	if (IsEmpty()) return StockCodeTransfer(_T("600000.SH")); // 当没有证券可查询时，返回一个有效字符串
	string strReturn = _T("");
	size_t iCount = 0;
	while ((iStockIndex < Size()) && (iCount++ < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
		strReturn += StockCodeTransfer(GetItemSymbol(iStockIndex));
		if (iCount < lTotalNumber) { // 如果不是最后一个，则添加后缀
			strReturn += strDelimiter;
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
}
