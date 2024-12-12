#pragma once

import FireBird.Market.World;
import FireBird.Product.VirtualWorldMarket;

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWorldMarketWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	// �����ú���
	bool __Test_checkAccessRight(CWebDataPtr pWebData); // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
};
