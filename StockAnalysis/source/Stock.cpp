#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include"Accessory.h"

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
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
}

void CStock::Reset(void) {
  m_stockBasicInfo.Reset();
  m_stockCalculatedInfo.Reset();

  m_lCurrentCanselSellVolume = m_lCurrentCanselBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_dCurrentGuadanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fActive = false;

  m_fDayLineLoaded = false;
  m_fDayLineNeedUpdate = true;
  m_fDayLineReadFromWeb = false; // 从网络上读取了日线历史数据
  m_fDayLineNeedSaving = false;
  m_lDayLineBufferLength = 0;
  m_pDayLineBuffer = nullptr;

  m_fInquiringOnce = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayLineUpdated = false;

  m_fHaveFirstRTData = false;  // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
  m_pLastRTData = nullptr;

  ClearRTDataDeque();

  m_fDebugLoadDayLineFirst = false;
}

void CStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataDequeSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CRTDataPtr pRTData = PopRTData();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::ProcessNeteaseDayLineData(void) {
  long iCount = 0;
  char* pCurrentPos = m_pDayLineBuffer;
  char* pTestPos = m_pDayLineBuffer;
  vector<CDayLinePtr> vTempDayLine;
  long lLength = m_lDayLineBufferLength;

  ASSERT(m_fDayLineReadFromWeb);
  ASSERT(m_fDayLineNeedSaving == false);
  if (lLength == 0) { // 没有数据读入？此种状态是查询的股票为无效（不存在）号码
    return false;
  }

  while (*pCurrentPos != 0X0d) {
    if ((*pCurrentPos == 0x0a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  if (*pCurrentPos != 0x0a) return false;
  iCount++;
  pCurrentPos++;
  iCount++;
  shared_ptr<CDayLine> pDayLine;

  pTestPos = m_pDayLineBuffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  if (iCount == lLength) {// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天
    // ASSERT(!m_vChinaMarketAStock[lIndex]->m_fActive); 当一个股票IPO后但尚未上市时，股票代码存在但没有日线数据。取消此断言判断。
    // 有些股票在上市后出现被收购或其他情况，导致日线数据不再更新。此种情况不能设置此股票为无效代码
    if (GetDayLineEndDay() == 19900101) { // 如果初始日线结束日期从来没有变更过，则此股票代码尚未被使用过
      SetIPOStatus(__STOCK_NULL__);   // 此股票代码尚未使用。
      //TRACE("无效股票代码：%s\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(Index)->GetStockCode()));
    }
    else { // 已经退市的股票
      if (GetDayLineEndDay() + 100 < gl_systemTime.GetDay()) {
        SetIPOStatus(__STOCK_DELISTED__);   // 此股票代码已经退市。
      }
      //TRACE("%S 没有可更新的日线数据\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(Index)->GetStockCode()));
    }
    SetDayLineNeedUpdate(false); // 都不需要更新日线数据
    return false;
  }

  long iTemp = 0;
  CString strTemp;
  pTestPos = m_pDayLineBuffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  while (iCount < lLength) {
    pDayLine = make_shared<CDayLine>();
    if (!pDayLine->ProcessNeteaseData(GetStockCode(), pCurrentPos, iTemp)) { // 处理一条日线数据
      TRACE(_T("%s 日线数据出错\n"), pDayLine->GetStockCode());
      // 清除已暂存的日线数据
      vTempDayLine.clear();
      return false; // 数据出错，放弃载入
    }
    iCount += iTemp;
    pTestPos = m_pDayLineBuffer;
    pTestPos += iCount;
    ASSERT(*pTestPos == *pCurrentPos);
    if (!IsActive()) { // 新的股票代码？
      // 生成新股票
      SetActive(true);
      SetDayLineLoaded(false);
      SetMarket(pDayLine->GetMarket());
      SetStockCode(pDayLine->GetStockCode()); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
      SetStockName(pDayLine->GetStockName());// 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
      strTemp = GetStockCode().Right(6); // 截取股票代码右边的六个数字
      gl_ChinaStockMarket.SetTotalActiveStock(gl_ChinaStockMarket.GetTotalActiveStock() + 1);
    }
    vTempDayLine.push_back(pDayLine); // 暂存于临时vector中，因为网易日线数据的时间顺序是颠倒的，最新的在最前面
  }
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("日线下载完成.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  SetDayLineNeedUpdate(false); // 日线数据下载完毕，不需要申请新数据了。
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_systemTime.GetDay()) { // 提取到的股票日线数据其最新日不是上个月的这个交易日（退市了或相似情况），给一个月的时间观察。
    SetIPOStatus(__STOCK_DELISTED__); // 已退市或暂停交易。
  }
  else {
    SetIPOStatus(__STOCK_IPOED__); // 正常交易股票
  }

  m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->GetDay() < gl_systemTime.GetDay()) { // 不要存储今日日线数据（今日日线数据由实时数据生成）.
      // 当新股第一天上市时，其日线只有一天，而且在这里扔掉了，导致其日线容器为空。处理时注意。
      // 由于是调取gl_lLastTradeDay及之前的日线数据，故而新股的日线容器肯定为空。
      m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();
  SetDayLineReadFromWeb(false); // 处理完后清除标识
  SetDayLineLoaded(true);
  SetDayLineNeedSavingFlag(true); // 设置存储日线标识

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CStock::SaveDayLine(void) {
  CSetDayLine setDayLine;
  long lIndex = 0;
  long lSize = 0;
  CDayLinePtr pDayLine;

  CCriticalSection cs;
  CSingleLock s(&cs);
  s.Lock();
  if (s.IsLocked()) {
    lSize = m_vDayLine.size();
    setDayLine.m_strFilter = _T("[ID] = 1"); // 采用主键作为搜索Index.必须设置，否则会把所有的数据读入，浪费时间
    s.Unlock();
  }
  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
    pDayLine = m_vDayLine.at(i);
    if (GetDayLineEndDay() >= pDayLine->GetDay()) continue; // 存储过的日线数据不用存储
    pDayLine->AppendData(setDayLine);
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // 更新最新日线日期和起始日线日期
  s.Lock();
  if (s.IsLocked()) {
    if (GetDayLineEndDay() < m_vDayLine.at(m_vDayLine.size() - 1)->GetDay()) {
      SetDayLineStartDay(m_vDayLine.at(0)->GetDay());
      SetDayLineEndDay(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay());
    }
    s.Unlock();
  }

  return true;
}

void CStock::SaveBasicInfo(CSetDayLine& setDayLine) {
  m_stockBasicInfo.SaveBasicInfo(setDayLine);
}

void CStock::SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo) {
  m_stockCalculatedInfo.SaveTodayInfo(setDayLineInfo);
}

void CStock::SaveTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  m_stockBasicInfo.SaveTempInfo(setDayLineToday);
  m_stockCalculatedInfo.SaveTempInfo(setDayLineToday);
}

////////////////////////////////////////////////////////////////////////////
//
// 只有一处调用本函数，就是在系统初始化时装入之前计算出的信息（如果开市了的话），故而未知成交数量这项需要特殊处理
//
////////////////////////////////////////////////////////////////////////////
void CStock::LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday) {
  m_stockCalculatedInfo.LoadAndCalculateTempInfo(setDayLineToday);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 更新股票当前的状态（用于显示）
//
//
///////////////////////////////////////////////////////////////////////////////////////////
void CStock::UpdateStatus(CRTDataPtr pRTData) {
  m_stockBasicInfo.UpdateStatus(pRTData);
}

bool CStock::LoadDayLineAndDayLineInfo(void) {
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;

  // 装入DayLine数据
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += gl_ChinaStockMarket.m_pCurrentStock->GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Day]");
  setDayLine.Open();
  LoadDayLine(setDayLine);
  setDayLine.Close();

  // 装入DayLineInfo数据
  setDayLineInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineInfo.m_strFilter += gl_ChinaStockMarket.m_pCurrentStock->GetStockCode();
  setDayLineInfo.m_strFilter += _T("'");
  setDayLineInfo.m_strSort = _T("[Day]");
  setDayLineInfo.Open();
  LoadDayLineInfo(setDayLineInfo);
  setDayLineInfo.Close();

  return true;
}

bool CStock::LoadDayLine(CSetDayLine& setDayLine) {
  CDayLinePtr pDayLine;

  ASSERT(m_fDebugLoadDayLineFirst == false);
  // 装入DayLine数据
  m_vDayLine.clear();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    m_vDayLine.push_back(pDayLine);
    setDayLine.MoveNext();
  }
  m_fDebugLoadDayLineFirst = true;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// 装载DayLineInfo表必须在装载DayLine表之后。
//
//
////////////////////////////////////////////////////////////////////////////
bool CStock::LoadDayLineInfo(CSetDayLineInfo& setDayLineInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  ASSERT(m_fDebugLoadDayLineFirst);

  while (!setDayLineInfo.IsEOF()) {
    pDayLine = m_vDayLine[iPosition];
    while ((pDayLine->GetDay() < setDayLineInfo.m_Day)
           && (m_vDayLine.size() > (iPosition + 1))) {
      iPosition++;
      pDayLine = m_vDayLine[iPosition];
    }
    if (pDayLine->GetDay() == setDayLineInfo.m_Day) {
      pDayLine->LoadData(setDayLineInfo);
    }
    if (m_vDayLine.size() <= (iPosition + 1)) break;
    setDayLineInfo.MoveNext();
  }
  m_fDebugLoadDayLineFirst = false;
  return true;
}

bool CStock::CalculateDayLineRS(void) {
  CalculateDayLineRS(3);
  CalculateDayLineRS(5);
  CalculateDayLineRS(10);
  CalculateDayLineRS(30);
  CalculateDayLineRS(60);
  CalculateDayLineRS(120);
  return true;
}

bool CStock::CalculateDayLineRS(long lNumber) {
  double dTempRS = 0;
  const long lTotalNumber = m_vDayLine.size();
  for (int i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrong();
    }
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 此函数被工作线程ThreadCalculatingRTDataProc调用，尽量减少对全局变量的操作。
//
// 交易情况和挂单情况似乎分开计算比较好，思路容易清晰。目前来看计算能力尚够，但要考虑旧机器的速度了。
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CStock::ProcessRTData(void) {
  CRTDataPtr pRTData;

  const long lTotalNumber = GetRTDataDequeSize(); //  缓存队列的长度。采用同步机制获取其数值.
  if (lTotalNumber == 0) return false;
  // 以下为计算挂单变化、股票活跃度、大单买卖情况
  for (long i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // 采用同步机制获取数据
    if (pRTData->IsActive()) { // 数据有效
      UpdateStatus(pRTData);   // 更新股票现时状态。
      ProcessOneRTData(pRTData);
      CheckCurrentRTData();
      ShowCurrentTransaction();
      ShowCurrentInformationofCancelingGuadan();
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
bool CStock::ProcessOneRTData(CRTDataPtr pRTData) {
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
void CStock::InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData) {
  SetLastRTDataPtr(pRTData);
  SetHavingFirstRTData(true);
  // 第一次挂单量无法判断买卖状态，故而设置其为无法判断。如果之前已经运行过系统，此次是开盘中途登录的，则系统存储了临时数据于数据库中，
  // 在系统启动时已经将此临时状态读入了，故而m_lUnknownVolume不为零，而是为了计算方便设置为临时数据的m_lUnknownVolume-m_lVolume，
  // 这样加上m_pLastRTData->GetVolume()，即得到当前的m_lUnknownVolume.
  // 因为：m_lUnknownVolume = 当前的成交量 - 之前的成交量 + 之前的无法判断成交量
  m_stockCalculatedInfo.SetUnknownVolume(m_stockCalculatedInfo.GetUnknownVolume() + m_pLastRTData->GetVolume());
  // 设置第一次的挂单映射。
  for (int j = 0; j < 5; j++) {
    SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
    SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
  }
}

void CStock::CalculateOneRTData(CRTDataPtr pRTData) {
  long lCurrentGuadanTransactionPrice = 0;

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
  SetLastRTDataPtr(pRTData);
}

void CStock::CalculateOneDeal(CRTDataPtr pRTData, long lCurrentGuadanTransactionPrice) {
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
  ASSERT(m_stockBasicInfo.GetTransactionTime() >= pRTData->GetTransactionTime());
  const INT64 I = pRTData->GetVolume();
  const INT64 j = m_stockCalculatedInfo.GetOrdinaryBuyVolume() + m_stockCalculatedInfo.GetOrdinarySellVolume()
    + m_stockCalculatedInfo.GetAttackBuyVolume() + m_stockCalculatedInfo.GetAttackSellVolume()
    + +m_stockCalculatedInfo.GetStrongBuyVolume() + m_stockCalculatedInfo.GetStrongSellVolume() + m_stockCalculatedInfo.GetUnknownVolume();
  ASSERT(pRTData->GetVolume() == m_stockCalculatedInfo.GetOrdinaryBuyVolume() + m_stockCalculatedInfo.GetOrdinarySellVolume()
         + m_stockCalculatedInfo.GetAttackBuyVolume() + m_stockCalculatedInfo.GetAttackSellVolume()
         + m_stockCalculatedInfo.GetStrongBuyVolume() + m_stockCalculatedInfo.GetStrongSellVolume() + m_stockCalculatedInfo.GetUnknownVolume());
}

void CStock::IncreaseTransactionNumber(void) {
  m_stockCalculatedInfo.IncreaseTransactionNumber(); // 成交数加一。
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow5000();
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow50000();
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow200000();
  }
  else {
    m_stockCalculatedInfo.IncreaseTransactionNumberAbove200000();
  }
  ASSERT(m_stockCalculatedInfo.GetTransactionNumber() == (m_stockCalculatedInfo.GetTransactionNumberAbove200000()
                                                          + m_stockCalculatedInfo.GetTransactionNumberBelow200000()
                                                          + m_stockCalculatedInfo.GetTransactionNumberBelow50000() + m_stockCalculatedInfo.GetTransactionNumberBelow5000()));
}

void CStock::CalculateOrdinaryBuySell(long lCurrentGuadanTransactionPrice) {
  if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) { //一般性买入
    IncreaseOrdinaryBuyVolume(m_lCurrentGuadanTransactionVolume);
    m_nCurrentTransactionType = __ORDINARY_BUY__;
  }
  else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // 一般性卖出
    m_nCurrentTransactionType = __ORDINARY_SELL__;
    IncreaseOrdinarySellVolume(m_lCurrentGuadanTransactionVolume);
  }
  else { // 买卖混杂，不分析。
    m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
    IncreaseUnknownVolume(m_lCurrentGuadanTransactionVolume);
  }
}

void CStock::CalculateAttackBuy(void) {
  m_nCurrentTransactionType = __ATTACK_BUY__;
  IncreaseAttackBuyVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackBuyVolume();
}

void CStock::CalculateStrongBuy(void) {
  m_nCurrentTransactionType = __STRONG_BUY__;
  IncreaseStrongBuyVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackBuyVolume();
}

void CStock::CalculateAttackBuyVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    IncreaseAttackBuyBelow50000(m_lCurrentGuadanTransactionVolume);
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    IncreaseAttackBuyBelow200000(m_lCurrentGuadanTransactionVolume);
  }
  else {
    IncreaseAttackBuyAbove200000(m_lCurrentGuadanTransactionVolume);
  }
}

