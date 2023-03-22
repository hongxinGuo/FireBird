#pragma once

#include"VirtualStock.h"

#include"DataCryptoDayLine.h"

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
	virtual void Reset();
	virtual int GetRatio() const override final { return 1000; } // 比例为1000

	virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus();

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData();

public:
	CDataCryptoDayLine m_dataDayLine;

protected:
};

typedef shared_ptr<CFinnhubCryptoSymbol> CFinnhubCryptoSymbolPtr;
typedef shared_ptr<vector<CFinnhubCryptoSymbolPtr>> CFinnhubCryptoSymbolVectorPtr;
