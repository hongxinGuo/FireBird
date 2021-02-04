#pragma once

#include"stdafx.h"
#include"GlobeSymbolDef.h"

#include"SetActiveStakeCode.h"

using namespace std;
#include<memory>

class CStakeCode : public CObject {
public:
  CStakeCode();
  ~CStakeCode(void);

  void LoadActiveStakeCodeDB(CSetActiveStakeCode& setStakeCode);
  void SaveActiveStakeCodeDB(CSetActiveStakeCode& setStakeCode);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_lOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_lOffsetInContainer = lValue; }

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STAKE_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STAKE_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STAKE_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STAKE_NOT_CHECKED__); }
protected:
  WORD m_wMarket;	// 1：上海市场（不区分细类）；2：深圳市场（不区分细类）；3：上海指数；4：深圳指数；：上海三版；6：深圳中小板；
                // 7：上海B股；8：深圳B股；9：上海科创版; 10：深圳创业板；
                // 上海市场采用单数标示，深圳市场采用双数标示。目前暂时不用，先查看涉及到哪些函数需要修改。
  CString m_strStockCode; // 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString m_strStockName; // 股票名称
  CStringW m_strStakeNameReadIn; // 读入的股票名称（UniCode制式，目前暂未使用）
  long m_lDayLineStartDate;	// 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
  long m_lOffsetInContainer; // 在容器中的偏移量
  long m_lDayLineEndDate;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long m_lIPOStatus; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
                 // 未上市（无效股票代码）为__STAKE_NULL__；正常为__STAKE_IPOED__；已通过IPO但尚未上市或退市为__STAKE_DELISTED；其他情况尚未出现，留待以后处理。
  short	m_nHand;	// 每手股数
};

typedef shared_ptr<CStakeCode> CStakeCodePtr;