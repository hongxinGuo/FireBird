#pragma once
#include"SetTiingoIndustry.h"

using namespace std;
#include<memory>

class CTiingoIndustry : public CObject {
public:
	CTiingoIndustry();

	void Append(CSetTiingoIndustry& setTiingoIndustry);
	void Load(CSetTiingoIndustry& setTiingoIndustry);

	void SetUpdated(bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
	CString m_strIndustry;
	CString m_strSector;

	bool m_fUpdated;
};

typedef shared_ptr<CTiingoIndustry> CTiingoIndustryPtr;
typedef shared_ptr<vector<CTiingoIndustryPtr>> CTiingoIndustryVectorPtr;
