#pragma once

#include"VirtualStock.h"

#include"ContainerCryptoDayLine.h"

#include<memory>
#include<vector>

class CFinnhubCryptoSymbol : public CVirtualStock {
public:
	CFinnhubCryptoSymbol();
	// 不允许赋值。
	CFinnhubCryptoSymbol(const CFinnhubCryptoSymbol&) = delete;
	CFinnhubCryptoSymbol& operator=(const CFinnhubCryptoSymbol&) = delete;
	CFinnhubCryptoSymbol(const CFinnhubCryptoSymbol&&) noexcept = delete;
	CFinnhubCryptoSymbol& operator=(const CFinnhubCryptoSymbol&&) noexcept = delete;
	void Reset() override;
	int GetRatio() const override { return 1000; } // 比例为1000

	virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus();

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;

	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData() const;

public:
	CContainerCryptoDayLine m_dataDayLine;
};

typedef shared_ptr<CFinnhubCryptoSymbol> CFinnhubCryptoSymbolPtr;
typedef shared_ptr<vector<CFinnhubCryptoSymbolPtr>> CFinnhubCryptoSymbolsPtr;
