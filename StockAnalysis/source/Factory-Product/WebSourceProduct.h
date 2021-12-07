#pragma once

using namespace std;
#include<memory>

class CWebSourceDataProduct : public CObject {
	CWebSourceDataProduct() {}
	~CWebSourceDataProduct() {}
public:
	virtual void CreatMessage(void) = 0;
	virtual void ProcessMessage(void) = 0;
	virtual void ReturnProduct(void) = 0;
};

typedef shared_ptr<CWebSourceDataProduct> CWebSourceDataProductPtr;
