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
	if (IsEmpty()) return StockCodeTransfer(_T("600000.SH")); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	CString strReturn = _T("");
	int iCount = 0;
	while ((iStockIndex < Size()) && (iCount++ < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		strReturn += StockCodeTransfer(GetItemSymbol(iStockIndex));
		if (iCount < lTotalNumber) { // ����������һ��������Ӻ�׺
			strReturn += strDelimiter;
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
}
