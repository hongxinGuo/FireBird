#pragma once

#include"StockSection.h"
#include"ChinaStock.h"
#include"SetStockSection.h"

using namespace std;
#include<vector>
#include<map>

class CDataStockSection : public CObject {
public:
	CDataStockSection();
	~CDataStockSection();
	void Reset(void);

	void LoadDB(void);
	void UpdateDB(void);

protected:
	vector<CString> m_vCurrentSectionStockCode; // 当前股票集的第一个代码。字符串的格式为600000.SS、sz000001
	vector<CStockSectionPtr> m_vStockSection; // 共2000个，上海深圳各1000，证券代码上三位是否已经被使用。
	bool m_fUpdateStockSection; // 更新StockSection标识
};
