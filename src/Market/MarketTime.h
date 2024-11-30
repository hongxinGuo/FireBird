#pragma once

class CMarketTime {
public:
	CMarketTime();
	CMarketTime(CString strLocalPosition);
	~CMarketTime() = default;

protected:
};

using CMarketTimePtr = shared_ptr<CMarketTime>;
