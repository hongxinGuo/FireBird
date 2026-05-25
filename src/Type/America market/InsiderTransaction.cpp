#include"pch.h"

#include"InsiderTransaction.h"

CInsiderTransaction::CInsiderTransaction() {
	m_strSymbol = " ";
	m_strPersonName = " ";
	m_lShare = 0;
	m_lShareChange = 0;
	m_lFilingDate = 19800101;
	m_lTransactionDate = 19800101;
	m_strTransactionCode = "";
	m_dTransactionPrice = 0.0;
}
