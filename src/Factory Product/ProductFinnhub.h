#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWorldMarketWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	bool CheckAccessRight(CWebDataPtr pWebData) final; // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
	void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�
};
