#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"ProductWebData.h"

class CProductFinnhub : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhub)
		CProductFinnhub() {}
	~CProductFinnhub() {}

protected:
};
