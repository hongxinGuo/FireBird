#pragma once

#include"ProductWebData.h"

class CProductFinnhubForexSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexSymbol)
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexSymbol> CProductFinnhubForexSymbolPtr;