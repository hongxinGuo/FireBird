#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"ProductNeteaseDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CProductNeteaseDayLineTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductNeteaseDayLine neteaseDayLine;
	};

	TEST_F(CProductNeteaseDayLineTest, TestInitialize) {
		EXPECT_STREQ(neteaseDayLine.GetInquiryFunction(), _T("http://quotes.money.163.com/service/chddata.html?code="));
	}

	TEST_F(CProductNeteaseDayLineTest, TestCreateMessage) {
		EXPECT_STREQ(neteaseDayLine.CreateMessage(), _T("http://quotes.money.163.com/service/chddata.html?code=")) << "�����������ݵ������ַ�����CNeteaseDayLineWebInquiry����ɣ���Product���趯��";
	}

	TEST_F(CProductNeteaseDayLineTest, TestParseAndStoreWebData) {
		const auto pData = make_shared<CWebData>();
		const CString strNeteaseDayLine = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n");
		pData->Test_SetBuffer_(strNeteaseDayLine);
		pData->SetStockCode(_T("600000.SS"));
		neteaseDayLine.ParseAndStoreWebData(pData);

		EXPECT_EQ(gl_pChinaMarket->DayLineSize(), 1);
		const auto pNeteaseDayLine = gl_pChinaMarket->PopDayLine();
		EXPECT_EQ(pNeteaseDayLine->GetStockCode(), _T("600000.SS"));

		const auto vDayLine = pNeteaseDayLine->GetProcessedDayLine();
		EXPECT_EQ(vDayLine.size(), 2);
		EXPECT_STREQ(vDayLine.at(0)->m_strStockSymbol, _T("600000.SS")) << "�������߲��������н�����Ʊ���룬����ֱ��ʹ�ñ���ѯ��Ʊ�Ĵ���";
		EXPECT_EQ(vDayLine.at(0)->m_lDate, 20190723) << "�������������ͣ����µ���������ǰ�档����������ݽ�����������";
		EXPECT_EQ(vDayLine.at(1)->m_lDate, 20190724);
	}
}