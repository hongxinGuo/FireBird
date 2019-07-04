#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>

enum {
  __STOCK_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
  __STOCK_IPOED__ = 255,    // 正常股票标识
  __STOCK_NULL__ = 0,       // 无效代码（此代码的股票不存在）
  __STOCK_DELISTED__ = 1,   // 已退市的股票标识
};

class CStockID {					//证券标识
public:
  CStockID();
  ~CStockID();

  WORD			m_wMarket;				// 市场
  CString		m_strStockCode;		// 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString   m_strStockName;		// 股票名称
  long			m_nIndex;					// 在容器中的偏移量
  long      m_lDayLineStartDay; // 日线起始日。应该设置后就没有变化了，可以从数据中验证：起始日的前收盘价等与其他日有些区别。
  long			m_lDayLineEndDay;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long      m_lNewestDayLineDay; // 最新的日线数据日。如果处理了实时数据，则此日期要新于m_lDayLineEndDay
  long			m_lIPOed;					// 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
                              // 未上市（无效股票代码）为__STOCK_NULL__；正常为__STOCK_IPOED__；已通过IPO但尚未上市或退市为__STOCK_DELISTED；其他情况尚未出现，留待以后处理。
  bool			m_fActive;				// 是否本日内有数据读入。由新浪实时行情处理函数来设置。
  bool			m_fDayLineNeedUpdated; // 日线需要更新。默认为真
  bool			m_fInquiriingOnce;// 是否被查询一次。（无论m_fIPOed是否为真，都要在运行中查询一次股票日线情况，自然是留待最后再查）。
};

typedef shared_ptr<CStockID> StockIDPtr;

