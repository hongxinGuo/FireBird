#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductTiingo : public CVirtualWorldMarketWebProduct {
public:
	CProductTiingo() = default;
	~CProductTiingo() override = default;

	bool CheckAccessRight(CWebDataPtr pWebData) final; // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
	void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�
};
