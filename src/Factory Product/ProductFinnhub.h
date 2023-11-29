#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductFinnhub : public CVirtualWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	bool CheckNoRightToAccess(CWebDataPtr pWebData) final;
	void CheckAndAddInaccessibleExchange() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�
};
