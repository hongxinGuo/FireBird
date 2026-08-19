#pragma once

#include"VirtualStock.h"

class CAlpacaStock : public CVirtualStock {
public:
	CAlpacaStock();
	// ≤ª‘ –Ì∏≥÷µ°£
	CAlpacaStock(const CAlpacaStock&) = default;
	CAlpacaStock& operator=(const CAlpacaStock&) = delete;
	CAlpacaStock(const CAlpacaStock&&) noexcept = delete;
	CAlpacaStock& operator=(const CAlpacaStock&&) noexcept = delete;
	~CAlpacaStock() override = default;

	int GetRatio() const final { return 1000000; }

protected:
	string m_id{ " " };
	string m_class{ " " };
	//string m_exchanged{ " " };
	//string m_symbol{ " " };
	//string m_name{ " " };
	bool m_active{ true };
	bool m_tradable{ true };
	bool m_marginable{ true };
	int m_maintenance_margin_requirement{ 100 };
	int m_margin_requirement_long{ 100 };
	int m_margin_requirement_short{ 100 };
	bool m_shortable{ true };
	bool m_easy_to_borrow{ true };
	string m_borrow_status{ "hard_to_borrow" };
};

using CAlpacaStockPtr = shared_ptr<CAlpacaStock>;
using CAlpacaStocksPtr = shared_ptr<vector<CAlpacaStockPtr>>;
