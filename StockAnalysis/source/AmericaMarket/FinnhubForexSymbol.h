#pragma once
#include"VirtualStock.h"

#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CProductFinnhubForexSymbol : public CVirtualStock {
public:
	CProductFinnhubForexSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override final { return 1000; }

	virtual bool UpdateDayLineDB(void);
	bool SaveDayLine(void);

	void SetCheckingDayLineStatus(void);

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
	void UnloadDayLine(void) { m_vDayLine.resize(0); }
	size_t GetDayLineSize(void) const noexcept { return m_vDayLine.size(); }
	void UpdateDayLineStartEndDate(void);
	bool HaveNewDayLineData(void);

public:
	vector<CDayLinePtr> m_vDayLine;

protected:
};

typedef shared_ptr<CProductFinnhubForexSymbol> CForexSymbolPtr;
typedef shared_ptr<vector<CForexSymbolPtr>> CForexSymbolVectorPtr;
