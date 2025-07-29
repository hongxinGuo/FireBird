#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

namespace FireBirdTest {
	class CWebDataTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(webData.GetCurrentPos(), 0);
			EXPECT_EQ(webData.GetTime(), 0);
			EXPECT_EQ(webData.GetBufferLength(), 0) << "初始化时未操作此变量";
			webData.Resize(1024 * 1024); // 设置1M存储空间
			webData.ResetCurrentPos();
			EXPECT_EQ(webData.GetCurrentPos(), 0);

			const string sSinaWebData = _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\nvar hq_str_sh600601=\"方正科技,2.610,2.610,2.650,2.680,2.590,2.650,2.660,28791397,76170308.000,440800,2.650,709200,2.640,760400,2.630,564900,2.620,356900,2.610,913800,2.660,994000,2.670,684700,2.680,462100,2.690,531600,2.700,2024-01-19,15:00:00,00,\";\n");
			sinaWebData.Test_SetBuffer_(sSinaWebData);
			sinaWebData.ResetCurrentPos();

			const string sBadSinaWebData = _T("ar hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n");
			sinaWebData2.Test_SetBuffer_(sBadSinaWebData);
			sinaWebData2.ResetCurrentPos();
			const string sBadSinaWebData2 = _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\"b\n");
			sinaWebData3.Test_SetBuffer_(sBadSinaWebData2);
			sinaWebData3.ResetCurrentPos();
			const string sBadSinaWebData3 = _T("aar hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\nvar hq_str_sh600601=\"方正科技,2.610,2.610,2.650,2.680,2.590,2.650,2.660,28791397,76170308.000,440800,2.650,709200,2.640,760400,2.630,564900,2.620,356900,2.610,913800,2.660,994000,2.670,684700,2.680,462100,2.690,531600,2.700,2024-01-19,15:00:00,00,\";\n");
			sinaWebData4.Test_SetBuffer_(sBadSinaWebData3);
			sinaWebData3.ResetCurrentPos();

			const string sTengxunWebData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600601=\"1~浦发银行~600601~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.82\";\n");
			tengxunWebData.Test_SetBuffer_(sTengxunWebData);
			tengxunWebData.ResetCurrentPos();

			const string sBadTengxunWebData = _T("a_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600601=\"1~浦发银行~600601~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.82\";\n");
			tengxunWebData2.Test_SetBuffer_(sBadTengxunWebData);
			tengxunWebData2.ResetCurrentPos();
			const string sBadTengxunWebData2 = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\"a\n");
			tengxunWebData3.Test_SetBuffer_(sBadTengxunWebData2);
			tengxunWebData3.ResetCurrentPos();
			const string sBadTengxunWebData3 = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\"a\n");
			tengxunWebData4.Test_SetBuffer_(sBadTengxunWebData3);
			tengxunWebData4.ResetCurrentPos();
		}

		void TearDown() override {
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CWebData webData;
		CWebData sinaWebData;
		CWebData sinaWebData2;
		CWebData sinaWebData3;
		CWebData sinaWebData4;

		CWebData tengxunWebData;
		CWebData tengxunWebData2;
		CWebData tengxunWebData3;
		CWebData tengxunWebData4;
	};

	TEST_F(CWebDataTest, TestOutOfRange) {
		webData.Resize(100);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.OutOfRange());
		webData.SetCurrentPos(99);
		EXPECT_FALSE(webData.OutOfRange());
		webData.SetCurrentPos(100);
		EXPECT_TRUE(webData.OutOfRange());
	}

	TEST_F(CWebDataTest, TestIsLastDataParagraph) {
		webData.Resize(100);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.IsLastDataParagraph());
		webData.SetCurrentPos(97);
		EXPECT_FALSE(webData.IsLastDataParagraph());
		webData.SetCurrentPos(98);
		EXPECT_TRUE(webData.IsLastDataParagraph());
		webData.SetCurrentPos(99);
		EXPECT_TRUE(webData.IsLastDataParagraph());
		webData.SetCurrentPos(100);
		EXPECT_TRUE(webData.IsLastDataParagraph());
	}

	TEST_F(CWebDataTest, TestGetTime) {
		EXPECT_EQ(webData.GetTime(), 0);
		webData.SetTime(101010101010);
		EXPECT_EQ(webData.GetTime(), 101010101010);
	}

	TEST_F(CWebDataTest, TestGetStockCode) {
		EXPECT_TRUE(webData.GetStockCode().compare( _T("")) == 0);
		webData.SetStockCode(_T("abcdefg"));
		EXPECT_TRUE(webData.GetStockCode().compare( _T("abcdefg")) == 0);
	}
	TEST_F(CWebDataTest, TestGetCurrentPos) {
		EXPECT_EQ(webData.GetCurrentPos(), 0);
		webData.IncreaseCurrentPos(5);
		EXPECT_EQ(webData.GetCurrentPos(), 5);
	}

	TEST_F(CWebDataTest, TestIncreaseCurrentPos) {
		webData.IncreaseCurrentPos();
		EXPECT_EQ(webData.GetCurrentPos(), 1);
		webData.IncreaseCurrentPos(10);
		EXPECT_EQ(webData.GetCurrentPos(), 11);
	}

	TEST_F(CWebDataTest, TestGetData1) {
		char buffer[10];

		EXPECT_EQ(webData.GetBufferLength(), 1024 * 1024);
		EXPECT_EQ(webData.GetCurrentPos(), 0);
		webData.SetCurrentPos(1024 * 1024 - 9);
		EXPECT_FALSE(webData.GetData(buffer, 10)) << "越界";
	}

	TEST_F(CWebDataTest, TestGetData2) {
		webData.SetData(0, 'a');
		EXPECT_EQ(webData.GetData(0), 'a');
		webData.SetData(10, 'a');
		EXPECT_EQ(webData.GetData(10), 'a');
	}

	TEST_F(CWebDataTest, TestGetData4) {
		string strTest = "abcdefg";
		webData.SetCurrentPos(0);
		EXPECT_TRUE(webData.SetData(strTest.c_str(), strTest.length()));
		char buffer[8];
		EXPECT_TRUE(webData.GetData(buffer, 7));
		buffer[7] = 0x000;
		string strTest2 = buffer;
		EXPECT_EQ(strTest, strTest2);

		webData.SetCurrentPos(1000);
		EXPECT_TRUE(webData.SetData(strTest.c_str(), strTest.length()));
		EXPECT_TRUE(webData.GetData(buffer, 7));
		buffer[7] = 0x000;
		strTest2 = buffer;
		EXPECT_EQ(strTest, strTest2);

		webData.Resize(6);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.SetData(strTest.c_str(), strTest.length())) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
	}

	TEST_F(CWebDataTest, TestGetCurrentPosData) {
		webData.SetCurrentPos(1);
		webData.SetData(1, 'a');
		EXPECT_EQ(webData.GetCurrentPosData(), 'a');
		webData.SetCurrentPos(2);
		webData.SetCurrentPosData('b');
		EXPECT_EQ(webData.GetCurrentPosData(), 'b');
	}

	TEST_F(CWebDataTest, TestGetCurrentSinaData) {
		string_view sv = sinaWebData.GetCurrentSinaData();

		EXPECT_EQ(sv.at(0), 'v');
		EXPECT_EQ(sv.at(11), 's') << "第一个数据的股票代码是sh600000";
		EXPECT_EQ(sv.at(18), '0') << "第一个数据的股票代码是sh600000";
		EXPECT_EQ(sv.at(sv.length() - 1), ';') << "新浪实时数据以字符';'结束";

		sv = sinaWebData.GetCurrentSinaData();

		EXPECT_EQ(sv.at(0), 'v');
		EXPECT_EQ(sv.at(11), 's') << "第一个数据的股票代码是sh600000";
		EXPECT_EQ(sv.at(18), '1') << "第二个数据的股票代码是sh600601";
		EXPECT_EQ(sv.at(sv.length() - 1), ';') << "新浪实时数据以字符';'结束";
	}

	TEST_F(CWebDataTest, TestGetCurrentTengxunData) {
		string_view sv = tengxunWebData.GetCurrentTengxunData();

		EXPECT_EQ(sv.at(0), 'v');
		EXPECT_EQ(sv.at(9), '0') << "第一个数据的股票代码是sh600000";
		EXPECT_EQ(sv.at(sv.length() - 1), ';') << "腾讯实时数据以字符';'结束";

		sv = tengxunWebData.GetCurrentTengxunData();

		EXPECT_EQ(sv.at(0), 'v');
		EXPECT_EQ(sv.at(9), '1') << "第一个数据的股票代码是sh600000";
		EXPECT_EQ(sv.at(sv.length() - 1), ';') << "腾讯实时数据以字符';'结束";
	}
}
