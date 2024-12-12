#pragma once

#include<memory>
import FireBird.Set.TiingoIndustry;

class CTiingoIndustry final {
public:
	CTiingoIndustry();
	~CTiingoIndustry() = default;

	void Append(CSetTiingoIndustry& setTiingoIndustry) const;
	void Load(const CSetTiingoIndustry& setTiingoIndustry);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated() const noexcept { return m_fUpdated; }

public:
	CString m_strIndustry;
	CString m_strSector;

	bool m_fUpdated;
};

using CTiingoIndustryPtr = shared_ptr<CTiingoIndustry>;
using CTiingoIndustrysPtr = shared_ptr<vector<CTiingoIndustryPtr>>;
