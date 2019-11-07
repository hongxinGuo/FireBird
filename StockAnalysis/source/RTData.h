///////////////////////////////////////////////////////////////////////////////////////////////
//
// 个股实时数据包, 以新浪实时股票行情接口(hq.sinajs.cn\list=)和腾讯实时股票行情接口为标准
//
// 为了计算方便，开盘价等使用放大1000倍的整型来表示真实数据，日线数据也如此。在存储时，使用DECIMAL(10,3)来记录，这样比较容易，不需多次转换数据类型。
// 挂单量、成交量和成交金额、市值等采用实际数值。
//
// 新浪实时行情制式：hq.sinajs.cn\list=sh600001,sz002389
// var hq_str_sh601006=”大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32”;
//
// 这个字符串由许多数据拼接在一起，不同含义的数据用逗号隔开了，按照程序员的思路，顺序号从0开始。
// 0：”大秦铁路”，股票名字；
// 1：”27.55″，今日开盘价；
// 2：”27.25″，昨日收盘价；
// 3：”26.91″，当前价格；
// 4：”27.55″，今日最高价；
// 5：”26.20″，今日最低价；
// 6：”26.91″，竞买价，即“买一”报价；
// 7：”26.92″，竞卖价，即“卖一”报价；
// 8：”22114263″，成交的股票数，由于股票交易以一百股为基本单位，所以在使用时，通常把该值除以一百；
// 9：”589824680″，成交金额，单位为“元”，为了一目了然，通常以“万元”为成交金额的单位，所以通常把该值除以一万；
// 10：”4695″，“买一”申请4695股，即47手；
// 11：”26.91″，“买一”报价；
// 12：”57590″，“买二”
// 13：”26.90″，“买二”
// 14：”14700″，“买三”
// 15：”26.89″，“买三”
// 16：”14300″，“买四”
// 17：”26.88″，“买四”
// 18：”15100″，“买五”
// 19：”26.87″，“买五”
// 20：”3100″，“卖一”申报3100股，即31手；
// 21：”26.92″，“卖一”报价
// (22, 23), (24, 25), (26, 27), (28, 29)分别为“卖二”至“卖四的情况”
// 30：”2008 - 01 - 11″，日期；
// 31：”15:05 : 32″，时间；
//
//
//
// 腾讯最新行情，访问数据接口：http://qt.gtimg.cn/q=sz002818,sh600001
// v_sz000001 = "51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~
//              15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//              20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//              15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~
//              ~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";
//
// 0: 51（或1）  1 沪A， 51 深A
// 1 : 平安银行  名字
// 2 : 000001   代码
// 3 : 15.59    现价
// 4 : 15.90    昨收
// 5 : 15.75    今开
// 6 : 1046363  成交量（手）
// 7 : 518391   外盘
// 8 : 527971   内盘 （上面第一行结束）
// 9 : 15.58    买一
// 10 : 2365    买一量（手）
// 11 - 18 : 买二 - 买五
// 19 : 15.59   卖一
// 20 : 661     卖一量
// 21 - 28 : 卖二 - 卖五
// 29 : （空）   最近逐笔成交，免费行情此段为空（上面第二行结束）
// 30 : 20190930154003  时间
// 31 : 0.31    涨跌
// 32 : 1.95    涨跌 %
// 33 : 15.89   最高
// 34 : 15.57   最低
// 35 : 15.59/1046363/1645828527    价格 / 成交量（手） / 成交额（元）
// 36 : 1046363 成交量（手）
// 37 : 164583  成交额（万）
// 38 : 0.54    换手率
// 39 : 11.27   市盈率（TTM）
// 40 : 空      不明      (上面第三行结束）
// 41 : 15.89   最高
// 42 : 15.57   最低
// 43 : 2.01    振幅
// 44 : 3025.38 流通市值（亿）
// 45 : 3025.38 总市值（亿）
// 46 : 1.15    市净率
// 47 : 17.49   涨停价
// 48 : 14.31   跌停价
// 49 ：0.73    量比
// 50 ：-12617  委差
// 51 ：15.73   不明
// 52 ： 9.82    动态市盈率
// 53 ： 12.19   静态市盈率   （上面第四行结束）
//
// 随后的字段含义不明，未找到官方说明。
//
//
// 网易实时行情，访问端口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

enum {
  __INVALID_RT_WEB_DATA__ = 0,
  __SINA_RT_WEB_DATA__ = 1,
  __TENGXUN_RT_WEB_DATA__ = 2,
  __NETEASE_RT_WEB_DATA__ = 3,
};

#include"stdafx.h"
#include"afxinet.h"

using namespace std;
#include<memory>
#include<array>
#include<map>

class CRTData;

typedef shared_ptr<CRTData> CRTDataPtr;

class CRTData final : public CObject {
public:
  // 初始化
  CRTData(void);

  void Reset(void);
  bool SetData(CRTData& data);

