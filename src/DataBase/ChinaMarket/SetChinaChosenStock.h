// SetChinaChosenStock.h : CSetChinaChosenStock 的声明

#pragma once
#include"VirtualSetChosenStake.h"

class CSetChinaChosenStock : public CVirtualSetChosenStake {
public:
	CSetChinaChosenStock(const CString& strSchema = "ChinaMarket", const CString& strTable = "china_choice_stock", CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
