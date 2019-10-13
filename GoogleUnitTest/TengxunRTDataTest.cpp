#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"RTData.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(CStockRTDataTest, TestTengxunInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockRTData RTData;
    EXPECT_EQ(RTData.GetTransactionTime(), 0);
    EXPECT_EQ(RTData.GetMarket(), 0);
    EXPECT_STREQ(RTData.GetStockCode(), _T(""));
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
  struct TengxunRTData {
    TengxunRTData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // �޴�������
  TengxunRTData Data1(0, _T("v_sh600000=\"1~�ַ�����~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155938~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";"));
  // ���еļ۸��Ϊ0
  TengxunRTData Data2(1, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���е�������Ϊ��
  TengxunRTData Data3(2, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data4(3, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data5(4, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data6(5, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data7(6, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data8(7, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data9(8, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data10(9, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data11(10, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data12(11, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data13(12, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data14(13, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data15(14, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data16(15, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data17(16, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data18(17, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data19(18, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data20(19, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data21(20, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data22(21, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data23(22, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data24(23, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data25(24, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data26(25, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data27(26, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data28(27, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data29(28, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data30(29, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data31(30, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ���ָ�ֵ
  TengxunRTData Data32(31, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // û��ʵʱ����
  TengxunRTData Data33(32, _T("v_sh600002=\"1~��³����~600002~0.00~0.00~0.00~0~0~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~~20191011091000~0.00~0.00~0.00~0.00~0.00 / 0 / 0~0~0~0.00~0.00~D~0.00~0.00~0.00~0.00~0.00~0.00~-1~-1~0.00~0~0.00~0.00~0.00~~~~0.00~0.00~0~~GP-A~~~0.00\";"));
  // ��ʽ����ǰ׺��
  TengxunRTData Data34(33, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ������
  TengxunRTData Data35(34, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ��������sh����sz��
  TengxunRTData Data36(35, _T("v_sa600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ��������sh����sz��
  TengxunRTData Data37(36, _T("v_as600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ������Ʊ������治��'='��
  TengxunRTData Data38(37, _T("v_sh600601`\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ����'='�ú��治��'"'��
  TengxunRTData Data39(38, _T("v_sh600601='1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));
  // ��ʽ������Ʊ������治��'='��
  TengxunRTData Data40(39, _T("v_sh600601?\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";"));

  class CalculateTengxunRTDataTest : public::testing::TestWithParam<TengxunRTData*> {
  protected:
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      TengxunRTData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;
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
    }

    void TearDown(void) override {
      // clearup
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CStockRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestTengxunRTData, CalculateTengxunRTDataTest, testing::Values(&Data1, &Data2, &Data3,
    &Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
    &Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
    &Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30,
    &Data31, &Data32, &Data33, &Data34, &Data35, &Data36, &Data37, &Data38, &Data39
  ));

  TEST_P(CalculateTengxunRTDataTest, TestTengxunRTData) {
    bool fSucceed = m_RTData.ReadTengxunData(m_pCurrentPos, m_lCountPos);
    time_t ttime;
    tm tm_;
    tm_.tm_year = 2019 - 1900;
    tm_.tm_mon = 7 - 1;
    tm_.tm_mday = 16;
    tm_.tm_hour = 15;
    tm_.tm_min = 0;
    tm_.tm_sec = 0;
    ttime = mktime(&tm_);
    switch (m_iCount) {
    case 0:
      EXPECT_TRUE(fSucceed); // û�д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
      EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
      break;
    case 1:
      EXPECT_TRUE(fSucceed); // û�д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sz002385"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("ƽ������"));
      EXPECT_EQ(m_RTData.GetOpen(), -1);
      EXPECT_EQ(m_RTData.GetLastClose(), -1);
      EXPECT_EQ(m_RTData.GetNew(), -1);
      EXPECT_EQ(m_RTData.GetHigh(), -1);
      EXPECT_EQ(m_RTData.GetLow(), -1);
      EXPECT_EQ(m_RTData.GetBuy(), -1);
      EXPECT_EQ(m_RTData.GetSell(), -1);
      EXPECT_EQ(m_RTData.GetVolume(), 21606007);
      EXPECT_EQ(m_RTData.GetAmount(), 248901949);
      EXPECT_EQ(m_RTData.GetVBuy(0), 19900);
      EXPECT_EQ(m_RTData.GetPBuy(0), -1);
      EXPECT_EQ(m_RTData.GetVBuy(1), 54700);
      EXPECT_EQ(m_RTData.GetPBuy(1), -1);
      EXPECT_EQ(m_RTData.GetVBuy(2), 561500);
      EXPECT_EQ(m_RTData.GetPBuy(2), -1);
      EXPECT_EQ(m_RTData.GetVBuy(3), 105600);
      EXPECT_EQ(m_RTData.GetPBuy(3), -1);
      EXPECT_EQ(m_RTData.GetVBuy(4), 172400);
      EXPECT_EQ(m_RTData.GetPBuy(4), -1);
      EXPECT_EQ(m_RTData.GetVSell(0), 259981);
      EXPECT_EQ(m_RTData.GetPSell(0), -1);
      EXPECT_EQ(m_RTData.GetVSell(1), 206108);
      EXPECT_EQ(m_RTData.GetPSell(1), -1);
      EXPECT_EQ(m_RTData.GetVSell(2), 325641);
      EXPECT_EQ(m_RTData.GetPSell(2), -1);
      EXPECT_EQ(m_RTData.GetVSell(3), 215109);
      EXPECT_EQ(m_RTData.GetPSell(3), -1);
      EXPECT_EQ(m_RTData.GetVSell(4), 262900);
      EXPECT_EQ(m_RTData.GetPSell(4), -1);
      EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
      break;
    case 2:
      EXPECT_TRUE(fSucceed); // û�д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      EXPECT_EQ(m_RTData.GetLow(), 11440);
      EXPECT_EQ(m_RTData.GetBuy(), 11540);
      EXPECT_EQ(m_RTData.GetSell(), 11550);
      EXPECT_EQ(m_RTData.GetVolume(), -1);
      EXPECT_EQ(m_RTData.GetAmount(), -1);
      EXPECT_EQ(m_RTData.GetVBuy(0), -1);
      EXPECT_EQ(m_RTData.GetPBuy(0), 11540);
      EXPECT_EQ(m_RTData.GetVBuy(1), -1);
      EXPECT_EQ(m_RTData.GetPBuy(1), 11530);
      EXPECT_EQ(m_RTData.GetVBuy(2), -1);
      EXPECT_EQ(m_RTData.GetPBuy(2), 11520);
      EXPECT_EQ(m_RTData.GetVBuy(3), -1);
      EXPECT_EQ(m_RTData.GetPBuy(3), 11510);
      EXPECT_EQ(m_RTData.GetVBuy(4), -1);
      EXPECT_EQ(m_RTData.GetPBuy(4), 11500);
      EXPECT_EQ(m_RTData.GetVSell(0), -1);
      EXPECT_EQ(m_RTData.GetPSell(0), 11550);
      EXPECT_EQ(m_RTData.GetVSell(1), -1);
      EXPECT_EQ(m_RTData.GetPSell(1), 11560);
      EXPECT_EQ(m_RTData.GetVSell(2), -1);
      EXPECT_EQ(m_RTData.GetPSell(2), 11570);
      EXPECT_EQ(m_RTData.GetVSell(3), -1);
      EXPECT_EQ(m_RTData.GetPSell(3), 11580);
      EXPECT_EQ(m_RTData.GetVSell(4), -1);
      EXPECT_EQ(m_RTData.GetPSell(4), 11590);
      EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
      break;
    case 3:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      break;
    case 4:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      break;
    case 5:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      break;
    case 6:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      break;
    case 7:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      break;
    case 8:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      EXPECT_EQ(m_RTData.GetLow(), 11440);
      break;
    case 9:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      EXPECT_EQ(m_RTData.GetLow(), 11440);
      EXPECT_EQ(m_RTData.GetBuy(), 11540);
      break;
    case 10:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      EXPECT_EQ(m_RTData.GetLow(), 11440);
      EXPECT_EQ(m_RTData.GetBuy(), 11540);
      EXPECT_EQ(m_RTData.GetSell(), 11550);
      break;
    case 11:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
      EXPECT_EQ(m_RTData.GetOpen(), 11510);
      EXPECT_EQ(m_RTData.GetLastClose(), 11490);
      EXPECT_EQ(m_RTData.GetNew(), 11560);
      EXPECT_EQ(m_RTData.GetHigh(), 11570);
      EXPECT_EQ(m_RTData.GetLow(), 11440);
      EXPECT_EQ(m_RTData.GetBuy(), 11540);
      EXPECT_EQ(m_RTData.GetSell(), 11550);
      EXPECT_EQ(m_RTData.GetVolume(), 21606007);
      break;
    case 12:
      EXPECT_FALSE(fSucceed); // û�д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 13:
      EXPECT_FALSE(fSucceed); // û�д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 14:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 15:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 16:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 17:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 18:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 19:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 20:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 21:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 22:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 23:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 24:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 25:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 26:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 27:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 28:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 29:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 30:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 31:
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
      EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    case 32: // û��ʵʱ����
      EXPECT_TRUE(fSucceed); // ��ȡ��ȷ
      EXPECT_STREQ(m_RTData.GetStockCode(), _T("sz000001"));
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ���ǻ�Ծ��Ʊ
      break;
    case 33: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 34: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 35: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 36: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 37: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 38: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    case 39: // �д���ǰ׺����
      EXPECT_FALSE(fSucceed); // �д���
      EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
      break;
    default:
      break;
    }
  }

  struct ReadTengxunOneValueData {
    ReadTengxunOneValueData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  // �ɹ�
  ReadTengxunOneValueData rdata1(1, _T("11.050~"));
  // С�������λ
  ReadTengxunOneValueData rdata2(2, _T("11.05~"));
  // С�����һλ
  ReadTengxunOneValueData rdata3(3, _T("11.0~"));
  // С����ǰ����0x00a
  ReadTengxunOneValueData rdata4(4, _T("1\n1.050~"));
  // С��������0x00a
  ReadTengxunOneValueData rdata5(5, _T("11.0\n50~"));
  // ȱ��','
  ReadTengxunOneValueData rdata6(6, _T("11.050"));
  // ��ȡС�������е���ֵ
  ReadTengxunOneValueData rdata7(7, _T("11.050001~"));
  // 0x00a�����ڡ�����ǰ��
  ReadTengxunOneValueData rdata8(8, _T("11.05000\n~"));
  // ֻ��~
  ReadTengxunOneValueData rdata9(9, _T("~"));

  class ReadTengxunOneValueTest : public::testing::TestWithParam<ReadTengxunOneValueData*> {
  protected:
    void SetUp(void) override {
      ReadTengxunOneValueData* pData = GetParam();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pData = new char[lLength + 1];
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pData[lLength] = 0x000;
      m_pCurrentPos = m_pData;
      m_lCountPos = 0;
    }

    void TearDown(void) override {
      // clearup
      delete m_pData;
    }

  public:
    int m_iCount;
    char* m_pData;
    char* m_pCurrentPos;
    long m_lCountPos = 0;
    CStockRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestReadTengxunOneValue, ReadTengxunOneValueTest,
    testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8, &rdata9
    ));

  // ���ַ���ת��ΪINT64
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue1) {
    INT64 llTemp = 0;
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pCurrentPos, llTemp, m_lCountPos);
    switch (m_iCount) {
    case 1:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 7);
      EXPECT_EQ(llTemp, 11);
      break;
    case 2:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 6);
      EXPECT_EQ(llTemp, 11);
      break;
    case 3:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 5);
      EXPECT_EQ(llTemp, 11);
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
      EXPECT_EQ(llTemp, 11);
      break;
    case 8:
      EXPECT_FALSE(fSucceed);
      break;
    case 9:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 1);
      EXPECT_EQ(llTemp, 0);
      break;
    default:
      break;
    }
  }


  // ���ַ���ת��Ϊ������
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue2) {
    long lTemp = 0;
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pCurrentPos, lTemp, m_lCountPos);
    switch (m_iCount) {
    case 1:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 7);
      EXPECT_EQ(lTemp, 11);
      break;
    case 2:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 6);
      EXPECT_EQ(lTemp, 11);
      break;
    case 3:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 5);
      EXPECT_EQ(lTemp, 11);
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
      EXPECT_EQ(lTemp, 11);
      break;
    case 8:
      EXPECT_FALSE(fSucceed);
      break;
    case 9:
      EXPECT_TRUE(fSucceed);
      EXPECT_EQ(m_lCountPos, 1);
      EXPECT_EQ(lTemp, 0);
      break;
    default:
      break;
    }
  }

  // ����buffer��
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue3) {
    char buffer[30];
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pCurrentPos, buffer, m_lCountPos);
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
      EXPECT_STREQ(str, _T("11.050001"));
      break;
    case 8:
      EXPECT_FALSE(fSucceed);
      break;
    case 9:
      EXPECT_TRUE(fSucceed);
      EXPECT_STREQ(str, _T(""));
      break;
    default:
      break;
    }
  }

}  
