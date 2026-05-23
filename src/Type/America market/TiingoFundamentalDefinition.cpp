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

