#include"pch.h"
module;
#include"nlohmannjsonHeader.h"

#include"SetTiingoCrypto.h"

#include<memory>
#include<vector>
export module FireBird.Stock.TiingoCrypto;

import FireBird.Stock.Virtual;

export {
	class CTiingoCrypto : public CVirtualStock {
	public:
		CTiingoCrypto();
		~CTiingoCrypto() override = default;
		int GetRatio() const override { return 3; };

		void Load(CSetTiingoCrypto& setTiingoCrypto);
		void Append(CSetTiingoCrypto& setTiingoCrypto);
		void Save(CSetTiingoCrypto& setTiingoCrypto);
		void Update(CSetTiingoCrypto& setTiingoCrypto);

		//CString m_strTicker; //由VirtualStock中的m_strSymbol代替
		CString m_strName{ _T("") };
		//CString m_strDescription; // 已废弃。由CVirtualStock中的m_strDescription代替
		CString m_strBaseCurrency{ _T("") };
		CString m_strQuoteCurrency{ _T("") };
	};

	using CTiingoCryptoPtr = shared_ptr<CTiingoCrypto>;
	using CTiingoCryptosPtr = shared_ptr<vector<CTiingoCryptoPtr>>;
}