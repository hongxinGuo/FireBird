#include "pch.h"

#include "VirtualContainer.h"

using namespace std;

CVirtualContainer::CVirtualContainer() {
	CVirtualContainer::Reset();
}

void CVirtualContainer::Reset() {
	m_lNeteaseRTDataInquiringIndex = 0;
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

long CVirtualContainer::GetNextIndex(long lIndex) {
	if (++lIndex >= Size()) { lIndex = 0; }
	return lIndex;
}

CString CVirtualContainer::GetNextStockInquiringMiddleStr(long& iStockIndex, const CString& strDelimiter, long lTotalNumber, CString (*StockCodeTransfer)(const CString& str)) {
	if (IsEmpty()) return StockCodeTransfer(_T("600000.SH")); // 当没有证券可查询时，返回一个有效字符串
	CString strReturn = _T("");
	int iCount = 0;
	while ((iStockIndex < Size()) && (iCount++ < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
		strReturn += StockCodeTransfer(GetItemSymbol(iStockIndex));
		if (iCount < lTotalNumber) { // 如果不是最后一个，则添加后缀
			strReturn += strDelimiter;
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
}
