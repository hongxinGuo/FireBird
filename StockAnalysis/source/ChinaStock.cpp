#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include"Accessory.h"

#include "ChinaStock.h"
#include"ChinaMarket.h"

using namespace std;
#include"string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChinaStock::CChinaStock() : CObject() {
  Reset();
}

CChinaStock::~CChinaStock(void) {
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
}

void CChinaStock::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_lOffsetInContainer = -1;
  m_lDayLineStartDay = __CHINA_MARKET_BEGIN_DAY__; //
  m_lDayLineEndDay = __CHINA_MARKET_BEGIN_DAY__; //
  m_lIPOStatus = __STOCK_NOT_CHECKED__;   // 默认状态为无效股票代码。
  m_nHand = 100;

  m_TransactionTime = 0;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  m_lHighLimit = m_lLowLimit = 0;
  m_llVolume = 0;
  m_llAmount = 0;
  m_lUpDown = 0;
  m_dUpDownRate = 0;
  m_dChangeHandRate = 0;
  m_llTotalValue = m_llCurrentValue = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_dRealtimeRelativeStrong = 0;
  m_dRealtimeRelativeStrongIndex = 0;

  m_lAttackBuyAmount = 0;
  m_lAttackSellAmount = 0;
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
  m_lCanceledBuyVolume = 0;
  m_lCanceledSellVolume = 0;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;

  m_lOrdinaryBuyNumberBelow5000 = 0;
  m_lOrdinaryBuyNumberBelow10000 = 0;
  m_lOrdinaryBuyNumberBelow20000 = 0;
  m_lOrdinaryBuyNumberBelow50000 = 0;
  m_lOrdinaryBuyNumberBelow100000 = 0;
  m_lOrdinaryBuyNumberBelow200000 = 0;
  m_lOrdinaryBuyNumberAbove200000 = 0;
  m_lOrdinarySellNumberBelow5000 = 0;
  m_lOrdinarySellNumberBelow10000 = 0;
  m_lOrdinarySellNumberBelow20000 = 0;
  m_lOrdinarySellNumberBelow50000 = 0;
  m_lOrdinarySellNumberBelow100000 = 0;
  m_lOrdinarySellNumberBelow200000 = 0;
  m_lOrdinarySellNumberAbove200000 = 0;
  m_lOrdinaryBuyVolumeBelow5000 = 0;
  m_lOrdinaryBuyVolumeBelow10000 = 0;
  m_lOrdinaryBuyVolumeBelow20000 = 0;
  m_lOrdinaryBuyVolumeBelow50000 = 0;
  m_lOrdinaryBuyVolumeBelow100000 = 0;
  m_lOrdinaryBuyVolumeBelow200000 = 0;
  m_lOrdinaryBuyVolumeAbove200000 = 0;
  m_lOrdinarySellVolumeBelow5000 = 0;
  m_lOrdinarySellVolumeBelow10000 = 0;
  m_lOrdinarySellVolumeBelow20000 = 0;
  m_lOrdinarySellVolumeBelow50000 = 0;
  m_lOrdinarySellVolumeBelow100000 = 0;
  m_lOrdinarySellVolumeBelow200000 = 0;
  m_lOrdinarySellVolumeAbove200000 = 0;

  m_lCanceledBuyVolumeBelow5000 = 0;
  m_lCanceledBuyVolumeBelow10000 = 0;
  m_lCanceledBuyVolumeBelow20000 = 0;
  m_lCanceledBuyVolumeBelow50000 = 0;
  m_lCanceledBuyVolumeBelow100000 = 0;
  m_lCanceledBuyVolumeBelow200000 = 0;
  m_lCanceledBuyVolumeAbove200000 = 0;
  m_lCanceledSellVolumeBelow5000 = 0;
  m_lCanceledSellVolumeBelow10000 = 0;
  m_lCanceledSellVolumeBelow20000 = 0;
  m_lCanceledSellVolumeBelow50000 = 0;
  m_lCanceledSellVolumeBelow100000 = 0;
  m_lCanceledSellVolumeBelow200000 = 0;
  m_lCanceledSellVolumeAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

  m_llLastSavedVolume = 0;

  m_lCurrentCanceledSellVolume = m_lCurrentCanceledBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_dCurrentGuadanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fActive = false;

  m_fDayLineLoaded = false;
  m_fDayLineNeedUpdate = true;
  m_fDayLineNeedProcess = false; // 从网络上读取了日线历史数据
  m_fDayLineNeedSaving = false;
  m_lDayLineBufferLength = 0;
  m_pDayLineBuffer = nullptr;

  m_fChoiced = false;
  m_fSaveToChoicedStockDB = false;

  m_fDayLineDBUpdated = false;

  m_fHaveFirstRTData = false;  // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
  m_fNeedProcessRTData = true;
  m_fRTDataCalculated = false;
  m_fRecordRTData = false;
  m_pLastRTData = nullptr;

  ClearRTDataDeque();

  m_fLoadDayLineFirst = false;
}

void CChinaStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataQueueSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CWebRTDataPtr pRTData = PopRTData();
  }
}

bool CChinaStock::HaveNewDayLineData(void) {
  if (m_vDayLine.size() <= 0) return false;
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDay() > GetDayLineEndDay()) return true;
  else return false;
}

void CChinaStock::SetDayLineNeedSaving(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedSaveNumber();
  }
  else {
    ASSERT(m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedSaveNumber();
  }
}

bool CChinaStock::IsDayLineNeedSavingAndClearFlag(void) {
  bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  if (fNeedSaveing) gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedSaveNumber();
  return fNeedSaveing;
}

