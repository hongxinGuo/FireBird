#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"VirtualProductWebData.h"

class CProductFinnhub : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductFinnhub)
		CProductFinnhub() {}
	~CProductFinnhub() {}

protected:
};
