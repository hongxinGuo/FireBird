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
  m_lIPOed = __STOCK_NOT_CHECKED__;   // 默认状态为无效股票代码。
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
  m_dRelativeStrong = 0;

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
  m_lCancelBuyVolume = 0;
  m_lCancelSellVolume = 0;
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

  m_lCancelBuyVolumeBelow5000 = 0;
  m_lCancelBuyVolumeBelow10000 = 0;
  m_lCancelBuyVolumeBelow20000 = 0;
  m_lCancelBuyVolumeBelow50000 = 0;
  m_lCancelBuyVolumeBelow100000 = 0;
  m_lCancelBuyVolumeBelow200000 = 0;
  m_lCancelBuyVolumeAbove200000 = 0;
  m_lCancelSellVolumeBelow5000 = 0;
  m_lCancelSellVolumeBelow10000 = 0;
  m_lCancelSellVolumeBelow20000 = 0;
  m_lCancelSellVolumeBelow50000 = 0;
  m_lCancelSellVolumeBelow100000 = 0;
  m_lCancelSellVolumeBelow200000 = 0;
  m_lCancelSellVolumeAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

  m_llLastSavedVolume = 0;

  m_lCurrentCanselSellVolume = m_lCurrentCanselBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

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
  m_fMinLineUpdated = false;
  m_fDayLineUpdated = false;

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
    CRTDataPtr pRTData = PopRTData();
  }
}

bool CChinaStock::HaveNewDayLineData(void) {
  if (m_vDayLine.size() <= 0) return false;
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > GetDayLineEndDay()) return true;
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
      if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetDay(), 30)) {
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
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("日线下载完成.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  if (gl_pChinaStockMarket->IsEarlyThen(vTempDayLine.at(0)->GetDay(), gl_pChinaStockMarket->GetDay(), 30)) { // 提取到的股票日线数据其最新日早于上个月的这个交易日（退市了或相似情况，给一个月的时间观察）。
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
  SetStockName(strStockName);// 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
  gl_pChinaStockMarket->SetTotalActiveStock(gl_pChinaStockMarket->GetTotalActiveStock() + 1);
}

void CChinaStock::StoreDayLine(vector<CDayLinePtr>& vTempDayLine) {
  CDayLinePtr pDayLine = nullptr;
  m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
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

void CChinaStock::SaveBasicInfo(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());
  setDayLine.m_Day = FormatToDay(m_TransactionTime);
  setDayLine.m_Market = m_wMarket;
  setDayLine.m_StockCode = m_strStockCode;
  setDayLine.m_StockName = m_strStockName;
  setDayLine.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLine.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLine.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLine.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLine.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLine.m_Volume = ConvertValueToString(m_llVolume);
  setDayLine.m_Amount = ConvertValueToString(m_llAmount);
  setDayLine.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDayLine.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  if (m_llTotalValue != 0) setDayLine.m_ChangeHandRate = ConvertValueToString((double)100 * m_llAmount / m_llTotalValue);
  else setDayLine.m_ChangeHandRate = ConvertValueToString(0);
  setDayLine.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLine.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
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
  setDayLineToday.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineToday.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
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
}

void CChinaStock::UpdateStatus(CRTDataPtr pRTData) {
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
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveDayLine(void) {
  CSetDayLine setDayLine;
  long lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(m_vDayLine.size() > 0);
  ASSERT(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > m_lDayLineEndDay);

  lSize = m_vDayLine.size();
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += GetStockCode() + _T("'");
  setDayLine.m_strSort = _T("[Day]");

  setDayLine.Open();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    vDayLine.push_back(pDayLine);
    lCurrentPos++;
    setDayLine.MoveNext();
  }
  setDayLine.Close();
  if (vDayLine.size() == 0) {
    SetDayLineStartDay(gl_pChinaStockMarket->GetDay());
    SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__);
  }
  else {
    SetDayLineStartDay(vDayLine.at(0)->GetDay());
    SetDayLineEndDay(vDayLine.at(vDayLine.size() - 1)->GetDay());
  }

  lSizeOfOldDayLine = lCurrentPos;
  lCurrentPos = 0;
  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
    pDayLine = m_vDayLine.at(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetDay() < pDayLine->GetDay())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetDay() > pDayLine->GetDay()) {
        pDayLine->AppendData(setDayLine);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendData(setDayLine);
      fNeedUpdate = true;
    }
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // 更新最新日线日期和起始日线日期
  if (fNeedUpdate) {
    UpdateDayLineStartEndDay();
  }

  return fNeedUpdate;
}

