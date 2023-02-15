#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"

#include<memory>

#include "WebRTData.h"
using std::make_shared;

namespace FireBirdTest {
	class jsonParseTest : public testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann1) {
		json js;
		string s{_T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}")};
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg֮ǰȱ���ַ�':'
		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann2) {
		json js;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // �ų�ǰ���NoUse�ͺ����NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann3) {
		json js;
		string s{_T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo")};

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // �ų�ǰ���NoUse�ͺ����NoUseToo

		EXPECT_TRUE(js.empty());

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann4) {
		json js;
		CString s = _T("{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s));
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));
		s = _T("{\"eventName\":\"subscribe\",\"authorization\"\"abcdefg\"}"); // abcdefg֮ǰȱ���ַ�':'

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann5) {
		json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");
		EXPECT_TRUE(CreateJsonWithNlohmann(js, s, 5, 8)); // �ų�ǰ���NoUse�ͺ����NoUseToo
		const string sSubscribe = js.at((_T("eventName")));
		EXPECT_STREQ(sSubscribe.c_str(), _T("subscribe"));

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestCreateJsonWithNlohmann6) {
		json js;
		CString s = _T("NoUse{\"eventName\":\"subscribe\",\"authorization\":\"abcdefg\"}NoUseToo");

		EXPECT_FALSE(CreateJsonWithNlohmann(js, s, 0, 0)); // �ų�ǰ���NoUse�ͺ����NoUseToo

		EXPECT_TRUE(js.empty());

		while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	TEST_F(jsonParseTest, TestParseSinaRTData1) {
		// ��ȷ����������
		const CString strData = _T("var hq_str_sh600000=\"�ַ�����,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\nvar hq_str_sh600001=\"�ַ�����,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseSinaRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 2);
		EXPECT_STREQ(vData->at(0)->GetSymbol(), _T("600000.SS"));
		EXPECT_STREQ(vData->at(1)->GetSymbol(), _T("600001.SS"));
	}

	TEST_F(jsonParseTest, TestParseSinaRTData2) {
		// ��һ��������ȷ���ڶ��������޷�����
		const CString strData = _T("var hq_str_sh600000=\"�ַ�����,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\nvar hq_str_sh600001=\"�ַ�����,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseSinaRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 1);
		EXPECT_STREQ(vData->at(0)->GetSymbol(), _T("600000.SS"));

		// �ָ�ԭ״
		while (gl_systemMessage.ErrorMessageSize() > 0) {
			gl_systemMessage.PopErrorMessage();
		}
	}

	TEST_F(jsonParseTest, TestParseSinaRTData3) {
		// ��һ�������޷��������ڶ���������ȷ
		const CString strData = _T("var hq_str_sh600000=\"�ַ�����,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\nvar hq_str_sh600001=\"�ַ�����,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseSinaRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 0);

		// �ָ�ԭ״
		while (gl_systemMessage.ErrorMessageSize() > 0) {
			gl_systemMessage.PopErrorMessage();
		}
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData1) {
		// ��ȷ����������
		const CString strData = _T("v_sh600000=\"1~�ַ�����~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~�ַ�����~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseTengxunRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 2);
		EXPECT_STREQ(vData->at(0)->GetSymbol(), _T("600000.SS"));
		EXPECT_STREQ(vData->at(1)->GetSymbol(), _T("600001.SS"));
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData2) {
		// ��Ч����
		const CString strData = _T("v_pv_none_match=\"1\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseTengxunRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 0);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData3) {
		// ��һ�����ݲ���ȷ
		const CString strData = _T("v_zz600000=\"1~�ַ�����~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~�ַ�����~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseTengxunRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 0);
		EXPECT_GT(gl_systemMessage.ErrorMessageSize(), 0);

		// �ָ�ԭ״
		while (gl_systemMessage.ErrorMessageSize() > 0) {
			gl_systemMessage.PopErrorMessage();
		}
	}

	TEST_F(jsonParseTest, TestParseTengxunRTData4) {
		// �ڶ������ݲ���ȷ
		const CString strData = _T("v_sh600000=\"1~�ַ�����~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_zz600001=\"1~�ַ�����~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);

		const auto vData = ParseTengxunRTData(pData);

		EXPECT_TRUE(vData != nullptr);
		EXPECT_EQ(vData->size(), 1);
		EXPECT_STREQ(vData->at(0)->GetSymbol(), _T("600000.SS"));
		EXPECT_GT(gl_systemMessage.ErrorMessageSize(), 0);

		// �ָ�ԭ״
		while (gl_systemMessage.ErrorMessageSize() > 0) {
			gl_systemMessage.PopErrorMessage();
		}
	}

	TEST_F(jsonParseTest, TestParseTengxunDayLine) {
		const string sTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		auto pjs = make_shared<json>();
		EXPECT_TRUE(CreateJsonWithNlohmann(*pjs, sTengxunDayLine, 0, 0));
		shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(pjs.get(), _T("sh600601"));

		EXPECT_EQ(pvDayLine->size(), 2);
		EXPECT_EQ(pvDayLine->at(0)->GetMarketDate(), 20230119);
		EXPECT_EQ(pvDayLine->at(0)->GetOpen(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetClose(), 2600);
		EXPECT_EQ(pvDayLine->at(0)->GetHigh(), 2610);
		EXPECT_EQ(pvDayLine->at(0)->GetLow(), 2550);
		EXPECT_EQ(pvDayLine->at(0)->GetLastClose(), 0) << "��һ������û������������";
		EXPECT_EQ(pvDayLine->at(0)->GetVolume(), 8616200);
		EXPECT_EQ(pvDayLine->at(1)->GetMarketDate(), 20230120);
		EXPECT_EQ(pvDayLine->at(1)->GetOpen(), 2600);
		EXPECT_EQ(pvDayLine->at(1)->GetClose(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetHigh(), 2620);
		EXPECT_EQ(pvDayLine->at(1)->GetLow(), 2590);
		EXPECT_EQ(pvDayLine->at(1)->GetLastClose(), 2600) << "�ڶ�������ʹ��ǰһ�����������";
		EXPECT_EQ(pvDayLine->at(1)->GetVolume(), 10073500);
	}
}