  bool Compare(CRTDataPtr pRTData);

  // 从字符指针处读入新浪制式数据。此指针开始处为var hq_str_s,遇到\n(回车)结束
  bool CRTData::ReadSinaData(char*& pCurrentPos, long& lTotalRead);

  // 从字符指针处读入腾讯制式数据。此指针开始处为v_s,遇到\n(回车)结束
  bool CRTData::ReadTengxunData(char*& pCurrentPos, long& lTotalRead);

  // 从字符指针处读入网易制式数据。此指针开始处为_ntes_quote_callback,遇到\n(回车)结束
  bool CRTData::ReadNeteaseData(char*& pCurrentPos, long& lTotalRead);

public:
  // 读取新浪实时数据函数
  bool ReadSinaOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead); // 从file中读入一个长整型
  bool ReadSinaOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead); // 从file中读入一个长整型
  bool ReadSinaOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead); // 从file中读入一个浮点数据，最后字符为‘，’。
  // 从file中读入一个浮点数据，抛弃其中的逗号，最后字符为‘，’。
  bool ReadSinaOneValueExceptPeriod(char*& pCurrentPos, long& lReturnValue, long& lCounter);
  bool ReadSinaOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter);

  // 读取腾讯实时数据函数
  bool ReadTengxunOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead); // 从file中读入一个长整型
  bool ReadTengxunOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead); // 从file中读入一个INT64整型
  bool ReadTengxunOneValue(char*& pCurrentPos, double& dReturnValue, long& lTotalRead); // 从file中读入一个浮点数
  bool ReadTengxunOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead); // 从file中读入一个浮点数据，最后字符为‘~’。

  // 读取网易实时数据函数
  long GetNeteaseSymbolIndex(CString strSymbol);
  bool GetNeteaseIndexAndValue(char*& pCurrentPos, long& lTotalRead, long& lIndex, CString& strValue); // 从field中读取一个索引和一个以字符串表示的值
  bool SetValue(long lIndex, CString strValue);

public:
  void SetDataSource(long lDataSource) noexcept { m_lDataSource = lDataSource; }
  long GetDataSource(void) noexcept { return m_lDataSource; }
  time_t GetTransactionTime(void) noexcept { return m_time; }
  void SetTransactionTime(time_t time) noexcept { m_time = time; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetBuy(void) noexcept { return m_lBuy; }
  void SetBuy(long lValue) noexcept { m_lBuy = lValue; }
  long GetSell(void) { return m_lSell; }
  void SetSell(long lValue) { m_lSell = lValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  void SetPBuy(int iIndex, long lValue) { m_lPBuy.at(iIndex) = lValue; }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  void SetVBuy(int iIndex, long lValue) { m_lVBuy.at(iIndex) = lValue; }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  void SetPSell(int iIndex, long lValue) { m_lPSell.at(iIndex) = lValue; }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetVSell(int iIndex, long lValue) { m_lVSell.at(iIndex) = lValue; }

  bool IsActive(void) noexcept { return m_fActive; }
  bool IsDataTimeAtCurrentDay(void);
  //void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  bool IsValidDataSource(void) noexcept { if (m_lDataSource != __INVALID_RT_WEB_DATA__) return true; else return false; }

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

  // 数据
public:

protected:
  long m_lDataSource; // 实时数据来源标识。1：新浪网站；2：腾讯网站；3：网易网站；。。。
  // Serialized data
  time_t m_time;	// 交易发生时的时间
  WORD m_wMarket;	// 市场标示.目前为__SHANGHAI_MARKET__和__SHENZHEN_MARKET__两个市场
  CString m_strStockCode;// 证券代码, sh600001，sz002389，
  CString m_strStockName; // 证券名称
  long m_lOpen;	// 今日开盘。单位：0.001元
  long m_lLastClose;// 昨日收盘。单位：0.001元
  long m_lNew; // 今日最新。单位：0.001元
  long m_lHigh;	// 今日最高。单位：0.001元
  long m_lLow; // 今日最低。单位：0.001元
  long m_lBuy; // 竞买价。单位：0.001元
  long m_lSell; // 竞卖价。单位：0.001元
  INT64 m_llVolume; // 总成交量。单位：股
  INT64 m_llAmount; // 总成交金额。单位：元
  INT64 m_llTotalValue;	// 总市值。单位：万元
  INT64 m_llCurrentValue;	// 流通市值。单位：万元
  array<long, 5> m_lPBuy; // 买盘价1--5。单位：0.001元
  array<long, 5> m_lVBuy;	// 买盘量1--5。单位：股
  array<long, 5> m_lPSell;// 卖盘价1--5。单位：0.001元
  array<long, 5> m_lVSell;// 卖盘量1--5。单位: 股

// 非存储数据
  bool m_fActive; // 本股票是否存在实时数据
  map<CString, long> m_mapNeteaseSymbolToIndex; // 网易实时数据字段映射
};
