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
	void Reset() override;
	int GetRatio() const final { return 1000; }

	virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus();

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData();

public:
	CDataForexDayLine m_dataDayLine;

protected:
};

using CForexSymbolPtr = shared_ptr<CFinnhubForexSymbol>;
using CForexSymbolVectorPtr = shared_ptr<vector<CForexSymbolPtr>>;
