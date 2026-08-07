#pragma once

#include"ProductFinnhub.h"

class CDayLine;
class CWebData;

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
	shared_ptr<vector<CDayLine>> ParseFinnhubStockCandle(const shared_ptr<CWebData>& pWebData);
};

using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
