#include"pch.h"
#include"accessory.h"
#include"InsiderTransaction.h"

CInsiderTransaction::CInsiderTransaction() : CObject() {
	m_strSymbol = _T(" ");
	m_strPersonName = _T(" ");
	m_lShare = 0;
	m_lChange = 0;
	m_lFilingDate = 19800101;
	m_lTransactionDate = 19800101;
	m_strTransactionCode = _T(" ");
	m_dTransactionPrice = 0.0;
}

void CInsiderTransaction::Append(CSetInsiderTransaction& setInsiderTransaction) {
	setInsiderTransaction.AddNew();
	Save(setInsiderTransaction);
	setInsiderTransaction.Update();
}

void CInsiderTransaction::Update(CSetInsiderTransaction& setInsiderTransaction) {
	setInsiderTransaction.Edit();
	Save(setInsiderTransaction);
	setInsiderTransaction.Update();
}

void CInsiderTransaction::Save(CSetInsiderTransaction& setInsiderTransaction) {
	setInsiderTransaction.m_Symbol = m_strSymbol;
	setInsiderTransaction.m_PersonName = m_strPersonName.Left(100); // 人名最多100个字符
	setInsiderTransaction.m_Share = ConvertValueToString(m_lShare); // 交易股数有可能超过int的范围，故而使用INT64。
	setInsiderTransaction.m_Change = ConvertValueToString(m_lChange);// 交易股数有可能超过int的范围，故而使用INT64。
	setInsiderTransaction.m_FilingDate = m_lFilingDate;
	setInsiderTransaction.m_TransactionDate = m_lTransactionDate;
	setInsiderTransaction.m_TransactionCode = m_strTransactionCode.Left(4); // 交易代码最多4个字符
	setInsiderTransaction.m_TransactionPrice = m_dTransactionPrice;
}

void CInsiderTransaction::Load(CSetInsiderTransaction& setInsiderTransaction) {
	m_strSymbol = setInsiderTransaction.m_Symbol;
	m_strPersonName = setInsiderTransaction.m_PersonName;
	m_lShare = atoll(setInsiderTransaction.m_Share);
	m_lChange = atoll(setInsiderTransaction.m_Change);
	m_lFilingDate = setInsiderTransaction.m_FilingDate;
	m_lTransactionDate = setInsiderTransaction.m_TransactionDate;
	m_strTransactionCode = setInsiderTransaction.m_TransactionCode;
	m_dTransactionPrice = setInsiderTransaction.m_TransactionPrice;
}