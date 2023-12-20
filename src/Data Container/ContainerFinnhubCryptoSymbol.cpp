#include "pch.h"

#include "ContainerFinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"
#include"FinnhubCryptoSymbol.h"

#include<memory>

#include "InfoReport.h"
using std::make_shared;

CContainerFinnhubCryptoSymbol::CContainerFinnhubCryptoSymbol() {
	CContainerFinnhubCryptoSymbol::Reset();
}

void CContainerFinnhubCryptoSymbol::Reset() {
	CContainerVirtualStock::Reset();

	m_lLastTotalSymbol = 0;
}

bool CContainerFinnhubCryptoSymbol::LoadDB() {
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	setCryptoSymbol.m_strSort = _T("[Symbol]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsSymbol(setCryptoSymbol.m_Symbol)) {
			pSymbol = make_shared<CFinnhubCryptoSymbol>();
			pSymbol->LoadSymbol(setCryptoSymbol);
			pSymbol->SetCheckingDayLineStatus();
			if (m_mapSymbol.contains(pSymbol->GetSymbol())) { gl_systemMessage.PushErrorMessage(_T("Finnhub Crypto发现重复代码：") + pSymbol->GetSymbol()); }
			Add(pSymbol);
		}
		else {
			setCryptoSymbol.Delete();
		}
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();
	m_lLastTotalSymbol = static_cast<long>(m_vStock.size());

	return true;
}

bool CContainerFinnhubCryptoSymbol::UpdateDB() {
	try {
		const long lTotalCryptoSymbol = static_cast<long>(m_vStock.size());
		CFinnhubCryptoSymbolPtr pSymbol;
		CSetFinnhubCryptoSymbol setCryptoSymbol;
		bool fUpdateSymbol = false;

		if (m_lLastTotalSymbol < lTotalCryptoSymbol) {
			setCryptoSymbol.Open();
			setCryptoSymbol.m_pDatabase->BeginTrans();
			for (auto l = m_lLastTotalSymbol; l < lTotalCryptoSymbol; l++) {
				pSymbol = GetSymbol(l);
				pSymbol->AppendSymbol(setCryptoSymbol);
			}
			setCryptoSymbol.m_pDatabase->CommitTrans();
			setCryptoSymbol.Close();
			m_lLastTotalSymbol = lTotalCryptoSymbol;
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
					pSymbol = GetSymbol(setCryptoSymbol.m_Symbol);
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
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}
