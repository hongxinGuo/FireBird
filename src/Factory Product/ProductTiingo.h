#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductTiingo : public CVirtualWorldMarketWebProduct {
public:
	CProductTiingo() = default;
	~CProductTiingo() override = default;

	void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;

	void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	// �����ú���
	bool __Test_checkAccessRight(CWebDataPtr pWebData) final; // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
};
