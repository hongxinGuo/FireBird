#include "pch.h"

#include"containerFinnhubForexSymbol.h"
#include"SetFinnhubForexSymbol.h"
#include"FinnhubForexSymbol.h"

using namespace std;

CContainerFinnhubForexSymbol::CContainerFinnhubForexSymbol() {
	Reset();
}

void CContainerFinnhubForexSymbol::Reset(void) {
	CContainerVirtualStock::Reset();

	m_lLastTotalForexSymbol = 0;
}

bool CContainerFinnhubForexSymbol::LoadDB(void) {
	CSetFinnhubForexSymbol setForexSymbol;
	int i = 0;

	setForexSymbol.m_strSort = _T("[Symbol]");
	setForexSymbol.Open();
	setForexSymbol.m_pDatabase->BeginTrans();
	while (!setForexSymbol.IsEOF()) {
		if (!IsInSymbolMap(setForexSymbol.m_Symbol)) {
			const auto pSymbol = make_shared<CFinnhubForexSymbol>();
			pSymbol->LoadSymbol(setForexSymbol);
			pSymbol->SetCheckingDayLineStatus();
			Add(pSymbol);
		}
		else { setForexSymbol.Delete(); }
		setForexSymbol.MoveNext();
	}
	setForexSymbol.m_pDatabase->CommitTrans();
	setForexSymbol.Close();
	m_lLastTotalForexSymbol = m_vStock.size();

	return true;
}

bool CContainerFinnhubForexSymbol::UpdateDB(void) {
	const long lTotalForexSymbol = m_vStock.size();
	CForexSymbolPtr pSymbol = nullptr;
	CSetFinnhubForexSymbol setForexSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalForexSymbol < lTotalForexSymbol) {
		setForexSymbol.Open();
		setForexSymbol.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalForexSymbol; l < lTotalForexSymbol; l++) {
			pSymbol = dynamic_pointer_cast<CFinnhubForexSymbol>(m_vStock.at(l));
			pSymbol->AppendSymbol(setForexSymbol);
		}
		setForexSymbol.m_pDatabase->CommitTrans();
		setForexSymbol.Close();
		m_lLastTotalForexSymbol = lTotalForexSymbol;
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
				pSymbol = dynamic_pointer_cast<CFinnhubForexSymbol>(m_vStock.at(m_mapSymbol.at(setForexSymbol.m_Symbol)));
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