bool CChinaStock::TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) {
  // 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
  char* p = pNeteaseWebDayLineData->GetBufferAddr();
  if (m_pDayLineBuffer != nullptr) delete m_pDayLineBuffer;
  m_pDayLineBuffer = new char[pNeteaseWebDayLineData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
  char* pbuffer = m_pDayLineBuffer;
  for (int i = 0; i < pNeteaseWebDayLineData->GetByteReaded() + 1; i++) {
    *pbuffer++ = *p++;
  }
  m_lDayLineBufferLength = pNeteaseWebDayLineData->GetByteReaded();
  SetDayLineNeedProcess(true);

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessNeteaseDayLineData(void) {
  char* pTestPos = m_pDayLineBuffer;
  vector<CDayLinePtr> vTempDayLine;
  shared_ptr<CDayLine> pDayLine;

  ASSERT(m_fDayLineNeedProcess);
  ASSERT(m_fDayLineNeedSaving == false);
  if (m_lDayLineBufferLength == 0) { // 没有数据读入？此种状态是查询的股票为无效（不存在）号码
    return false;
  }

  ASSERT(m_pDayLineBuffer[m_lDayLineBufferLength] == 0x000); // 最后字符为增加的0x000.
  ResetCurrentPos();
  if (!SkipNeteaseDayLineInformationHeader()) return false;

  pTestPos = m_pDayLineBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  if (m_llCurrentPos == m_lDayLineBufferLength) {// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天
    if (GetDayLineEndDay() == __CHINA_MARKET_BEGIN_DAY__) { // 如果初始日线结束日期从来没有变更过，则此股票代码尚未被使用过
      SetIPOStatus(__STOCK_NULL__);   // 此股票代码尚未使用。
      //TRACE("无效股票代码:%s\n", GetStockCode().GetBuffer());
    }
    else { // 已经退市的股票
      if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetFormatedMarketDay(), 30)) {
        SetIPOStatus(__STOCK_DELISTED__);   // 此股票代码已经退市。
      }
      //TRACE("%S没有可更新的日线数据\n", GetStockCode().GetBuffer());
    }
    return false;
  }

  CString strTemp;
  pTestPos = m_pDayLineBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  while (m_llCurrentPos < m_lDayLineBufferLength) {
    pDayLine = make_shared<CDayLine>();
    if (!pDayLine->ProcessNeteaseData(GetStockCode(), m_pCurrentPos, m_llCurrentPos)) { // 处理一条日线数据
      TRACE(_T("%s日线数据出错\n"), pDayLine->GetStockCode().GetBuffer());
      // 清除已暂存的日线数据
      vTempDayLine.clear();
      return false; // 数据出错，放弃载入
    }
    pTestPos = m_pDayLineBuffer + m_llCurrentPos;
    ASSERT(*pTestPos == *m_pCurrentPos);
    if (!IsActive()) { // 新的股票代码？
      // 生成新股票
      SetTodayActive(pDayLine->GetMarket(), pDayLine->GetStockCode(), pDayLine->GetStockName());
      TRACE("下载日线函数生成新的活跃股票%s\n", GetStockCode().GetBuffer());
    }
    vTempDayLine.push_back(pDayLine); // 暂存于临时vector中，因为网易日线数据的时间顺序是颠倒的，最新的在最前面
  }
  ReportDayLineDownLoaded();
  if (gl_pChinaStockMarket->IsEarlyThen(vTempDayLine.at(0)->GetFormatedMarketDay(), gl_pChinaStockMarket->GetFormatedMarketDay(), 30)) { // 提取到的股票日线数据其最新日早于上个月的这个交易日（退市了或相似情况，给一个月的时间观察）。
    SetIPOStatus(__STOCK_DELISTED__); // 已退市或暂停交易。
  }
  else {
    SetIPOStatus(__STOCK_IPOED__); // 正常交易股票
  }

  m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->IsActive()) {
      // 清除掉不再交易（停牌或退市）的股票日线
      m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
  SetDayLineLoaded(true);
  SetDayLineNeedSaving(true); // 设置存储日线标识

  return true;
}

bool CChinaStock::SkipNeteaseDayLineInformationHeader() {
  ASSERT(m_pCurrentPos == m_pDayLineBuffer);
  ASSERT(m_llCurrentPos == 0);
  while (*m_pCurrentPos != 0X0d) {
    if ((*m_pCurrentPos == 0x0a) || (*m_pCurrentPos == 0x000)) {
      return false;
    }
    IncreaseCurrentPos();
  }
  IncreaseCurrentPos();
  if (*m_pCurrentPos != 0x0a) {
    return false;
  }
  IncreaseCurrentPos();
  return true;
}

void CChinaStock::SetTodayActive(WORD wMarket, CString strStockCode, CString strStockName) {
  SetActive(true);
  SetDayLineLoaded(false);
  SetMarket(wMarket);
  SetStockCode(strStockCode); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
  if (strStockName != _T("")) SetStockName(strStockName);// 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
  gl_pChinaStockMarket->SetTotalActiveStock(gl_pChinaStockMarket->GetTotalActiveStock() + 1);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::UpdateDayLine(vector<CDayLinePtr>& vTempDayLine) {
  CDayLinePtr pDayLine = nullptr;
  m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = 0; i < vTempDayLine.size(); i++) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->IsActive()) {
      // 清除掉不再交易（停牌或退市后出现的）的股票日线
      m_vDayLine.push_back(pDayLine);
    }
  }
  SetDayLineLoaded(true);
}

void CChinaStock::ReportDayLineDownLoaded(void) {
  CString strTemp = GetStockCode();
  strTemp += _T("日线下载完成.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::SaveTodayBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo) {
  ASSERT(psetDayLineBasicInfo->IsOpen());
  psetDayLineBasicInfo->m_Day = FormatToDay(m_TransactionTime);
  psetDayLineBasicInfo->m_Market = m_wMarket;
  psetDayLineBasicInfo->m_StockCode = m_strStockCode;
  psetDayLineBasicInfo->m_StockName = m_strStockName;
  psetDayLineBasicInfo->m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  psetDayLineBasicInfo->m_Open = ConvertValueToString(m_lOpen, 1000);
  psetDayLineBasicInfo->m_High = ConvertValueToString(m_lHigh, 1000);
  psetDayLineBasicInfo->m_Low = ConvertValueToString(m_lLow, 1000);
  psetDayLineBasicInfo->m_Close = ConvertValueToString(m_lNew, 1000);
  psetDayLineBasicInfo->m_Volume = ConvertValueToString(m_llVolume);
  psetDayLineBasicInfo->m_Amount = ConvertValueToString(m_llAmount);
  psetDayLineBasicInfo->m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  psetDayLineBasicInfo->m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  if (m_llTotalValue != 0) psetDayLineBasicInfo->m_ChangeHandRate = ConvertValueToString((double)100 * m_llAmount / m_llTotalValue);
  else psetDayLineBasicInfo->m_ChangeHandRate = ConvertValueToString(0);
  psetDayLineBasicInfo->m_TotalValue = ConvertValueToString(m_llTotalValue);
  psetDayLineBasicInfo->m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CChinaStock::SaveTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  setDayLineToday.m_Day = FormatToDay(m_TransactionTime);
  setDayLineToday.m_Market = m_wMarket;
  setDayLineToday.m_StockCode = m_strStockCode;
  setDayLineToday.m_StockName = m_strStockName;
  setDayLineToday.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLineToday.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLineToday.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLineToday.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLineToday.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLineToday.m_Volume = ConvertValueToString(m_llVolume);
  setDayLineToday.m_Amount = ConvertValueToString(m_llAmount);
  setDayLineToday.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDayLineToday.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  setDayLineToday.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLineToday.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
  setDayLineToday.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineToday.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineToday.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineToday.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineToday.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);
  setDayLineToday.m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  setDayLineToday.m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  setDayLineToday.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineToday.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineToday.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineToday.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineToday.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineToday.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineToday.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineToday.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineToday.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineToday.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineToday.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineToday.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineToday.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  setDayLineToday.m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  setDayLineToday.m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  setDayLineToday.m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  setDayLineToday.m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  setDayLineToday.m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  setDayLineToday.m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  setDayLineToday.m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  setDayLineToday.m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  setDayLineToday.m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  setDayLineToday.m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  setDayLineToday.m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  setDayLineToday.m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  setDayLineToday.m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  setDayLineToday.m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  setDayLineToday.m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  setDayLineToday.m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  setDayLineToday.m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  setDayLineToday.m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  setDayLineToday.m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  setDayLineToday.m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  setDayLineToday.m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  setDayLineToday.m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  setDayLineToday.m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  setDayLineToday.m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  setDayLineToday.m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  setDayLineToday.m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  setDayLineToday.m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  setDayLineToday.m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  setDayLineToday.m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  setDayLineToday.m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  setDayLineToday.m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  setDayLineToday.m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  setDayLineToday.m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  setDayLineToday.m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  setDayLineToday.m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  setDayLineToday.m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  setDayLineToday.m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);
}

