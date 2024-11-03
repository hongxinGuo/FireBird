#include "pch.h"

#include "ContainerFinnhubCrypto.h"
#include"SetFinnhubCryptoSymbol.h"
#include"FinnhubCrypto.h"

#include "InfoReport.h"

CContainerFinnhubCrypto::CContainerFinnhubCrypto() {
	CContainerFinnhubCrypto::Reset();
}

CContainerFinnhubCrypto::~CContainerFinnhubCrypto() {
	//for (const auto& pStock : m_vStock) {
	//pStock->SetUpdateProfileDB(true);
	//}
	//UpdateDB();
}

void CContainerFinnhubCrypto::Reset() {
	CContainerVirtualStock::Reset();

	m_llLastTotalSymbol = 0;
}

bool CContainerFinnhubCrypto::LoadDB() {
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	CFinnhubCryptoPtr pSymbol = nullptr;

	setCryptoSymbol.m_strSort = _T("[Symbol]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsSymbol(setCryptoSymbol.m_Symbol)) {
			pSymbol = make_shared<CFinnhubCrypto>();
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
	m_llLastTotalSymbol = m_vStock.size();

	return true;
}

bool CContainerFinnhubCrypto::UpdateDB() {
	try {
		const auto lTotalCryptoSymbol = m_vStock.size();
		CFinnhubCryptoPtr pSymbol;
		CSetFinnhubCryptoSymbol setCryptoSymbol;
		bool fUpdateSymbol = false;

		if (m_llLastTotalSymbol < lTotalCryptoSymbol) {
			setCryptoSymbol.Open();
			setCryptoSymbol.m_pDatabase->BeginTrans();
			for (auto l = m_llLastTotalSymbol; l < lTotalCryptoSymbol; l++) {
				pSymbol = GetSymbol(l);
				pSymbol->AppendSymbol(setCryptoSymbol);
			}
			setCryptoSymbol.m_pDatabase->CommitTrans();
			setCryptoSymbol.Close();
			m_llLastTotalSymbol = lTotalCryptoSymbol;
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
				else {
					setCryptoSymbol.Delete();
				}
				setCryptoSymbol.MoveNext();
			}
			setCryptoSymbol.m_pDatabase->CommitTrans();
			setCryptoSymbol.Close();
		}
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}
