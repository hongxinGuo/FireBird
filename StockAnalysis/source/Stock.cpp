#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include "Stock.h"
#include"Market.h"

using namespace std;
#include"string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CStock::CStock() : CObject() {
	
  Reset();

}

CStock::~CStock(void) {

}

void CStock::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = "";
  m_strStockName = "";
  m_iStockCode = 0;
  m_fActive = false;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_lVolume = 0;
  m_lAmount = 0;
  m_dRelativeStrong = 0;
  m_nHand = 100;

  m_lHigh = m_lNew = m_lLow = 0;

  m_lAttackBuyAmount = 0;
  m_lAttackSellAmount = 0;
  m_lCurrentVolume = 0;
  m_lAttackBuyVolume = 0;
  m_lCurrentAttackBuy = 0;
  m_lAttackSellVolume = 0;
  m_lCurrentAttackSell = 0;
  m_lStrongBuyVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lStrongSellVolume = 0;
  m_lCurrentStrongSell = 0;
  m_lUnknownVolume = 0;
  m_lCurrentUnknown = 0;
  m_lCancelBuyVolume = 0;
  m_lCancelSellVolume = 0;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
  m_lCurrentCanselSellVolume = m_lCurrentCanselBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_lFirstDataVolume = 0;

  m_dCurrentGuaDanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fDayLineLoaded = false;
  m_fDayLineNeededSaving = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayKLineUpdated = false;

  m_fStartCalculating = false;  // 实时数据开始计算标识。第一个实时数据只能用来初始话系统，不能用于计算。从第二个数据开始计算才有效。

  m_pLastRTData = nullptr;

  long lTotalNumber = GetRTDataDequeSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CStockRTDataPtr pRTData = PopRTData();
  }
}

void CStock::operator =(CStock &pStock) {
  m_wMarket = pStock.m_wMarket;
  m_strStockCode = pStock.m_strStockCode;
  m_strStockName = pStock.m_strStockName;
  m_nHand = pStock.m_nHand;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 更新股票当前的状态（用于显示）
//
//
///////////////////////////////////////////////////////////////////////////////////////////
void CStock::UpdataCurrentStatus(CStockRTDataPtr pRTData)
{
  m_Time = pRTData->GetTime();
  m_lLastClose = pRTData->GetLastClose();
  m_lNew = pRTData->GetNew();
  m_lHigh = pRTData->GetHigh();
  m_lLow = pRTData->GetLow();
  m_lOpen = pRTData->GetOpen();
  m_lVolume = pRTData->GetVolume();
  m_lAmount = pRTData->GetAmount();
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = pRTData->GetPBuy(i);
    m_lPSell.at(i) = pRTData->GetPSell(i);
    m_lVBuy.at(i) = pRTData->GetVBuy(i);
    m_lVSell.at(i) = pRTData->GetVSell(i);
  }
}

bool CStock::LoadDayLine(CSetDayLine * psetDayLine)
{

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 此函数被工作线程ClientThreadCalculatingRTDataProc调用，尽量减少对全局变量的操作。
//
// 交易情况和挂单情况似乎分开计算比较好，思路容易清晰。目前来看计算能力尚够，但要考虑旧机器的速度了。
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CStock::CalculateRTData(void) {
  CStockRTDataPtr pRTData;

  const long lTotalNumber = GetRTDataDequeSize(); //  缓存队列的长度。采用同步机制获取其数值.
  // 以下为计算挂单变化、股票活跃度、大单买卖情况
  for (long i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // 采用同步机制获取数据
    if ((pRTData->GetNew() != 0) && (pRTData->GetOpen() != 0)) { // 数据有效
      CalculateOneRTData(pRTData);
      if ((m_lOrdinaryBuyVolume < 0) || (m_lOrdinarySellVolume < 0) || (m_lAttackBuyVolume < 0)
        || (m_lAttackSellVolume < 0) || (m_lStrongBuyVolume < 0) || (m_lStrongSellVolume < 0)) {
        int j = 0;
        if (m_lOrdinaryBuyVolume < 0) j = 1;
        if (m_lOrdinarySellVolume < 0) j += 2;
        if (m_lAttackBuyVolume < 0) j += 4;
        if (m_lAttackSellVolume < 0) j += 8;
        if (m_lStrongBuyVolume < 0) j += 16;
        if (m_lStrongSellVolume < 0) j += 32;
        TRACE("%06d %s Error in volume. Error  code = %d\n", gl_systemTime.GetTime(),  m_strStockCode, j);
      }
      // 显示当前交易情况
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_strStockCode) == 0) {
          if (gl_ChinaStockMarket.m_pCurrentStock->GetCurrentTransationVolume() > 0) {
            gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDanTransaction();
          }
        }
      }
      // 显示当前取消挂单的情况
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_strStockCode) == 0) {
          gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDan();
        }
      }

    }
  }
  UpdataCurrentStatus(pRTData); // 其他状态只更新一次即可。

  if (lTotalNumber == 0) return false;
  else return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算一个挂单。
