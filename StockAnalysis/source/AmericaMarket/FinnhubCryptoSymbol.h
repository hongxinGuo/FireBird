#pragma once

#include"VirtualStock.h"
#include"DayLine.h"

#include"DataCryptoDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CFinnhubCryptoSymbol : public CVirtualStock {
public:
	CFinnhubCryptoSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override final { return 1000; } // ±ÈÀýÎª1000

	virtual bool UpdateDayLineDB(void);

	void SetCheckingDayLineStatus(void);

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
	void UnloadDayLine(void) { m_vDayLine.resize(0); }
	size_t GetDayLineSize(void) const noexcept { return m_vDayLine.size(); }
	void UpdateDayLineStartEndDate(void);
	bool HaveNewDayLineData(void);

public:
	vector<CDayLinePtr> m_vDayLine;
	CDataCryptoDayLine m_dataDayLine;

protected:
};

typedef shared_ptr<CFinnhubCryptoSymbol> CFinnhubCryptoSymbolPtr;
typedef shared_ptr<vector<CFinnhubCryptoSymbolPtr>> CFinnhubCryptoSymbolVectorPtr;
