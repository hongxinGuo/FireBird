#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"
#include "WorldMarket.h"

using std::dynamic_pointer_cast;

class CProductTiingo : public CVirtualWebProduct {
public:
	CProductTiingo() = default;
	~CProductTiingo() override = default;

	[[nodiscard]] shared_ptr<CWorldMarket> GetMarket() const noexcept;
};