void CStock::CalculateAttackSell(void) {
  m_nCurrentTransactionType = __ATTACK_SELL__;
  IncreaseAttackSellVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackSellVolume();
}

void CStock::CalculateStrongSell(void) {
  m_nCurrentTransactionType = __STRONG_SELL__;
  IncreaseStrongSellVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackSellVolume();
}

void CStock::CalculateAttackSellVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    IncreaseAttackSellBelow50000(m_lCurrentGuadanTransactionVolume);
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    IncreaseAttackSellBelow200000(m_lCurrentGuadanTransactionVolume);
  }
  else {
    IncreaseAttackSellAbove200000(m_lCurrentGuadanTransactionVolume);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 计算挂单量的变化，由工作线程调用
//
// 采用map结构存储，这样简单且易于理解，唯一问题就是寻址花费时间长。需要测试后看看速度如何。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisGuadan(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice) {
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

void CStock::SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
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

void CStock::SetCurrentGuadan(CRTDataPtr pCurrentRTData) {
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

void CStock::CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // 撤单了的话
      m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i)));
    }
  }
}

void CStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // 撤单了的话
      m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i)));
    }
  }
}

bool CStock::HaveGuadan(long lPrice) {
  if (m_mapGuadan.find(lPrice) == m_mapGuadan.end()) return false;
  else if (m_mapGuadan.at(lPrice) == 0) return false;
  return true;
}

