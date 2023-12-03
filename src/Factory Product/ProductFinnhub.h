#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"
//#include "WorldMarket.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	[[nodiscard]] shared_ptr<CWorldMarket> GetMarket() const noexcept;
	bool CheckNoRightToAccess(CWebDataPtr pWebData) final;
	void AddInaccessibleExchange() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�
};
