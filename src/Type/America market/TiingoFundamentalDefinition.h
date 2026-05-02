#pragma once

#include"SetTiingoFundamentalDefinition.h"

#include<memory>
#include<vector>

using std::vector;

class CTiingoFundamentalDefinition final {
public:
	CTiingoFundamentalDefinition();
	CTiingoFundamentalDefinition(const CTiingoFundamentalDefinition&) = default;
	CTiingoFundamentalDefinition& operator=(const CTiingoFundamentalDefinition&) = default;
	CTiingoFundamentalDefinition(CTiingoFundamentalDefinition&&) = default;
	CTiingoFundamentalDefinition& operator=(CTiingoFundamentalDefinition&&) = default;
	~CTiingoFundamentalDefinition() = default;

	void Reset();

	void Load(const CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);
	void Append(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);
	void Save(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);

public:
	string m_strDataCode{ "" };
	string m_strName{ "" };
	string m_strDescription{ "" };
	string m_strStatementType{ "" };
	string m_strUnits{ "" };

	// 无需存储数据区
};

using CTiingoFundamentalDefinitionPtr = shared_ptr<CTiingoFundamentalDefinition>;
using CTiingoFundamentalDefinitionsPtr = shared_ptr<vector<CTiingoFundamentalDefinition>>;