bool CStock::CheckCurrentRTData() {
  if ((m_stockCalculatedInfo.GetOrdinaryBuyVolume() < 0) || (m_stockCalculatedInfo.GetOrdinarySellVolume() < 0)
      || (m_stockCalculatedInfo.GetAttackBuyVolume() < 0) || (m_stockCalculatedInfo.GetAttackSellVolume() < 0)
      || (m_stockCalculatedInfo.GetStrongBuyVolume() < 0) || (m_stockCalculatedInfo.GetStrongSellVolume() < 0)) {
    int j = 0;
    if (m_stockCalculatedInfo.GetOrdinaryBuyVolume() < 0) j = 1;
    if (m_stockCalculatedInfo.GetOrdinarySellVolume() < 0) j += 2;
    if (m_stockCalculatedInfo.GetAttackBuyVolume() < 0) j += 4;
    if (m_stockCalculatedInfo.GetAttackSellVolume() < 0) j += 8;
    if (m_stockCalculatedInfo.GetStrongBuyVolume() < 0) j += 16;
    if (m_stockCalculatedInfo.GetStrongSellVolume() < 0) j += 32;
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_systemTime.GetTime(), m_stockBasicInfo.GetStockCode(), j);
    return false;
  }
  return true;
}

void CStock::ShowCurrentTransaction() {
  // 显示当前交易情况
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      if (gl_ChinaStockMarket.m_pCurrentStock->GetCurrentTransationVolume() > 0) {
        gl_ChinaStockMarket.m_pCurrentStock->ReportGuadanTransaction();
      }
    }
  }
}

