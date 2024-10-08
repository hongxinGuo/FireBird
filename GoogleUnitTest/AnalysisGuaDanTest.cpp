#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaStock.h"

namespace FireBirdTest {
	struct GuadanData {
		// 共四十三个数据，一个序列号，二十个当前实时数据（挂单价位和数量），二十个上次实时数据（挂单价位和数量），一个成交类型，一个成交价格
		GuadanData(int count, int type, long price,
		           long dS5, long lS5, long dS4, long lS4, long dS3, long lS3, long dS2, long lS2, long dS1, long lS1,
		           long dB1, long lB1, long dB2, long lB2, long dB3, long lB3, long dB4, long lB4, long dB5, long lB5,
		           long dS5l, long lS5l, long dS4l, long lS4l, long dS3l, long lS3l, long dS2l, long lS2l, long dS1l,
		           long lS1l,
		           long dB1l, long lB1l, long dB2l, long lB2l, long dB3l, long lB3l, long dB4l, long lB4l, long dB5l,
		           long lB5l) {
			iCount = count;
			iType = type;
			lPrice = price;
			CurrentData.SetPSell(4, dS5);
			CurrentData.SetVSell(4, lS5);
			CurrentData.SetPSell(3, dS4);
			CurrentData.SetVSell(3, lS4);
			CurrentData.SetPSell(2, dS3);
			CurrentData.SetVSell(2, lS3);
			CurrentData.SetPSell(1, dS2);
			CurrentData.SetVSell(1, lS2);
			CurrentData.SetPSell(0, dS1);
			CurrentData.SetVSell(0, lS1);
			CurrentData.SetPBuy(4, dB5);
			CurrentData.SetVBuy(4, lB5);
			CurrentData.SetPBuy(3, dB4);
			CurrentData.SetVBuy(3, lB4);
			CurrentData.SetPBuy(2, dB3);
			CurrentData.SetVBuy(2, lB3);
			CurrentData.SetPBuy(1, dB2);
			CurrentData.SetVBuy(1, lB2);
			CurrentData.SetPBuy(0, dB1);
			CurrentData.SetVBuy(0, lB1);

			LastData.SetPSell(4, dS5l);
			LastData.SetVSell(4, lS5l);
			LastData.SetPSell(3, dS4l);
			LastData.SetVSell(3, lS4l);
			LastData.SetPSell(2, dS3l);
			LastData.SetVSell(2, lS3l);
			LastData.SetPSell(1, dS2l);
			LastData.SetVSell(1, lS2l);
			LastData.SetPSell(0, dS1l);
			LastData.SetVSell(0, lS1l);
			LastData.SetPBuy(4, dB5l);
			LastData.SetVBuy(4, lB5l);
			LastData.SetPBuy(3, dB4l);
			LastData.SetVBuy(3, lB4l);
			LastData.SetPBuy(2, dB3l);
			LastData.SetVBuy(2, lB3l);
			LastData.SetPBuy(1, dB2l);
			LastData.SetVBuy(1, lB2l);
			LastData.SetPBuy(0, dB1l);
			LastData.SetVBuy(0, lB1l);
		}

	public:
		int iCount; // 每次输入的数据都不一样，结果也相应的不同，故而需要进行不同的验证。使用此序列号来区分。
		CWebRTData CurrentData;
		CWebRTData LastData;
		int iType;
		long lPrice;
	};

