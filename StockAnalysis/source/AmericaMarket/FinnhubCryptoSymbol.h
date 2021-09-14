#pragma once
#include"VirtualStock.h"

#include"SetFinnhubCryptoSymbol.h"
#include"SetCryptoDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CFinnhubCryptoSymbol : public CVirtualStock {
public:
	CFinnhubCryptoSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override { return s_iRatio; }

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

private:
	static int s_iRatio;
};

typedef shared_ptr<CFinnhubCryptoSymbol> CCryptoSymbolPtr;
