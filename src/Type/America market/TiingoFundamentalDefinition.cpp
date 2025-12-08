#include"pch.h"

#include"TiingoFundamentalDefinition.h"

#include "CharSetTransfer.h"

CTiingoFundamentalDefinition::CTiingoFundamentalDefinition() {
	Reset();
}

void CTiingoFundamentalDefinition::Reset() {
	m_strDataCode = "";
	m_strName = "";
	m_strDescription = "";
	m_strStatementType = "";
	m_strUnits = "";
}

void CTiingoFundamentalDefinition::Load(const CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	m_strDataCode = T2Utf8(setTiingoFundamentalDefinition.m_dataCode);
	m_strName = T2Utf8(setTiingoFundamentalDefinition.m_name);
	m_strDescription = T2Utf8(setTiingoFundamentalDefinition.m_description);
	m_strStatementType = T2Utf8(setTiingoFundamentalDefinition.m_statementType);
	m_strUnits = T2Utf8(setTiingoFundamentalDefinition.m_units);
}

void CTiingoFundamentalDefinition::Append(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	setTiingoFundamentalDefinition.AddNew();
	Save(setTiingoFundamentalDefinition);
	setTiingoFundamentalDefinition.Update();
}

void CTiingoFundamentalDefinition::Save(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	if ((m_strDescription.length() > 600)
		|| (m_strDataCode.length() > 45)
		|| (m_strName.length() > 100)
		|| (m_strStatementType.length() > 45)
		|| (m_strUnits.length() > 45)) {
		string s = "Tiingo fundamental definitions ";
		s += m_strDataCode;
		s += " 字符串太长";
		gl_systemMessage.PushErrorMessage(s);
	}
	m_strDataCode = m_strDataCode.substr(0, 44);
	m_strName = m_strName.substr(0, 98);
	m_strDescription = m_strDescription.substr(0, 598);
	m_strStatementType = m_strStatementType.substr(0, 44);
	m_strUnits = m_strUnits.substr(0, 44);

	setTiingoFundamentalDefinition.m_dataCode = m_strDataCode.c_str();
	setTiingoFundamentalDefinition.m_name = m_strName.c_str();
	setTiingoFundamentalDefinition.m_description = m_strDescription.c_str();
	setTiingoFundamentalDefinition.m_statementType = m_strStatementType.c_str();
	setTiingoFundamentalDefinition.m_units = m_strUnits.c_str();
}
