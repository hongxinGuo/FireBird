#pragma once

#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductFinnhub : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductFinnhub)

	CProductFinnhub() {
	}

	~CProductFinnhub() override = default;

	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) override final;
	virtual bool AddInaccessibleExchangeIfNeeded(void) override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

protected:
};
