﻿#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"WebRTData.h"
#include"WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CWebRTDataTest1 : public Test {
	protected:
		static void SetUpTestSuite() {
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
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CWebRTDataTest1, TestTengxunInitialize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CWebRTData RTData;
		EXPECT_EQ(RTData.GetTimePoint().time_since_epoch().count(), 0);
		EXPECT_EQ(RTData.GetSymbol(), _T(""));
		EXPECT_EQ(RTData.GetStockName(), _T(""));
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

	TEST_F(CWebRTDataTest1, TestTengxunRTDataActive) {
		const time_t t = GetUTCTime();
		CWebRTData id;
		EXPECT_FALSE(id.CheckTengxunRTDataActive());
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 27; // 2019年11月27日是星期三。
		tm_.tm_hour = 12;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		const time_t tt = gl_pChinaMarket->TransferToUTCTime(&tm_);
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 7; // 2019年11月7日是星期三。
		tm_.tm_hour = 12;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		const time_t tt2 = gl_pChinaMarket->TransferToUTCTime(&tm_);
		TestSetUTCTime(tt);
		id.SetTransactionTime(tt2);
		EXPECT_FALSE(id.IsValidTime(14));
		EXPECT_FALSE(id.CheckTengxunRTDataActive()) << "High,Low,Open,Volume皆为零,且无效时间";
		id.SetOpen(10);
		EXPECT_FALSE(id.CheckTengxunRTDataActive()) << "无效时间";
		id.SetOpen(0);
		id.SetTransactionTime(tt);
		EXPECT_TRUE(id.IsValidTime(14));
		EXPECT_FALSE(id.CheckTengxunRTDataActive()) << "High,Low,Open,Volume皆为零";
		id.SetOpen(10);
		EXPECT_TRUE(id.CheckTengxunRTDataActive());
		id.SetOpen(0);
		id.SetVolume(10);
		EXPECT_TRUE(id.CheckTengxunRTDataActive());
		id.SetVolume(0);
		id.SetHigh(10);
		EXPECT_TRUE(id.CheckTengxunRTDataActive());
		id.SetHigh(0);
		id.SetLow(10);
		EXPECT_TRUE(id.CheckTengxunRTDataActive());

		// 恢复原状
		TestSetUTCTime(t);
	}

	struct TengxunRTData {
		TengxunRTData(int count, const string& Data) {
			m_iCount = count;
			m_strData = Data;
		}

	public:
		int m_iCount;
		string m_strData;
	};

	// 无错误数据
	TengxunRTData Data1(0, _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n"));
	// 无错误
	TengxunRTData Data2(1, _T("v_sz000001=\"51~方正科技~000001~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 所有的数量皆为零
	TengxunRTData Data3(2, _T("v_sh600601=\"1~方正科技~600601~0~0~0~0~0~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~-3689~3.49~-14.64~139.47~~~1.12~2905.90~0.00~0~~GP-A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data4(3, _T("v_sh600601=\"1~方正科技~600601~-3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data5(4, _T("v_sh600601=\"1~方正科技~600601~3.50~-3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data6(5, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~-3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data7(6, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~-83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data8(7, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~-3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data9(8, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~-2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data10(9, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~-3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data11(10, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~-2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data12(11, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~-3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data13(12, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~-2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data14(13, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~-3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data15(14, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~-760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data16(15, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~-3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data17(16, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~-1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data18(17, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~-3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data19(18, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~-2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data20(19, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~-3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data21(20, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~-2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data22(21, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~-3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data23(22, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~-1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data24(23, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~-3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data25(24, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~-2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data26(25, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~-3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data27(26, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~-3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data28(27, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data29(28, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data30(29, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data31(30, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 出现负值
	TengxunRTData Data32(31, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 没有实时数据
	TengxunRTData Data33(32, _T("v_sh600002=\"1~齐鲁退市~600002~0.00~0.00~0.00~0~0~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~~20191011091000~0.00~0.00~0.00~0.00~0.00 / 0 / 0~0~0~0.00~0.00~D~0.00~0.00~0.00~0.00~0.00~0.00~-1~-1~0.00~0~0.00~0.00~0.00~~~~0.00~0.00~0~~GP-A~~~0.00\";\n"));
	// 格式出错（）
	TengxunRTData Data35(34, _T("v=sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 格式出错（不是sh或者sz）
	TengxunRTData Data36(35, _T("v_sa600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 格式出错（不是sh或者sz）
	TengxunRTData Data37(36, _T("v_as600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 格式出错（股票代码后面不是'='号
	TengxunRTData Data38(37, _T("v_sh600601`\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 格式出错（'='好后面不是'"'号
	TengxunRTData Data39(38, _T("v_sh600601='1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 格式出错（股票代码后面不是'='号
	TengxunRTData Data40(39, _T("v_sh600601?\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
	// 两组正确的数据
	TengxunRTData Data41(40, _T("v_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\nv_sh600601=\"1~方正科技~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50/83346/29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));

	class CalculateTengxunWebRTDataTest : public::testing::TestWithParam<TengxunRTData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			const TengxunRTData* pData = GetParam();
			m_pTengxunWebRTData = make_shared<CWebData>();
			m_iCount = pData->m_iCount;
			m_lStringLength = pData->m_strData.length();
			m_pTengxunWebRTData->Resize(m_lStringLength);
			for (int i = 0; i < m_lStringLength; i++) {
				m_pTengxunWebRTData->SetData(i, pData->m_strData[i]);
			}
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

			svData = m_pTengxunWebRTData->GetCurrentTengxunData();
			m_pTengxunWebRTData->ResetCurrentPos();
		}

		void TearDown() override {
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_iCount;
		long m_lStringLength;
		CWebDataPtr m_pTengxunWebRTData;
		CWebRTData m_RTData;
		string_view svData;
	};

	INSTANTIATE_TEST_SUITE_P(CWebRTDataTest, CalculateTengxunWebRTDataTest, testing::Values(&Data1, &Data2, &Data3,
		                         &Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
		                         &Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
		                         &Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30,
		                         &Data31, &Data32, &Data33, &Data35, &Data36, &Data37, &Data38, &Data39, &Data40
	                         ));

	TEST_P(CalculateTengxunWebRTDataTest, TestParseTengxunData2) {
		m_RTData.ParseTengxunData(svData);
		time_t tTime;
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10 - 1;
		tm_.tm_mday = 11;
		tm_.tm_hour = 15; //
		tm_.tm_min = 58;
		tm_.tm_sec = 58;
		tTime = gl_pChinaMarket->TransferToUTCTime(&tm_);
		switch (m_iCount) {
		case 0:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("浦发银行"));
			EXPECT_EQ(m_RTData.GetOpen(), 12050);
			EXPECT_EQ(m_RTData.GetLastClose(), 11960);
			EXPECT_EQ(m_RTData.GetNew(), 12450);
			EXPECT_EQ(m_RTData.GetVolume(), 92030800);
			EXPECT_EQ(m_RTData.GetVBuy(0), 93800);
			EXPECT_EQ(m_RTData.GetPBuy(0), 12440);
			EXPECT_EQ(m_RTData.GetVBuy(1), 53500);
			EXPECT_EQ(m_RTData.GetPBuy(1), 12430);
			EXPECT_EQ(m_RTData.GetVBuy(2), 43500);
			EXPECT_EQ(m_RTData.GetPBuy(2), 12420);
			EXPECT_EQ(m_RTData.GetVBuy(3), 78400);
			EXPECT_EQ(m_RTData.GetPBuy(3), 12410);
			EXPECT_EQ(m_RTData.GetVBuy(4), 116700);
			EXPECT_EQ(m_RTData.GetPBuy(4), 12400);
			EXPECT_EQ(m_RTData.GetVSell(0), 49400);
			EXPECT_EQ(m_RTData.GetPSell(0), 12450);
			EXPECT_EQ(m_RTData.GetVSell(1), 939700);
			EXPECT_EQ(m_RTData.GetPSell(1), 12460);
			EXPECT_EQ(m_RTData.GetVSell(2), 515600);
			EXPECT_EQ(m_RTData.GetPSell(2), 12470);
			EXPECT_EQ(m_RTData.GetVSell(3), 747300);
			EXPECT_EQ(m_RTData.GetPSell(3), 12480);
			EXPECT_EQ(m_RTData.GetVSell(4), 551300);
			EXPECT_EQ(m_RTData.GetPSell(4), 12490);
			EXPECT_EQ(m_RTData.GetHighLimitFromTengxun(), 13160);
			EXPECT_EQ(m_RTData.GetLowLimitFromTengxun(), 10760);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 1:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("000001.SZ"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			EXPECT_EQ(m_RTData.GetVSell(4), 357700);
			EXPECT_EQ(m_RTData.GetPSell(4), 3550);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			break;
		case 2:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			break;
		case 3:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetHigh(), 3530);
			EXPECT_EQ(m_RTData.GetLow(), 3430);
			EXPECT_EQ(m_RTData.GetNew(), -3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetAmount(), 29058955);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			EXPECT_EQ(m_RTData.GetPSell(4), 3550);
			EXPECT_EQ(m_RTData.GetTimePoint().time_since_epoch().count(), tTime);
			EXPECT_EQ(m_RTData.GetCurrentValue(), 7682000000);
			EXPECT_EQ(m_RTData.GetTotalValue(), 7682000000);
			EXPECT_EQ(m_RTData.GetHighLimitFromTengxun(), 3810);
			EXPECT_EQ(m_RTData.GetLowLimitFromTengxun(), 3110);
			break;
		case 4:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), -3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			EXPECT_EQ(m_RTData.GetPSell(4), 3550);
			EXPECT_EQ(m_RTData.GetHighLimitFromTengxun(), 3810);
			EXPECT_EQ(m_RTData.GetLowLimitFromTengxun(), 3110);
			break;
		case 5:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), -3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			EXPECT_EQ(m_RTData.GetPSell(4), 3550);
			break;
		case 6:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			break;
		case 7:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			break;
		case 8:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			break;
		case 9:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			break;
		case 12:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			break;
		case 13:
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			break;
		case 14:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			break;
		case 16:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			break;
		case 17:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			break;
		case 19:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			break;
		case 20:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			break;
		case 21:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			break;
		case 22:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			break;
		case 23:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			break;
		case 24:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			break;
		case 25:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			break;
		case 26:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600601.SS"));
			EXPECT_EQ(m_RTData.GetStockName(), _T("方正科技"));
			EXPECT_EQ(m_RTData.GetOpen(), 3470);
			EXPECT_EQ(m_RTData.GetLastClose(), 3460);
			EXPECT_EQ(m_RTData.GetNew(), 3500);
			EXPECT_EQ(m_RTData.GetVolume(), 8334600);
			EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
			EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
			EXPECT_EQ(m_RTData.GetVBuy(1), 218900);
			EXPECT_EQ(m_RTData.GetPBuy(1), 3490);
			EXPECT_EQ(m_RTData.GetVBuy(2), 209300);
			EXPECT_EQ(m_RTData.GetPBuy(2), 3480);
			EXPECT_EQ(m_RTData.GetVBuy(3), 76000);
			EXPECT_EQ(m_RTData.GetPBuy(3), 3470);
			EXPECT_EQ(m_RTData.GetVBuy(4), 113200);
			EXPECT_EQ(m_RTData.GetPBuy(4), 3460);
			EXPECT_EQ(m_RTData.GetVSell(0), 222400);
			EXPECT_EQ(m_RTData.GetPSell(0), 3510);
			EXPECT_EQ(m_RTData.GetVSell(1), 284800);
			EXPECT_EQ(m_RTData.GetPSell(1), 3520);
			EXPECT_EQ(m_RTData.GetVSell(2), 141100);
			EXPECT_EQ(m_RTData.GetPSell(2), 3530);
			EXPECT_EQ(m_RTData.GetVSell(3), 202600);
			EXPECT_EQ(m_RTData.GetPSell(3), 3540);
			EXPECT_EQ(m_RTData.GetPSell(4), 3550);
			break;
		case 27:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			break;
		case 28:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			break;
		case 29:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			break;
		case 30:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			break;
		case 31:
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			break;
		case 32: // 没有实时数据
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos());
			EXPECT_EQ(m_RTData.GetSymbol(), _T("600002.SS"));
			EXPECT_EQ(m_RTData.GetHighLimitFromTengxun(), -1000); // 实际值为-1，不存储，故而仍是初始值0
			EXPECT_EQ(m_RTData.GetLowLimitFromTengxun(), -1000); // 实际值为-1， 不存储，故而仍是初始值0
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票不是活跃股票
			break;
		case 34: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 35: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 36: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 37: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 38: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 39: // 有错误，前缀出错
			EXPECT_EQ(0, m_pTengxunWebRTData->GetCurrentPos()) << "错误处理时读至结尾";
			EXPECT_FALSE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		case 40:  // 两组正确的数据
			EXPECT_GT(0, m_pTengxunWebRTData->GetCurrentPos());  // 此时只读了一组数据，
			EXPECT_TRUE(m_RTData.IsActive()); // 此股票是活跃股票
			m_pTengxunWebRTData->IncreaseCurrentPos(svData.length());
			svData = m_pTengxunWebRTData->GetCurrentTengxunData();
			m_RTData.ParseTengxunData(svData);
			EXPECT_TRUE(m_RTData.IsActive()); // 此股票是活跃股票
			break;
		default:
			break;
		}
	}
}
