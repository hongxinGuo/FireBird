#include"pch.h"

#include"SystemMessage.h"

#include"TiingoFundamentalDefinition.h"

CTiingoFundamentalDefinition::CTiingoFundamentalDefinition() {
	Reset();
}

void CTiingoFundamentalDefinition::Reset() {
	m_strDataCode = _T("");
	m_strName = _T("");
	m_strDescription = _T("");
	m_strStatementType = _T("");
	m_strUnits = _T("");
}

void CTiingoFundamentalDefinition::Load(const CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	m_strDataCode = setTiingoFundamentalDefinition.m_dataCode;
	m_strName = setTiingoFundamentalDefinition.m_name;
	m_strDescription = setTiingoFundamentalDefinition.m_description;
	m_strStatementType = setTiingoFundamentalDefinition.m_statementType;
	m_strUnits = setTiingoFundamentalDefinition.m_units;
}

void CTiingoFundamentalDefinition::Append(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	setTiingoFundamentalDefinition.AddNew();
	Save(setTiingoFundamentalDefinition);
	setTiingoFundamentalDefinition.Update();
}

void CTiingoFundamentalDefinition::Save(CSetTiingoFundamentalDefinition& setTiingoFundamentalDefinition) {
	if ((m_strDescription.GetLength() > 600)
		|| (m_strDataCode.GetLength() > 45)
		|| (m_strName.GetLength() > 100)
		|| (m_strStatementType.GetLength() > 45)
		|| (m_strUnits.GetLength() > 45)) {
		CString s = _T("Tiingo fundamental definitions ") + m_strDataCode + _T(" ×Ö·û´®Ì«³¤");
		gl_systemMessage.PushErrorMessage(s);
	}
	m_strDataCode = m_strDataCode.Left(44);
	m_strName = m_strName.Left(98);
	m_strDescription = m_strDescription.Left(598);
	m_strStatementType = m_strStatementType.Left(44);
	m_strUnits = m_strUnits.Left(44);

	setTiingoFundamentalDefinition.m_dataCode = m_strDataCode;
	setTiingoFundamentalDefinition.m_name = m_strName;
	setTiingoFundamentalDefinition.m_description = m_strDescription;
	setTiingoFundamentalDefinition.m_statementType = m_strStatementType;
	setTiingoFundamentalDefinition.m_units = m_strUnits;
}
