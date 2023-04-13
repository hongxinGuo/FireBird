#pragma once

#include"SetNaicsIndustry.h"

#include<memory>

class CNaicsIndustry final {
public:
	CNaicsIndustry();
	~CNaicsIndustry() = default;

	void Append(CSetNaicsIndustry& setNaicsIndustry) const;

	void Load(const CSetNaicsIndustry& setNaicsIndustry);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated() const noexcept { return m_fUpdated; }

public:
	CString m_strNaics;
	CString m_strNationalIndustry;
	CString m_strSector;
	CString m_strSubSector;

	bool m_fUpdated;
};

using CNaicsIndustryPtr = shared_ptr<CNaicsIndustry>;
