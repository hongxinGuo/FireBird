#include "pch.h"

#include "ContainerFinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"
#include"FinnhubCryptoSymbol.h"

#include<memory>
using std::make_shared;

CContainerFinnhubCryptoSymbol::CContainerFinnhubCryptoSymbol() { Reset(); }

void CContainerFinnhubCryptoSymbol::Reset(void) {
	m_lLastTotalCryptoSymbol = 0;
}

bool CContainerFinnhubCryptoSymbol::LoadDB(void) {
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	setCryptoSymbol.m_strSort = _T("[Symbol]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsInSymbolMap(setCryptoSymbol.m_Symbol)) {
			pSymbol = make_shared<CFinnhubCryptoSymbol>();
			pSymbol->LoadSymbol(setCryptoSymbol);
			pSymbol->SetCheckingDayLineStatus();
			if (m_mapSymbol.contains(pSymbol->GetSymbol())) { gl_systemMessage.PushErrorMessage(_T("Finnhub Crypto�����ظ����룺") + pSymbol->GetSymbol()); }
			Add(pSymbol);
		}
		else { setCryptoSymbol.Delete(); }
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();
	m_lLastTotalCryptoSymbol = static_cast<long>(m_vStock.size());

	return true;
}

bool CContainerFinnhubCryptoSymbol::UpdateDB(void) {
	const long lTotalCryptoSymbol = static_cast<long>(m_vStock.size());
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalCryptoSymbol < lTotalCryptoSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		for (auto l = m_lLastTotalCryptoSymbol; l < lTotalCryptoSymbol; l++) {
			pSymbol = dynamic_pointer_cast<CFinnhubCryptoSymbol>(m_vStock.at(l));
			pSymbol->AppendSymbol(setCryptoSymbol);
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
		m_lLastTotalCryptoSymbol = lTotalCryptoSymbol;
	}

	for (const auto& pSymbol2 : m_vStock) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			if (m_mapSymbol.contains(setCryptoSymbol.m_Symbol)) {
				pSymbol = dynamic_pointer_cast<CFinnhubCryptoSymbol>(m_vStock.at(m_mapSymbol.at(setCryptoSymbol.m_Symbol)));
				if (pSymbol->IsUpdateProfileDB()) {
					pSymbol->UpdateSymbol(setCryptoSymbol);
					pSymbol->SetUpdateProfileDB(false);
				}
			}
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
	}

	return true;
}