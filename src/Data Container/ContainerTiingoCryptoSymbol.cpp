#include "pch.h"
#include "ContainerTiingoCryptoSymbol.h"

#include "InfoReport.h"

CContainerTiingoCryptoSymbol::CContainerTiingoCryptoSymbol() {
	Reset();
}

void CContainerTiingoCryptoSymbol::Reset() {
	m_vTiingoCrypto.resize(0);
	m_mapTiingoCrypto.clear();
	m_lLastTotalTiingoCrypto = 0;
}

bool CContainerTiingoCryptoSymbol::Delete(const CTiingoCryptoPtr& pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsSymbol(pCryptoSymbol->GetSymbol())) return false;

	m_vTiingoCrypto.erase(m_vTiingoCrypto.begin() + m_mapTiingoCrypto.at(pCryptoSymbol->GetSymbol()));
	m_mapTiingoCrypto.erase(pCryptoSymbol->GetSymbol());

	return true;
}

void CContainerTiingoCryptoSymbol::Add(const CTiingoCryptoPtr& pCryptoSymbol) {
	ASSERT(!m_mapTiingoCrypto.contains(pCryptoSymbol->GetSymbol()));
	m_mapTiingoCrypto[pCryptoSymbol->GetSymbol()] = m_mapTiingoCrypto.size();
	m_vTiingoCrypto.push_back(pCryptoSymbol);
}

bool CContainerTiingoCryptoSymbol::LoadDB() {
	CSetTiingoCrypto setCryptoSymbol;

	setCryptoSymbol.m_strSort = _T("[Ticker]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsSymbol(setCryptoSymbol.m_Ticker)) {
			const auto pSymbol = make_shared<CTiingoCrypto>();
			pSymbol->Load(setCryptoSymbol);
			Add(pSymbol);
		}
		else { setCryptoSymbol.Delete(); }
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();
	m_lLastTotalTiingoCrypto = m_vTiingoCrypto.size();

	return true;
}

bool CContainerTiingoCryptoSymbol::UpdateDB() {
	const long lTotalTiingoCryptoSymbol = m_vTiingoCrypto.size();

	if (m_lLastTotalTiingoCrypto < lTotalTiingoCryptoSymbol) {
		try {
			CSetTiingoCrypto setCryptoSymbol;
			setCryptoSymbol.Open();
			setCryptoSymbol.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalTiingoCrypto; l < lTotalTiingoCryptoSymbol; l++) {
				const CTiingoCryptoPtr pSymbol = m_vTiingoCrypto.at(l);
				pSymbol->Append(setCryptoSymbol);
			}
			setCryptoSymbol.m_pDatabase->CommitTrans();
			setCryptoSymbol.Close();
			m_lLastTotalTiingoCrypto = lTotalTiingoCryptoSymbol;
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}

	return true;
}
