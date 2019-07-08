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
	
  m_wMarket = 0;
  m_strStockCode = "";
	m_strStockName = "";
	m_iStockCode = 0;
	m_fActive = false;
	m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = m_lPSell[i] = 0;
    m_lVBuy[i] = m_lVSell[i] = 0;
  }
  m_lVolume = 0;
  m_lAmount = 0;
   m_dRelativeStrong = 0;
  m_nHand = 100;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;
  Reset();

  m_fDayLineLoaded = false;
  m_fDayLineNeededSaving = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayKLineUpdated = false;

  m_fStartCalculating = false;  // 实时数据开始计算标识。第一个实时数据只能用来初始话系统，不能用于计算。从第二个数据开始计算才有效。

  m_lAttackBuyAbove200000 = m_lAttackBuyBelow200000 = m_lAttackBuyBelow50000 = 0;
  m_lAttackSellAbove200000 = m_lAttackSellBelow200000 = m_lAttackSellBelow50000 = 0;
  m_lCancelBuyVolume = m_lCancelSellVolume = 0;
  m_lStrongBuyVolume = m_lStrongSellVolume = 0;
  m_lOrdinaryBuyVolume = m_lOrdinarySellVolume = 0;
  m_lUnknownVolume = 0;
  m_pLastRTData = nullptr;
}

CStock::~CStock(void) {

}

void CStock::Reset(void) {
  m_lHigh = m_lNew = m_lLow = 0;
  m_lVolume = 0;
  m_lAmount = 0;

  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = m_lPSell[i] = 0;
    m_lVBuy[i] = m_lVSell[i] = 0;
  }
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

  m_lFirstDataVolume = 0;

  m_dCurrentGuaDanTransactionPrice = 0;
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
  m_Time = pRTData->m_time;
  m_lNew = pRTData->m_lNew;
  m_lHigh = pRTData->m_lHigh;
  m_lLow = pRTData->m_lLow;
  m_lVolume = pRTData->m_lVolume;
  m_lAmount = pRTData->m_lAmount;
  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = pRTData->m_lPBuy[i];
    m_lPSell[i] = pRTData->m_lPSell[i];
    m_lVBuy[i] = pRTData->m_lVBuy[i];
    m_lVSell[i] = pRTData->m_lVSell[i];
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
  CString str;
  char buffer[100];
  CStockRTDataPtr pRTData;
  long lCurrentGuaDanTransactionPrice = 0;

  long lTotalNumber = GetRTDataDequeSize(); //  缓存队列的长度。采用同步机制获取其数值.
  // 以下为计算挂单变化、股票活跃度、大单买卖情况
  for (int i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTStockData(); // 采用同步机制获取数据
    if ((pRTData->m_lNew != 0) && (pRTData->m_lOpen != 0)) { // 数据有效
      if (m_fStartCalculating) {
        m_lCurrentGuadanTransactionVolume = pRTData->m_lVolume - m_pLastRTData->m_lVolume;
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
          m_dCurrentGuaDanTransactionPrice = ((double)(pRTData->m_lAmount - m_pLastRTData->m_lAmount)) / m_lCurrentGuadanTransactionVolume;
          lCurrentGuaDanTransactionPrice = m_dCurrentGuaDanTransactionPrice * 1000;
          if (lCurrentGuaDanTransactionPrice >= m_pLastRTData->m_lPBuy[0]) { // 高于前买盘1
            if (lCurrentGuaDanTransactionPrice <= m_pLastRTData->m_lPSell[0]) { // 低于前卖盘1
              if ((m_pLastRTData->m_lPSell[0] - lCurrentGuaDanTransactionPrice) <= 2) { //一般性买入
                m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
                m_nCurrentTransactionType = __ORDINARY_BUY__;
              }
              else if ((lCurrentGuaDanTransactionPrice - m_pLastRTData->m_lPBuy[0]) <= 2) { // 一般性卖出
                m_nCurrentTransactionType = __ORDINARY_SELL__;
                m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
              }
              else { // 买卖混杂，不分析。
                m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
                m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
              }
            }
            else if (lCurrentGuaDanTransactionPrice < m_pLastRTData->m_lPSell[1]) { // 高于卖盘一，低于卖盘二。进攻型买入。AttackBuy
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
              m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
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

          }
          else if (lCurrentGuaDanTransactionPrice > m_pLastRTData->m_lPBuy[1]) { // 低于买盘一，高于买盘二。进攻型卖出，AttackSell
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
            m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
            m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
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
          ASSERT(m_Time >= pRTData->m_time);
          if( gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
            if (gl_ChinaStockMarket.m_pCurrentStock->m_strStockCode.Compare(m_strStockCode) == 0) {
              CTime ctime(pRTData->m_time);
              sprintf_s(buffer, "%02d:%02d:%02d %s %d股成交于%10.3f", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond(), m_strStockCode.GetBuffer(),
                m_lCurrentGuadanTransactionVolume, m_dCurrentGuaDanTransactionPrice);
              str = buffer;
              CString str1;
              switch (m_nCurrentTransactionType) {
              case __STRONG_BUY__:
                str1 = _T(" STRONG BUY");
                break;
              case __STRONG_SELL__:
                str1 = _T(" STRONG SELL");
                break;
              case __ATTACK_BUY__:
                str1 = _T(" ATTACK BUY");
                break;
              case __ATTACK_SELL__:
                str1 = _T(" ATTACK SELL");
                break;
              case __ORDINARY_BUY__:
                str1 = _T(" ORDINARY BUY");
                break;
              case __ORDINARY_SELL__:
                str1 = _T(" ORDINARY SELL");
                break;
              case __UNKNOWN_BUYSELL__:
                str1 = _T(" UNKNOWN BUYSELL");
                break;
              default:
                break;
              }
              str += str1;
              gl_systemMessage.PushWarningMessage(str); // 采用同步机制传送信息
            }
          }
        }

        // 下面开始分析挂单情况
        AnalysisingGuaDan(pRTData, m_pLastRTData, m_nCurrentTransactionType, m_dCurrentGuaDanTransactionPrice);

        // 更新前交易状态
        m_pLastRTData = pRTData;
      }
      else { // 第一个数据，初始化系统
        m_pLastRTData = pRTData;
        m_fStartCalculating = true;
        // 设置第一次的挂单映射。
        for (int j = 0; j < 5; j++) {
          m_mapGuaDan[pRTData->m_lPBuy[j]] = pRTData->m_lVBuy[i];
          m_mapGuaDan[pRTData->m_lPSell[j]] = pRTData->m_lVSell[i];
        }
      }
    }
  }

  if (lTotalNumber == 0) return false;
  else return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算挂单量的变化，由工作线程调用
//
// 采用map结构存储，这样简单且易于理解，唯一问题就是寻址花费时间长。需要测试后看看速度如何。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTRanscationType, double dCurrentTransactionPrice) {
  bool fNeedCheck[10]; // 需要检查的挂单位置。顺序为：Sell4, Sell3, ... Sell0, Buy0, .... Buy3, Buy4
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentCanselBuyVolume = 0;

  for (int i = 0; i < 10; i++) fNeedCheck[i] = true; // 预设为都要计算。

  // 确定需要计算哪些挂单。一共有十个，没有收到交易影响的都要计算。
  switch (nTRanscationType) {
  case __NO_TRANSACTION__: // 没有成交，则减少的量就是相应价位上的撤单。
    break;
  case __ATTACK_BUY__: // 卖单一已经消失，卖单二被影响。计算其他七个挂单。
    fNeedCheck[3] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __STRONG_BUY__: // 卖单一和卖单二消失，卖单三以上未定，需要计算。
    if (dCurrentTransactionPrice < pLastRTData->m_lPSell[2]) { // 卖单4和卖单5尚存
      fNeedCheck[2] = false;
    }
    else if (dCurrentTransactionPrice < pLastRTData->m_lPSell[3]) { // 卖单5尚存
      fNeedCheck[1] = false;
      fNeedCheck[2] = false;
    }
    else { // 所有的卖单都受影响
      fNeedCheck[0] = false;
      fNeedCheck[2] = false;
      fNeedCheck[1] = false;
    }
    fNeedCheck[3] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __ORDINARY_BUY__: // 卖单一被影响。计算其他八个挂单。
  case __UNKNOWN_BUYSELL__: // 卖单一和买单一被影响。计算其他八个挂单。
  case __ORDINARY_SELL__: // 买单一被影响。计算其他八个挂单。
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __ATTACK_SELL__: // 买单一消失，买单二被影响。计算其他七个挂单。
    fNeedCheck[4] = fNeedCheck[5] = fNeedCheck[6] = false;
    break;
  case __STRONG_SELL__: // 买单一和买单二消失，其他买单需要计算。
    if (pLastRTData->m_lPBuy[3] > dCurrentTransactionPrice) { // 所有买单都受影响
      fNeedCheck[7] = false;
      fNeedCheck[8] = false;
      fNeedCheck[9] = false;
    }
    else if (pLastRTData->m_lPBuy[2] > dCurrentTransactionPrice) { // 买单5尚存
      fNeedCheck[7] = false;
      fNeedCheck[8] = false;
    }
    else { // 买单4和买单5没变化
      fNeedCheck[7] = false;
    }
    fNeedCheck[6] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  default:
    ASSERT(0); // 不可能执行到此分支。
    break;
  }
  
  for (int i = 0; i < 5; i++) { // 将目前的十个挂单状态存入映射中，挂单状态更新为最新态
    m_mapGuaDan[pCurrentRTData->m_lPSell[i]] = pCurrentRTData->m_lVSell[i];
    m_mapGuaDan[pCurrentRTData->m_lPBuy[i]] = pCurrentRTData->m_lVBuy[i];
  }

  // 检查这十个挂单价位上股数的变化情况.将目前挂单状态与之前的十个价位（pLastRTData包含的）相比，查看变化
  for (int i = 0; i < 10; i++) {
    switch (i) {
    case 0: // 卖单五
      if (fNeedCheck[0]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[4]) == m_mapGuaDan.end()) { // 如果此价位上没有挂单
          m_mapGuaDan[pLastRTData->m_lPSell[4]] = pLastRTData->m_lVSell[4]; // 则在此价位上存储目前的股数（建立新的映射）。
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[4]) < pLastRTData->m_lVSell[4]) { // 撤单了的话
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[4] - m_mapGuaDan.at(pLastRTData->m_lPSell[4]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[4] - m_mapGuaDan.at(pLastRTData->m_lPSell[4]);
          }
          else { // 
            m_mapGuaDan.at(pLastRTData->m_lPSell[4]) = pLastRTData->m_lVSell[4];
          }
        }
      }
      break;
    case 1: // 卖单四
      if (fNeedCheck[1]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[3]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPSell[3]] = pLastRTData->m_lVSell[3];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[3]) < pLastRTData->m_lVSell[3]) { // 撤单了的话
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[3] - m_mapGuaDan.at(pLastRTData->m_lPSell[3]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[3] - m_mapGuaDan.at(pLastRTData->m_lPSell[3]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[3]) = pLastRTData->m_lVSell[3];
          }
        }
      }
      break;
    case 2: // 卖单三
      if (fNeedCheck[2]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[2]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPSell[2]] = pLastRTData->m_lVSell[2];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[2]) < pLastRTData->m_lVSell[2]) { // 撤单了的话
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[2] - m_mapGuaDan.at(pLastRTData->m_lPSell[2]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[2] - m_mapGuaDan.at(pLastRTData->m_lPSell[2]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[2]) = pLastRTData->m_lVSell[2];
          }
        }
      }
      break;
    case 3: // 卖单二
      if (fNeedCheck[3]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[1]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPSell[1]] = pLastRTData->m_lVSell[1];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[1]) < pLastRTData->m_lVSell[1]) { // 撤单了的话
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[1] - m_mapGuaDan.at(pLastRTData->m_lPSell[1]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[1] - m_mapGuaDan.at(pLastRTData->m_lPSell[1]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[1]) = pLastRTData->m_lVSell[1];
          }
        }
      }
      break;
    case 4: // 卖单一
      if (fNeedCheck[4]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[0]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPSell[0]] = pLastRTData->m_lVSell[0];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[0]) < pLastRTData->m_lVSell[0]) { // 撤单了的话
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[0] - m_mapGuaDan.at(pLastRTData->m_lPSell[0]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[0] - m_mapGuaDan.at(pLastRTData->m_lPSell[0]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[0]) = pLastRTData->m_lVSell[0];
          }
        }
      }
      break;
    case 5:   // 以下为买单变化情况，买单一
      if (fNeedCheck[5]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[0]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPBuy[0]] = pLastRTData->m_lVBuy[0];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[0]) < pLastRTData->m_lVBuy[0]) { // 撤单了的话
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[0] - m_mapGuaDan.at(pLastRTData->m_lPBuy[0]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[0] - m_mapGuaDan.at(pLastRTData->m_lPBuy[0]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[0]) = pLastRTData->m_lVBuy[0];
          }
        }
      }
      break;
    case 6: // 买单二
      if (fNeedCheck[6]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[1]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPBuy[1]] = pLastRTData->m_lVBuy[1];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[1]) < pLastRTData->m_lVBuy[1]) { // 撤单了的话
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[1] - m_mapGuaDan.at(pLastRTData->m_lPBuy[1]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[1] - m_mapGuaDan.at(pLastRTData->m_lPBuy[1]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[1]) = pLastRTData->m_lVBuy[1];
          }
        }
      }
      break;
    case 7: // 买单三
      if (fNeedCheck[7]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[2]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPBuy[2]] = pLastRTData->m_lVBuy[2];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[2]) < pLastRTData->m_lVBuy[2]) { // 撤单了的话
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[2] - m_mapGuaDan.at(pLastRTData->m_lPBuy[2]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[2] - m_mapGuaDan.at(pLastRTData->m_lPBuy[2]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[2]) = pLastRTData->m_lVBuy[2];
          }
        }
      }
      break;
    case 8: // 买单四
      if (fNeedCheck[8]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[3]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPBuy[3]] = pLastRTData->m_lVBuy[3];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[3]) < pLastRTData->m_lVBuy[3]) { // 撤单了的话
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[3] - m_mapGuaDan.at(pLastRTData->m_lPBuy[3]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[3] - m_mapGuaDan.at(pLastRTData->m_lPBuy[3]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[3]) = pLastRTData->m_lVBuy[3];
          }
        }
      }
      break;
    case 9: // 买单五
      if (fNeedCheck[9]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[4]) == m_mapGuaDan.end()) { // 此价位没有挂单
          m_mapGuaDan[pLastRTData->m_lPBuy[4]] = pLastRTData->m_lVBuy[4];
        }
        else {  // 有挂单
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[4]) < pLastRTData->m_lVBuy[4]) { // 撤单了的话
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[4] - m_mapGuaDan.at(pLastRTData->m_lPBuy[4]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[4] - m_mapGuaDan.at(pLastRTData->m_lPBuy[4]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[4]) = pLastRTData->m_lVBuy[4];
          }
        }
      }
      break;
    default : // 没有这个选项
      ASSERT(0);
    }
  }

  // 显示当前取消挂单的情况
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->m_strStockCode.Compare(m_strStockCode) == 0) {
      CString str1;
      char buffer[30];
      if (m_lCurrentCanselSellVolume > 0) {
        sprintf_s(buffer, "当前取消卖单量：%d", m_lCurrentCanselSellVolume);
        str1 = buffer;
        sprintf_s(buffer, "  总取消卖单量：%d", m_lCancelSellVolume);
        str1 += buffer;
        gl_systemMessage.PushDataBaseMessage(str1);   // 采用同步机制传递消息

      }
      if (m_lCurrentCanselBuyVolume > 0) {
        sprintf_s(buffer, "当前取消买单量：%d", m_lCurrentCanselBuyVolume);
        str1 = buffer;
        sprintf_s(buffer, "  总取消买单量：%d", m_lCancelBuyVolume);
        str1 += buffer;
        gl_systemMessage.PushTrace1Message(str1); // 采用同步机制传递消息

      }
    }
  }
  return(true);
}

bool CStock::SaveRealTimeData(CSetRealTimeData * psetRTData) {

  ASSERT(psetRTData->IsOpen());
  for ( auto pRTData : m_dequeRTStockData) {
    psetRTData->AddNew();
    psetRTData->m_Time = pRTData->m_time;
    psetRTData->m_lMarket = m_wMarket;
    psetRTData->m_StockCode = m_strStockCode;
    psetRTData->m_StockName = m_strStockName;
    psetRTData->m_CurrentPrice = (double)pRTData->m_lNew / 1000;
    psetRTData->m_High = (double)pRTData->m_lHigh / 1000;
    psetRTData->m_Low = (double)pRTData->m_lLow / 1000;
    psetRTData->m_LastClose = (double)pRTData->m_lLastClose / 1000;
    psetRTData->m_Open = (double)pRTData->m_lOpen / 1000;
    psetRTData->m_Volume = pRTData->m_lVolume;
    psetRTData->m_Amount = (double)pRTData->m_lAmount;
    psetRTData->m_Stroke = 0;
    psetRTData->m_PBuy1 = (double)pRTData->m_lPBuy[0] / 1000;
    psetRTData->m_VBuy1 = pRTData->m_lVBuy[0];
    psetRTData->m_PSell1 = (double)pRTData->m_lPSell[0] / 1000;
    psetRTData->m_VSell1 = pRTData->m_lVSell[0];

    psetRTData->m_PBuy2 = (double)pRTData->m_lPBuy[1] / 1000;
    psetRTData->m_VBuy2 = pRTData->m_lVBuy[1];
    psetRTData->m_PSell2 = (double)pRTData->m_lPSell[1] / 1000;
    psetRTData->m_VSell2 = pRTData->m_lVSell[1];

    psetRTData->m_PBuy3 = (double)pRTData->m_lPBuy[2] / 1000;
    psetRTData->m_VBuy3 = pRTData->m_lVBuy[2];
    psetRTData->m_PSell3 = (double)pRTData->m_lPSell[2] / 1000;
    psetRTData->m_VSell3 = pRTData->m_lVSell[2];

    psetRTData->m_PBuy4 = (double)pRTData->m_lPBuy[3] / 1000;
    psetRTData->m_VBuy4 = pRTData->m_lVBuy[3];
    psetRTData->m_PSell4 = (double)pRTData->m_lPSell[3] / 1000;
    psetRTData->m_VSell4 = pRTData->m_lVSell[3];

    psetRTData->m_PBuy5 = (double)pRTData->m_lPBuy[4] / 1000;
    psetRTData->m_VBuy5 = pRTData->m_lVBuy[4];
    psetRTData->m_PSell5 = (double)pRTData->m_lPSell[4] / 1000;
    psetRTData->m_VSell5 = pRTData->m_lVSell[4];
    psetRTData->Update();

  }

  return true;
}

void CStock::PushRTStockData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTStockData.push_back(pData);
    singleLock.Unlock();
  }
}

CStockRTDataPtr CStock::PopRTStockData(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTStockData.front();
    m_dequeRTStockData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  return nullptr;
}

long CStock::GetRTDataDequeSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

void CStock::SetDayLineNeedSavingFlag(bool fFlag)
{
  CSingleLock singleLock(&m_DayLineNeedSacingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineNeededSaving = fFlag;
    singleLock.Unlock();
  }
}

bool CStock::IsDayLineNeedSaving(void)
{
  CSingleLock singleLock(&m_DayLineNeedSacingLock);
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


///////////////////////////////////////////////////////////////////////////////////
//
//	将本股票的历史数据存入日线数据库(DayKLine.mdb)
//
//	一次处理不能太多，所以将数据库打开和关闭放在这里，如果放在上一层的话，
// 当处理的股票太多时(>400)，数据库引擎出现故障。
//
// 调用本函数最好使用On_Timer, On_Idle.在系统空闲时一次处理一只股票。
//
// 存储时要倒着存，使用m_lTotalDLDayKLine,因为有可能股票的日线数量大于下载的日线数量，
// 此存储函数只将下载的日线存入数据库。其他不管。
//
////////////////////////////////////////////////////////////////////////////////////



