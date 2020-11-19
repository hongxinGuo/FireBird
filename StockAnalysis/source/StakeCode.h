#pragma once

#include"stdafx.h"

#include"SetStakeCode.h"

using namespace std;
#include<memory>

class CStakeCode : public CObject {
public:
  void LoadStakeCodeDB(CSetStakeCode& setStakeCode);
  void SaveToDB(CSetStakeCode& setStakeCode);

  CString GetStakeCode(void) noexcept { return m_strStakeCode; }
  void SetStakeCode(CString str) noexcept { m_strStakeCode = str; }

  void SetStakeName(CString str) noexcept { m_strStakeName = str; }

protected:
  WORD m_wMarket;	// 1：上海市场（不区分细类）；2：深圳市场（不区分细类）；3：上海指数；4：深圳指数；：上海三版；6：深圳中小板；
                // 7：上海B股；8：深圳B股；9：上海科创版; 10：深圳创业板；
                // 上海市场采用单数标示，深圳市场采用双数标示。目前暂时不用，先查看涉及到哪些函数需要修改。
  CString m_strStakeCode; // 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString m_strStakeName; // 股票名称
  CStringW m_strStockNameReadIn; // 读入的股票名称（UniCode制式，目前暂未使用）
  long m_lDayLineStartDate;	// 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
  long m_lDayLineEndDate;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long m_lIPOStatus; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
                 // 未上市（无效股票代码）为__STOCK_NULL__；正常为__STOCK_IPOED__；已通过IPO但尚未上市或退市为__STOCK_DELISTED；其他情况尚未出现，留待以后处理。
  short	m_nHand;	// 每手股数
};

typedef shared_ptr<CStakeCode> CStakeCodePtr;