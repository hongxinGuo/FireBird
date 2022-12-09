#include "pch.h"

#include "DataFinnhubForexSymbol.h"
#include"SetFinnhubForexSymbol.h"

CDataFinnhubForexSymbol::CDataFinnhubForexSymbol() {
	Reset();
}

void CDataFinnhubForexSymbol::Reset(void) {
	m_vForexSymbol.resize(0);
	m_mapForexSymbol.clear();
	m_lLastTotalForexSymbol = 0;
}

bool CDataFinnhubForexSymbol::Delete(CForexSymbolPtr pForexSymbol) {
	if (pForexSymbol == nullptr) return false;
	if (!IsForexSymbol(pForexSymbol->GetSymbol())) return false;

	m_vForexSymbol.erase(m_vForexSymbol.begin() + m_mapForexSymbol.at(pForexSymbol->GetSymbol()));
	m_mapForexSymbol.erase(pForexSymbol->GetSymbol());

	return true;
}

void CDataFinnhubForexSymbol::Add(CForexSymbolPtr pForexSymbol) {
	m_mapForexSymbol[pForexSymbol->GetSymbol()] = m_mapForexSymbol.size();
	m_vForexSymbol.push_back(pForexSymbol);
}

bool CDataFinnhubForexSymbol::LoadDB(void) {
	CSetFinnhubForexSymbol setForexSymbol;
	int i = 0;

	setForexSymbol.m_strSort = _T("[Symbol]");
	setForexSymbol.Open();
	setForexSymbol.m_pDatabase->BeginTrans();
	while (!setForexSymbol.IsEOF()) {
		if (!IsForexSymbol(setForexSymbol.m_Symbol)) {
			const auto pSymbol = make_shared<CFinnhubForexSymbol>();
			pSymbol->LoadSymbol(setForexSymbol);
			pSymbol->SetCheckingDayLineStatus();
			Add(pSymbol);
		}
		else {
			setForexSymbol.Delete();
		}
		setForexSymbol.MoveNext();
	}
	setForexSymbol.m_pDatabase->CommitTrans();
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

bool CDataFinnhubForexSymbol::IsNeedUpdate(void) {
	for (auto& pForex : m_vForexSymbol) {
		if (pForex->IsUpdateProfileDB()) {
			return true;
		}
	}
	return false;
}
