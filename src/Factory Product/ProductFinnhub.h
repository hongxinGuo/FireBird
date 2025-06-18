#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWorldMarketWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;

	void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	// �����ú���
	bool __Test_checkAccessRight(CWebDataPtr pWebData) final; // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
};
