#include"pch.h"

//#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"WebData.h"
#include"DayLineWebData.h"

namespace FireBirdTest {
	struct NeteaseDayLineData {
		NeteaseDayLineData(int count, const CString& Symbol, const CString& Data) {
			m_iCount = count;
			m_strSymbol = Symbol;
			m_strData = Data;
		}

	public:
		int m_iCount;
		CString m_strSymbol;
		CString m_strData;
	};

	NeteaseDayLineData Data1(1, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NeteaseDayLineData Data2(2, _T("000001.SZ"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'000001,ƽ������,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NeteaseDayLineData Data3(3, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NeteaseDayLineData Data4(4, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NeteaseDayLineData Data5(5, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	NeteaseDayLineData Data6(6, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// ���й�Ʊ�������������ڵ�ǰ����30�죩
	NeteaseDayLineData Data7(7, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2018-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// ֻ�б�ͷ
	NeteaseDayLineData Data8(8, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n"));
	NeteaseDayLineData Data9(9, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-12-02,'600000,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n2019-12-03,'600000,��ֵ7030,3658.9802,None,None,None,3654.1602,4.82,,None,None,,\r\n"));
	// ����ʱ���ַ�������31���ַ�
	NeteaseDayLineData Data10(10, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-abckderjddfkjdasdfjdkj07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// ���󣺵�ǰ��ͨ��ֵ�ַ�������31��
	NeteaseDayLineData Data11(11, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.22912121236476736447734872e+11\r\n"));
	// ���ڶ��ź��ǵ�����(')
	NeteaseDayLineData Data12(12, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,,600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// ��;����\n
	NeteaseDayLineData Data13(13, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\n,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));
	// ��;����\r
	NeteaseDayLineData Data14(14, _T("600000.SS"), _T("����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23\r,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"));

	class NeteaseDayLineTest : public::testing::TestWithParam<NeteaseDayLineData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			const NeteaseDayLineData* pData = GetParam();
			const auto pWebData = make_shared<CWebData>();
			pWebData->Test_SetBuffer_(pData->m_strData);

			pDownLoadedDayLine = make_shared<CDayLineWebData>();
			pDownLoadedDayLine->SetStockCode(pData->m_strSymbol);
			pDownLoadedDayLine->TransferWebDataToBuffer(pWebData);
			m_iCount = pData->m_iCount;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_iCount;
		CDayLineWebDataPtr pDownLoadedDayLine;
		long lDate;
	};

	INSTANTIATE_TEST_SUITE_P(TestNetEaseDayLineData, NeteaseDayLineTest,
	                         testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8,
		                         &Data9, &Data10, &Data11, &Data12, &Data13, &Data14
	                         ));

	TEST_P(NeteaseDayLineTest, TestProcessNeteaseDayLineData) {
		const bool fSucceed = pDownLoadedDayLine->ProcessNeteaseDayLineData();
		switch (m_iCount) {
		case 1:
			EXPECT_TRUE(fSucceed);
			break;
		case 2:
			EXPECT_TRUE(fSucceed);
			break;
		case 3:
			EXPECT_TRUE(fSucceed);
			break;
		case 4:
		case 5:
			EXPECT_TRUE(fSucceed);
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			break;
		case 8: // ��Ч��Ʊ���룬ֻ�б�ͷ
			EXPECT_FALSE(fSucceed);
			break;
		case 9:
			EXPECT_TRUE(fSucceed);
			break;
		case 10: // ʱ���ַ�������30��
			EXPECT_TRUE(fSucceed);
			break;
		case 11: // ��ͨ��ֵ�ַ�������30��
			EXPECT_TRUE(fSucceed);
			break;
		case 12:
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}
}
