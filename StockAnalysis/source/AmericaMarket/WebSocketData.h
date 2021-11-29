#pragma once

using namespace std;
#include<memory>

class CTiingoCryptoSocket : public CObject {
public:
	CTiingoCryptoSocket() {
		m_chMessageType = ' ';
		m_strSymbol = _T("");
		m_strExchange = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
	}

public:
	char m_chMessageType; // 'Q' or 'T'
	CString m_strSymbol;
	CTime m_date;
	CString m_strExchange;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
	double m_dLastPrice;
	double m_dLastSize;
};

typedef shared_ptr<CTiingoCryptoSocket> CTiingoCryptoSocketPtr;

class CTiingoForexSocket : public CObject {
public:
	CTiingoForexSocket() {
		m_chMessageType = 'Q';
		m_strSymbol = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = 0;
	}

public:
	char m_chMessageType; // ±ÿ–Î «'Q'
	CString m_strSymbol;
	CTime m_date;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
};

typedef shared_ptr<CTiingoForexSocket> CTiingoForexSocketPtr;

class CTiingoIEXSocket : public CObject {
public:
	CTiingoIEXSocket() {
		m_chMessageType = 'Q';
		m_strSymbol = _T("");
		m_iNanoseconds = 0;
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
		m_iAfterHour = m_iHalted = m_iISO = m_iOddlot = m_iNMSRule611 = 0;
	}

public:
	char m_chMessageType; // 'Q', 'T', or 'B'
	CTime m_date;
	INT64 m_iNanoseconds;
	CString m_strSymbol;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
	double m_dLastPrice;
	double m_dLastSize;
	int m_iHalted; // 1 for halted, 0 for not halted
	int m_iAfterHour; // 1 for after hour, 0 for market hours
	int m_iISO; // 1 for intermarket sweep order, 0 for non-ISO order
	int m_iOddlot; // 1 for trade is an odd lot, 0 for trade is round or mix lot. Only available for Trade updates, null otherwise
	int m_iNMSRule611; // 1 if the trade is not subject to NMS Rule 611 (trade through), 0 if the trade is subject to Rule NMS 611. Only available for Trade updates, null otherwise.
};

typedef shared_ptr<CTiingoIEXSocket> CTiingoIEXSocketPtr;

class CFinnhubSocket : public CObject {
public:
	CFinnhubSocket() {
		m_strSymbol = _T("");
		m_dLastPrice = m_dLastVolume = 0;
		m_iSeconds = 0;
		m_strCode = _T("");
	}
public:
	CString m_strSymbol;
	double m_dLastPrice;
	INT64 m_iSeconds; // UNIX milliseconds timestamp
	double m_dLastVolume;
	CString m_strCode; // trade conditions
};

typedef shared_ptr<CFinnhubSocket> CFinnhubSocketPtr;
