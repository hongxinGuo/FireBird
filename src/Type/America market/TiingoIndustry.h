#pragma once

#include"SetTiingoIndustry.h"

#include<memory>

class CTiingoIndustry final {
public:
	CTiingoIndustry();
	virtual ~CTiingoIndustry() = default;

	void Append(CSetTiingoIndustry& setTiingoIndustry);
	void Load(CSetTiingoIndustry& setTiingoIndustry);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
	CString m_strIndustry;
	CString m_strSector;

	bool m_fUpdated;
};

using CTiingoIndustryPtr = shared_ptr<CTiingoIndustry>;
using CTiingoIndustryVectorPtr = shared_ptr<vector<CTiingoIndustryPtr>>;
