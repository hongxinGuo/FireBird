#pragma once
#include"VirtualStock.h"

#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace std;
#include<memory>
#include<vector>

class CFinnhubForexSymbol : public CVirtualStock {
public:
	CFinnhubForexSymbol();
	virtual void Reset(void);
	virtual int GetRatio(void) const override { return s_iRatio; }

	void Load(CSetFinnhubForexSymbol& setForexSymbol);
	void Append(CSetFinnhubForexSymbol& setForexSymbol);
	void Update(CSetFinnhubForexSymbol& setForexSymbol);
	void Save(CSetFinnhubForexSymbol& setForexSymbol);

	virtual bool UpdateDayLineDB(void);
	bool SaveDayLine(void);

	CString GetDescription(void) const { return m_strDescription; }
	void SetDescription(CString strDescription) { m_strDescription = strDescription; }
	CString GetDisplaySymbol(void) const { return m_strDisplaySymbol; }
	void SetDisplaySymbol(CString strSymbol) { m_strDisplaySymbol = strSymbol; }

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
	CString m_strDescription;
	CString m_strDisplaySymbol;

private:
	static int s_iRatio;
};

typedef shared_ptr<CFinnhubForexSymbol> CForexSymbolPtr;
