#include"pch.h"
export module FireBird.Product.Tiingo.Base;
;
import FireBird.Market.World;
import FireBird.Product.VirtualWorldMarket;

export {
	class CProductTiingo : public CVirtualWorldMarketWebProduct {
	public:
		CProductTiingo() = default;
		~CProductTiingo() override = default;

		void AddInaccessibleSymbol() override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

		// �����ú���
		bool __Test_checkAccessRight(CWebDataPtr pWebData); // todo ����ʹ�ã�׼��ɾ��֮. Ŀǰֻ���ڲ��Ժ����С�
	};
}
