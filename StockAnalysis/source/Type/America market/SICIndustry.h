#pragma once

#include"SetSICIndustry.h"

#include<memory>

class CSICIndustry final {
public:
	CSICIndustry();
	virtual ~CSICIndustry() = default;

	void Append(CSetSICIndustry& setSICIndustry);
	void Load(CSetSICIndustry& setSICIndustry);

	void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
	long m_lCode;
	CString m_strIndustry;
	CString m_strSector;

	bool m_fUpdated;
};

using CSICIndustryPtr = shared_ptr<CSICIndustry>;