void CChinaStock::UpdateDayLineStartEndDay(void) {
  if (m_vDayLine.at(0)->GetDay() < GetDayLineStartDay()) {
    SetDayLineStartDay(m_vDayLine.at(0)->GetDay());
    SetDayLineDBUpdated(true);
  }
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > GetDayLineEndDay()) {
    SetDayLineEndDay(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay());
    SetDayLineDBUpdated(true);
  }
}

void CChinaStock::SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo) {
  ASSERT(setDayLineInfo.IsOpen());
  setDayLineInfo.m_Day = FormatToDay(m_TransactionTime);
  setDayLineInfo.m_Market = m_wMarket;
  setDayLineInfo.m_StockCode = m_strStockCode;
  setDayLineInfo.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineInfo.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineInfo.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineInfo.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineInfo.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  setDayLineInfo.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineInfo.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  setDayLineInfo.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineInfo.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineInfo.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineInfo.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineInfo.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineInfo.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineInfo.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineInfo.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineInfo.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineInfo.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineInfo.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineInfo.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineInfo.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
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
  m_lCancelBuyVolume = atoll(setDayLineToday.m_CancelBuyVolume);
  m_lCancelSellVolume = atoll(setDayLineToday.m_CancelSellVolume);
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
}

bool CChinaStock::LoadDayLineAndDayLineInfo(void) {
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;

  // 装入DayLine数据
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Day]");
  setDayLine.Open();
  LoadDayLine(setDayLine);
  setDayLine.Close();

  // 装入DayLineInfo数据
  setDayLineInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineInfo.m_strFilter += GetStockCode();
  setDayLineInfo.m_strFilter += _T("'");
  setDayLineInfo.m_strSort = _T("[Day]");
  setDayLineInfo.Open();
  LoadDayLineInfo(setDayLineInfo);
  setDayLineInfo.Close();

  return true;
}

bool CChinaStock::LoadDayLine(CSetDayLine& setDayLine) {
  CDayLinePtr pDayLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDayLineFirst);
  // 装入DayLine数据
  m_vDayLine.clear();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    m_vDayLine.push_back(pDayLine);
    setDayLine.MoveNext();
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
bool CChinaStock::LoadDayLineInfo(CSetDayLineInfo& setDayLineInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDayLineFirst);

  while (!setDayLineInfo.IsEOF()) {
    pDayLine = m_vDayLine.at(iPosition);
    while ((pDayLine->GetDay() < setDayLineInfo.m_Day)
           && (m_vDayLine.size() > (iPosition + 1))) {
      iPosition++;
      pDayLine = m_vDayLine.at(iPosition);
    }
    if (pDayLine->GetDay() == setDayLineInfo.m_Day) {
      pDayLine->LoadData(setDayLineInfo);
    }
    if (m_vDayLine.size() <= (iPosition + 1)) break;
    setDayLineInfo.MoveNext();
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
    m_vDayLine.at(i)->m_d3DayRS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5DayRS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10DayRS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30DayRS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60DayRS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
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
    m_vDayLine.at(i)->m_d3DayRS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5DayRS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10DayRS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30DayRS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60DayRS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
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
    m_vDayLine.at(i)->m_d3DayRS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5DayRS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10DayRS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30DayRS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60DayRS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
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
  CRTDataPtr pRTData;

  INT64 lTotalNumber = GetRTDataQueueSize(); //  缓存队列的长度。采用同步机制获取其数值.
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
bool CChinaStock::ProcessOneRTData(CRTDataPtr pRTData) {
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
void CChinaStock::InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData) {
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

void CChinaStock::CalculateOneRTData(CRTDataPtr pRTData) {
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

void CChinaStock::CalculateOneDeal(CRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
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
  m_lCurrentCanselBuyVolume = 0;
  m_lCurrentCanselSellVolume = 0;
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
bool CChinaStock::AnalysisGuadan(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
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

void CChinaStock::SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
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

void CChinaStock::SetCurrentGuadan(CRTDataPtr pCurrentRTData) {
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

void CChinaStock::CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CChinaStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // 撤单了的话
      m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      CalculateCancelSellVolume(m_lCurrentCanselSellVolume);
      m_lCancelSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
    }
  }
}

void CChinaStock::CalculateCancelSellVolume(INT64 lCurrentCancelSellVolume) {
  if (lCurrentCancelSellVolume < 5000) {
    m_lCancelSellVolumeBelow5000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 10000) {
    m_lCancelSellVolumeBelow10000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 20000) {
    m_lCancelSellVolumeBelow20000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 50000) {
    m_lCancelSellVolumeBelow50000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 100000) {
    m_lCancelSellVolumeBelow100000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 200000) {
    m_lCancelSellVolumeBelow200000 += lCurrentCancelSellVolume;
  }
  else {
    m_lCancelSellVolumeAbove200000 += lCurrentCancelSellVolume;
  }
}

void CChinaStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // 撤单了的话
      m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      CalculateCancelBuyVolume(m_lCurrentCanselBuyVolume);
      m_lCancelBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
    }
  }
}

