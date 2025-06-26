#pragma once

#include"SetTiingoFundamentalDefinition.h"

#include<memory>
#include<vector>

using std::vector;

class CTiingoFundamentalDefinition final {
public:
	CTiingoFundamentalDefinition();
	// �������ƺ͸�ֵ��
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
	string m_strDataCode{ _T("") };
	string m_strName{ _T("") };
	string m_strDescription{ _T("") };
	string m_strStatementType{ _T("") };
	string m_strUnits{ _T("") };

	// ����洢������
};

using CTiingoFundamentalDefinitionPtr = shared_ptr<CTiingoFundamentalDefinition>;
using CTiingoFundamentalDefinitionsPtr = shared_ptr<vector<CTiingoFundamentalDefinitionPtr>>;
