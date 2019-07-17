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

class CStockRTData : public CObject {
public :
// 初始化
	CStockRTData( void );

	void ReSetData( void );
	bool SetData( CStockRTData & data );
	bool SetDataAll( CStockRTData & data );

  // 从字符指针处读入数据。此指针开始处为var hq_str_s,遇到\n(回车)结束
  bool CStockRTData::ReadData(char *& pCurrentPos, long & iTotalRead);

public:
  bool ReadOneValue(char *& pCurrentPos, char * buffer, long & lCounter); // 从file中读入一个浮点数据，最后字符为‘，’。
  // 从file中读入一个浮点数据，抛弃其中的逗号，最后字符为‘，’。
  bool ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter); 

public :


#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif


// 数据
public:
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
	long  	  m_lPBuy[5];							// 买盘价1 -- 5。单位：元
	long 		  m_lVBuy[5];							// 买盘量1 -- 5。单位：股
	long  	  m_lPSell[5];						// 卖盘价1 -- 5。单位：元
	long  		m_lVSell[5];						// 卖盘量1 -- 5。单位: 股

// 非存储数据
  bool      m_fActive;              // 本股票是否存在实时数据

};

typedef shared_ptr<CStockRTData> CStockRTDataPtr;

