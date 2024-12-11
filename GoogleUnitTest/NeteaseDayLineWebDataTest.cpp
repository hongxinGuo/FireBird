#include"pch.h"

 

import FireBird.Market.China;
#include"GeneralCheck.h"

import FireBird.Type.DayLineWebData;

namespace FireBirdTest {
	class CDownLoadedNeteaseDayLineTest : public testing::Test {
	protected:
		static void SetUpTestSuite() { // ��������ĳ�ʼ������
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
	};

	TEST_F(CDownLoadedNeteaseDayLineTest, TestSkipNeteaseDayLineFirstInformationLine) {
		CDayLineWebData DayLine;
		INT64 lCurrentPos = 0;

		string str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n");
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_TRUE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\n"); // ȱ��\r
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r"); // ȱ��\n
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ"); // ȱ��\r
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;
	}
};
