// CSetWorldChosenForex.h : CSetChinaChosenForex 的声明
#pragma once
#include"VirtualSetChosenStake.h"

class CSetWorldChosenForex : public CVirtualSetChosenStake {
public:
	CSetWorldChosenForex(const CString& strSchema = "stock_market", const CString& strTable = "world_choice_forex", CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
	~CSetWorldChosenForex() override = default;
};