	// 没有任何变化，无成交
	GuadanData Guadan1(0, NO_TRANSACTION_, 0,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 无成交，出现新的挂单位置。买单消失了20000， 卖单消失了20000
	GuadanData Guadan2(1, NO_TRANSACTION_, 0,
	                   10250, 10000, 10140, 20000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9870, 20000, 9860, 10000,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 无成交，出现新的挂单量
	GuadanData Guadan3(2, NO_TRANSACTION_, 0,
	                   10050, 10100, 10040, 10200, 10030, 10400, 10020, 10800, 10010, 11600,
	                   10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 无成交，出现撤单
	GuadanData Guadan4(3, NO_TRANSACTION_, 0,
	                   10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                   10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，一般型买入（比卖一价低），买卖单出现撤单。
	GuadanData Guadan5(4, ORDINARY_BUY_, 10009,
	                   10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                   10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
	GuadanData Guadan6(5, ATTACK_BUY_, 10019,
	                   10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                   10000, 10100, 9990, 10200, 9980, 10400, 9970, 10800, 9960, 11600,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，强买入（比卖二价高，此例高于卖三），卖单出现撤单，买单也出现撤单。
	GuadanData Guadan7(6, STRONG_BUY_, 10039,
	                   10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                   10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，强买入（比卖二价高，此例高于卖四），卖单出现撤单，买单也出现撤单。
	GuadanData Guadan8(7, STRONG_BUY_, 10049,
	                   10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                   10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                   10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                   10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，一般型卖出（比买一价高），买卖单出现撤单。
	GuadanData Guadan11(10, ORDINARY_SELL_, 10001,
	                    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，进攻型卖出（比买二价高），买卖单出现撤单，。
	GuadanData Guadan12(11, ATTACK_SELL_, 9991,
	                    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，强卖出（比买二价低，此例高于买三），买卖单出现撤单。
	GuadanData Guadan13(12, STRONG_SELL_, 9981,
	                    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，强卖出（比买二价低，此例低于买三），买卖单出现撤单。
	GuadanData Guadan14(13, STRONG_SELL_, 9971,
	                    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);
	// 有成交，强卖出（比买二价低，此例低于买四），买卖单出现撤单。
	GuadanData Guadan15(14, STRONG_SELL_, 9961,
	                    10050, 9900, 10040, 9800, 10030, 9600, 10020, 9200, 10010, 8400,
	                    10000, 9900, 9990, 9800, 9980, 9600, 9970, 9200, 9960, 8400,
	                    10050, 10000, 10040, 10000, 10030, 10000, 10020, 10000, 10010, 10000,
	                    10000, 10000, 9990, 10000, 9980, 10000, 9970, 10000, 9960, 10000);

	class RTDataGuadanTest : public testing::TestWithParam<GuadanData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const GuadanData* pData = GetParam();
			// 预设20个挂单
			for (int i = 0; i < 200; i += 10) {
				m_stock.TestSetGuadanDeque(9900 + i, 10000); // 所有的挂单量皆设置为一万股
			}
			pCurrentData = make_shared<CWebRTData>();
			for (int i = 0; i < 5; i++) {
				pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
				pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
				pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
				pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
			}
			pLastData = make_shared<CWebRTData>();
			for (int i = 0; i < 5; i++) {
				pLastData->SetVBuy(i, pData->LastData.GetVBuy(i));
				pLastData->SetPBuy(i, pData->LastData.GetPBuy(i));
				pLastData->SetVSell(i, pData->LastData.GetVSell(i));
				pLastData->SetPSell(i, pData->LastData.GetPSell(i));
			}
			for (int i = 0; i < 5; i++) {
				m_stock.SetGuadan(pLastData->GetPBuy(i), pLastData->GetVBuy(i));
				m_stock.SetGuadan(pLastData->GetPSell(i), pLastData->GetVSell(i));
			}
			lPrice = pData->lPrice;
			iType = pData->iType;
			iCount = pData->iCount;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int iCount;
		int iType;
		long lPrice;
		CWebRTDataPtr pCurrentData;
		CWebRTDataPtr pLastData;
		CChinaStock m_stock;
	};

	INSTANTIATE_TEST_SUITE_P(TestGuadanData, RTDataGuadanTest, testing::Values(&Guadan1, &Guadan2, &Guadan3, &Guadan4,
		                         &Guadan5, &Guadan6, &Guadan7,
		                         &Guadan11, &Guadan12, &Guadan13, &Guadan14, &Guadan15));

	TEST_P(RTDataGuadanTest, TestGuadan) {
		EXPECT_FALSE(m_stock.HaveFirstRTData());
		m_stock.SetLastRTData(pLastData);
		m_stock.SetCurrentTransactionType(iType);
		m_stock.AnalysisGuadan(pCurrentData, lPrice);
		switch (iCount) {
		case 0: // 无成交，无变化
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0);
			for (int i = 0; i < 200; i += 10) {
				EXPECT_EQ(m_stock.GetGuadan(9900 + i), 10000);
			}
			break;
		case 1: // 无成交，出现新的挂单位置：1025，1014， 987， 986，挂单量同时变化。
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 20000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 20000);
			for (int i = 9880; i < 9980; i += 10) {
				EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
			}
			for (int i = 9980; i <= 10030; i += 10) {
				EXPECT_EQ(m_stock.GetGuadan(i), 10000);
			}
			for (int i = 10050; i < 10140; i += 10) {
				EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
			}
			for (int i = 10150; i < 10250; i += 10) {
				EXPECT_EQ(m_stock.GetGuadan(i), 0); // 这几个的挂单被清空了
			}
			EXPECT_EQ(m_stock.GetGuadan(10250), 10000);
			EXPECT_EQ(m_stock.GetGuadan(10140), 20000);
			EXPECT_EQ(m_stock.GetGuadan(9870), 20000);
			EXPECT_EQ(m_stock.GetGuadan(9860), 10000);
			break;
		case 2: // 无成交，出现新的挂单量
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0);
			EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
			EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
			EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
			EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
			EXPECT_EQ(m_stock.GetGuadan(10010), 11600);
			EXPECT_EQ(m_stock.GetGuadan(10020), 10800);
			EXPECT_EQ(m_stock.GetGuadan(10030), 10400);
			EXPECT_EQ(m_stock.GetGuadan(10040), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10050), 10100);
			break;
		case 3: // 无成交，出现撤单。
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3100);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 3100);
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 4:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500); // 由于是正常买入，故卖一的撤单不计
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 5: // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 700); // 由于是进攻型买入，故卖一和卖二的撤单不计
			EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
			EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
			EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
			EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 6:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000); // 只要出现买卖盘，卖一和买一的撤单就不计算。
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 100); // 由于是强买入（此例低于卖四），故卖一、卖二、卖三和卖四的撤单不计
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 7:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000) << _T("只要出现买卖盘，卖一和买一的撤单就不计算");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0) << _T("由于是强买入（此例高于卖四），故卖一、卖二、卖三、卖四和卖五的撤单都不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 10:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("由于是正常买入，故卖一的撤单不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 11:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 2800);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("由于是进攻型买入，故卖一和卖二的撤单不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 12:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 2400) << _T("买一、买二和买三的撤单不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 13:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 1600) << _T("买一、买二、买三和买四的撤单不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 14:
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0) << _T("买一、买二、买三、买四和卖五的撤单都不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		default:
			break;
		}
	}

	class NeedCheckTest : public testing::TestWithParam<GuadanData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const GuadanData* pData = GetParam();
			// 预设20个挂单
			for (int i = 0; i < 200; i += 10) {
				m_stock.TestSetGuadanDeque(9900 + i, 10000); // 所有的挂单量皆设置为一万股
			}
			pCurrentData = make_shared<CWebRTData>();
			for (int i = 0; i < 5; i++) {
				pCurrentData->SetVBuy(i, pData->CurrentData.GetVBuy(i));
				pCurrentData->SetPBuy(i, pData->CurrentData.GetPBuy(i));
				pCurrentData->SetVSell(i, pData->CurrentData.GetVSell(i));
				pCurrentData->SetPSell(i, pData->CurrentData.GetPSell(i));
			}
			pLastData = make_shared<CWebRTData>();
			for (int i = 0; i < 5; i++) {
				pLastData->SetVBuy(i, pData->LastData.GetVBuy(i));
				pLastData->SetPBuy(i, pData->LastData.GetPBuy(i));
				pLastData->SetVSell(i, pData->LastData.GetVSell(i));
				pLastData->SetPSell(i, pData->LastData.GetPSell(i));
			}
			for (int i = 0; i < 5; i++) {
				m_stock.SetGuadan(pLastData->GetPBuy(i), pLastData->GetVBuy(i));
				m_stock.SetGuadan(pLastData->GetPSell(i), pLastData->GetVSell(i));
			}
			lPrice = pData->lPrice;
			iType = pData->iType;
			iCount = pData->iCount;
			m_stock.SetCanceledBuyVolume(0);
			m_stock.SetCanceledSellVolume(0);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int iCount;
		int iType;
		long lPrice;
		CWebRTDataPtr pCurrentData;
		CWebRTDataPtr pLastData;
		CChinaStock m_stock;
	};

	INSTANTIATE_TEST_SUITE_P(TestNeedCheck, NeedCheckTest, testing::Values(&Guadan1, &Guadan2, &Guadan3, &Guadan4,
		                         &Guadan5, &Guadan6, &Guadan7, &Guadan8,
		                         &Guadan11, &Guadan12, &Guadan13, &Guadan14, &Guadan15));

	TEST_P(NeedCheckTest, TestNeedCheck) {
		array<bool, 10> fNeedCheck{ true, true, true, true, true, true, true, true, true, true };

		EXPECT_FALSE(m_stock.HaveFirstRTData());
		m_stock.SetLastRTData(pLastData);
		m_stock.SetCurrentTransactionType(iType);
		for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // 预设为都要计算。

		m_stock.SelectGuadanThatNeedToCalculate(pCurrentData, lPrice, fNeedCheck);
		switch (iCount) {
		case 0: // 无成交
			for (int i = 0; i < 10; i++) {
				EXPECT_TRUE(fNeedCheck.at(i));
			}
			break;
		case 1: // 无成交，出现新的挂单位置
			for (int i = 0; i < 10; i++) {
				EXPECT_TRUE(fNeedCheck.at(i));
			}
			break;
		case 2: // 无成交，出现新的挂单量
			for (int i = 0; i < 10; i++) {
				EXPECT_TRUE(fNeedCheck.at(i));
			}
			break;
		case 3: // 无成交，出现撤单
			for (int i = 0; i < 10; i++) {
				EXPECT_TRUE(fNeedCheck.at(i));
			}
			break;
		case 4: // 有成交，一般型买入（比卖一价低），买卖单出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_TRUE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 5: // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_FALSE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_TRUE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 6: // 有成交，强买入（比卖二价高，此例高于卖三），卖单出现撤单，买单也出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_FALSE(fNeedCheck.at(1));
			EXPECT_FALSE(fNeedCheck.at(2));
			EXPECT_FALSE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_TRUE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 7: // 有成交，强买入（比卖二价高，此例高于卖四），卖单出现撤单，买单也出现撤单。
			EXPECT_FALSE(fNeedCheck.at(0)) << _T("所有的卖单都受影响，无需检查挂单");
			EXPECT_FALSE(fNeedCheck.at(1));
			EXPECT_FALSE(fNeedCheck.at(2));
			EXPECT_FALSE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_TRUE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 10: // 有成交，一般型卖出（比买一价高），买卖单出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_TRUE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 11: // 有成交，进攻型卖出（比买二价高），买卖单出现撤单，
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_FALSE(fNeedCheck.at(6));
			EXPECT_TRUE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 12: // 有成交，强卖出（比买二价低，此例高于买三），买卖单出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_FALSE(fNeedCheck.at(6));
			EXPECT_FALSE(fNeedCheck.at(7));
			EXPECT_TRUE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 13: // 有成交，强卖出（比买二价低，此例低于买三），买卖单出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_FALSE(fNeedCheck.at(6));
			EXPECT_FALSE(fNeedCheck.at(7));
			EXPECT_FALSE(fNeedCheck.at(8));
			EXPECT_TRUE(fNeedCheck.at(9));
			break;
		case 14: // 有成交，强卖出（比买二价低，此例低于买四），买卖单出现撤单。
			EXPECT_TRUE(fNeedCheck.at(0));
			EXPECT_TRUE(fNeedCheck.at(1));
			EXPECT_TRUE(fNeedCheck.at(2));
			EXPECT_TRUE(fNeedCheck.at(3));
			EXPECT_FALSE(fNeedCheck.at(4));
			EXPECT_FALSE(fNeedCheck.at(5));
			EXPECT_FALSE(fNeedCheck.at(6));
			EXPECT_FALSE(fNeedCheck.at(7));
			EXPECT_FALSE(fNeedCheck.at(8));
			EXPECT_FALSE(fNeedCheck.at(9));
			break;
		default:
			ASSERT(0);
			break;
		}
		m_stock.SetCurrentGuadan(pCurrentData);
		switch (iCount) {
		case 0: // 无成交，出现新的挂单位置
			for (int i = 0; i < 10; i++) {
				EXPECT_EQ(m_stock.GetGuadan(9960 + i * 10), 10000);
			}
			break;
		case 1: // 无成交，出现新的挂单位置
			EXPECT_FALSE(m_stock.HaveGuadan(9850));
			EXPECT_EQ(m_stock.GetGuadan(9860), 10000);
			EXPECT_EQ(m_stock.GetGuadan(9870), 20000);
			for (int i = 0; i < 10; i++) {
				EXPECT_FALSE(m_stock.HaveGuadan(9880 + i * 10));
			}
			for (int i = 0; i < 6; i++) {
				EXPECT_EQ(m_stock.GetGuadan(9980 + i * 10), 10000);
			}
			for (int i = 0; i < 10; i++) {
				EXPECT_FALSE(m_stock.HaveGuadan(10040 + i * 10));
			}
			EXPECT_EQ(m_stock.GetGuadan(10140), 20000);
			for (int i = 0; i < 10; i++) {
				EXPECT_FALSE(m_stock.HaveGuadan(10150 + i * 10));
			}
			EXPECT_EQ(m_stock.GetGuadan(10250), 10000);
			EXPECT_FALSE(m_stock.HaveGuadan(10260));
			break;
		case 2: // 无成交，出现新的挂单量
			EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
			EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
			EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
			EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
			EXPECT_EQ(m_stock.GetGuadan(10010), 11600);
			EXPECT_EQ(m_stock.GetGuadan(10020), 10800);
			EXPECT_EQ(m_stock.GetGuadan(10030), 10400);
			EXPECT_EQ(m_stock.GetGuadan(10040), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10050), 10100);
			break;
		case 3: // 无成交，出现撤单
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 4: // 有成交，一般型买入（比卖一价低），买卖单出现撤单。
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 5: // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
			EXPECT_EQ(m_stock.GetGuadan(9960), 11600);
			EXPECT_EQ(m_stock.GetGuadan(9970), 10800);
			EXPECT_EQ(m_stock.GetGuadan(9980), 10400);
			EXPECT_EQ(m_stock.GetGuadan(9990), 10200);
			EXPECT_EQ(m_stock.GetGuadan(10000), 10100);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 6:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 7:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 10:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 11:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 12:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 13:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		case 14:
			EXPECT_EQ(m_stock.GetGuadan(9960), 8400);
			EXPECT_EQ(m_stock.GetGuadan(9970), 9200);
			EXPECT_EQ(m_stock.GetGuadan(9980), 9600);
			EXPECT_EQ(m_stock.GetGuadan(9990), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10000), 9900);
			EXPECT_EQ(m_stock.GetGuadan(10010), 8400);
			EXPECT_EQ(m_stock.GetGuadan(10020), 9200);
			EXPECT_EQ(m_stock.GetGuadan(10030), 9600);
			EXPECT_EQ(m_stock.GetGuadan(10040), 9800);
			EXPECT_EQ(m_stock.GetGuadan(10050), 9900);
			break;
		default:
			ASSERT(0);
			break;
		}
		m_stock.CheckGuadan(pCurrentData, fNeedCheck);
		switch (iCount) {
		case 0: // 无变化
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0);
			break;
		case 1: // 无成交，出现新的挂单位置.买单消失了20000（9960和9970处）， 卖单消失了20000（10050和10040处）
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 20000);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 20000);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 20000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 20000);
			break;
		case 2: // 无成交，出现新的挂单量
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0);
			break;
		case 3: // 无成交，出现撤单
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 3100);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 3100);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3100);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 3100);
			break;
		case 4: // 有成交，一般型买入（比卖一价低），买卖单出现撤单。(不考虑卖一和买一挂单量）
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500);
			break;
		case 5: // 有成交，进攻型买入（比卖二价低），卖单出现撤单，买单增单。
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 700);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 700);
			break;
		case 6:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 100);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000); // 只要出现买卖盘，卖一和买一的撤单就不计算。
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 100) << _T("由于是强买入（此例低于卖四），故卖一、卖二、卖三和卖四的撤单不计");
			break;
		case 7:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 0);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000) << _T("只要出现买卖盘，卖一和买一的撤单就不计算");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 0) << _T("由于是强买入（此例高于卖四），故卖一、卖二、卖三、卖四和卖五的撤单都不计");
			break;
		case 10:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 3000);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("由于是正常买入，故卖一的撤单不计");
			break;
		case 11:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 2800);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 2800);
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("由于是进攻型买入，故卖一和卖二的撤单不计");
			break;
		case 12:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 2400);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 2400) << _T("买一、买二、买三的撤单不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			break;
		case 13:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 1600);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 1600) << _T("买一、买二、买三和买四的撤单不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			break;
		case 14:
			EXPECT_EQ(m_stock.GetCurrentCanceledBuyVolume(), 0);
			EXPECT_EQ(m_stock.GetCurrentCanceledSellVolume(), 1500);
			EXPECT_EQ(m_stock.GetCanceledBuyVolume(), 0) << _T("买一、买二、买三、买四和买五的撤单都不计");
			EXPECT_EQ(m_stock.GetCanceledSellVolume(), 1500) << _T("只要有成交，卖一的撤单就不计");
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}
