#include "pch.h"

#include "DataFinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"

CDataFinnhubCryptoSymbol::CDataFinnhubCryptoSymbol() {
	Reset();
}

CDataFinnhubCryptoSymbol::~CDataFinnhubCryptoSymbol() {
}

void CDataFinnhubCryptoSymbol::Reset(void) {
	m_vCryptoSymbol.resize(0);
	m_mapCryptoSymbol.clear();
	m_lLastTotalCryptoSymbol = 0;
}

bool CDataFinnhubCryptoSymbol::Delete(CFinnhubCryptoSymbolPtr pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsFinnhubCryptoSymbol(pCryptoSymbol->GetSymbol())) return false;

	m_mapCryptoSymbol.erase(pCryptoSymbol->GetSymbol());
	auto it = find(m_vCryptoSymbol.begin(), m_vCryptoSymbol.end(), pCryptoSymbol);
	m_vCryptoSymbol.erase(it);

	return true;
}

void CDataFinnhubCryptoSymbol::Add(CFinnhubCryptoSymbolPtr pCryptoSymbol) {
	m_mapCryptoSymbol[pCryptoSymbol->GetSymbol()] = m_mapCryptoSymbol.size();
	m_vCryptoSymbol.push_back(pCryptoSymbol);
}

bool CDataFinnhubCryptoSymbol::LoadDB(void) {
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	int i = 0;

	setCryptoSymbol.Open();
	while (!setCryptoSymbol.IsEOF()) {
		pSymbol = make_shared<CFinnhubCryptoSymbol>();
		pSymbol->LoadSymbol(setCryptoSymbol);
		pSymbol->SetCheckingDayLineStatus();
		m_vCryptoSymbol.push_back(pSymbol);
		m_mapCryptoSymbol[pSymbol->GetSymbol()] = i++;
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.Close();
	m_lLastTotalCryptoSymbol = m_vCryptoSymbol.size();

	return true;
}

bool CDataFinnhubCryptoSymbol::UpdateDB(void) {
	const long lTotalCryptoSymbol = m_vCryptoSymbol.size();
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalCryptoSymbol < lTotalCryptoSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalCryptoSymbol; l < lTotalCryptoSymbol; l++) {
			pSymbol = m_vCryptoSymbol.at(l);
			pSymbol->AppendSymbol(setCryptoSymbol);
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
		m_lLastTotalCryptoSymbol = lTotalCryptoSymbol;
	}

	for (auto& pSymbol2 : m_vCryptoSymbol) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			if (m_mapCryptoSymbol.find(setCryptoSymbol.m_Symbol) != m_mapCryptoSymbol.end()) {
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