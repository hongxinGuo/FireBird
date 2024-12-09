#include "pch.h"

module FireBird.Container.Virtual;

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

CString CVirtualContainer::GetNextStockInquiringMiddleStr(size_t& iStockIndex, const CString& strDelimiter, size_t lTotalNumber, CString (*StockCodeTransfer)(const CString& str)) {
	if (IsEmpty()) return StockCodeTransfer(_T("600000.SH")); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	CString strReturn = _T("");
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
