#pragma once

import FireBird.Stock.Virtual;
#include"ContainerForexDayLine.h"
#include"nlohmannjsonHeader.h"

#include<memory>
#include<vector>

class CFinnhubForex : public CVirtualStock {
public:
	CFinnhubForex();
	// ≤ª‘ –Ì∏≥÷µ°£
	CFinnhubForex(const CFinnhubForex&) = delete;
	CFinnhubForex& operator=(const CFinnhubForex&) = delete;
	CFinnhubForex(const CFinnhubForex&&) noexcept = delete;
	CFinnhubForex& operator=(const CFinnhubForex&&) noexcept = delete;

	int GetRatio() const final { return 1000; }

	virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus();

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime);

	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData();

	CContainerForexDayLine m_dataDayLine;
};

using CForexSymbolPtr = shared_ptr<CFinnhubForex>;
using CForexSymbolsPtr = shared_ptr<vector<CForexSymbolPtr>>;
