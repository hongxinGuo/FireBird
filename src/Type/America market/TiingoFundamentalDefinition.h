#pragma once

import FireBird.Set.TiingoFundamentalDefinition;

#include<memory>
#include<vector>

using std::vector;

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
	CString m_strDataCode{ _T("") };
	CString m_strName{ _T("") };
	CString m_strDescription{ _T("") };
	CString m_strStatementType{ _T("") };
	CString m_strUnits{ _T("") };

	// 无需存储数据区
};

using CTiingoFundamentalDefinitionPtr = shared_ptr<CTiingoFundamentalDefinition>;
using CTiingoFundamentalDefinitionsPtr = shared_ptr<vector<CTiingoFundamentalDefinitionPtr>>;
