//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// �������ݵ����룬����Factory-Productģʽ�������ݶ�Ӧ��Ӧ��Product��
/// Factoryֻ�����������
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
class CVirtualWebProduct;
typedef shared_ptr<CVirtualWebProduct> CVirtualProductWebDataPtr;
class CVirtualMarket;
typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
//typedef weak_ptr<CVirtualMarket> CVirtualMarketWeakPtr;

class CVirtualDataFactory {
public:
	CVirtualDataFactory() = default;

	virtual ~CVirtualDataFactory() = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
};
