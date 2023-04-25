#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h" // 网易日线历史数据的读取在CChinaMarket类中。

#include"DayLine.h"
#include"DayLineWebData.h"

#include<vector>

namespace FireBirdTest {
	struct NetEaseDayLineData {
		NetEaseDayLineData(int count, const CString& Data) {
			m_iCount = count;
			m_strData = Data;
		}

	public:
		int m_iCount;
		CString m_strData;
	};

	NetEaseDayLineData Data1(1, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data2(2, _T("2019-07-23,'000001,平安银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data3(3, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data4(4, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data5(5, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data6(6, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data7(7, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data8(8, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NetEaseDayLineData Data9(9, _T("2019-12-02,'000834,价值7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
	// 错误：时间字符串超过31个字符
	NetEaseDayLineData Data10(10, _T("2019-abckderjddfkjdasdfjdkj07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// 错误：当前流通市值字符串超过31个
	NetEaseDayLineData Data11(11, _T("2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
	// 日期逗号后不是单引号(')
	NetEaseDayLineData Data12(12, _T("2019-07-23,,600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// 中途遇到\n
	NetEaseDayLineData Data13(13, _T("2019-07-23\n,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// 中途遇到\r
	NetEaseDayLineData Data14(14, _T("2019-07-23\r,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// 缺少项
	NetEaseDayLineData Data15(15, _T("2019-12-02,'000834,价值7030,,None,None,None,3654.1602,4.82,,None,None,,\r\n"));

	class ProcessNeteaseDayLineTest : public::testing::TestWithParam<NetEaseDayLineData*> {
	protected:
		void SetUp() override {
			GeneralCheck();
			const NetEaseDayLineData* pData = GetParam();
			m_iCount = pData->m_iCount;
			const long lLength = pData->m_strData.GetLength();
			m_pData.resize(lLength);
			for (int i = 0; i < lLength; i++) {
				m_pData.at(i) = pData->m_strData.GetAt(i);
			}
			m_lCountPos = 0;

			m_DayLine.SetAmount(-1);
			m_DayLine.SetVolume(-1);
			m_DayLine.SetOpen(-1);
			m_DayLine.SetLastClose(-1);
			m_DayLine.SetHigh(-1);
			m_DayLine.SetLow(-1);

			m_DayLinePtr = make_shared<CDayLine>();
			m_DayLinePtr->SetClose(-1);
			m_DayLinePtr->SetHigh(-1);
			m_DayLinePtr->SetLow(-1);
			m_DayLinePtr->SetOpen(-1);
			m_DayLinePtr->SetLastClose(-1);
			m_DayLinePtr->SetUpDown(-1);
			m_DayLinePtr->SetUpDownRate(-1);
			m_DayLinePtr->SetVolume(-1);
			m_DayLinePtr->SetAmount(-1);
			m_DayLinePtr->SetTotalValue(-1);
			m_DayLinePtr->SetCurrentValue(-1);

			pWebData = make_shared<CWebData>();
			pWebData->Test_SetBuffer_(pData->m_strData);
			pDownLoadedDayLine = make_shared<CDayLineWebData>();
			pDownLoadedDayLine->TransferWebDataToBuffer(pWebData);
			pDownLoadedDayLine->SetStockCode(_T("600000.SS"));
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	public:
		int m_iCount;
		vector<char> m_pData;
		INT64 m_lCountPos = 0;
		CDayLine m_DayLine;
		CDayLinePtr m_DayLinePtr;

		CWebDataPtr pWebData;
		CDayLineWebDataPtr pDownLoadedDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestNetEaseDayLineData, ProcessNeteaseDayLineTest,
	                         testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8,
		                         &Data9, &Data10, &Data11, &Data12, &Data13, &Data14
	                         ));

	TEST_P(ProcessNeteaseDayLineTest, ProcessOneNeteaseDayLineData) {
		m_DayLinePtr = pDownLoadedDayLine->ProcessOneNeteaseDayLineData();
		switch (m_iCount) {
		case 1:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			EXPECT_STREQ(m_DayLinePtr->GetStockSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_DayLinePtr->GetDisplaySymbol(), _T("浦发银行"));
			EXPECT_EQ(m_DayLinePtr->GetClose(), 11490);
			EXPECT_EQ(m_DayLinePtr->GetHigh(), 11560);
			EXPECT_EQ(m_DayLinePtr->GetLow(), 11430);
			EXPECT_EQ(m_DayLinePtr->GetOpen(), 11430);
			EXPECT_EQ(m_DayLinePtr->GetLastClose(), 11480);
			break;
		case 2:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 3:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 4:
		case 5:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 6:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 7:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 8:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			break;
		case 9:
			EXPECT_TRUE(m_DayLinePtr != nullptr);
			EXPECT_STREQ(m_DayLinePtr->GetDisplaySymbol(), _T("价值7030"));
			EXPECT_EQ(m_DayLinePtr->GetClose(), 3658980);
			EXPECT_EQ(m_DayLinePtr->GetLastClose(), 3654160);
			EXPECT_EQ(m_DayLinePtr->GetHigh(), 0);
			EXPECT_EQ(m_DayLinePtr->GetLow(), 0);
			EXPECT_EQ(m_DayLinePtr->GetOpen(), 0);
			EXPECT_EQ(m_DayLinePtr->GetVolume(), 0);
			EXPECT_EQ(m_DayLinePtr->GetAmount(), 0);
			break;
		case 10: // 时间字符串超过30个
			EXPECT_TRUE(m_DayLinePtr == nullptr);
			break;
		case 11: // 流通市值字符串超过30个
			EXPECT_TRUE(m_DayLinePtr == nullptr);
			break;
		case 12:
			EXPECT_TRUE(m_DayLinePtr == nullptr);
			break;
		default:
			break;
		}
	}

	struct ReadDayLineOneValueData {
		ReadDayLineOneValueData(int count, const CString& Data) {
			m_iCount = count;
			m_strData = Data;
		}

	public:
		int m_iCount;
		CString m_strData;
	};

	// 成功
	ReadDayLineOneValueData rdata1(1, _T("11.050,"));
	// 小数点后两位
	ReadDayLineOneValueData rdata2(2, _T("11.05,"));
	// 小数点后一位
	ReadDayLineOneValueData rdata3(3, _T("11.0,"));
	// 小数点前出现0x00a
	ReadDayLineOneValueData rdata4(4, _T("1\n1.050,"));
	// 小数点后出现0x00a
	ReadDayLineOneValueData rdata5(5, _T("11.0\n50,"));
	// 缺少','
	ReadDayLineOneValueData rdata6(6, _T("11.050"));
	// 读取小数点后三位后，放弃其后多余的数值
	ReadDayLineOneValueData rdata7(7, _T("11.050000,"));
	// 0x00a出现于‘，’前。
	ReadDayLineOneValueData rdata8(8, _T("11.05000\n,"));
	// 成功
	ReadDayLineOneValueData rdata9(9, _T("技术领先,8864.664"));
	// 成功
	ReadDayLineOneValueData rdata10(10, _T("none, 8864.664"));

	class ReadDayLineOneValueTest2 : public::testing::TestWithParam<ReadDayLineOneValueData*> {
	protected:
		void SetUp() override {
			GeneralCheck();
			ReadDayLineOneValueData* pData = GetParam();
			m_iCount = pData->m_iCount;
			long lLength = pData->m_strData.GetLength();
			m_pData.resize(lLength);
			for (int i = 0; i < lLength; i++) {
				m_pData[i] = pData->m_strData[i];
			}
			m_lCountPos = 0;
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	public:
		int m_iCount;
		string m_pData;
		INT64 m_lCountPos = 0;
		CDayLineWebData NeteaseData;
	};

	INSTANTIATE_TEST_SUITE_P(TestReadDayLineOneValue, ReadDayLineOneValueTest2,
	                         testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8, &rdata9, &rdata10
	                         ));

	TEST_P(ReadDayLineOneValueTest2, TestReadOneValue3) {
		char buffer[30];
		bool fSucceed = NeteaseData.ReadOneValueOfNeteaseDayLine(m_pData, buffer, m_lCountPos);
		CString str;
		str = buffer;
		switch (m_iCount) {
		case 1:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_lCountPos, 7);
			EXPECT_STREQ(str, _T("11.050"));
			break;
		case 2:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_lCountPos, 6);
			EXPECT_STREQ(str, _T("11.05"));
			break;
		case 3:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_lCountPos, 5);
			EXPECT_STREQ(str, _T("11.0"));
			break;
		case 4:
			EXPECT_FALSE(fSucceed);
			break;
		case 5:
			EXPECT_FALSE(fSucceed);
			break;
		case 6:
			EXPECT_FALSE(fSucceed);
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_lCountPos, 10);
			EXPECT_STREQ(str, _T("11.050000"));
			break;
		case 8:
			EXPECT_FALSE(fSucceed);
			break;
		case 9:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(str, _T("技术领先"));
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(str, _T("none"));
			break;
		default:
			break;
		}
	}
}