// 如果是第一次计算，则只设置初始状态。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::CalculateOneRTData(CStockRTDataPtr pRTData) {
  long lCurrentGuaDanTransactionPrice = 0;

  if (IsStartCalculating()) {
    m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
    if (m_lCurrentGuadanTransactionVolume == 0) { // 无成交？
      // 检查挂单情况
      m_nCurrentTransactionType = __NO_TRANSACTION__;
    }
    else { // 有成交？
      m_lTransactionNumber++; // 成交数加一。
      if (m_lCurrentGuadanTransactionVolume < 5000) {
        m_lTransactionNumberBelow5000++;
      }
      else if (m_lCurrentGuadanTransactionVolume < 50000) {
        m_lTransactionNumberBelow50000++;
      }
      else if (m_lCurrentGuadanTransactionVolume < 200000) {
        m_lTransactionNumberBelow200000++;
      }
      else {
        m_lTransactionNumberAbove200000++;
      }
       lCurrentGuaDanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // 生成比较用的价格（放大一千倍后采用长整型）
       m_dCurrentGuaDanTransactionPrice = static_cast<double>(lCurrentGuaDanTransactionPrice) / 1000; // 变换成实际价格
      if (lCurrentGuaDanTransactionPrice >= m_pLastRTData->GetPBuy(0)) { // 高于前买盘1
        if (lCurrentGuaDanTransactionPrice <= m_pLastRTData->GetPSell(0)) { // 低于前卖盘1
          if ((m_pLastRTData->GetPSell(0) - lCurrentGuaDanTransactionPrice) <= 2) { //一般性买入
            m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
            m_nCurrentTransactionType = __ORDINARY_BUY__;
          }
          else if ((lCurrentGuaDanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // 一般性卖出
            m_nCurrentTransactionType = __ORDINARY_SELL__;
            m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
          }
          else { // 买卖混杂，不分析。
            m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
            m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
          }
        }
        else if (lCurrentGuaDanTransactionPrice < m_pLastRTData->GetPSell(1)) { // 高于卖盘一，低于卖盘二。进攻型买入。AttackBuy
          m_nCurrentTransactionType = __ATTACK_BUY__;
          m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
          if (m_lCurrentGuadanTransactionVolume < 50000) {
            m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
          }
          else if (m_lCurrentGuadanTransactionVolume < 200000) {
            m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
          }
          else {
            m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
          }
        }
        else { // 高于卖盘二。强力买入。StrongBuy。
          m_nCurrentTransactionType = __STRONG_BUY__;
          m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
          if (m_lCurrentGuadanTransactionVolume < 50000) {
            m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
          }
          else if (m_lCurrentGuadanTransactionVolume < 200000) {
            m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
          }
          else {
            m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
          }
        }
      } // 高于前买一
      else if (lCurrentGuaDanTransactionPrice > m_pLastRTData->GetPBuy(1)) { // 低于买盘一，高于买盘二。进攻型卖出，AttackSell
        m_nCurrentTransactionType = __ATTACK_SELL__;
        m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
        if (m_lCurrentGuadanTransactionVolume < 50000) {
          m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
        }
        else if (m_lCurrentGuadanTransactionVolume < 200000) {
          m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
        }
        else {
          m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
        }
      }
      else { // 低于买盘二。强力卖出。StrongSell
        m_nCurrentTransactionType = __STRONG_SELL__;
        m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
        if (m_lCurrentGuadanTransactionVolume < 50000) {
          m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
        }
        else if (m_lCurrentGuadanTransactionVolume < 200000) {
          m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
        }
        else {
          m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
        }
      }
      ASSERT(m_Time >= pRTData->GetTime());
      const INT64 I = pRTData->GetVolume();
      const INT64 j = m_lOrdinaryBuyVolume + m_lOrdinarySellVolume
        + m_lAttackBuyVolume + m_lAttackSellVolume + +m_lStrongBuyVolume + m_lStrongSellVolume + m_lUnknownVolume;
      ASSERT(pRTData->GetVolume() == m_lOrdinaryBuyVolume + m_lOrdinarySellVolume
        + m_lAttackBuyVolume + m_lAttackSellVolume + m_lStrongBuyVolume + m_lStrongSellVolume + m_lUnknownVolume);
    }

    // 下面开始分析挂单情况
    AnalysisingGuaDan(pRTData, m_pLastRTData, m_nCurrentTransactionType, lCurrentGuaDanTransactionPrice);

    // 更新前交易状态
    m_pLastRTData = pRTData;
  }
  else { // 第一个数据，初始化系统
    m_pLastRTData = pRTData;
    SetStartCalculating(true);
    m_lUnknownVolume += m_pLastRTData->GetVolume(); // 第一次挂单量无法判断买卖状态
    // 设置第一次的挂单映射。
    for (int j = 0; j < 5; j++) {
      SetGuaDan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
      SetGuaDan(pRTData->GetPSell(j), pRTData->GetVSell(j));
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算挂单量的变化，由工作线程调用
//
// 采用map结构存储，这样简单且易于理解，唯一问题就是寻址花费时间长。需要测试后看看速度如何。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTRanscationType, long lCurrentTransactionPrice) {
  array<bool, 10> fNeedCheck{true,true,true,true,true,true,true,true,true,true}; // 需要检查的挂单位置。顺序为：Sell4, Sell3, ... Sell0, Buy0, .... Buy3, Buy4
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentCanselBuyVolume = 0;

  for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // 预设为都要计算。

  // 确定需要计算哪些挂单。一共有十个，没有收到交易影响的都要计算。
  switch (nTRanscationType) {
  case __NO_TRANSACTION__: // 没有成交，则减少的量就是相应价位上的撤单。
    break;
  case __ATTACK_BUY__: // 卖单一已经消失，卖单二被影响。计算其他七个挂单。
    fNeedCheck.at(3) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __STRONG_BUY__: // 卖单一和卖单二消失，卖单三以上未定，需要计算。
    if (lCurrentTransactionPrice < pLastRTData->GetPSell(2)) { // 卖单4和卖单5尚存
      fNeedCheck.at(2) = false;
    }
    else if (lCurrentTransactionPrice < pLastRTData->GetPSell(3)) { // 卖单5尚存
      fNeedCheck.at(1) = false;
      fNeedCheck.at(2) = false;
    }
    else { // 所有的卖单都受影响
      fNeedCheck.at(0) = false;
      fNeedCheck.at(2) = false;
      fNeedCheck.at(1) = false;
    }
    fNeedCheck.at(3) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __ORDINARY_BUY__: // 卖单一被影响。计算其他八个挂单。
  case __UNKNOWN_BUYSELL__: // 卖单一和买单一被影响。计算其他八个挂单。
  case __ORDINARY_SELL__: // 买单一被影响。计算其他八个挂单。
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __ATTACK_SELL__: // 买单一消失，买单二被影响。计算其他七个挂单。
    fNeedCheck.at(4) = fNeedCheck.at(5) = fNeedCheck.at(6) = false;
    break;
  case __STRONG_SELL__: // 买单一和买单二消失，其他买单需要计算。
    if (pLastRTData->GetPBuy(3) > lCurrentTransactionPrice) { // 所有买单都受影响
      fNeedCheck.at(7) = false;
      fNeedCheck.at(8) = false;
      fNeedCheck.at(9) = false;
    }
    else if (pLastRTData->GetPBuy(2) > lCurrentTransactionPrice) { // 买单5尚存
      fNeedCheck.at(7) = false;
      fNeedCheck.at(8) = false;
    }
    else { // 买单4和买单5没变化
      fNeedCheck.at(7) = false;
    }
    fNeedCheck.at(6) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  default:
    ASSERT(0); // 不可能执行到此分支。
    break;
  }

  if ((pCurrentRTData->GetPSell(4) - pCurrentRTData->GetPBuy(4)) > 90) { // 十个价位之间有空位
    // 空位处可能是成交了，也可能是撤单了，目前不考虑这些细节，统一认为是成交了（不计算撤单）。以后再分析之。
    // 先清空当前挂单之间的挂单数量，然后填上当前量。如果有空当的话，则自然清空了。
    for (int i2 = pCurrentRTData->GetPBuy(4); i2 <= pCurrentRTData->GetPSell(4); i2 += 10) {
      m_mapGuaDan[i2] = 0; // 此处需要设置值，有可能之前为空，故而不能使用at操作符，而是用[]下标操作。
    }
  }
  for (int i = 0; i < 5; i++) { // 将目前的十个挂单状态存入映射中，挂单状态更新为最新态
    SetGuaDan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
    SetGuaDan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
  }

  // 检查这十个挂单价位上股数的变化情况.将目前挂单状态与之前的十个价位（pLastRTData包含的）相比，查看变化
  for (int i = 0; i < 10; i++) {
    switch (i) {
    case 0: // 卖单五
      if (fNeedCheck.at(0)) {
        if (GetGuaDan(pLastRTData->GetPSell(4)) < pLastRTData->GetVSell(4)) { // 撤单了的话
          m_lCurrentCanselSellVolume += pLastRTData->GetVSell(4) - GetGuaDan(pLastRTData->GetPSell(4));
          m_lCancelSellVolume += pLastRTData->GetVSell(4) - GetGuaDan(pLastRTData->GetPSell(4));
        }
      }
      break;
    case 1: // 卖单四
      if (fNeedCheck.at(1)) {
        if (GetGuaDan(pLastRTData->GetPSell(3)) < pLastRTData->GetVSell(3)) { // 撤单了的话
          m_lCurrentCanselSellVolume += pLastRTData->GetVSell(3) - GetGuaDan(pLastRTData->GetPSell(3));
          m_lCancelSellVolume += pLastRTData->GetVSell(3) - GetGuaDan(pLastRTData->GetPSell(3));
        }
      }
      break;
    case 2: // 卖单三
      if (fNeedCheck.at(2)) {
        if (GetGuaDan(pLastRTData->GetPSell(2)) < pLastRTData->GetVSell(2)) { // 撤单了的话
          m_lCurrentCanselSellVolume += pLastRTData->GetVSell(2) - GetGuaDan(pLastRTData->GetPSell(2));
          m_lCancelSellVolume += pLastRTData->GetVSell(2) - GetGuaDan(pLastRTData->GetPSell(2));
        }
      }
      break;
    case 3: // 卖单二
      if (fNeedCheck.at(3)) {
        if (GetGuaDan(pLastRTData->GetPSell(1)) < pLastRTData->GetVSell(1)) { // 撤单了的话
          m_lCurrentCanselSellVolume += pLastRTData->GetVSell(1) - GetGuaDan(pLastRTData->GetPSell(1));
          m_lCancelSellVolume += pLastRTData->GetVSell(1) - GetGuaDan(pLastRTData->GetPSell(1));
        }
      }
      break;
    case 4: // 卖单一
      if(fNeedCheck.at(4)) {
        if (GetGuaDan(pLastRTData->GetPSell(0)) < pLastRTData->GetVSell(0)) { // 撤单了的话
          m_lCurrentCanselSellVolume += pLastRTData->GetVSell(0) - GetGuaDan(pLastRTData->GetPSell(0));
          m_lCancelSellVolume += pLastRTData->GetVSell(0) - GetGuaDan(pLastRTData->GetPSell(0));
        }
      }
      break;
    case 5:   // 以下为买单变化情况，买单一
      if (fNeedCheck.at(5)) {
        if (GetGuaDan(pLastRTData->GetPBuy(0)) < pLastRTData->GetVBuy(0)) { // 撤单了的话
          m_lCurrentCanselBuyVolume += pLastRTData->GetVBuy(0) - GetGuaDan(pLastRTData->GetPBuy(0));
          m_lCancelBuyVolume += pLastRTData->GetVBuy(0) - GetGuaDan(pLastRTData->GetPBuy(0));
        }
      }
      break;
    case 6: // 买单二
      if (fNeedCheck.at(6)) {
        if (GetGuaDan(pLastRTData->GetPBuy(1)) < pLastRTData->GetVBuy(1)) { // 撤单了的话
          m_lCurrentCanselBuyVolume += pLastRTData->GetVBuy(1) - GetGuaDan(pLastRTData->GetPBuy(1));
          m_lCancelBuyVolume += pLastRTData->GetVBuy(1) - GetGuaDan(pLastRTData->GetPBuy(1));
        }
      }
      break;
    case 7: // 买单三
      if (fNeedCheck.at(7)) {
        if (GetGuaDan(pLastRTData->GetPBuy(2)) < pLastRTData->GetVBuy(2)) { // 撤单了的话
          m_lCurrentCanselBuyVolume += pLastRTData->GetVBuy(2) - GetGuaDan(pLastRTData->GetPBuy(2));
          m_lCancelBuyVolume += pLastRTData->GetVBuy(2) - GetGuaDan(pLastRTData->GetPBuy(2));
        }
      }
      break;
    case 8: // 买单四
      if (fNeedCheck.at(8)) {
        if (GetGuaDan(pLastRTData->GetPBuy(3)) < pLastRTData->GetVBuy(3)) { // 撤单了的话
          m_lCurrentCanselBuyVolume += pLastRTData->GetVBuy(3) - GetGuaDan(pLastRTData->GetPBuy(3));
          m_lCancelBuyVolume += pLastRTData->GetVBuy(3) - GetGuaDan(pLastRTData->GetPBuy(3));
        }
      }
      break;
    case 9: // 买单五
      if (fNeedCheck.at(9)) {
        if (GetGuaDan(pLastRTData->GetPBuy(4)) < pLastRTData->GetVBuy(4)) { // 撤单了的话
          m_lCurrentCanselBuyVolume += pLastRTData->GetVBuy(4) - GetGuaDan(pLastRTData->GetPBuy(4));
          m_lCancelBuyVolume += pLastRTData->GetVBuy(4) - GetGuaDan(pLastRTData->GetPBuy(4));
        }
      }
      break;
    default : // 没有这个选项
      ASSERT(0);
    }
  }

  return(true);
}

void CStock::ReportGuaDanTransaction(void)
{
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, " %s %I64d股成交于%10.3f    ", m_strStockCode.GetBuffer(),
    m_lCurrentGuadanTransactionVolume, m_dCurrentGuaDanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
    str1 = _T(" STRONG BUY");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lStrongBuyVolume);
    break;
  case __STRONG_SELL__:
    str1 = _T(" STRONG SELL");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lStrongSellVolume);
    break;
  case __ATTACK_BUY__:
    str1 = _T(" ATTACK BUY");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lAttackBuyVolume);
    break;
  case __ATTACK_SELL__:
    str1 = _T(" ATTACK SELL");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lAttackSellVolume);
    break;
  case __ORDINARY_BUY__:
    str1 = _T(" ORDINARY BUY");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lOrdinaryBuyVolume);
    break;
  case __ORDINARY_SELL__:
    str1 = _T(" ORDINARY SELL");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lOrdinarySellVolume);
    break;
  case __UNKNOWN_BUYSELL__:
    str1 = _T(" UNKNOWN BUYSELL");
    sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_lUnknownVolume);
    break;
  default:
    break;
  }
  str += str1;
  // 显示当前成交情况
  gl_systemMessage.PushTransactionMessage(str); // 采用同步机制传送信息

  str2 = strTime;
  str2 += str1;
  str2 += buffer;
  gl_systemMessage.PushTrace2Message(str2);


}

void CStock::ReportGuaDan(void)
{
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, "当前取消卖单量：%I64d", m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, "  总取消卖单量：%I64d", m_lCancelSellVolume);
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // 采用同步机制传递消息

  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, "当前取消买单量：%I64d", m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, "  总取消买单量：%I64d", m_lCancelBuyVolume);
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // 采用同步机制传递消息

  }
}

bool CStock::SaveRealTimeData(CSetRealTimeData * psetRTData) {
  ASSERT(psetRTData != nullptr);
  ASSERT(psetRTData->IsOpen());
  for ( auto pRTData : m_dequeRTData) {
    psetRTData->AddNew();
    psetRTData->m_Time = pRTData->GetTime();
    psetRTData->m_lMarket = m_wMarket;
    psetRTData->m_StockCode = m_strStockCode;
    psetRTData->m_StockName = m_strStockName;
    psetRTData->m_CurrentPrice = static_cast<double>(pRTData->GetNew()) / 1000;
    psetRTData->m_High = static_cast<double>(pRTData->GetHigh()) / 1000;
    psetRTData->m_Low = static_cast<double>(pRTData->GetLow()) / 1000;
    psetRTData->m_LastClose = static_cast<double>(pRTData->GetLastClose()) / 1000;
    psetRTData->m_Open = static_cast<double>(pRTData->GetOpen()) / 1000;
    psetRTData->m_Volume = pRTData->GetVolume();
    psetRTData->m_Amount = static_cast<double>(pRTData->GetAmount());
    psetRTData->m_Stroke = 0;
    psetRTData->m_PBuy1 = static_cast<double>(pRTData->GetPBuy(0)) / 1000;
    psetRTData->m_VBuy1 = pRTData->GetVBuy(0);
    psetRTData->m_PSell1 = static_cast<double>(pRTData->GetPSell(0)) / 1000;
    psetRTData->m_VSell1 = pRTData->GetVSell(0);

    psetRTData->m_PBuy2 = static_cast<double>(pRTData->GetPBuy(1)) / 1000;
    psetRTData->m_VBuy2 = pRTData->GetVBuy(1);
    psetRTData->m_PSell2 = static_cast<double>(pRTData->GetPSell(1)) / 1000;
    psetRTData->m_VSell2 = pRTData->GetVSell(1);

    psetRTData->m_PBuy3 = static_cast<double>(pRTData->GetPBuy(2)) / 1000;
    psetRTData->m_VBuy3 = pRTData->GetVBuy(2);
    psetRTData->m_PSell3 = static_cast<double>(pRTData->GetPSell(2)) / 1000;
    psetRTData->m_VSell3 = pRTData->GetVSell(2);

    psetRTData->m_PBuy4 = static_cast<double>(pRTData->GetPBuy(3)) / 1000;
    psetRTData->m_VBuy4 = pRTData->GetVBuy(3);
    psetRTData->m_PSell4 = static_cast<double>(pRTData->GetPSell(3)) / 1000;
    psetRTData->m_VSell4 = pRTData->GetVSell(3);

    psetRTData->m_PBuy5 = static_cast<double>(pRTData->GetPBuy(4)) / 1000;
    psetRTData->m_VBuy5 = pRTData->GetVBuy(4);
    psetRTData->m_PSell5 = static_cast<double>(pRTData->GetPSell(4)) / 1000;
    psetRTData->m_VSell5 = pRTData->GetVSell(4);
    psetRTData->Update();

  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制存储实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CStock::PushRTData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTData.push_back(pData);
    singleLock.Unlock();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CStockRTDataPtr CStock::PopRTData(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    m_dequeRTData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  return nullptr;
}

CStockRTDataPtr CStock::GetRTDataAtHead(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    singleLock.Unlock();
    return pData;
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取栈内的实时数据数量。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
long CStock::GetRTDataDequeSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTData.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制设置日线是否需要存储标识。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

void CStock::SetDayLineNeedSavingFlag(bool fFlag)
{
  CSingleLock singleLock(&m_DayLineNeedSavingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineNeededSaving = fFlag;
    singleLock.Unlock();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取日线是否需要存储标识。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::IsDayLineNeedSaving(void)
{
  CSingleLock singleLock(&m_DayLineNeedSavingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag = m_fDayLineNeededSaving;
    singleLock.Unlock();
    return fFlag;
  }
  return false;
}

#ifdef _DEBUG
void CStock::AssertValid() const
{
  CObject::AssertValid();
}

void CStock::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}

#endif //_DEBUG


