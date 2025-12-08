#include "pch.h"
#include "ContainerTiingoCryptoSymbol.h"

#include "CharSetTransfer.h"
#include "InfoReport.h"

CContainerTiingoCryptoSymbol::CContainerTiingoCryptoSymbol() {
	CContainerTiingoCryptoSymbol::Reset();
}

CContainerTiingoCryptoSymbol::~CContainerTiingoCryptoSymbol() {
	//for (const auto& pStock : m_vStock) {
	//pStock->SetUpdateProfileDB(true);
	//}
	//UpdateDB();
}

void CContainerTiingoCryptoSymbol::Reset() {
	CContainerVirtualStock::Reset();
}

bool CContainerTiingoCryptoSymbol::LoadDB() {
	CSetTiingoCrypto setCryptoSymbol;

	setCryptoSymbol.m_strSort = "[Ticker]";
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsSymbol(T2Utf8(setCryptoSymbol.m_Ticker))) {
			const auto pSymbol = make_shared<CTiingoCrypto>();
			pSymbol->Load(setCryptoSymbol);
			Add(pSymbol);
		}
		else {
			setCryptoSymbol.Delete();
		}
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();

	return true;
}

void CContainerTiingoCryptoSymbol::UpdateDB() {
	if (IsUpdateProfileDB()) {
		try {
			CSetTiingoCrypto setWorldCrypto;
			setWorldCrypto.m_strSort = "[Ticker]";
			setWorldCrypto.Open();
			setWorldCrypto.m_pDatabase->BeginTrans();
			while (!setWorldCrypto.IsEOF()) {	//更新原有的代码集状态
				if (IsSymbol(T2Utf8(setWorldCrypto.m_Ticker))) {
					const CTiingoCryptoPtr pCrypto = GetCrypto(T2Utf8(setWorldCrypto.m_Ticker));
					ASSERT(pCrypto != nullptr);
					if (pCrypto->IsUpdateProfileDB()) {
						pCrypto->Update(setWorldCrypto);
						pCrypto->SetUpdateProfileDB(false);
					}
				}
				else {
					setWorldCrypto.Delete();
				}
				setWorldCrypto.MoveNext();
			}
			for (size_t l = 0; l < m_vStock.size(); l++) {
				const CTiingoCryptoPtr pCrypto = GetCrypto(l);
				ASSERT(pCrypto != nullptr);
				if (pCrypto->IsUpdateProfileDB()) {
					pCrypto->Append(setWorldCrypto);
					pCrypto->SetUpdateProfileDB(false);
				}
			}
			setWorldCrypto.m_pDatabase->CommitTrans();
			setWorldCrypto.Close();
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
}
