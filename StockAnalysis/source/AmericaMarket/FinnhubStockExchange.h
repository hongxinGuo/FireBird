#pragma once
#include"SetFinnhubStockExchange.h"

using namespace std;
#include<memory>

class CFinnhubStockExchange : public CObject {
public:
	CFinnhubStockExchange();

	void Append(CSetFinnhubStockExchange& setFinnhubExchange);

	void Load(CSetFinnhubStockExchange& setFinnhubExchange);

	void SetUpdated(bool fFlag) noexcept { m_fUpdated = fFlag; }
	bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	bool m_fUpdated;
};

typedef shared_ptr<CFinnhubStockExchange> CFinnhubExchangePtr;