void CChinaStock::CalculateCancelBuyVolume(INT64 lCurrentCancelBuyVolume) {
  if (lCurrentCancelBuyVolume < 5000) {
    m_lCancelBuyVolumeBelow5000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 10000) {
    m_lCancelBuyVolumeBelow10000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 20000) {
    m_lCancelBuyVolumeBelow20000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 50000) {
    m_lCancelBuyVolumeBelow50000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 100000) {
    m_lCancelBuyVolumeBelow100000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 200000) {
    m_lCancelBuyVolumeBelow200000 += lCurrentCancelBuyVolume;
  }
  else {
    m_lCancelBuyVolumeAbove200000 += lCurrentCancelBuyVolume;
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
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaStockMarket->GetTime(), GetStockCode().GetBuffer(), j);
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

void CChinaStock::ShowCurrentInformationofCancelingGuadan(void) {
  // 显示当前取消挂单的情况
  ReportGuadan();
}

void CChinaStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
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
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, _T("当前取消卖单量：%I64d"), m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  总取消卖单量：%I64d"), GetCancelSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // 采用同步机制传递消息
  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, _T("当前取消买单量：%I64d"), m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  总取消买单量：%I64d"), GetCancelBuyVolume());
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
  if (GetIPOStatus() == __STOCK_IPOED__) { // 如果此股票是活跃股票
    if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetDay(), 30)) { // 如果此股票的日线历史数据已经早于一个月了，则设置此股票状态为已退市
      setStockCode.m_IPOed = __STOCK_DELISTED__;
    }
    else {
      setStockCode.m_IPOed = GetIPOStatus();
    }
  }
  else {
    setStockCode.m_IPOed = GetIPOStatus();
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
  SetIPOStatus(setStockCode.m_IPOed);
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
  else if (GetIPOStatus() == __STOCK_NULL__) { // 无效代码不需更新日线数据
    SetDayLineNeedUpdate(false);
  }
  else if (GetIPOStatus() == __STOCK_DELISTED__) { // 退市股票如果已下载过日线数据，则每星期一复查日线数据
    if ((gl_pChinaStockMarket->GetDayOfWeek() != 1) && (GetDayLineEndDay() != __CHINA_MARKET_BEGIN_DAY__)) {
      SetDayLineNeedUpdate(false);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制存储实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::PushRTData(CRTDataPtr pData) {
  m_qRTData.push(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CRTDataPtr CChinaStock::PopRTData(void) {
  CRTDataPtr pData = m_qRTData.front();
  m_qRTData.pop();
  return pData;
}

CRTDataPtr CChinaStock::GetRTDataAtHead(void) {
  return m_qRTData.front();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取栈内的实时数据数量。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CChinaStock::GetRTDataQueueSize(void) {
  return m_qRTData.size();
}

bool CChinaStock::IsSameStock(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);
  if (m_lOffsetInContainer == pStock->GetOffset()) return true;
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
  const COLORREF crBlue(RGB(0, 0, 255)), crGreen(RGB(0, 255, 0));
  CPen penGreen1(PS_SOLID, 1, crGreen);
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
  pDC->SelectObject(&penGreen1);
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
    lDay = (*it)->GetDay();
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
    TRACE(_T("%s股数%d\n"), GetStockCode().GetBuffer(), GetVolume());
    TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
          GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
    return false;
  }
  else return true;
}

#ifdef _DEBUG
void CChinaStock::AssertValid() const {
  CObject::AssertValid();
}

void CChinaStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG