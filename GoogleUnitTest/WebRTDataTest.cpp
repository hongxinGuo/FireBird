#include"pch.h"

 

#include"globedef.h"

import FireBird.System.Message;

#include"GeneralCheck.h"

#include"ChinaMarket.h"
import FireBird.Type.WebRTData;
#include"nlohmann/json_fwd.hpp"
import FireBird.Type.WebData;

namespace FireBirdTest {
	class CStockWebRTDataTest : public testing::Test {
	protected:
		static void SetUpTestSuite() {
			// 本测试类的初始化函数
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CStockWebRTDataTest, TestInitialize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CWebRTData RTData;
		EXPECT_EQ(RTData.GetTimePoint().time_since_epoch().count(), 0);
		EXPECT_STREQ(RTData.GetSymbol(), _T(""));
		EXPECT_STREQ(RTData.GetStockName(), _T(""));
		EXPECT_EQ(RTData.GetOpen(), 0);
		EXPECT_EQ(RTData.GetLastClose(), 0);
		EXPECT_EQ(RTData.GetNew(), 0);
		EXPECT_EQ(RTData.GetHigh(), 0);
		EXPECT_EQ(RTData.GetLow(), 0);
		EXPECT_EQ(RTData.GetBuy(), 0);
		EXPECT_EQ(RTData.GetSell(), 0);
		EXPECT_EQ(RTData.GetVolume(), 0);
		EXPECT_EQ(RTData.GetAmount(), 0);
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(RTData.GetVBuy(i), 0);
			EXPECT_EQ(RTData.GetPBuy(i), 0);
			EXPECT_EQ(RTData.GetVSell(i), 0);
			EXPECT_EQ(RTData.GetPSell(i), 0);
		}
		EXPECT_FALSE(RTData.IsActive());
	}

	TEST_F(CStockWebRTDataTest, TestGetDatasource) {
		CWebRTData rtData;
		EXPECT_EQ(rtData.GetDataSource(), INVALID_RT_WEB_DATA_);
		EXPECT_FALSE(rtData.IsValidDataSource());
		rtData.SetDataSource(SINA_RT_WEB_DATA_);
		EXPECT_EQ(rtData.GetDataSource(), SINA_RT_WEB_DATA_);
		EXPECT_TRUE(rtData.IsValidDataSource());
	}

	TEST_F(CStockWebRTDataTest, TestGetStockCode) {
		CWebRTData rtData;
		rtData.SetSymbol(_T("600000.SS"));
		EXPECT_STREQ(rtData.GetSymbol(), _T("600000.SS"));
	}

	TEST_F(CStockWebRTDataTest, TestIsActive) {
		CWebRTData rtData;
		EXPECT_FALSE(rtData.IsActive());
		rtData.SetActive(true);
		EXPECT_TRUE(rtData.IsActive());
		rtData.SetActive(false);
		EXPECT_FALSE(rtData.IsActive());
	}

	TEST_F(CStockWebRTDataTest, TestGetLastClose) {
		CWebRTData rtData;
		rtData.SetLastClose(1010);
		EXPECT_EQ(rtData.GetLastClose(), 1010);
	}

	TEST_F(CStockWebRTDataTest, TestGetOpen) {
		CWebRTData rtData;
		rtData.SetOpen(20202);
		EXPECT_EQ(rtData.GetOpen(), 20202);
	}

	TEST_F(CStockWebRTDataTest, TestGetHigh) {
		CWebRTData rtData;
		rtData.SetHigh(30303);
		EXPECT_EQ(rtData.GetHigh(), 30303);
	}

	TEST_F(CStockWebRTDataTest, TestGetLow) {
		CWebRTData rtData;
		rtData.SetLow(40404);
		EXPECT_EQ(rtData.GetLow(), 40404);
	}

	TEST_F(CStockWebRTDataTest, TestGetNew) {
		CWebRTData rtData;
		rtData.SetNew(50505);
		EXPECT_EQ(rtData.GetNew(), 50505);
	}

	TEST_F(CStockWebRTDataTest, TestGetAmount) {
		CWebRTData rtData;
		rtData.SetAmount(60606);
		EXPECT_EQ(rtData.GetAmount(), 60606);
	}

	TEST_F(CStockWebRTDataTest, TestGetVolume) {
		CWebRTData rtData;
		rtData.SetVolume(70707);
		EXPECT_EQ(rtData.GetVolume(), 70707);
	}

	TEST_F(CStockWebRTDataTest, TestGetTotalValue) {
		CWebRTData rtData;
		rtData.SetTotalValue(88);
		EXPECT_EQ(rtData.GetTotalValue(), 88);
	}

	TEST_F(CStockWebRTDataTest, TestGetCurrentValue) {
		CWebRTData rtData;
		rtData.SetCurrentValue(999);
		EXPECT_EQ(rtData.GetCurrentValue(), 999);
	}

	TEST_F(CStockWebRTDataTest, TestGetBuy) {
		CWebRTData rtData;
		rtData.SetBuy(1111);
		EXPECT_EQ(rtData.GetBuy(), 1111);
	}

	TEST_F(CStockWebRTDataTest, TestGetSell) {
		CWebRTData rtData;
		rtData.SetSell(2222);
		EXPECT_EQ(rtData.GetSell(), 2222);
	}

	TEST_F(CStockWebRTDataTest, TestGetHighLimit) {
		CWebRTData rtData;
		rtData.SetHighLimitFromTengxun(4444);
		EXPECT_EQ(rtData.GetHighLimitFromTengxun(), 4444);
	}

	TEST_F(CStockWebRTDataTest, TestGetLowLimit) {
		CWebRTData rtData;
		rtData.SetLowLimitFromTengxun(5555);
		EXPECT_EQ(rtData.GetLowLimitFromTengxun(), 5555);
	}

	TEST_F(CStockWebRTDataTest, TestGetPBuy) {
		CWebRTData rtData;
		for (int i = 0; i < 5; i++) {
			rtData.SetPBuy(i, i + 10201);
			EXPECT_EQ(rtData.GetPBuy(i), i + 10201);
		}
	}

	TEST_F(CStockWebRTDataTest, TestGetVBuy) {
		CWebRTData rtData;
		for (int i = 0; i < 5; i++) {
			rtData.SetVBuy(i, i + 10101);
			EXPECT_EQ(rtData.GetVBuy(i), i + 10101);
		}
	}

	TEST_F(CStockWebRTDataTest, TestGetPSell) {
		CWebRTData rtData;
		for (int i = 0; i < 5; i++) {
			rtData.SetPSell(i, i + 30101);
			EXPECT_EQ(rtData.GetPSell(i), i + 30101);
		}
	}

	TEST_F(CStockWebRTDataTest, TestGetVSell) {
		CWebRTData rtData;
		for (int i = 0; i < 5; i++) {
			rtData.SetVSell(i, i + 50101);
			EXPECT_EQ(rtData.GetVSell(i), i + 50101);
		}
	}

	TEST_F(CStockWebRTDataTest, TestIsDataTimeAtCurrentDate) {
		const time_t time = GetUTCTime();
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 7; // 2019年11月7日是星期四。
		tm_.tm_hour = 12;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		const time_t tt = gl_pChinaMarket->TransferToUTCTime(&tm_);
		TestSetUTCTime(tt);
		CWebRTData data;
		data.SetTransactionTime(tt);
		EXPECT_TRUE(data.IsValidTime(14));
		data.SetTransactionTime(tt - 3600 * 24 * 14);
		EXPECT_TRUE(data.IsValidTime(14));
		data.SetTransactionTime(tt - 3600 * 24 * 14 - 1);
		EXPECT_FALSE(data.IsValidTime(14));

		data.SetTransactionTime(tt + 1);
		EXPECT_FALSE(data.IsValidTime(14)) << _T("数据有问题：成交时间晚于当前时间");

		// 恢复原状
		TestSetUTCTime(time);
	}

	TEST_F(CStockWebRTDataTest, TestCheckSinaRTDataMarket) {
		const time_t tTime = GetUTCTime();
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 7; // 2019年11月7日是星期四。
		tm_.tm_hour = 12;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		const time_t tt = gl_pChinaMarket->TransferToUTCTime(&tm_);
		TestSetUTCTime(tt);
		CWebRTData data;
		data.SetTransactionTime(tt);
		EXPECT_TRUE(data.CheckSinaRTDataActive());
		data.SetTransactionTime(tt - 3600 * 24 * 14);
		EXPECT_TRUE(data.CheckSinaRTDataActive());
		data.SetTransactionTime(tt - 3600 * 24 * 14 - 1);
		EXPECT_FALSE(data.CheckSinaRTDataActive());

		// 恢复原状
		TestSetUTCTime(tTime);
	}

	struct SinaRTData {
		SinaRTData(int count, const CString& Data) {
			m_iCount = count;
			m_strData = Data;
		}

	public:
		int m_iCount;
		CString m_strData;
	};

	// 无错误数据。只有交易日期没有交易时间，使用默认闭市时间（15:00:00).
	SinaRTData Data1(
		0, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,,00,\";\n"));
	// 所有的价格皆为0,交易时间为12:00:00
	SinaRTData Data2(1, _T("var hq_str_sz002385=\"平安银行,0,0,0,0,0,0,0,21606007,248901949.000,19900,0,54700,0,561500,0,105600,0,172400,0,259981,0,206108,0,325641,0,215109,0,262900,0,2019-07-16,12:00:00,00,\";\n"));
	// 所有的数量皆为零
	SinaRTData Data3(2, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,0,0,0,11.540,0,11.530,0,11.520,0,11.510,0,11.500,0,11.550,0,11.560,0,11.570,0,11.580,0,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data4(3, _T("var hq_str_sh600000=\"浦发银行,-11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data5(4, _T("var hq_str_sh600000=\"浦发银行,11.510,-11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data6(5, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,-11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data7(6, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,-11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data8(7, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,-11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data9(8, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,-11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data10(9, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,-11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data11(10, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,-21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data12(11, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,-248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data13(12, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,-19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data14(13, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,-11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data15(14, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,-54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data16(15, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,-11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data17(16, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,-561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data18(17, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,-11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data19(18, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,-105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data20(19, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,-11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data21(20, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,-172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data22(21, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,-11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data23(22, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,-259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data24(23, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,-11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data25(24, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,-206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data26(25, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,-11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data27(26, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,-325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data28(27, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,-11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data29(28, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,-215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data30(29, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,-11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data31(30, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,-262900,11.590,2019-07-16,15:00:00,00,\";\n"));
	// 出现负值
	SinaRTData Data32(31, _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 没有实时数据
	SinaRTData Data33(32, _T("var hq_str_sz000001=\"\";\n"));
	// 格式出错（前缀）
	SinaRTData Data34(33, _T("var hq_st_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 格式出错（）
	SinaRTData Data35(34, _T("var hq_str_sa600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 格式出错（不是sh或者sz）
	SinaRTData Data36(35, _T("var hq_str_sa600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 格式出错（股票代码后面不是'='号
	SinaRTData Data37(36, _T("var hq_str_sa600000a\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 格式出错（'='好后面不是'"'号
	SinaRTData Data38(37, _T("var hq_str_sa600000a'浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 格式出错（股票代码后面不是'='号
	SinaRTData Data39(38, _T("var hq_str_sa600000a\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,-11.590,2019-07-16,15:00:00,00,\";\n"));
	// 没有实时数据的两段数据
	SinaRTData Data41(40, _T("var hq_str_sz000001=\"\";\nvar hq_str_sz000001=\"\";\n"));

	class CalculateSinaRTDataTest : public testing::TestWithParam<SinaRTData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			SinaRTData* pData = GetParam();
			m_pSinaWebRTData = make_shared<CWebData>();
			m_iCount = pData->m_iCount;
			m_lStringLength = pData->m_strData.GetLength();
			m_pSinaWebRTData->Resize(m_lStringLength);
			m_pSinaWebRTData->SetData(pData->m_strData.GetBuffer(), m_lStringLength);
			for (int i = 0; i < 5; i++) {
				m_RTData.SetPBuy(i, -1);
				m_RTData.SetPSell(i, -1);
				m_RTData.SetVBuy(i, -1);
				m_RTData.SetVSell(i, -1);
			}
			m_RTData.SetAmount(-1);
			m_RTData.SetVolume(-1);
			m_RTData.SetOpen(-1);
			m_RTData.SetNew(-1);
			m_RTData.SetLastClose(-1);
			m_RTData.SetHigh(-1);
			m_RTData.SetLow(-1);
			m_RTData.SetSell(-1);
			m_RTData.SetBuy(-1);

			svData = m_pSinaWebRTData->GetCurrentSinaData();
			m_pSinaWebRTData->ResetCurrentPos();
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_iCount;
		char* m_pData;
		long m_lStringLength;
		CWebDataPtr m_pSinaWebRTData;
		CWebRTData m_RTData;
		string_view svData;
	};

	INSTANTIATE_TEST_SUITE_P(TestSinaRTData, CalculateSinaRTDataTest, testing::Values(&Data1, &Data2, &Data3,
		                         &Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
		                         &Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
		                         &Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30,
		                         &Data31, &Data32, &Data33, &Data34, &Data35, &Data36, &Data37, &Data38, &Data39, &Data41
	                         ));

	TEST_P(CalculateSinaRTDataTest, TestSinaRTData) {
		time_t tTime, tUTCTime;
		tm tm_; // 该日期为：2019-07-16 15:00:00，就是数据中的时间。
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 7 - 1;
		tm_.tm_mday = 16;
		tm_.tm_hour = 15;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		tTime = gl_pChinaMarket->TransferToUTCTime(&tm_);
		tUTCTime = GetUTCTime();
		TestSetUTCTime(tTime);
		switch (m_iCount) {
		case 33: // 有错误，前缀出错
		case 34: // 有错误，前缀出错
		case 35: // 有错误，前缀出错
		case 36: // 有错误，前缀出错
		case 37: // 有错误，前缀出错
		case 38: // 有错误，前缀出错
			EXPECT_THROW(m_RTData.ParseSinaData(svData), std::exception);
			break;
		default:
			m_RTData.ParseSinaData(svData);
			break;
		}
		EXPECT_EQ(m_pSinaWebRTData->GetCurrentPos(), 0) << "不移动当前位置指针";
		switch (m_iCount) {
		case 0:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			EXPECT_EQ(m_RTData.GetPSell(4), 11590);
			EXPECT_EQ(m_RTData.GetTransactionTime(), tTime);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 1: // 所有价格皆为零
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("002385.SZ"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("平安银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 0);
			EXPECT_EQ(m_RTData.GetLastClose(), 0);
			EXPECT_EQ(m_RTData.GetNew(), 0);
			EXPECT_EQ(m_RTData.GetHigh(), 0);
			EXPECT_EQ(m_RTData.GetLow(), 0);
			EXPECT_EQ(m_RTData.GetBuy(), 0);
			EXPECT_EQ(m_RTData.GetSell(), 0);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 0);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 0);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 0);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 0);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 0);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 0);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 0);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 0);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 0);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			EXPECT_EQ(m_RTData.GetPSell(4), 0);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime - 3 * 3600) << "交易时间不是默认的15:00:00,而是12:00:00";
			break;
		case 2:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 0);
			EXPECT_EQ(m_RTData.GetAmount(), 0);
			EXPECT_EQ(m_RTData.GetVBuy(0), 0);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 0);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 0);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 0);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 0);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 0);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 0);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 0);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 0);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 0);
			EXPECT_EQ(m_RTData.GetPSell(4), 11590);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 3:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), -11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			EXPECT_EQ(m_RTData.GetPSell(4), 11590);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 4:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), -11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			EXPECT_EQ(m_RTData.GetPSell(4), 11590);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 5:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), -11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			EXPECT_EQ(m_RTData.GetPSell(4), 11590);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 6:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			break;
		case 7:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			break;
		case 8:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			break;
		case 9:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			break;
		case 10: // 有错误
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			break;
		case 11:
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			break;
		case 12: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			break;
		case 13: // 有错误
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			break;
		case 14: // 有错误
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			break;
		case 15: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			break;
		case 16: // 有错误
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			break;
		case 17: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			break;
		case 18: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			break;
		case 19: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			break;
		case 20: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			break;
		case 21: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			break;
		case 22: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			break;
		case 23: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			break;
		case 24: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			break;
		case 25: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			break;
		case 26: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			break;
		case 27: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			break;
		case 28: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			break;
		case 29: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			break;
		case 30: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			break;
		case 31: // 出现负值
			EXPECT_TRUE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 11510);
			EXPECT_EQ(m_RTData.GetLastClose(), 11490);
			EXPECT_EQ(m_RTData.GetNew(), 11560);
			EXPECT_EQ(m_RTData.GetHigh(), 11570);
			EXPECT_EQ(m_RTData.GetLow(), 11440);
			EXPECT_EQ(m_RTData.GetBuy(), 11540);
			EXPECT_EQ(m_RTData.GetSell(), 11550);
			EXPECT_EQ(m_RTData.GetVolume(), 21606007);
			EXPECT_EQ(m_RTData.GetAmount(), 248901949);
			EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
			EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
			EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
			EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
			EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
			EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
			EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
			EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
			EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
			EXPECT_EQ(m_RTData.GetVSell(0), 259981);
			EXPECT_EQ(m_RTData.GetPSell(0), 11550);
			EXPECT_EQ(m_RTData.GetVSell(1), 206108);
			EXPECT_EQ(m_RTData.GetPSell(1), 11560);
			EXPECT_EQ(m_RTData.GetVSell(2), 325641);
			EXPECT_EQ(m_RTData.GetPSell(2), 11570);
			EXPECT_EQ(m_RTData.GetVSell(3), 215109);
			EXPECT_EQ(m_RTData.GetPSell(3), 11580);
			EXPECT_EQ(m_RTData.GetVSell(4), 262900);
			break;
		case 32: // 没有实时数据
			EXPECT_FALSE(m_RTData.IsActive());
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("000001.SZ"));
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票不是活跃股票
			break;
		case 40: // 没有实时数据的两段数据
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("000001.SZ"));
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票不是活跃股票
			break;
		default:
			break;
		}
		// 恢复原态
		TestSetUTCTime(tUTCTime);
	}
}
