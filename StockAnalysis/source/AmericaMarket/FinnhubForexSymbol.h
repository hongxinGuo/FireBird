#pragma once

#include"VirtualStock.h"
#include"DayLine.h"
#include"DataForexDayLine.h"
#include"SetForexDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CFinnhubForexSymbol : public CVirtualStock {
public:
	CFinnhubForexSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override final { return 1000; }

	virtual bool UpdateDayLineDB(void) { return m_dataDayLine.SaveDB(m_strSymbol); }

	void SetCheckingDayLineStatus(void);

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine(void) { m_dataDayLine.Unload(); }
	size_t GetDayLineSize(void) const noexcept { return m_dataDayLine.GetDataSize(); }
	void UpdateDayLineStartEndDate(void);
	bool HaveNewDayLineData(void);

public:
	CDataForexDayLine m_dataDayLine;

protected:
};

typedef shared_ptr<CFinnhubForexSymbol> CForexSymbolPtr;
typedef shared_ptr<vector<CForexSymbolPtr>> CForexSymbolVectorPtr;
