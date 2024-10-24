#pragma once

#include"VirtualStock.h"
#include"ContainerForexDayLine.h"

#include<memory>
#include<vector>

class CFinnhubForexSymbol : public CVirtualStock {
public:
	CFinnhubForexSymbol();
	// ��������ֵ��
	CFinnhubForexSymbol(const CFinnhubForexSymbol&) = delete;
	CFinnhubForexSymbol& operator=(const CFinnhubForexSymbol&) = delete;
	CFinnhubForexSymbol(const CFinnhubForexSymbol&&) noexcept = delete;
	CFinnhubForexSymbol& operator=(const CFinnhubForexSymbol&&) noexcept = delete;
	void Reset() override;
	int GetRatio() const final { return 1000; }

	virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus();

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;

	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData() const;

	CContainerForexDayLine m_dataDayLine;
};

using CForexSymbolPtr = shared_ptr<CFinnhubForexSymbol>;
using CForexSymbolsPtr = shared_ptr<vector<CForexSymbolPtr>>;
