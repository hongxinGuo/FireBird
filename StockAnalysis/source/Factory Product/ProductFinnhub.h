#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductFinnhub : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductFinnhub)

	CProductFinnhub() { }

	~CProductFinnhub() override = default;

	bool CheckNoRightToAccess(CWebDataPtr pWebData) final;
	bool AddInaccessibleExchangeIfNeeded(void) override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

protected:
};
