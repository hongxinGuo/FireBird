#include"pch.h"

#include"TiingoFinancialState.h"

CTiingoFinancialState::CTiingoFinancialState() {
	Reset();
}

void CTiingoFinancialState::Reset() {
}

void CTiingoFinancialState::Load(const CSetTiingoFinancialState& setTiingoFinancialState) {
}

void CTiingoFinancialState::Append(CSetTiingoFinancialState& setTiingoFinancialState) {
	setTiingoFinancialState.AddNew();
	Save(setTiingoFinancialState);
	setTiingoFinancialState.Update();
}

void CTiingoFinancialState::Save(CSetTiingoFinancialState& setTiingoFinancialState) {
}
