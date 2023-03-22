#include "pch.h"

#include"containerFinnhubForexSymbol.h"
#include"SetFinnhubForexSymbol.h"
#include"FinnhubForexSymbol.h"

using namespace std;

CContainerFinnhubForexSymbol::CContainerFinnhubForexSymbol() {
	CContainerFinnhubForexSymbol::Reset();
}

void CContainerFinnhubForexSymbol::Reset() {
	CContainerVirtualStock::Reset();

	m_lastTotalSymbol = 0;
}

bool CContainerFinnhubForexSymbol::LoadDB() {
	CSetFinnhubForexSymbol setForexSymbol;

	setForexSymbol.m_strSort = _T("[Symbol]");
	setForexSymbol.Open();
	setForexSymbol.m_pDatabase->BeginTrans();
	while (!setForexSymbol.IsEOF()) {
		if (!IsSymbol(setForexSymbol.m_Symbol)) {
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
	m_lastTotalSymbol = m_vStock.size();

	return true;
}

bool CContainerFinnhubForexSymbol::UpdateDB() {
	const auto lTotalForexSymbol = m_vStock.size();
	CForexSymbolPtr pSymbol;
	CSetFinnhubForexSymbol setForexSymbol;
	bool fUpdateSymbol = false;

	if (m_lastTotalSymbol < lTotalForexSymbol) {
		setForexSymbol.Open();
		setForexSymbol.m_pDatabase->BeginTrans();
		for (auto l = m_lastTotalSymbol; l < lTotalForexSymbol; l++) {
			pSymbol = GetSymbol(l);
			pSymbol->AppendSymbol(setForexSymbol);
		}
		setForexSymbol.m_pDatabase->CommitTrans();
		setForexSymbol.Close();
		m_lastTotalSymbol = lTotalForexSymbol;
	}

	for (const auto& pSymbol2 : m_vStock) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setForexSymbol.Open();
		setForexSymbol.m_pDatabase->BeginTrans();
		while (!setForexSymbol.IsEOF()) {
			if (m_mapSymbol.contains(setForexSymbol.m_Symbol)) {
				pSymbol = GetSymbol(setForexSymbol.m_Symbol);
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