void CChinaStock::UpdateStatus(CWebRTDataPtr pRTData) {
  SetTransactionTime(pRTData->GetTransactionTime());
  SetLastClose(pRTData->GetLastClose());
  SetNew(pRTData->GetNew());
  SetHigh(pRTData->GetHigh());
  SetLow(pRTData->GetLow());
  SetOpen(pRTData->GetOpen());
  SetVolume(pRTData->GetVolume());
  SetAmount(pRTData->GetAmount());
  SetUpDown(m_lNew - m_lLastClose);
  if (m_lLastClose != 0) SetUpDownRate((double)m_lUpDown * 100 / m_lLastClose);
  else SetUpDownRate(0);
  // 新浪实时数据是没有总市值和流通市值这两项的，故而需要判断一下是否为零
  if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
  if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
  if (m_llTotalValue != 0) m_dChangeHandRate = (double)m_llAmount * 100 / m_llTotalValue;
  else m_dChangeHandRate = 0;
  for (int i = 0; i < 5; i++) {
    SetPBuy(i, pRTData->GetPBuy(i));
    SetVBuy(i, pRTData->GetVBuy(i));
    SetPSell(i, pRTData->GetPSell(i));
    SetVSell(i, pRTData->GetVSell(i));
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
// 当存在旧日线历史数据时，本函数只是更新。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveDayLineBasicInfo(void) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  size_t lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(m_vDayLine.size() > 0);
  ASSERT(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDay() > m_lDayLineEndDay);

  lSize = m_vDayLine.size();
  setDayLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineBasicInfo.m_strFilter += GetStockCode() + _T("'");
  setDayLineBasicInfo.m_strSort = _T("[Day]");

  setDayLineBasicInfo.Open();
  while (!setDayLineBasicInfo.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadBasicData(&setDayLineBasicInfo);
    vDayLine.push_back(pDayLine);
    lCurrentPos++;
    setDayLineBasicInfo.MoveNext();
  }
  setDayLineBasicInfo.Close();

  lSizeOfOldDayLine = lCurrentPos;
  lCurrentPos = 0;
  setDayLineBasicInfo.Open();
  setDayLineBasicInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
    pDayLine = m_vDayLine.at(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDay() < pDayLine->GetFormatedMarketDay())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetFormatedMarketDay() > pDayLine->GetFormatedMarketDay()) {
        pDayLine->AppendData(&setDayLineBasicInfo);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendData(&setDayLineBasicInfo);
      fNeedUpdate = true;
    }
  }
  setDayLineBasicInfo.m_pDatabase->CommitTrans();
  setDayLineBasicInfo.Close();

  return fNeedUpdate;
}

void CChinaStock::UpdateDayLineStartEndDay(void) {
  if (m_vDayLine.size() == 0) {
    SetDayLineStartDay(__CHINA_MARKET_BEGIN_DAY__);
    SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__);
  }
  else {
    if (m_vDayLine.at(0)->GetFormatedMarketDay() < GetDayLineStartDay()) {
      SetDayLineStartDay(m_vDayLine.at(0)->GetFormatedMarketDay());
      SetDayLineDBUpdated(true);
    }
    if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDay() > GetDayLineEndDay()) {
      SetDayLineEndDay(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDay());
      SetDayLineDBUpdated(true);
    }
  }
}

void CChinaStock::SaveTodayExtendInfo(CSetDayLineExtendInfo* psetDayLineExtendInfo) {
  ASSERT(psetDayLineExtendInfo->IsOpen());
  psetDayLineExtendInfo->m_Day = FormatToDay(m_TransactionTime);
  psetDayLineExtendInfo->m_Market = m_wMarket;
  psetDayLineExtendInfo->m_StockCode = m_strStockCode;
  psetDayLineExtendInfo->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  psetDayLineExtendInfo->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  psetDayLineExtendInfo->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  psetDayLineExtendInfo->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  psetDayLineExtendInfo->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  psetDayLineExtendInfo->m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  psetDayLineExtendInfo->m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  psetDayLineExtendInfo->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  psetDayLineExtendInfo->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  psetDayLineExtendInfo->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  psetDayLineExtendInfo->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  psetDayLineExtendInfo->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  psetDayLineExtendInfo->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  psetDayLineExtendInfo->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  psetDayLineExtendInfo->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  psetDayLineExtendInfo->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  psetDayLineExtendInfo->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  psetDayLineExtendInfo->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  psetDayLineExtendInfo->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  psetDayLineExtendInfo->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);

  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  psetDayLineExtendInfo->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  psetDayLineExtendInfo->m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  psetDayLineExtendInfo->m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  psetDayLineExtendInfo->m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  psetDayLineExtendInfo->m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  psetDayLineExtendInfo->m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  psetDayLineExtendInfo->m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  psetDayLineExtendInfo->m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);
}

