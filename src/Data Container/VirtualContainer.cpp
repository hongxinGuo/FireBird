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
	if (IsEmpty()) return StockCodeTransfer(_T("600000.SH")); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	string strReturn = _T("");
	size_t iCount = 0;
	while ((iStockIndex < Size()) && (iCount++ < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		strReturn += StockCodeTransfer(GetItemSymbol(iStockIndex));
		if (iCount < lTotalNumber) { // ����������һ��������Ӻ�׺
			strReturn += strDelimiter;
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
}
