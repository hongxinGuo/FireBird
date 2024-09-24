#pragma once

#include"SetTiingoFundamentalDefinition.h"

#include<memory>
#include<vector>

class CTiingoFundamentalDefinition final {
public:
	CTiingoFundamentalDefinition();
	// 不允许复制和赋值。
	CTiingoFundamentalDefinition(const CTiingoFundamentalDefinition&) = delete;
	CTiingoFundamentalDefinition& operator=(const CTiingoFundamentalDefinition&) = delete;
	CTiingoFundamentalDefinition(const CTiingoFundamentalDefinition&&) noexcept = delete;
	CTiingoFundamentalDefinition& operator=(const CTiingoFundamentalDefinition&&) noexcept = delete;
	~CTiingoFundamentalDefinition() = default;

	void Reset();

	void Load(const CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);
	void Append(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);
	void Save(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition);

public:
	CString m_strDataCode;
	CString m_strName;
	CString m_strDescription;
	CString m_strStatementType;
	CString m_strUnit;

	// 无需存储数据区
};

using CTiingoFundamentalDefinitionPtr = shared_ptr<CTiingoFundamentalDefinition>;
using CTiingoFundamentalDefinitionsPtr = shared_ptr<vector<CTiingoFundamentalDefinitionPtr>>;
