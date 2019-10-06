///////////////////////////////////////////////////////////////////////////////////////////////
//
// 个股实时数据包, 以新浪实时股票行情接口(hq.sinajs.cn\list=)为标准
//
// 为了计算方便，开盘价等使用放大1000倍的整型来表示真实数据，日线数据也如此。在存储时，使用DECIMAL(10,3)来记录，这样比较容易，不需多次转换数据类型。
// 挂单量、成交量和成交金额、市值等采用实际数值。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"stdafx.h"
#include"afxinet.h"

using namespace std;
#include<memory>
#include<array>

class CStockRTData;

typedef shared_ptr<CStockRTData> CStockRTDataPtr;

class CStockRTData : public CObject {
public:
  // 初始化
  CStockRTData(void);

  void Reset(void);
  bool SetData(CStockRTData& data);
  bool SetDataAll(CStockRTData& data);

  // 从字符指针处读入新浪制式数据。此指针开始处为var hq_str_s,遇到\n(回车)结束
  bool CStockRTData::ReadSinaData(char*& pCurrentPos, long& iTotalRead);

  // 从字符指针处读入腾讯制式数据。此指针开始处为var hq_str_s,遇到\n(回车)结束
  bool CStockRTData::ReadTengxunData(char*& pCurrentPos, long& iTotalRead);

  // 比较大小运算符
  bool operator < (const CStockRTData& rtData) const {
    return m_time < rtData.m_time;
  }

  // 比较大小运算符
  bool operator < (const CStockRTDataPtr rtDataPtr) const {
    return m_time < rtDataPtr->m_time;
  }

public:
  bool ReadOneValue(char*& pCurrentPos, char* buffer, long& lCounter); // 从file中读入一个浮点数据，最后字符为‘，’。
  // 从file中读入一个浮点数据，抛弃其中的逗号，最后字符为‘，’。
  bool ReadOneValueExceptperiod(char*& pCurrentPos, char* buffer, long& lCounter);

public:
  time_t GetTransactionTime(void) noexcept { return m_time; }
  void SetTransactionTime(time_t time) noexcept { m_time = time; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }
  int GetCode(void) noexcept { return m_iStockCode; }
  void SetCode(int iStockCode) noexcept { m_iStockCode = iStockCode; }
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
  INT64 GetAmount(void) noexcept { return m_lAmount; }
  void SetAmount(INT64 llValue) noexcept { m_lAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_lVolume; }
  void SetVolume(INT64 llValue) noexcept { m_lVolume = llValue; }
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
  //void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

  // 数据
public:

protected:
  // Serialized data
  time_t		m_time;									// 交易发生时的时间
  WORD	    m_wMarket;				      // 市场标示
  CString		m_strStockCode;					// 证券代码,
  int       m_iStockCode;           // 证券代码（数字）
  CString   m_strStockName;         // 证券名称
  long  	  m_lOpen;								// 今日开盘。单位：元
  long  	  m_lLastClose;				    // 昨日收盘。单位：元
  long  	  m_lNew;									// 今日最新。单位：元
  long  	  m_lHigh;								// 今日最高。单位：元
  long  	  m_lLow;									// 今日最低。单位：元
  long      m_lBuy;                // 竞买价。单位：元
  long      m_lSell;               // 竞卖价。单位：元
  INT64	    m_lVolume;							// 总成交量。单位：股
  INT64     m_lAmount;							// 总成交金额。单位：元
  array<long, 5>  	  m_lPBuy;							// 买盘价1 -- 5。单位：元
  array<long, 5> 		  m_lVBuy;							// 买盘量1 -- 5。单位：股
  array<long, 5>  	  m_lPSell;						// 卖盘价1 -- 5。单位：元
  array<long, 5>  		m_lVSell;						// 卖盘量1 -- 5。单位: 股

// 非存储数据
  bool      m_fActive;              // 本股票是否存在实时数据
};
