#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"RTData.h"

namespace StockAnalysisTest {
  TEST(CRTDataTest, TestTengxunInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CRTData RTData;
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
  TengxunRTData Data1(0, _T("v_sh600000=\"1~�ַ�����~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n"));
  // �޴���
  TengxunRTData Data2(1, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���е�������Ϊ��
  TengxunRTData Data3(2, _T("v_sh600601=\"1~�����Ƽ�~600601~0~0~0~0~0~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data4(3, _T("v_sh600601=\"1~�����Ƽ�~600601~-3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data5(4, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~-3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data6(5, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~-3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data7(6, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~-83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data8(7, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~-3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data9(8, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~-2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data10(9, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~-3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data11(10, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~-2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data12(11, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~-3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data13(12, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~-2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data14(13, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~-3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data15(14, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~-760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data16(15, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~-3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data17(16, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~-1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data18(17, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~-3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data19(18, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~-2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data20(19, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~-3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data21(20, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~-2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data22(21, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~-3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data23(22, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~-1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data24(23, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~-3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data25(24, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~-2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data26(25, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~-3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data27(26, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~-3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data28(27, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data29(28, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data30(29, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data31(30, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ���ָ�ֵ
  TengxunRTData Data32(31, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // û��ʵʱ����
  TengxunRTData Data33(32, _T("v_sh600002=\"1~��³����~600002~0.00~0.00~0.00~0~0~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~0.00~0~~20191011091000~0.00~0.00~0.00~0.00~0.00 / 0 / 0~0~0~0.00~0.00~D~0.00~0.00~0.00~0.00~0.00~0.00~-1~-1~0.00~0~0.00~0.00~0.00~~~~0.00~0.00~0~~GP-A~~~0.00\";\n"));
  // ��ʽ����ǰ׺��
  TengxunRTData Data34(33, _T("a_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ������
  TengxunRTData Data35(34, _T("v=sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ��������sh����sz��
  TengxunRTData Data36(35, _T("v_sa600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ��������sh����sz��
  TengxunRTData Data37(36, _T("v_as600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ������Ʊ������治��'='��
  TengxunRTData Data38(37, _T("v_sh600601`\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ����'='�ú��治��'"'��
  TengxunRTData Data39(38, _T("v_sh600601='1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ��ʽ������Ʊ������治��'='��
  TengxunRTData Data40(39, _T("v_sh600601?\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));
  // ������ȷ������
  TengxunRTData Data41(40, _T("v_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\nv_sh600601=\"1~�����Ƽ�~600601~3.50~3.46~3.47~83346~49036~34309~3.50~2223~3.49~2189~3.48~2093~3.47~760~3.46~1132~3.51~2224~3.52~2848~3.53~1411~3.54~2026~3.55~3577~~20191011155858~0.04~1.16~3.53~3.43~3.50 / 83346 / 29058955~83346~2906~0.38~- 123.29~~3.53~3.43~2.89~76.82~76.82~2.61~3.81~3.11~1.10~- 3689~3.49~- 14.64~139.47~~~1.12~2905.90~0.00~0~~GP - A~39.44~~0.29\";\n"));

  class CalculateTengxunRTDataTest : public::testing::TestWithParam<TengxunRTData*> {
  protected:
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      TengxunRTData* pData = GetParam();
      m_pTengxunWebRTData = make_shared<CWebRTData>();
      m_iCount = pData->m_iCount;
      m_lStringLength = pData->m_strData.GetLength();
      m_pTengxunWebRTData->m_pDataBuffer = new char[m_lStringLength + 1];
      m_pData = m_pTengxunWebRTData->GetBufferAddr();
      for (int i = 0; i < m_lStringLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pTengxunWebRTData->ResetCurrentPos();
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
    }

  public:
    int m_iCount;
    char* m_pData;
    long m_lStringLength;
    CWebRTDataPtr m_pTengxunWebRTData;
    CRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(CRTDataTest, CalculateTengxunRTDataTest, testing::Values(&Data1, &Data2, &Data3,
                                                                                   &Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
                                                                                   &Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
                                                                                   &Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30,
                                                                                   &Data31, &Data32, &Data33, &Data34, &Data35, &Data36, &Data37, &Data38, &Data39, &Data40
  ));

  TEST_P(CalculateTengxunRTDataTest, TestReadTengxunData) {
    bool fSucceed = m_RTData.ReadTengxunData(m_pTengxunWebRTData);
    time_t ttime;
    tm tm_;
    tm_.tm_year = 2019 - 1900;
    tm_.tm_mon = 10 - 1;
    tm_.tm_mday = 11;
    tm_.tm_hour = 15;
    tm_.tm_min = 58;
    tm_.tm_sec = 58;
    ttime = mktime(&tm_);
    switch (m_iCount) {
    case 0:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600000"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�ַ�����"));
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
    EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
    break;
    case 1:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
    break;
    case 2:
    EXPECT_TRUE(fSucceed); // û�д��󣬽�Ϊ��
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    break;
    case 3:
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    break;
    case 4:
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    break;
    case 5:
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetLastClose(), 3460);
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    break;
    case 6:
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetOpen(), 3470);
    EXPECT_EQ(m_RTData.GetLastClose(), 3460);
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    break;
    case 7:
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetOpen(), 3470);
    EXPECT_EQ(m_RTData.GetLastClose(), 3460);
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    EXPECT_EQ(m_RTData.GetVolume(), 8334600);
    break;
    case 8:
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetOpen(), 3470);
    EXPECT_EQ(m_RTData.GetLastClose(), 3460);
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    EXPECT_EQ(m_RTData.GetVolume(), 8334600);
    EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
    break;
    case 9:
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
    EXPECT_EQ(m_RTData.GetOpen(), 3470);
    EXPECT_EQ(m_RTData.GetLastClose(), 3460);
    EXPECT_EQ(m_RTData.GetNew(), 3500);
    EXPECT_EQ(m_RTData.GetVolume(), 8334600);
    EXPECT_EQ(m_RTData.GetVBuy(0), 222300);
    EXPECT_EQ(m_RTData.GetPBuy(0), 3500);
    break;
    case 12:
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // �д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600601"));
    EXPECT_STREQ(m_RTData.GetStockName(), _T("�����Ƽ�"));
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
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    break;
    case 28:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    break;
    case 29:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    break;
    case 30:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    break;
    case 31:
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    break;
    case 32: // û��ʵʱ����
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_STREQ(m_RTData.GetStockCode(), _T("sh600002"));
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ���ǻ�Ծ��Ʊ
    break;
    case 33: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 34: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 35: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 36: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 37: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 38: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 39: // �д���ǰ׺����
    EXPECT_FALSE(fSucceed); // �д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());
    EXPECT_FALSE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    break;
    case 40:  // ������ȷ������
    EXPECT_TRUE(fSucceed); // û�д���
    EXPECT_GT(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());  // ��ʱֻ����һ�����ݣ�
    EXPECT_TRUE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
    EXPECT_TRUE(m_RTData.ReadTengxunData(m_pTengxunWebRTData));
    EXPECT_EQ(m_lStringLength, m_pTengxunWebRTData->GetCurrentPos());  // ��ʱ�����
    EXPECT_TRUE(m_RTData.IsActive()); // �˹�Ʊ�ǻ�Ծ��Ʊ
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
      m_pTengxunWebRTData = make_shared<CWebRTData>();
      m_iCount = pData->m_iCount;
      long lLength = pData->m_strData.GetLength();
      m_pTengxunWebRTData->m_pDataBuffer = new char[lLength + 1];
      m_pData = m_pTengxunWebRTData->GetBufferAddr();
      for (int i = 0; i < lLength; i++) {
        m_pData[i] = pData->m_strData[i];
      }
      m_pData[lLength] = 0x000;
      m_pTengxunWebRTData->ResetCurrentPos();
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    char* m_pData;
    CWebRTDataPtr m_pTengxunWebRTData;
    CRTData m_RTData;
  };

  INSTANTIATE_TEST_CASE_P(TestReadTengxunOneValue, ReadTengxunOneValueTest,
                          testing::Values(&rdata1, &rdata2, &rdata3, &rdata4, &rdata5, &rdata6, &rdata7, &rdata8, &rdata9
                          ));

  // ���ַ���ת��ΪINT64
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue1) {
    INT64 llTemp = 0;
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pTengxunWebRTData, llTemp);
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 7);
    EXPECT_EQ(llTemp, 11);
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 6);
    EXPECT_EQ(llTemp, 11);
    break;
    case 3:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 5);
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
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 10);
    EXPECT_EQ(llTemp, 11);
    break;
    case 8:
    EXPECT_FALSE(fSucceed);
    break;
    case 9:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 1);
    EXPECT_EQ(llTemp, 0);
    break;
    default:
    break;
    }
  }

  // ���ַ���ת��Ϊ������
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue2) {
    long lTemp = 0;
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pTengxunWebRTData, lTemp);
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 7);
    EXPECT_EQ(lTemp, 11);
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 6);
    EXPECT_EQ(lTemp, 11);
    break;
    case 3:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 5);
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
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 10);
    EXPECT_EQ(lTemp, 11);
    break;
    case 8:
    EXPECT_FALSE(fSucceed);
    break;
    case 9:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 1);
    EXPECT_EQ(lTemp, 0);
    break;
    default:
    break;
    }
  }

  // ����buffer��
  TEST_P(ReadTengxunOneValueTest, TestReadTengxunOneValue3) {
    char buffer[30];
    bool fSucceed = m_RTData.ReadTengxunOneValue(m_pTengxunWebRTData, buffer);
    CString str;
    str = buffer;
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 7);
    EXPECT_STREQ(str, _T("11.050"));
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 6);
    EXPECT_STREQ(str, _T("11.05"));
    break;
    case 3:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 5);
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
    EXPECT_EQ(m_pTengxunWebRTData->GetCurrentPos(), 10);
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