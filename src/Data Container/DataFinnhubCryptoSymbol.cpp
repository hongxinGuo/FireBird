#include "pch.h"

#include "DataFinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"

#include<memory>
using std::make_shared;

CDataFinnhubCryptoSymbol::CDataFinnhubCryptoSymbol() { Reset(); }

void CDataFinnhubCryptoSymbol::Reset(void) {
	m_vCryptoSymbol.resize(0);
	m_mapCryptoSymbol.clear();
	m_lLastTotalCryptoSymbol = 0;
}

bool CDataFinnhubCryptoSymbol::Delete(CFinnhubCryptoSymbolPtr pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsFinnhubCryptoSymbol(pCryptoSymbol->GetSymbol())) return false;

	m_vCryptoSymbol.erase(m_vCryptoSymbol.begin() + m_mapCryptoSymbol.at(pCryptoSymbol->GetSymbol()));
	m_mapCryptoSymbol.erase(pCryptoSymbol->GetSymbol());

	return true;
}

void CDataFinnhubCryptoSymbol::Add(CFinnhubCryptoSymbolPtr pCryptoSymbol) {
	m_mapCryptoSymbol[pCryptoSymbol->GetSymbol()] = m_mapCryptoSymbol.size();
	m_vCryptoSymbol.push_back(pCryptoSymbol);
}

bool CDataFinnhubCryptoSymbol::LoadDB(void) {
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	setCryptoSymbol.m_strSort = _T("[Symbol]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsFinnhubCryptoSymbol(setCryptoSymbol.m_Symbol)) {
			pSymbol = make_shared<CFinnhubCryptoSymbol>();
			pSymbol->LoadSymbol(setCryptoSymbol);
			pSymbol->SetCheckingDayLineStatus();
			if (m_mapCryptoSymbol.contains(pSymbol->GetSymbol())) { gl_systemMessage.PushErrorMessage(_T("Finnhub Crypto发现重复代码：") + pSymbol->GetSymbol()); }
			Add(pSymbol);
		}
		else { setCryptoSymbol.Delete(); }
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();
	m_lLastTotalCryptoSymbol = static_cast<long>(m_vCryptoSymbol.size());

	return true;
}

bool CDataFinnhubCryptoSymbol::UpdateDB(void) {
	const long lTotalCryptoSymbol = static_cast<long>(m_vCryptoSymbol.size());
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalCryptoSymbol < lTotalCryptoSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		for (auto l = m_lLastTotalCryptoSymbol; l < lTotalCryptoSymbol; l++) {
			pSymbol = m_vCryptoSymbol.at(l);
			pSymbol->AppendSymbol(setCryptoSymbol);
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
		m_lLastTotalCryptoSymbol = lTotalCryptoSymbol;
	}

	for (const auto& pSymbol2 : m_vCryptoSymbol) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			if (m_mapCryptoSymbol.contains(setCryptoSymbol.m_Symbol)) {
				pSymbol = m_vCryptoSymbol.at(m_mapCryptoSymbol.at(setCryptoSymbol.m_Symbol));
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

bool CDataFinnhubCryptoSymbol::IsNeedUpdate(void) {
	for (const auto& pCrypto : m_vCryptoSymbol) { if (pCrypto->IsUpdateProfileDB()) { return true; } }
	return false;
}
