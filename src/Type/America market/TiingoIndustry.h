#pragma once

#include"SetTiingoIndustry.h"

#include<memory>

class CTiingoIndustry final {
public:
	CTiingoIndustry();
	~CTiingoIndustry() = default;

	void Append(CSetTiingoIndustry& setTiingoIndustry) const;
	void Load(const CSetTiingoIndustry& setTiingoIndustry);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated() const noexcept { return m_fUpdated; }

public:
	string m_strIndustry;
	string m_strSector;

	bool m_fUpdated;
};

using CTiingoIndustryPtr = shared_ptr<CTiingoIndustry>;
using CTiingoIndustriesPtr = shared_ptr<vector<CTiingoIndustryPtr>>;