////////////////////////////////////////////////////////////////////////////
//
// 只有一处调用本函数，就是在系统初始化时装入之前计算出的信息（如果开市了的话）.
// 需要同时更新总成交股数，并暂存此股数（用于计算未明情况成交量。 总成交股数在新的实时数据来临时会同步更新，故而无法用于计算）
//
////////////////////////////////////////////////////////////////////////////
void CChinaStock::LoadTempInfo(CSetDayLineToday& setDayLineToday) {
  m_lUnknownVolume = atoll(setDayLineToday.m_UnknownVolume);

  m_lTransactionNumber = atol(setDayLineToday.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineToday.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineToday.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineToday.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineToday.m_TransactionNumberAbove200000);
  m_lCanceledBuyVolume = atoll(setDayLineToday.m_CanceledBuyVolume);
  m_lCanceledSellVolume = atoll(setDayLineToday.m_CanceledSellVolume);
  m_lAttackBuyVolume = atoll(setDayLineToday.m_AttackBuyVolume);
  m_lAttackSellVolume = atoll(setDayLineToday.m_AttackSellVolume);
  m_lStrongBuyVolume = atoll(setDayLineToday.m_StrongBuyVolume);
  m_lStrongSellVolume = atoll(setDayLineToday.m_StrongSellVolume);
  m_lOrdinaryBuyVolume = atoll(setDayLineToday.m_OrdinaryBuyVolume);
  m_lOrdinarySellVolume = atoll(setDayLineToday.m_OrdinarySellVolume);
  m_lAttackBuyBelow50000 = atoll(setDayLineToday.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atoll(setDayLineToday.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atoll(setDayLineToday.m_AttackBuyAbove200000);
  m_lAttackSellBelow50000 = atoll(setDayLineToday.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atoll(setDayLineToday.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atoll(setDayLineToday.m_AttackSellAbove200000);
  m_llVolume = atoll(setDayLineToday.m_Volume);
  m_llLastSavedVolume = atoll(setDayLineToday.m_Volume);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(setDayLineToday.m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(setDayLineToday.m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(setDayLineToday.m_OrdinarySellVolumeAbove200000);
  m_lOrdinaryBuyNumberBelow5000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(setDayLineToday.m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(setDayLineToday.m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(setDayLineToday.m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(setDayLineToday.m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(setDayLineToday.m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(setDayLineToday.m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(setDayLineToday.m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(setDayLineToday.m_CanceledSellVolumeAbove200000);
}

bool CChinaStock::LoadDayLine(void) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  CSetDayLineExtendInfo setDayLineExtendInfo;

  // 装入DayLine数据
  setDayLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineBasicInfo.m_strFilter += GetStockCode();
  setDayLineBasicInfo.m_strFilter += _T("'");
  setDayLineBasicInfo.m_strSort = _T("[Day]");
  setDayLineBasicInfo.Open();
  LoadDayLineBasicInfo(&setDayLineBasicInfo);
  setDayLineBasicInfo.Close();

  // 装入DayLineInfo数据
  setDayLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineExtendInfo.m_strFilter += GetStockCode();
  setDayLineExtendInfo.m_strFilter += _T("'");
  setDayLineExtendInfo.m_strSort = _T("[Day]");
  setDayLineExtendInfo.Open();
  LoadDayLineExtendInfo(&setDayLineExtendInfo);
  setDayLineExtendInfo.Close();

  m_fDayLineLoaded = true;
  return true;
}

bool CChinaStock::LoadDayLineBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo) {
  CDayLinePtr pDayLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDayLineFirst);
  // 装入DayLine数据
  m_vDayLine.clear();
  while (!psetDayLineBasicInfo->IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadBasicData(psetDayLineBasicInfo);
    m_vDayLine.push_back(pDayLine);
    psetDayLineBasicInfo->MoveNext();
  }
  m_fLoadDayLineFirst = true;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// 装载DayLineInfo表必须在装载DayLine表之后。
//
//
////////////////////////////////////////////////////////////////////////////
bool CChinaStock::LoadDayLineExtendInfo(CSetDayLineExtendInfo* psetDayLineExtendInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDayLineFirst);

  while (!psetDayLineExtendInfo->IsEOF()) {
    pDayLine = m_vDayLine.at(iPosition);
    while ((pDayLine->GetFormatedMarketDay() < psetDayLineExtendInfo->m_Day)
           && (m_vDayLine.size() > (iPosition + 1))) {
      iPosition++;
      pDayLine = m_vDayLine.at(iPosition);
    }
    if (pDayLine->GetFormatedMarketDay() == psetDayLineExtendInfo->m_Day) {
      pDayLine->LoadExtendData(psetDayLineExtendInfo);
    }
    if (m_vDayLine.size() <= (iPosition + 1)) break;
    psetDayLineExtendInfo->MoveNext();
  }
  m_fLoadDayLineFirst = false;
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrong(void) {
  CalculateDayLineRS(3);
  CalculateDayLineRS(5);
  CalculateDayLineRS(10);
  CalculateDayLineRS(30);
  CalculateDayLineRS(60);
  CalculateDayLineRS(120);
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrongLogarithm(void) {
  CalculateDayLineRSLogarithm(3);
  CalculateDayLineRSLogarithm(5);
  CalculateDayLineRSLogarithm(10);
  CalculateDayLineRSLogarithm(30);
  CalculateDayLineRSLogarithm(60);
  CalculateDayLineRSLogarithm(120);
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrongIndex(void) {
  CalculateDayLineRSIndex(3);
  CalculateDayLineRSIndex(5);
  CalculateDayLineRSIndex(10);
  CalculateDayLineRSIndex(30);
  CalculateDayLineRSIndex(60);
  CalculateDayLineRSIndex(120);
  return true;
}

bool CChinaStock::CalculateDayLineRSLogarithm(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrongLogarithm();
    }
    switch (lNumber) {
    case 3:
    m_vDayLine.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}

bool CChinaStock::CalculateDayLineRS(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrong();
    }
    switch (lNumber) {
    case 3:
    m_vDayLine.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}

bool CChinaStock::CalculateDayLineRSIndex(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrongIndex();
    }
    switch (lNumber) {
    case 3:
    m_vDayLine.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}

bool CChinaStock::CalculateWeekLineRelativeStrong(void) {
  return m_WeekLine.CalculateRelativeStrong();
}

bool CChinaStock::CalculateWeekLineRelativeStrongLogarithm(void) {
  return m_WeekLine.CalculateRelativeStrongLogarithm();
}

bool CChinaStock::CalculateWeekLineRelativeStrongIndex(void) {
  return m_WeekLine.CalculateRelativeStrongIndex();
}

bool CChinaStock::Calculate10RSStrong2StockSet(void) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0;

  ASSERT(m_fDayLineLoaded);
  size_t iDayLineSize = GetDayLineSize();
  if (iDayLineSize > 155) {
    m_vRS10Day.resize(iDayLineSize);
    CalculateDayLineRelativeStrongIndex();
    GetRS10Day(m_vRS10Day);
    int i = 0;
    for (i = iDayLineSize - 1; i > iDayLineSize - 5; i--) {
      if (m_vRS10Day.at(i) > 55) iCountFirst++;
      if (iCountFirst >= 3) break;
    }
    if (iCountFirst >= 3) {
      while (m_vRS10Day.at(i) > 53) i--;
      for (int j = i; j > i - 50; j--) {
        if (m_vRS10Day.at(j) > 55) iCountSecond++;
      }
      if (iCountSecond >= 3) {
        return true;
      }
    }
  }

  return false;
}

bool CChinaStock::Calculate10RSStrong1StockSet(void) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0, iCountThird = 0;

  ASSERT(m_fDayLineLoaded);
  size_t iDayLineSize = GetDayLineSize();

  if (iDayLineSize < 350) return false;
  m_vRS10Day.resize(iDayLineSize);
  CalculateDayLineRelativeStrongIndex();
  GetRS10Day(m_vRS10Day);
  int i = 0, j = 0;

  for (i = iDayLineSize - 1; i > iDayLineSize - 40; i--) {
    if (m_vRS10Day.at(i) > 55) iCountFirst++;
    if (iCountFirst >= 3) break;
  }
  if (iCountFirst < 3) return false;
  while (m_vRS10Day.at(i) > 53) i--;
  for (j = i; j > i - 80; j--) {
    if (m_vRS10Day.at(j) < 55) iCountSecond++;
    if (iCountSecond >= 78) break;
  }
  if (iCountSecond < 78) return false;
  while (m_vRS10Day.at(j) > 53) j--;
  for (int k = j; k > j - 130; k--) {
    if (m_vRS10Day.at(k) > 47) iCountThird++;
    if (iCountThird > 128) break;
  }
  if (iCountThird < 128) return false;

  return true;
}

bool CChinaStock::Calculate10RSStrongStockSet(CRSReference* pRef) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0, iCountThird = 0, iCountFourth = 0;
  bool fFind1 = false, fFind2 = false, fFind3 = false, fFind4 = false;
  double dStrong1, dStrong2, dStrong3, dStrong4;
  bool fFindHigh1, fFindHigh2, fFindHigh3, fFindHigh4;

  if (pRef->m_dRSStrong[0] < 0) {
    dStrong1 = -pRef->m_dRSStrong[0];
    fFindHigh1 = false;
  }
  else {
    dStrong1 = pRef->m_dRSStrong[0];
    fFindHigh1 = true;
  }
  if (pRef->m_dRSStrong[1] < 0) {
    dStrong2 = -pRef->m_dRSStrong[1];
    fFindHigh2 = false;
  }
  else {
    dStrong2 = pRef->m_dRSStrong[1];
    fFindHigh2 = true;
  }
  if (pRef->m_dRSStrong[2] < 0) {
    dStrong3 = -pRef->m_dRSStrong[2];
    fFindHigh3 = false;
  }
  else {
    dStrong3 = pRef->m_dRSStrong[2];
    fFindHigh3 = true;
  }
  if (pRef->m_dRSStrong[3] < 0) {
    dStrong4 = -pRef->m_dRSStrong[3];
    fFindHigh4 = false;
  }
  else {
    dStrong4 = pRef->m_dRSStrong[3];
    fFindHigh4 = true;
  }

  ASSERT(m_fDayLineLoaded);
  size_t iDayLineSize = GetDayLineSize();
  if ((iDayLineSize < (pRef->m_lDayLength[0] + pRef->m_lDayLength[1] + pRef->m_lDayLength[2] + 10))
      || (iDayLineSize < pRef->m_lDayLength[3] + 10)) return false;

  m_vRS10Day.resize(iDayLineSize);
  CalculateDayLineRelativeStrongIndex();
  GetRS10Day(m_vRS10Day);
  int i = 0, j = 0;

  if (pRef->m_lDayLength[0] == 0) fFind1 = true;
  else if (fFindHigh1) {
    for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
      if (m_vRS10Day.at(i) > dStrong1) iCountFirst++;
      if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
        fFind1 = true;
        break;
      }
    }
  }
  else {
    for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
      if (m_vRS10Day.at(i) < dStrong1) iCountFirst++;
      if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
        fFind1 = true;
        break;
      }
    }
  }
  if (!fFind1) return false;
  while (m_vRS10Day.at(i) > (dStrong1 - 2)) i--;

  if (pRef->m_lDayLength[1] == 0) fFind2 = true;
  else if (fFindHigh2) {
    for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
      if (m_vRS10Day.at(j) > dStrong2) iCountSecond++;
      if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
        fFind2 = true;
        break;
      }
    }
  }
  else {
    for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
      if (m_vRS10Day.at(j) < dStrong2) iCountSecond++;
      if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
        fFind2 = true;
        break;
      }
    }
  }
  if (!fFind2) return false;
  while (m_vRS10Day.at(j) > (dStrong2 - 2)) j--;

  if (pRef->m_lDayLength[2] == 0) fFind3 = true;
  else if (fFindHigh3) {
    for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
      if (m_vRS10Day.at(k) > dStrong3) iCountThird++;
      if (iCountThird >= pRef->m_lStrongDayLength[2]) {
        fFind3 = true;
        break;
      }
    }
  }
  else {
    for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
      if (m_vRS10Day.at(k) < dStrong3) iCountThird++;
      if (iCountThird >= pRef->m_lStrongDayLength[2]) {
        fFind3 = true;
        break;
      }
    }
  }
  if (!fFind3) return false;

  if (pRef->m_lDayLength[3] == 0) return true; // DayLength4为零的话，不做通盘选择
  else if (fFindHigh4) {
    for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
      if (m_vRS10Day.at(i) > dStrong4) iCountFourth++;
      if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
        fFind4 = true;
        break;
      }
    }
  }
  else {
    for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
      if (m_vRS10Day.at(i) < dStrong4) iCountFourth++;
      if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
        fFind4 = true;
        break;
      }
    }
  }
  if (!fFind4) return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 此函数被工作线程ThreadCalculatingRTDataProc调用，尽量减少对全局变量的操作。
