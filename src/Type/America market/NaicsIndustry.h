#pragma once

class CNaicsIndustry final {
public:
	CNaicsIndustry();
	~CNaicsIndustry() = default;

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated() const noexcept { return m_fUpdated; }

public:
	string m_strNaics;
	string m_strNationalIndustry;
	string m_strSector;
	string m_strSubSector;

	bool m_fUpdated;
};

using CNaicsIndustryPtr = shared_ptr<CNaicsIndustry>;
