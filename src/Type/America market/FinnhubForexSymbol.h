#pragma once

#include"VirtualStock.h"
#include"DataForexDayLine.h"

#include<memory>
#include<vector>

class CFinnhubForexSymbol : public CVirtualStock {
public:
	CFinnhubForexSymbol();
	// ≤ª‘ –Ì∏≥÷µ°£
	CFinnhubForexSymbol(const CFinnhubForexSymbol&) = delete;
	CFinnhubForexSymbol& operator=(const CFinnhubForexSymbol&) = delete;
	CFinnhubForexSymbol(const CFinnhubForexSymbol&&) noexcept = delete;
	CFinnhubForexSymbol& operator=(const CFinnhubForexSymbol&&) noexcept = delete;
	void Reset(void) override;
	int GetRatio(void) const final { return 1000; }

	virtual bool UpdateDayLineDB(void) { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus(void);

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine(void) { m_dataDayLine.Unload(); }
	size_t GetDayLineSize(void) const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate(void);
	bool HaveNewDayLineData(void);

public:
	CDataForexDayLine m_dataDayLine;

protected:
};

using CForexSymbolPtr = shared_ptr<CFinnhubForexSymbol>;
using CForexSymbolVectorPtr = shared_ptr<vector<CForexSymbolPtr>>;
