#pragma once

#include"VirtualStock.h"
#include"DayLine.h"

#include"DataCryptoDayLine.h"
#include"SetCryptoDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CFinnhubCryptoSymbol : public CVirtualStock {
public:
	CFinnhubCryptoSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override final { return 1000; } // ±ÈÀýÎª1000

	virtual bool UpdateDayLineDB(void) { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus(void);

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine(void) { m_dataDayLine.Unload(); }
	size_t GetDayLineSize(void) const noexcept { return m_dataDayLine.GetDataSize(); }
	void UpdateDayLineStartEndDate(void);
	bool HaveNewDayLineData(void);

public:
	CDataCryptoDayLine m_dataDayLine;

protected:
};

typedef shared_ptr<CFinnhubCryptoSymbol> CFinnhubCryptoSymbolPtr;
typedef shared_ptr<vector<CFinnhubCryptoSymbolPtr>> CFinnhubCryptoSymbolVectorPtr;