void CStock::ShowCurrentInformationofCancelingGuadan(void) {
  // 显示当前取消挂单的情况
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      gl_ChinaStockMarket.m_pCurrentStock->ReportGuadan();
    }
  }
}

void CStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, " %s %I64d股成交于%10.3f    ", m_stockBasicInfo.GetStockCode().GetBuffer(),
            m_lCurrentGuadanTransactionVolume, m_dCurrentGuadanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
  str1 = _T(" STRONG BUY");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongBuyVolume());
  break;
  case __STRONG_SELL__:
  str1 = _T(" STRONG SELL");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongSellVolume());
  break;
  case __ATTACK_BUY__:
  str1 = _T(" ATTACK BUY");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackBuyVolume());
  break;
  case __ATTACK_SELL__:
  str1 = _T(" ATTACK SELL");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackSellVolume());
  break;
  case __ORDINARY_BUY__:
  str1 = _T(" ORDINARY BUY");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinaryBuyVolume());
  break;
  case __ORDINARY_SELL__:
  str1 = _T(" ORDINARY SELL");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinarySellVolume());
  break;
  case __UNKNOWN_BUYSELL__:
  str1 = _T(" UNKNOWN BUYSELL");
  sprintf_s(buffer, ": %I64d，  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetUnknownVolume());
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

void CStock::ReportGuadan(void) {
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, "当前取消卖单量：%I64d", m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, "  总取消卖单量：%I64d", m_stockCalculatedInfo.GetCancelSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // 采用同步机制传递消息
  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, "当前取消买单量：%I64d", m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, "  总取消买单量：%I64d", m_stockCalculatedInfo.GetCancelBuyVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // 采用同步机制传递消息
  }
}

bool CStock::SaveRealTimeData(CSetRealTimeData& setRTData) {
  ASSERT(setRTData.IsOpen());
  for (auto pRTData : m_dequeRTData) {
    pRTData->SetMarket(GetMarket());
    pRTData->SetStockCode(GetStockCode());
    pRTData->SetStockName(GetStockName());
    pRTData->AppendData(setRTData);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制存储实时数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CStock::PushRTData(CRTDataPtr pData) {
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
CRTDataPtr CStock::PopRTData(void) {
  CRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    m_dequeRTData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return nullptr;
}

CRTDataPtr CStock::GetRTDataAtHead(void) {
  CRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 采用同步机制提取栈内的实时数据数量。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
long CStock::GetRTDataDequeSize(void) {
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTData.size();
    singleLock.Unlock();
    return lCount;
  }
  ASSERT(0);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CStock::TodayDataIsActive(void) {
  if (!m_fActive) return false;
  else {
    return IsTodayDataChanged();
  }
}

bool CStock::IsTodayDataChanged(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

#ifdef _DEBUG
void CStock::AssertValid() const {
  CObject::AssertValid();
}

void CStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG