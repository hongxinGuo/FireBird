#include"pch.h"

#include "DataChinaStockSymbol.h"

CDataChinaStockSymbol::CDataChinaStockSymbol(void) {
	Reset();
}

CDataChinaStockSymbol::~CDataChinaStockSymbol() {
}

void CDataChinaStockSymbol::Reset(void) {
	m_vStockSymbol.resize(0);
	m_mapStockSymbol.clear();
}

bool CDataChinaStockSymbol::Delete(CString strSymbol) {
	if (!IsStockSymbol(strSymbol)) return false;

	m_vStockSymbol.erase(m_vStockSymbol.begin() + m_mapStockSymbol.at(strSymbol));
	m_mapStockSymbol.erase(strSymbol);

	return true;
}

void CDataChinaStockSymbol::Add(CString strSymbol) {
	m_mapStockSymbol[strSymbol] = m_vStockSymbol.size();
	m_vStockSymbol.push_back(strSymbol);
}

CString CDataChinaStockSymbol::GetNextSinaStockInquiringMiddleStr(CString strPostfix, long lTotalNumber) {
	CString strReturn = _T("");
	long lEndPosition = m_vStockSymbol.size();
	static long s_lIndex = 0;

	if (0 == lEndPosition) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vStockSymbol.at(s_lIndex));  // �õ���һ����Ʊ����
	IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((s_lIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToSina(m_vStockSymbol.at(s_lIndex));
		IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	}
	if (s_lIndex > 0) s_lIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

long CDataChinaStockSymbol::IncreaseStockInquiringIndex(long& lIndex, long lEndPosition) {
	if (++lIndex >= lEndPosition) {
		lIndex = 0;
	}
	return lIndex;
}

CString CDataChinaStockSymbol::GetNextNeteaseStockInquiringMiddleStr(CString strPostfix, long lTotalNumber) {
	CString strReturn;
	long lEndPosition = m_vStockSymbol.size();
	static long s_lIndex = 0;

	if (0 == lEndPosition) return _T("0600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToNetease(m_vStockSymbol.at(s_lIndex));  // �õ���һ����Ʊ����
	IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((s_lIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToNetease(m_vStockSymbol.at(s_lIndex));  // �õ���һ����Ʊ����
		IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	}
	if (s_lIndex > 0) s_lIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}