//
// 交易情况和挂单情况似乎分开计算比较好，思路容易清晰。目前来看计算能力尚够，但要考虑旧机器的速度了。
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessRTData(void) {
  CWebRTDataPtr pRTData;

  INT64 lTotalNumber = GetRTDataQueueSize(); //  缓存队列的长度。采用同步机制获取其数值.
  if (lTotalNumber == 0) return false;
  // 以下为计算挂单变化、股票活跃度、大单买卖情况
  for (INT64 i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // 采用同步机制获取数据
    if (pRTData->IsActive()) { // 数据有效
      UpdateStatus(pRTData);   // 更新股票现时状态。
      if (gl_pChinaStockMarket->IsMarketOpened() && IsNeedProcessRTData()) {// 开市时间内计算具体情况。指数类股票无需计算交易情况和挂单变化
        ProcessOneRTData(pRTData);
        CheckCurrentRTData();
        m_fRTDataCalculated = true;
      }
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算一个挂单。
// 如果是第一次计算，则只设置初始状态。
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessOneRTData(CWebRTDataPtr pRTData) {
  if (HaveFirstRTData()) { // 如果开始计算（第二次收到实时数据及以后）
    CalculateOneRTData(pRTData);
  }
  else { // 第一次收到实时数据，则只初始化系统而不计算
    InitializeCalculatingRTDataEnvionment(pRTData);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 第一次收到实时数据时，只初始化系统，不计算（因为没有初始数据）
//
////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::InitializeCalculatingRTDataEnvionment(CWebRTDataPtr pRTData) {
  SetLastRTData(pRTData);
  SetHavingFirstRTData(true);
  // 第一次挂单量无法判断买卖状态，故而设置其为无法判断。如果之前已经运行过系统，此次是开盘中途登录的，则系统存储了临时数据于数据库中，
  // 在系统启动时已经将此临时状态读入了，故而m_lUnknownVolume不为零，故而需要重新计算m_lUnknownVolume.
  // m_lUnknownVolume = 当前的成交量 - 之前临时存储的成交量 + 之前存储的m_lUnkonwnVolume.
  SetVolume(pRTData->GetVolume());
  SetUnknownVolume(GetUnknownVolume() + m_pLastRTData->GetVolume() - m_llLastSavedVolume);

  // 设置第一次的挂单映射。
  for (int j = 0; j < 5; j++) {
    SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
    SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
  }
}

void CChinaStock::CalculateOneRTData(CWebRTDataPtr pRTData) {
  long lCurrentGuadanTransactionPrice = 0;

  ResetCalculatingData();
  m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
  if (m_lCurrentGuadanTransactionVolume == 0) { // 无成交？
    // 检查挂单情况
    lCurrentGuadanTransactionPrice = 0;
    m_nCurrentTransactionType = __NO_TRANSACTION__;
  }
  else {
    lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // 生成比较用的价格（放大一千倍后采用长整型）
    CalculateOneDeal(pRTData, lCurrentGuadanTransactionPrice);
  }

  // 下面开始分析挂单情况
  AnalysisGuadan(pRTData, lCurrentGuadanTransactionPrice);

  // 更新前交易状态
  SetLastRTData(pRTData);
}

void CChinaStock::CalculateOneDeal(CWebRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
  IncreaseTransactionNumber();
  lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // 生成比较用的价格（放大一千倍后采用长整型）
  m_dCurrentGuadanTransactionPrice = static_cast<double>(lCurrentGuadanTransactionPrice) / 1000; // 变换成实际价格
  if (lCurrentGuadanTransactionPrice >= m_pLastRTData->GetPBuy(0)) { // 高于前买盘1
    if (lCurrentGuadanTransactionPrice <= m_pLastRTData->GetPSell(0)) { // 低于前卖盘1
      CalculateOrdinaryBuySell(lCurrentGuadanTransactionPrice);
    }
    else if (lCurrentGuadanTransactionPrice < m_pLastRTData->GetPSell(1)) { // 高于卖盘一，低于卖盘二。进攻型买入。AttackBuy
      CalculateAttackBuy();
    }
    else { // 高于卖盘二。强力买入。StrongBuy。
      CalculateStrongBuy();
    }
  } // 高于前买一
  else if (lCurrentGuadanTransactionPrice > m_pLastRTData->GetPBuy(1)) { // 低于买盘一，高于买盘二。进攻型卖出，AttackSell
    CalculateAttackSell();
  }
  else { // 低于买盘二。强力卖出。StrongSell
    CalculateStrongSell();
  }
  ASSERT(GetTransactionTime() >= pRTData->GetTransactionTime());
  const INT64 I = pRTData->GetVolume();
  const INT64 j = GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
    + GetAttackBuyVolume() + GetAttackSellVolume()
    + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume();
  ASSERT(pRTData->GetVolume() == GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
         + GetAttackBuyVolume() + GetAttackSellVolume()
         + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume());
}

void CChinaStock::IncreaseTransactionNumber(void) {
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
  ASSERT(GetTransactionNumber() == (GetTransactionNumberAbove200000()
                                    + GetTransactionNumberBelow200000()
                                    + GetTransactionNumberBelow50000() + GetTransactionNumberBelow5000()));
}

void CChinaStock::CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice) {
  if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) { //一般性买入
    m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
    m_nCurrentTransactionType = __ORDINARY_BUY__;
    CalculateOrdinaryBuyVolume();
  }
  else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // 一般性卖出
    m_nCurrentTransactionType = __ORDINARY_SELL__;
    m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
    CalculateOrdinarySellVolume();
  }
  else { // 买卖混杂，不分析。
    m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
    m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateOrdinaryBuyVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_lOrdinaryBuyNumberBelow5000++;
    m_lOrdinaryBuyVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 10000) {
    m_lOrdinaryBuyNumberBelow10000++;
    m_lOrdinaryBuyVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 20000) {
    m_lOrdinaryBuyNumberBelow20000++;
    m_lOrdinaryBuyVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lOrdinaryBuyNumberBelow50000++;
    m_lOrdinaryBuyVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 100000) {
    m_lOrdinaryBuyNumberBelow100000++;
    m_lOrdinaryBuyVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lOrdinaryBuyNumberBelow200000++;
    m_lOrdinaryBuyVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume >= 200000) {
    m_lOrdinaryBuyNumberAbove200000++;
    m_lOrdinaryBuyVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateOrdinarySellVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_lOrdinarySellNumberBelow5000++;
    m_lOrdinarySellVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 10000) {
    m_lOrdinarySellNumberBelow10000++;
    m_lOrdinarySellVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 20000) {
    m_lOrdinarySellNumberBelow20000++;
    m_lOrdinarySellVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lOrdinarySellNumberBelow50000++;
    m_lOrdinarySellVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 100000) {
    m_lOrdinarySellNumberBelow100000++;
    m_lOrdinarySellVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lOrdinarySellNumberBelow200000++;
    m_lOrdinarySellVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume >= 200000) {
    m_lOrdinarySellNumberAbove200000++;
    m_lOrdinarySellVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateAttackBuy(void) {
  m_nCurrentTransactionType = __ATTACK_BUY__;
  m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackBuyVolume();
}

void CChinaStock::CalculateStrongBuy(void) {
  m_nCurrentTransactionType = __STRONG_BUY__;
  m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackBuyVolume();
}

void CChinaStock::CalculateAttackBuyVolume(void) {
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

void CChinaStock::CalculateAttackSell(void) {
  m_nCurrentTransactionType = __ATTACK_SELL__;
  m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackSellVolume();
}

void CChinaStock::CalculateStrongSell(void) {
  m_nCurrentTransactionType = __STRONG_SELL__;
  m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackSellVolume();
}

void CChinaStock::CalculateAttackSellVolume(void) {
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

void CChinaStock::ResetCalculatingData(void) {
  m_lCurrentCanceledBuyVolume = 0;
  m_lCurrentCanceledSellVolume = 0;
  m_lCurrentGuadanTransactionVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lCurrentStrongSell = 0;
  m_lCurrentUnknown = 0;
  m_lCurrentAttackBuy = 0;
  m_lCurrentAttackSell = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算挂单量的变化，由工作线程调用
//
// 采用map结构存储，这样简单且易于理解，唯一问题就是寻址花费时间长。需要测试后看看速度如何(测试后速度没任何问题）。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::AnalysisGuadan(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
  // 需要检查的挂单位置。顺序为：卖单4, 卖单3, ... 卖单0, 卖单0, .... 买单3, 买单4
  // 卖单买单谁在前面无所谓，但计算时需要记住此顺序。
  array<bool, 10> fNeedCheck{ true,true,true,true,true,true,true,true,true,true };

  for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // 预设为都要计算。
  SelectGuadanThatNeedToCalculate(pCurrentRTData, lCurrentTransactionPrice, fNeedCheck);
  SetCurrentGuadan(pCurrentRTData);
  // 检查这十个挂单价位上股数的变化情况.将目前挂单状态与之前的十个价位（m_pLastRTData包含的）相比，查看变化
  CheckGuadan(pCurrentRTData, fNeedCheck);

  return(true);
}

void CChinaStock::SelectGuadanThatNeedToCalculate(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
  // 确定需要计算哪些挂单。一共有十个，没有受到交易影响的都要计算。
  switch (m_nCurrentTransactionType) {
  case __NO_TRANSACTION__: // 没有成交，则减少的量就是相应价位上的撤单。
  ASSERT(lCurrentTransactionPrice == 0);
  break;
  case __ATTACK_BUY__: // 卖单一已经消失，卖单二被影响。计算其他七个挂单。
  fNeedCheck.at(3) = false;
  fNeedCheck.at(4) = fNeedCheck.at(5) = false;
  break;
  case __STRONG_BUY__: // 卖单一和卖单二消失，卖单三以上未定，需要计算。
  if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(2)) { // 卖单4和卖单5尚存
    fNeedCheck.at(2) = false;
  }
  else if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(3)) { // 卖单5尚存
    fNeedCheck.at(1) = false;
    fNeedCheck.at(2) = false;
  }
  else { // 所有的卖单都受影响
    fNeedCheck.at(0) = false;
    fNeedCheck.at(2) = false;
    fNeedCheck.at(1) = false;
  }
  fNeedCheck.at(3) = false;
  fNeedCheck.at(4) = false;
  fNeedCheck.at(5) = false;
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
  if (m_pLastRTData->GetPBuy(3) > lCurrentTransactionPrice) { // 所有买单都受影响
    fNeedCheck.at(7) = false;
    fNeedCheck.at(8) = false;
    fNeedCheck.at(9) = false;
  }
  else if (m_pLastRTData->GetPBuy(2) > lCurrentTransactionPrice) { // 买单5尚存
    fNeedCheck.at(7) = false;
    fNeedCheck.at(8) = false;
  }
  else { // 买单4和买单5没变化
    fNeedCheck.at(7) = false;
  }
  fNeedCheck.at(6) = false;
  fNeedCheck.at(5) = false;
  fNeedCheck.at(4) = false;
  break;
  default:
  ASSERT(0); // 不可能执行到此分支。
  break;
  }
}

void CChinaStock::SetCurrentGuadan(CWebRTDataPtr pCurrentRTData) {
  // 空位处可能是成交了，也可能是撤单了，目前不考虑这些细节，统一认为是成交了（不计算撤单）。以后再分析之。
  // 先清空当前挂单之间的挂单数量，然后填上当前量。如果有空当的话，则自然清空了。
  for (int i = pCurrentRTData->GetPBuy(4); i <= pCurrentRTData->GetPSell(4); i += 10) {
    m_mapGuadan[i] = 0; // 此处需要设置值，有可能之前为空，故而不能使用at操作符，而是用[]下标操作。
  }
  for (int i = 0; i < 5; i++) { // 将目前的十个挂单状态存入映射中，挂单状态更新为最新态
    SetGuadan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
    SetGuadan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
  }
}

void CChinaStock::CheckGuadan(CWebRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CChinaStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  INT64 iTemp = 0;
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // 撤单了的话
      iTemp = m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      m_lCurrentCanceledSellVolume += iTemp;
      CalculateCanceledSellVolume(iTemp);
      m_lCanceledSellVolume += iTemp;
    }
  }
  ASSERT(m_lCanceledSellVolume == (m_lCanceledSellVolumeAbove200000 + m_lCanceledSellVolumeBelow10000 + m_lCanceledSellVolumeBelow100000
                                   + m_lCanceledSellVolumeBelow20000 + m_lCanceledSellVolumeBelow200000 + m_lCanceledSellVolumeBelow5000
                                   + m_lCanceledSellVolumeBelow50000));
}

void CChinaStock::CalculateCanceledSellVolume(INT64 lCurrentCancelSellVolume) {
  if (lCurrentCancelSellVolume < 5000) {
    m_lCanceledSellVolumeBelow5000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 10000) {
    m_lCanceledSellVolumeBelow10000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 20000) {
    m_lCanceledSellVolumeBelow20000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 50000) {
    m_lCanceledSellVolumeBelow50000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 100000) {
    m_lCanceledSellVolumeBelow100000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 200000) {
    m_lCanceledSellVolumeBelow200000 += lCurrentCancelSellVolume;
  }
  else {
    m_lCanceledSellVolumeAbove200000 += lCurrentCancelSellVolume;
  }
}

void CChinaStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  INT64 iTemp = 0;
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // 撤单了的话
      iTemp = m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      m_lCurrentCanceledBuyVolume += iTemp;
      CalculateCanceledBuyVolume(iTemp);
      m_lCanceledBuyVolume += iTemp;
    }
  }
  ASSERT(m_lCanceledBuyVolume == (m_lCanceledBuyVolumeAbove200000 + m_lCanceledBuyVolumeBelow10000 + m_lCanceledBuyVolumeBelow100000
                                  + m_lCanceledBuyVolumeBelow20000 + m_lCanceledBuyVolumeBelow200000 + m_lCanceledBuyVolumeBelow5000
                                  + m_lCanceledBuyVolumeBelow50000));
}

void CChinaStock::CalculateCanceledBuyVolume(INT64 lCurrentCancelBuyVolume) {
  if (lCurrentCancelBuyVolume < 5000) {
    m_lCanceledBuyVolumeBelow5000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 10000) {
    m_lCanceledBuyVolumeBelow10000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 20000) {
    m_lCanceledBuyVolumeBelow20000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 50000) {
    m_lCanceledBuyVolumeBelow50000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 100000) {
    m_lCanceledBuyVolumeBelow100000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 200000) {
    m_lCanceledBuyVolumeBelow200000 += lCurrentCancelBuyVolume;
  }
  else {
    m_lCanceledBuyVolumeAbove200000 += lCurrentCancelBuyVolume;
  }
}

bool CChinaStock::HaveGuadan(INT64 lPrice) {
  if (m_mapGuadan.find(lPrice) == m_mapGuadan.end()) return false;
  else if (m_mapGuadan.at(lPrice) == 0) return false;
  return true;
}

bool CChinaStock::CheckCurrentRTData() {
  if ((GetOrdinaryBuyVolume() < 0) || (GetOrdinarySellVolume() < 0)
      || (GetAttackBuyVolume() < 0) || (GetAttackSellVolume() < 0)
      || (GetStrongBuyVolume() < 0) || (GetStrongSellVolume() < 0)) {
    int j = 0;
    if (GetOrdinaryBuyVolume() < 0) j = 1;
    if (GetOrdinarySellVolume() < 0) j += 2;
    if (GetAttackBuyVolume() < 0) j += 4;
    if (GetAttackSellVolume() < 0) j += 8;
    if (GetStrongBuyVolume() < 0) j += 16;
    if (GetStrongSellVolume() < 0) j += 32;
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaStockMarket->GetFormatedMarketTime(), GetStockCode().GetBuffer(), j);
    return false;
  }
  return true;
}

void CChinaStock::ShowCurrentTransaction() {
  // 显示当前交易情况

  if (GetCurrentTransationVolume() > 0) {
    ReportGuadanTransaction();
  }
}

void CChinaStock::ShowCurrentInformationOfCancelingGuadan(void) {
  // 显示当前取消挂单的情况
  ReportGuadan();
}

void CChinaStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, _T("%02d:%02d:%02d"), ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, _T(" %s %I64d股成交于%10.3f    "), GetStockCode().GetBuffer(),
            m_lCurrentGuadanTransactionVolume, m_dCurrentGuadanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
  str1 = _T(" STRONG BUY");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongBuyVolume());
  break;
  case __STRONG_SELL__:
  str1 = _T(" STRONG SELL");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongSellVolume());
  break;
  case __ATTACK_BUY__:
  str1 = _T(" ATTACK BUY");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackBuyVolume());
  break;
  case __ATTACK_SELL__:
  str1 = _T(" ATTACK SELL");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackSellVolume());
  break;
  case __ORDINARY_BUY__:
  str1 = _T(" ORDINARY BUY");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinaryBuyVolume());
  break;
  case __ORDINARY_SELL__:
  str1 = _T(" ORDINARY SELL");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinarySellVolume());
  break;
  case __UNKNOWN_BUYSELL__:
  str1 = _T(" UNKNOWN BUYSELL");
  sprintf_s(buffer, _T(": %I64d，  %I64d"), m_lCurrentGuadanTransactionVolume, GetUnknownVolume());
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

void CChinaStock::ReportGuadan(void) {
  CString str1;
  char buffer[30];
  if (m_lCurrentCanceledSellVolume > 0) {
    sprintf_s(buffer, _T("当前取消卖单量：%I64d"), m_lCurrentCanceledSellVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  总取消卖单量：%I64d"), GetCanceledSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // 采用同步机制传递消息
  }
  if (m_lCurrentCanceledBuyVolume > 0) {
    sprintf_s(buffer, _T("当前取消买单量：%I64d"), m_lCurrentCanceledBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  总取消买单量：%I64d"), GetCanceledBuyVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // 采用同步机制传递消息
  }
}

void CChinaStock::SaveStockCodeDB(CSetStockCode& setStockCode) {
  CString str;
  setStockCode.m_Counter = GetOffset();
  setStockCode.m_StockType = GetMarket();
  setStockCode.m_StockCode = GetStockCode();
  if (GetStockName() != _T("")) {   // 如果此股票ID有了新的名字，
    setStockCode.m_StockName = GetStockName(); // 则存储新的名字
  }
  if (IsIPOed()) { // 如果此股票是活跃股票
    if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetFormatedMarketDay(), 30)) { // 如果此股票的日线历史数据已经早于一个月了，则设置此股票状态为已退市
      setStockCode.m_IPOStatus = __STOCK_DELISTED__;
    }
    else {
      setStockCode.m_IPOStatus = GetIPOStatus();
    }
  }
  else {
    setStockCode.m_IPOStatus = GetIPOStatus();
  }
  setStockCode.m_DayLineStartDay = GetDayLineStartDay();
  setStockCode.m_DayLineEndDay = GetDayLineEndDay();
}

void CChinaStock::AppendStockCodeDB(CSetStockCode& setStockCode) {
  setStockCode.AddNew();
  SaveStockCodeDB(setStockCode);
  setStockCode.Update();
}

bool CChinaStock::LoadStockCodeDB(CSetStockCode& setStockCode) {
  SetStockCode(setStockCode.m_StockCode);
  CString str = setStockCode.m_StockName; // 用str中间过渡一下，就可以读取UniCode制式的m_StockName了。
  SetStockName(str);
  SetIPOStatus(setStockCode.m_IPOStatus);
  SetDayLineStartDay(setStockCode.m_DayLineStartDay);
  if (GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // 有时一个股票会有多个记录，以最后的日期为准。
    SetDayLineEndDay(setStockCode.m_DayLineEndDay);
  }
  SetCheckingDayLineStatus();
  return true;
}

void CChinaStock::SetCheckingDayLineStatus(void) {
  ASSERT(IsDayLineNeedUpdate());
  // 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
  if (gl_pChinaStockMarket->GetLastTradeDay() <= GetDayLineEndDay()) { // 最新日线数据为今日或者上一个交易日的数据。
    SetDayLineNeedUpdate(false); // 日线数据不需要更新
  }
  else if (IsNullStock()) { // 无效代码不需更新日线数据
    SetDayLineNeedUpdate(false);
  }
  else if (IsDelisted()) { // 退市股票如果已下载过日线数据，则每星期一复查日线数据
    if ((gl_pChinaStockMarket->GetDayOfWeek() != 1) && (GetDayLineEndDay() != __CHINA_MARKET_BEGIN_DAY__)) {
      SetDayLineNeedUpdate(false);
    }
  }
}

bool CChinaStock::BuildWeekLine(void) {
  if (IsNullStock()) return true;
  if (!IsDayLineLoaded()) {
    LoadDayLine();
  }
  if (GetDayLineSize() <= 0) return true;

  CalculatingWeekLine();

  SaveWeekLine();
  if (gl_pChinaStockMarket->GetCurrentStock() != nullptr) {
    if (gl_pChinaStockMarket->GetCurrentStock()->GetOffset() != m_lOffsetInContainer) {
      UnloadDayLine();
      UnloadWeekLine();
    }
  }
  else {
    UnloadDayLine();
    UnloadWeekLine();
  }
  return true;
}

bool CChinaStock::SaveWeekLine() {
  return m_WeekLine.SaveData();
}

bool CChinaStock::SaveWeekLineBasicInfo() {
  m_WeekLine.SaveBasicInfo();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 只存储有交易记录的扩展数据。对于没有信息的直接跨过。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveWeekLineExtendInfo() {
  m_WeekLine.SaveExtendInfo();

  return true;
}

bool CChinaStock::LoadWeekLine() {
  return m_WeekLine.LoadData();
}

bool CChinaStock::LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  return m_WeekLine.LoadBasicInfo(psetWeekLineBasicInfo);
}

bool CChinaStock::LoadWeekLineExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  return m_WeekLine.LoadExtendInfo(psetWeekLineExtendInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制存储实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::PushRTData(CWebRTDataPtr pData) {
  m_qRTData.PushData(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CWebRTDataPtr CChinaStock::PopRTData(void) {
  return m_qRTData.PopData();
}

CWebRTDataPtr CChinaStock::GetRTDataAtHead(void) {
  return m_qRTData.GetHead();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取栈内的实时数据数量。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CChinaStock::GetRTDataQueueSize(void) {
  return m_qRTData.GetDataSize();
}

bool CChinaStock::IsSameStock(CChinaStockPtr pStock) {
  if (pStock == nullptr) return false;
  else if (m_lOffsetInContainer == pStock->GetOffset()) return true;
  else return false;
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive(void) {
  if (!m_fActive) return false;
  else {
    return IsTodayDataChanged();
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

void CChinaStock::SetDayLineNeedUpdate(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedUpdateNumber();
  }
  else {
    ASSERT(m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedUpdateNumber();
  }
}

void CChinaStock::SetDayLineNeedProcess(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedProcessNumber();
  }
  else {
    ASSERT(m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedProcessNumber();
  }
}

void CChinaStock::ShowDayLine(CDC* pDC, CRect rectClient) {
  const COLORREF crBlue(RGB(0, 0, 255)), crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)), crRed(RGB(255, 0, 0));
  CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
  long lHigh = 0;
  long lDay;
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  it--;
  int i = 0, y = 0;
  long lLow = (*it)->GetLow();
  for (; it != m_vDayLine.begin(); it--) {
    if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
    if ((*it)->GetLow() > 0) {
      if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow();
    }
    if (3 * i > m_vDayLine.size()) break;
    if (rectClient.right <= 3 * i) break; // 画到
    else i++;
  }

  it = m_vDayLine.end();
  it--;
  i = 0;
  long x = 0;
  pDC->SelectObject(&penRed1);
  for (; it != m_vDayLine.begin(); it--) {
    x = rectClient.right - 2 - i * 3;
    y = (0.5 - (double)((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    pDC->MoveTo(x, y);
    if ((*it)->GetHigh() == (*it)->GetLow()) {
      y = y - 1;
    }
    else {
      y = (0.5 - (double)((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    }
    pDC->LineTo(x, y);
    lDay = (*it)->GetFormatedMarketDay();
    i++;
    if (3 * i > m_vDayLine.size()) break;
    if (rectClient.right <= 3 * i) break; // 画到窗口左边框为止
  }
}

void CChinaStock::GetRS1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongIndex();
  }
}

void CChinaStock::GetRSIndex1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongIndex();
  }
}

void CChinaStock::GetRSLogarithm1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongLogarithm();
  }
}

void CChinaStock::GetRS3Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get3DayRS();
  }
}

void CChinaStock::GetRS5Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get5DayRS();
  }
}

void CChinaStock::GetRS10Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get10DayRS();
  }
}

void CChinaStock::GetRS30Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get30DayRS();
  }
}

void CChinaStock::GetRS60Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get60DayRS();
  }
}

void CChinaStock::GetRS120Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get120DayRS();
  }
}

void CChinaStock::__TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer) {
  char* p;
  if (m_pDayLineBuffer != nullptr) delete m_pDayLineBuffer;
  m_pDayLineBuffer = new char[lBufferLength + 1];
  p = m_pDayLineBuffer;
  for (int i = 0; i < lBufferLength; i++) {
    *p++ = pDayLineBuffer[i];
  }
  m_pDayLineBuffer[lBufferLength] = 0x000;
  m_lDayLineBufferLength = lBufferLength;
}

bool CChinaStock::IsVolumeConsistence(void) noexcept {
  if (GetVolume() != GetOrdinaryBuyVolume() + GetOrdinarySellVolume() + GetAttackBuyVolume()
      + GetAttackSellVolume() + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume()) {
    TRACE(_T("%14Id %s股数%d\n"), FormatToDayTime(m_TransactionTime), GetStockCode().GetBuffer(), GetVolume());
    TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
          GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
    return false;
  }
  else return true;
}

bool CChinaStock::CalculatingWeekLine(void) {
  ASSERT(IsDayLineLoaded());
  ASSERT(m_vDayLine.size() > 0);
  long i = 0;
  CWeekLinePtr pWeekLine = nullptr;

  m_WeekLine.Unload();
  do {
    pWeekLine = CreateNewWeekLine(i);
    m_WeekLine.StoreData(pWeekLine);
  } while (i < m_vDayLine.size());

  return true;
}

CWeekLinePtr CChinaStock::CreateNewWeekLine(long& lCurrentDayLinePos) {
  ASSERT(m_vDayLine.size() > 0);
  long lNextMonday = GetNextMonday(m_vDayLine.at(lCurrentDayLinePos)->GetFormatedMarketDay());
  long lNewestDay = m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDay();
  CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
  if (lNextMonday < lNewestDay) { // 中间数据
    while (m_vDayLine.at(lCurrentDayLinePos)->GetFormatedMarketDay() < lNextMonday) {
      pWeekLine->CreateWeekLine(m_vDayLine.at(lCurrentDayLinePos++));
    }
  }
  else { // 最后一组数据
    while (lCurrentDayLinePos <= (m_vDayLine.size() - 1)) {
      pWeekLine->CreateWeekLine(m_vDayLine.at(lCurrentDayLinePos++));
    }
  }

  if (pWeekLine->GetLastClose() > 0) {
    pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetLastClose());
  }
  else {
    pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetOpen());
  }

  return pWeekLine;
}

#ifdef _DEBUG
void CChinaStock::AssertValid() const {
  CObject::AssertValid();
}

void CChinaStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG