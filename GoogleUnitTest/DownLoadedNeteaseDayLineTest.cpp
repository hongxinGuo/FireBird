#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"DownLoadedNeteaseDayLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // ����������ʷ����

	class CDownLoadedNeteaseDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // ��������ĳ�ʼ������
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();
		}
	};

	TEST_F(CDownLoadedNeteaseDayLineTest, TestSkipNeteaseDayLineFirstInformationLine) {
		CNeteaseDayLineWebInquiry DayLineWebInquiry;
		CDownLoadedNeteaseDayLine DayLine;

		CString str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n");
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteReaded(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLineWebInquiry.SetData(str.GetLength(), 0x000);
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_TRUE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\n"); // ȱ��\r
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteReaded(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLineWebInquiry.SetData(str.GetLength(), 0x000);
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r"); // ȱ��\n
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteReaded(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLineWebInquiry.SetData(str.GetLength(), 0x000);
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ"); // ȱ��\r��ֱ������0x000
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteReaded(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLineWebInquiry.SetData(str.GetLength(), 0x000);
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);
	}

	TEST_F(CDownLoadedNeteaseDayLineTest, TestTransferNeteaseDayLineWebDataToBuffer) {
		CNeteaseDayLineWebInquiry DayLineWebInquiry;
		CDownLoadedNeteaseDayLine DayLine;
		CString str = _T("abcedfg\r\n");
		EXPECT_EQ(DayLine.GetDayLineBufferLength(), 0);
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteReaded(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_EQ(DayLine.GetDayLineBufferLength(), str.GetLength());
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
	}
};