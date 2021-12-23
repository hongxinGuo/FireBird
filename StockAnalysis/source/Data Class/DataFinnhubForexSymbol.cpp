#include "pch.h"

#include "DataFinnhubForexSymbol.h"
#include"SetFinnhubForexSymbol.h"

CDataFinnhubForexSymbol::CDataFinnhubForexSymbol() {
	Reset();
}

CDataFinnhubForexSymbol::~CDataFinnhubForexSymbol() {
}

void CDataFinnhubForexSymbol::Reset(void) {
	m_vForexSymbol.resize(0);
	m_mapForexSymbol.clear();
	m_lLastTotalForexSymbol = 0;
}

bool CDataFinnhubForexSymbol::Delete(CForexSymbolPtr pForexSymbol) {
	if (pForexSymbol == nullptr) return false;
	if (!IsForexSymbol(pForexSymbol->GetSymbol())) return false;

	m_mapForexSymbol.erase(pForexSymbol->GetSymbol());
	auto it = find(m_vForexSymbol.begin(), m_vForexSymbol.end(), pForexSymbol);
	m_vForexSymbol.erase(it);

	return true;
}

void CDataFinnhubForexSymbol::Add(CForexSymbolPtr pForexSymbol) {
	m_mapForexSymbol[pForexSymbol->GetSymbol()] = m_mapForexSymbol.size();
	m_vForexSymbol.push_back(pForexSymbol);
}

bool CDataFinnhubForexSymbol::LoadDB(void) {
	CSetFinnhubForexSymbol setForexSymbol;
	CForexSymbolPtr pSymbol = nullptr;
	int i = 0;

	setForexSymbol.Open();
	while (!setForexSymbol.IsEOF()) {
		pSymbol = make_shared<CProductFinnhubForexSymbol>();
		pSymbol->LoadSymbol(setForexSymbol);
		pSymbol->SetCheckingDayLineStatus();
		m_vForexSymbol.push_back(pSymbol);
		m_mapForexSymbol[pSymbol->GetSymbol()] = i++;
		setForexSymbol.MoveNext();
	}
	setForexSymbol.Close();
	m_lLastTotalForexSymbol = m_vForexSymbol.size();

	return true;
}

bool CDataFinnhubForexSymbol::UpdateDB(void) {
	const long lTotalForexSymbol = m_vForexSymbol.size();
	CForexSymbolPtr pSymbol = nullptr;
	CSetFinnhubForexSymbol setForexSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalForexSymbol < lTotalForexSymbol) {
		setForexSymbol.Open();
		setForexSymbol.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalForexSymbol; l < lTotalForexSymbol; l++) {
			pSymbol = m_vForexSymbol.at(l);
			pSymbol->AppendSymbol(setForexSymbol);
		}
		setForexSymbol.m_pDatabase->CommitTrans();
		setForexSymbol.Close();
		m_lLastTotalForexSymbol = lTotalForexSymbol;
	}

	for (auto& pSymbol2 : m_vForexSymbol) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setForexSymbol.Open();
		setForexSymbol.m_pDatabase->BeginTrans();
		while (!setForexSymbol.IsEOF()) {
			if (m_mapForexSymbol.find(setForexSymbol.m_Symbol) != m_mapForexSymbol.end()) {
				pSymbol = m_vForexSymbol.at(m_mapForexSymbol.at(setForexSymbol.m_Symbol));
				if (pSymbol->IsUpdateProfileDB()) {
					pSymbol->UpdateSymbol(setForexSymbol);
					pSymbol->SetUpdateProfileDB(false);
				}
			}
			setForexSymbol.MoveNext();
		}
		setForexSymbol.m_pDatabase->CommitTrans();
		setForexSymbol.Close();
	}

	return true;
}