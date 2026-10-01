#include "pch.h"

#include "VirtualContainer.h"

#include "ChinaStockCodeConverter.h"

CVirtualContainer::CVirtualContainer() {
	CVirtualContainer::Reset();
}

void CVirtualContainer::Reset() {
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

size_t CVirtualContainer::GetNextIndex(size_t lIndex) {
	if (++lIndex >= Size()) { lIndex = 0; }
	return lIndex;
}

string CVirtualContainer::GetNextStockInquiringMiddleStr(size_t& iStockIndex, const string& strDelimiter, size_t lTotalNumber) {
	if (IsEmpty()) return XferStandardToSina("600000.SH"); // 当没有证券可查询时，返回一个有效字符串
	string strReturn;
	size_t iCount = 0;
	size_t size = Size();
	while ((iStockIndex < size) && (iCount++ < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
		auto symbol = GetItemSymbol(iStockIndex);
		ABSL_DCHECK(!symbol.empty());
		strReturn += XferStandardToSina(symbol);
		if (iCount < lTotalNumber) { // 如果不是最后一个，则添加后缀
			strReturn += strDelimiter;
		}
		ABSL_DCHECK(strReturn.size() < 10485760);
		iStockIndex = GetNextIndex(iStockIndex);
	}
	return strReturn;
}

string CVirtualContainer::GetNextSinaStockInquiringMiddleStr(const size_t lTotalNumber) {
	return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, ",", lTotalNumber);
}

string CVirtualContainer::GetNextTengxunStockInquiringMiddleStr(const size_t lTotalNumber) {
	return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, ",", lTotalNumber);
}
