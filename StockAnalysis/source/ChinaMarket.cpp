//////////////////////////////////////////////////////////////////////////////////////////////////

//#include"stdafx.h"

#include"globedef.h"
#include"Thread.h"

#include"TransferSharedPtr.h"

#include"WebInquirer.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"SetDayLineInfo.h"
#include"SetDayLineToday.h"
#include"SetOption.h"
#include"SetCrweberIndex.h"

// 信号量必须声明为全局变量（为了初始化）
Semaphore gl_SaveOneStockDayLine(4);  // 此信号量用于生成日线历史数据库
Semaphore gl_ProcessSinaRTDataQueue(1);   // 新浪实时数据处理同时只允许一个线程存在
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);
Semaphore gl_SemaphoreCalculateDayLineRS(gl_cMaxCalculatingRSThreads);

CRTDataContainer gl_RTDataContainer;
CWebInquirer gl_WebInquirer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChinaMarket::CChinaMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("ChinaMarket市场变量只允许存在一个实例\n");
    ASSERT(0);
  }
  m_strMarketId = _T("中国股票市场");
  m_lTimeZoneOffset = -8 * 3600; // 北京标准时间位于东八区，超前GMT8小时
  m_fSaveRTData = false; // 此存储实时数据标识，用于存储供测试函数用的实时数据。目前任务已经完成。
  Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局变量的解析位于程序退出的最后，要晚于CMainFrame的解析。故而如果要想将系统退出的过程放在这里，需要研究。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChinaMarket::~CChinaMarket() {
}

void CChinaMarket::ResetMarket(void) {
  TRACE(_T("重置中国股市\n"));
  CString str = _T("重置中国股市于北京标准时间：");
  str += GetMarketTimeString();
  gl_systemMessage.PushInformationMessage(str);
  while (gl_ThreadStatus.IsCalculatingRS() || gl_ThreadStatus.IsCalculatingRTData() || gl_ThreadStatus.IsSavingTempData()
         || gl_ThreadStatus.IsSavingDayLine()) {
    Sleep(1);
  }
  Reset();
  LoadStockCodeDB();
  LoadOptionDB();
}

void CChinaMarket::Reset(void) {
  CalculateTime(); // 初始化市场时间
  m_lTotalActiveStock = 0; // 初始时股票池中的股票数量为零

  m_llRTDataReceived = 0;

  m_fLoadedSelectedStock = false;
  m_fSystemReady = false;    // 市场初始状态为未设置好。
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  m_ttNewestTransactionTime = 0;

  if (GetTime() >= 150400) { // 中国股票市场已经闭市
    m_fTodayStockProcessed = true; // 闭市后才执行本系统，则认为已经处理过今日股票数据了。
  }
  else m_fTodayStockProcessed = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = __CHINA_MARKET_BEGIN_DAY__;

  m_fSaveDayLine = false;

  m_fTodayTempDataLoaded = false;

  m_fRTDataSetCleared = false;

  m_fCheckActiveStock = true;  //检查当日活跃股票，必须为真。

  m_fGetRTData = true;
  m_iCountDownSlowReadingRTData = 3; // 400毫秒每次

  m_fUsingSinaRTDataReceiver = true; // 使用新浪实时数据提取器
  m_fUsingTengxunRTDataReceiver = true; // 默认状态下读取腾讯实时行情
  m_fUsingNeteaseRTDataReceiver = false; // 不使用网易实时数据提取器

  m_fUpdateStockCodeDB = false;

  m_iDayLineNeedProcess = 0;
  m_iDayLineNeedSave = 0;
  m_iDayLineNeedUpdate = 0;

  m_lSinaRTDataInquiringIndex = 0;
  m_lTengxunRTDataInquiringIndex = 0;
  m_lNeteaseRTDataInquiringIndex = 0;
  m_lNeteaseDayLineDataInquiringIndex = 0;

  m_pCurrentStock = nullptr;

  // 生成股票代码池
  CreateTotalStockContainer();
}

#ifdef _DEBUG
void CChinaMarket::AssertValid() const {
  CVirtualMarket::AssertValid();
}

void CChinaMarket::Dump(CDumpContext& dc) const {
  CVirtualMarket::Dump(dc);
}
#endif //_DEBUG

bool CChinaMarket::CheckMarketReady(void) noexcept {
  if (!m_fSystemReady) {
    if (m_llRTDataReceived > m_lTotalStock * 3) {
      m_fSystemReady = true;
    }
  }
  return m_fSystemReady;
}

bool CChinaMarket::ChangeCurrentStockToNextStock(void) {
  ASSERT(m_pCurrentStock != nullptr);
  long lIndex = m_pCurrentStock->GetOffset();
  CChinaStockPtr pStock = m_pCurrentStock;
  bool fFound = false;
  int i = 1;
  while (!fFound) {
    if ((lIndex + i) < 12000) {
      pStock = gl_ChinaStockMarket.GetStock(lIndex + i);
    }
    else {
      pStock = gl_ChinaStockMarket.GetStock(lIndex + i - 12000);
    }
    if (pStock->GetIPOStatus() != 0) fFound = true;
    i++;
  }
  SetCurrentStock(pStock);
  return true;
}

bool CChinaMarket::ChangeCurrentStockToPrevStock(void) {
  ASSERT(m_pCurrentStock != nullptr);
  long lIndex = m_pCurrentStock->GetOffset();
  CChinaStockPtr pStock = m_pCurrentStock;
  bool fFound = false;
  int i = 1;
  while (!fFound) {
    if ((lIndex - i) >= 0) {
      pStock = gl_ChinaStockMarket.GetStock(lIndex - i);
    }
    else {
      pStock = gl_ChinaStockMarket.GetStock(lIndex + 12000 - i);
    }
    if (pStock->GetIPOStatus() != 0) fFound = true;
    i++;
  }
  SetCurrentStock(pStock);
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CChinaMarket的初始函数调用一次。
// 这个函数需要其他全局变量初始化的支持，故而gl_ChinaStockMarket实例需要放在所有全局变量的最后。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CreateTotalStockContainer(void) {
  char buffer[10]{};

  CChinaStockPtr pStock = nullptr;
  int iCount = 0;

  // 清空之前的数据（如果有的话。在Reset时，这两个容器中就存有数据）。
  m_vChinaMarketAStock.clear();
  m_mapChinaMarketAStock.clear();

  // 生成上海股票代码
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // 上海主板
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // 使用下标生成新的映射
    ASSERT(pStock->IsDayLineNeedUpdate());
    m_iDayLineNeedUpdate++;
  }

  // 生成三版股票代码
  for (int i = 603000; i < 604000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // 上海三板
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成科创版股票代码
  for (int i = 688000; i < 689000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // 上海科创板
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成B股股票代码
  for (int i = 900000; i < 901000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // 上海B股
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成上海指数代码
  for (int i = 0; i < 1000; i++) {
    CString str = _T("sh");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // 上海指数
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(false);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  ///////////////////////////////////////////////
  // 生成深圳主板股票代码
  for (int i = 0; i < 2000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // 深圳主板
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成深圳中小板股票代码
  for (int i = 2000; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // 深圳中小板
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成B股股票代码
  for (int i = 200000; i < 201000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // 深圳B股
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成创业板股票代码
  for (int i = 300000; i < 301000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // 深圳创业板
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }

  // 生成深圳指数
  for (int i = 399000; i < 400000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // 深圳指数
    pStock->SetNeedProcessRTData(false);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// 使用下标生成新的映射
    m_iDayLineNeedUpdate++;
  }
  m_lTotalStock = m_vChinaMarketAStock.size();
  ASSERT(m_lTotalStock == m_iDayLineNeedUpdate);
  ASSERT(m_iDayLineNeedUpdate == 12000); // 目前总查询股票数为12000个。
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成网易日线股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
//
//  此函数是检查m_vChinaMarketAStock股票池
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::CreateNeteaseDayLineInquiringStr() {
  bool fFound = false;
  int iCount = 0;
  CString strTemp, str = _T("");
  while (!fFound && (iCount++ < 1000)) {
    CChinaStockPtr pStock = m_vChinaMarketAStock.at(m_lNeteaseDayLineDataInquiringIndex);
    if (!pStock->IsDayLineNeedUpdate()) { // 日线数据不需要更新。在系统初始时，设置此m_fDayLineNeedUpdate标识
      // TRACE("%S 日线数据无需更新\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->GetIPOStatus() == __STOCK_NULL__) {	// 尚未使用过的股票代码无需查询日线数据
      pStock->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
      // TRACE("无效股票代码：%S, 无需查询日线数据\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->GetDayLineEndDay() >= GetLastTradeDay()) { //上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
      pStock->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
      // TRACE("%S 日线数据本日已更新\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->IsDayLineNeedProcess()) { // 日线数据已下载但尚未处理（一般此情况不会出现）
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else {
      fFound = true;
    }
  }

  if (iCount >= 1000) { //  没有找到需要申请日线的股票
    TRACE("未找到需更新日线历史数据的股票\n");
    return _T("");
  }

  // 找到了需申请日线历史数据的股票（siCounter为索引）
  CChinaStockPtr pStock = m_vChinaMarketAStock.at(m_lNeteaseDayLineDataInquiringIndex);
  ASSERT(!pStock->IsDayLineNeedSaving());
  ASSERT(!pStock->IsDayLineNeedProcess());
  pStock->SetDayLineNeedUpdate(false);
  switch (pStock->GetMarket()) { // 转换成网易日线数据申请制式（上海为‘0’，深圳为‘1’）
  case __SHANGHAI_MARKET__: // 上海市场？
  case __SHANGHAI_MAIN__: // 上海主板？
  case __SHANGHAI_INDEX__: // 上海指数
  case __SHANGHAI_3BAN__: // 上海3板
  case __SHANGHAI_KECHUANG__: // 上海科创板
  case __SHANGHAI_B_SHARE__: // 上海B股
  str += '0'; // 上海市场标识
  break;
  case __SHENZHEN_MARKET__: // 深圳市场？
  case __SHENZHEN_INDEX__: // 深圳指数
  case __SHENZHEN_3BAN__: // 深圳中小板
  case __SHENZHEN_B_SHARE__: // 深圳B股
  case __SHENZHEN_CHUANGYE__: // 深圳创业板
  case __SHENZHEN_MAIN__: // 深圳主板
  str += '1'; // 深圳市场标识
  break;
  default: // 越界
  ASSERT(0);
  }
  str += pStock->GetStockCode().Right(6); // 取股票代码的右边六位数字。
  IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
  return str;
}

long CChinaMarket::IncreaseStockInquiringIndex(long& lIndex) {
  if (++lIndex == m_lTotalStock) {
    lIndex = 0;
  }
  return lIndex;
}

/////////////////////////////////////////////////////////////////////////
//
//	得到分时线偏移量。09:30为0，15:00为240,步长为1分钟
//
//
////////////////////////////////////////////////////////////////////////
long CChinaMarket::GetMinLineOffset(CChinaStock sID, time_t Time) {
  ASSERT(Time >= 0);
  tm tmTemp{};
  time_t t = 0;
  long lIndex = 0;

  localtime_s(&tmTemp, &Time);
  tmTemp.tm_hour = (9 - 8);			// time_t, tm使用的是国际标准时(UTC),故北京时间09：30即UTC的01：30。要减去8小时
  tmTemp.tm_min = 30;
  tmTemp.tm_sec = 0;
  t = mktime(&tmTemp);
  lIndex = (Time - t) / 60;
  if (lIndex < 0) lIndex = 0;
  if ((lIndex >= 120) && (lIndex < 209)) lIndex = 119;
  if (lIndex >= 210) lIndex -= 90;
  if (lIndex >= 240) lIndex = 239;

  ASSERT((lIndex >= 0) && (lIndex < 240));
  return(lIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		判断pStock是否为沪深A股。
//		沪市A股代码以6开头，深市A股代码以00开头。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsAStock(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);

  return(IsAStock(pStock->GetStockCode()));
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		判断strStockCode是否为沪深A股主板的股票代码。
//		沪市A股代码以600或601开头，深市A股代码以000或001开头。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsAStock(CString strStockCode) {
  if (strStockCode[0] == 's') {
    if ((strStockCode[1] == 'h') && (strStockCode[2] == '6') && (strStockCode[3] == '0')) {
      if ((strStockCode[4] == '0') || (strStockCode[4] == '1')) {
        return true;
      }
    }
    else {
      if ((strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
        if ((strStockCode[4] == '0') || (strStockCode[4] == '2')) {
          return true;
        }
      }
    }
  }
  return(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		判断代码是否为沪深股票。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsStock(CString strStockCode) {
  CChinaStockPtr pStock = GetStock(strStockCode);
  if (pStock != nullptr) {
    return(true);
  }
  else {
    return(false);
  }
}

bool CChinaMarket::IsDayLineDBUpdated(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineDBUpdated()) return true;
  }
  return false;
}

void CChinaMarket::ClearDayLineDBUpdatedFlag(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->SetDayLineDBUpdated(false);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CChinaMarket::GetTotalAttackBuyAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive() && IsAStock(pStock)) {
      lAmount += pStock->GetAttackBuyAmount();
    }
  }
  return(lAmount);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
INT64 CChinaMarket::GetTotalAttackSellAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive() && IsAStock(pStock)) {
      lAmount += pStock->GetAttackSellAmount();
    }
  }
  return(lAmount);
}

bool CChinaMarket::TaskGetNeteaseDayLineFromWeb(void) {
  ASSERT(IsSystemReady());
  if (m_iDayLineNeedUpdate > 0) {
    // 抓取日线数据.
    // 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
    return(gl_WebInquirer.GetNeteaseDayLineData());
  }
  else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，由SchedulingTaskPerSecond函数调用,每三秒执行一次。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数用到大量的全局变量，还是放在主线程为好。工作线程目前还是只做计算个股的挂单情况。
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskDistributeSinaRTDataToProperStock(void) {
  gl_ProcessSinaRTDataQueue.Wait();
  CChinaStockPtr pStock;
  const long lTotalNumber = gl_RTDataContainer.GetSinaRTDataSize();
  CString strVolume;

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CRTDataPtr pRTData = gl_RTDataContainer.PopSinaRTData();
    if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("新浪实时数据源设置有误"));
      continue;
    }
    if (pRTData->IsActive()) { // 此实时数据有效？
      if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
        m_ttNewestTransactionTime = pRTData->GetTransactionTime();
      }
      pStock = GetStock(pRTData->GetStockCode());
      if (!pStock->IsActive()) {
        if (pRTData->IsValidTime()) {
          pStock->SetTodayActive(pRTData->GetMarket(), pRTData->GetStockCode(), pRTData->GetStockName());
          pStock->SetIPOStatus(__STOCK_IPOED__);
        }
      }
      if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // 新的数据？
        pStock->PushRTData(pRTData); // 存储新的数据至数据池
        if (pStock->IsRecordRTData()) {
          StoreChoiceRTData(pRTData);
        }
        pStock->SetTransactionTime(pRTData->GetTransactionTime());   // 设置最新接受到实时数据的时间
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // 设置接收到实时数据标识
  ASSERT(gl_RTDataContainer.GetSinaRTDataSize() == 0); // 必须一次处理全体数据。
  gl_ProcessSinaRTDataQueue.Signal();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询新浪实时股票数据的字符串
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetSinaInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return GetNextInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber, fSkipUnactiveStock);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询腾讯实时股票数据的字符串
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetTengxunInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  ASSERT(IsSystemReady());
  return GetNextInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber, fSkipUnactiveStock);
}

CString CChinaMarket::GetNeteaseInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str = _T("");
  CString strStockCode, strRight6, strLeft2, strPrefix;

  ASSERT(IsSystemReady());
  if (fSkipUnactiveStock) StepToActiveStockIndex(m_lNeteaseRTDataInquiringIndex);
  strStockCode = m_vChinaMarketAStock.at(m_lNeteaseRTDataInquiringIndex)->GetStockCode();
  IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex);
  strRight6 = strStockCode.Right(6);
  strLeft2 = strStockCode.Left(2);
  if (strLeft2.Compare(_T("sh")) == 0) {
    strPrefix = _T("0");
  }
  else strPrefix = _T("1");
  str += strPrefix + strRight6;  // 得到第一个股票代码
  int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
  while ((m_lNeteaseRTDataInquiringIndex < m_lTotalStock) && (iCount < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
    if (fSkipUnactiveStock) StepToActiveStockIndex(m_lNeteaseRTDataInquiringIndex);
    iCount++;
    str += _T(",");
    strStockCode = m_vChinaMarketAStock.at(m_lNeteaseRTDataInquiringIndex)->GetStockCode();
    strRight6 = strStockCode.Right(6);
    strLeft2 = strStockCode.Left(2);
    if (strLeft2.Compare(_T("sh")) == 0) {
      strPrefix = _T("0");
    }
    else strPrefix = _T("1");
    str += strPrefix + strRight6;  // 得到第一个股票代码
    if (m_lNeteaseRTDataInquiringIndex == 0) break;
    IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex);
  }
  if (m_lNeteaseRTDataInquiringIndex > 0) m_lNeteaseRTDataInquiringIndex--;// 退后一步，防止最后一个股票查询错误（其实不必要了）

  return str;
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(CString str) {
  long lLength = str.GetLength();
  char* p = str.GetBuffer();
  int i = 0;
  char buffer[8];
  CString strStockCode, strRight;

  while (i < lLength) {
    strncpy_s(buffer, p, 7);
    p += 8;
    i += 8;
    buffer[7] = 0x000;
    strRight = buffer;
    if (buffer[0] == '0') strStockCode = "sh";
    else if (buffer[0] == '1') strStockCode = "sz";
    else return false;
    strStockCode += strRight.Right(6);
    CChinaStockPtr pStock = GetStock(strStockCode);
    if (pStock == nullptr) {
      CString strReport = _T("网易日线查询股票代码错误：");
      TRACE(_T("网易日线查询股票代码错误：%s\n"), strStockCode.GetBuffer());
      strReport += strStockCode;
      gl_systemMessage.PushInnerSystemInformationMessage(strReport);
      return false;
    }
  }
  return true;
}

CString CChinaMarket::GetNextInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock) {
  CString str = _T("");

  if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
  str += m_vChinaMarketAStock.at(iStockIndex)->GetStockCode();  // 得到第一个股票代码
  IncreaseStockInquiringIndex(iStockIndex);
  int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
  while ((iStockIndex < gl_ChinaStockMarket.GetTotalStock()) && (iCount < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
    if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
    iCount++;
    str += strPostfix;
    str += m_vChinaMarketAStock.at(iStockIndex)->GetStockCode();
    if (iStockIndex == 0) break;
    IncreaseStockInquiringIndex(iStockIndex);
  }
  if (iStockIndex > 0) iStockIndex--; // 退后一步，防止最后一个股票查询错误（其实不必要了）

  return str;
}

bool CChinaMarket::StepToActiveStockIndex(long& iStockIndex) {
  while (!m_vChinaMarketAStock.at(iStockIndex)->IsActive()) {
    IncreaseStockInquiringIndex(iStockIndex);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 由工作线程ThreadCalculatingRTDataProc调用，注意全局变量的使用
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessRTData(void) {
  ASSERT(gl_ThreadStatus.IsRTDataNeedCalculate());
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive()) {
      pStock->ProcessRTData();
    }
  }
  return true;
}

bool CChinaMarket::TaskProcessWebRTDataGetFromSinaServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  long lTotalData = gl_WebInquirer.GetSinaRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopSinaRTData();
    pWebDataReceived->SetCurrentPos(0);
    while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
      CRTDataPtr pRTData = make_shared<CRTData>();
      if (pRTData->ReadSinaData(pWebDataReceived)) {
        m_llRTDataReceived++;
        gl_RTDataContainer.PushSinaRTData(pRTData); // 将此实时数据指针存入实时数据队列
      }
      else return false;  // 后面的数据出问题，抛掉不用。
    }
  }
  return true;
}

void CChinaMarket::StoreChoiceRTData(CRTDataPtr pRTData) {
  m_qRTData.push(pRTData);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取网易制式实时数据，返回值是所读数据是否出现格式错误。
// 开始处为第一个{，结束处为倒数第二个}。如果尚有数据需处理，则被处理的字符为','；如果没有数据了，则被处理的字符为' '。
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// 网易实时数据缺少关键性的成交金额一项，故而无法作为基本数据，只能作为补充用。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromNeteaseServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  int iCount = 0;

  long lTotalData = gl_WebInquirer.GetNeteaseRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
    pWebDataReceived->m_pCurrentPos = pWebDataReceived->m_pDataBuffer;
    pWebDataReceived->ResetCurrentPos();
    if (!IsInvalidNeteaseRTData(pWebDataReceived)) {
      if (!IsValidNeteaseRTDataPrefix(pWebDataReceived)) return false;
      iCount = 0;
      while (!((*pWebDataReceived->m_pCurrentPos == ' ') || (pWebDataReceived->m_lCurrentPos >= (pWebDataReceived->m_lBufferLength - 4)))) {
        CRTDataPtr pRTData = make_shared<CRTData>();
        if (pRTData->SecceedReadingNeteaseData(pWebDataReceived)) {
          iCount++;
          gl_RTDataContainer.PushNeteaseRTData(pRTData); // 将此实时数据指针存入实时数据队列
           // 检测一下
          CheckNeteaseRTData(pRTData);
        }
        else return false;  // 后面的数据出问题，抛掉不用。
      }
      //TRACE(_T("ReadNetease正常结束,共接收了%d个数据\n"), iCount);
    }
  }
  return true;
}

bool CChinaMarket::IsInvalidNeteaseRTData(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("_ntes_quote_callback({ });"); // 此为无效股票查询到的数据格式，共26个字符
  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 26);
  buffer[26] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(strInvalidStock) == 0) {
    ASSERT(pWebDataReceived->GetBufferLength() == 26);
    return true;
  }
  else return false;
}

bool CChinaMarket::IsValidNeteaseRTDataPrefix(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("_ntes_quote_callback("); // 此为无效股票查询到的数据格式，共22个字符

  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 21); // 读入"_ntes_quote_callback("
  buffer[21] = 0x000;
  CString str1;
  str1 = buffer;
  if (strInvalidStock.Compare(str1) != 0) { // 数据格式出错
    return false;
  }
  pWebDataReceived->IncreaseCurrentPos(21);
  return true;
}

void CChinaMarket::CheckNeteaseRTData(CRTDataPtr pRTData) {
  // 检测一下
  CString str;
  ASSERT(pRTData->GetDataSource() == __NETEASE_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CChinaStockPtr pStock = nullptr;
    if ((pStock = gl_ChinaStockMarket.GetStock(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T(" 网易实时检测到不处于活跃状态");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T(" 无效股票代码（网易实时数据）");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
  }
}

bool CChinaMarket::TaskProcessNeteaseRTData(void) {
  CRTDataPtr pRTData = nullptr;
  long lTotalData = gl_RTDataContainer.GetNeteaseRTDataSize();

  for (long i = 0; i < lTotalData; i++) {
    // 目前不使用网易实时数据，这里只是简单地取出后扔掉。
    pRTData = gl_RTDataContainer.PopNeteaseRTData();
    pRTData = nullptr;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时数据，如果遇到被查询股票代码为非上市时，只是简单略过，不返回数据。故而查询900个股票，返回的数据量要小于900.
// 只有当所有的查询股票皆为非上市时，才返回一个21个字符串：v_pv_none_match=\"1\";\n
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromTengxunServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  int j = 0;

  long lTotalData = gl_WebInquirer.GetTengxunRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopTengxunRTData();
    pWebDataReceived->ResetCurrentPos();
    if (!IsInvalidTengxunRTData(pWebDataReceived)) { // 处理这21个字符串的函数可以放在这里，也可以放在最前面。
      j = 0;
      while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
        CRTDataPtr pRTData = make_shared<CRTData>();
        if (pRTData->ReadTengxunData(pWebDataReceived)) {
          j++;
          gl_RTDataContainer.PushTengxunRTData(pRTData); // 将此实时数据指针存入实时数据队列
            //gl_QueueSinaRTDataForSave.PushRTData(pRTData); // 同时存入待存储实时数据队列

            // 检测一下
          CheckTengxunRTData(pRTData);
        }
        else return false;  // 后面的数据出问题，抛掉不用。
      }
      //TRACE(_T("接收到%d个腾讯实时数据\n"), j);
    }
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsInvalidTengxunRTData(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 21);
  buffer[21] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(strInvalidStock) == 0) {
    ASSERT(pWebDataReceived->GetBufferLength() == 21);
    return true;
  }
  else return false;
}

void CChinaMarket::CheckTengxunRTData(CRTDataPtr pRTData) {
  CString str;
  ASSERT(pRTData->GetDataSource() == __TENGXUN_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CChinaStockPtr pStock = nullptr;
    if ((pStock = gl_ChinaStockMarket.GetStock(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T("腾讯实时检测到不处于活跃状态");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T("无效股票代码（腾讯实时数据）");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
  }
}

bool CChinaMarket::TaskProcessTengxunRTData(void) {
  CRTDataPtr pRTData = nullptr;
  long lTotalData = gl_RTDataContainer.GetTengxunRTDataSize();

  for (long i = 0; i < lTotalData; i++) {
    pRTData = gl_RTDataContainer.PopTengxunRTData();
    if (pRTData->IsActive()) {
      auto pStock = gl_ChinaStockMarket.GetStock(pRTData->GetStockCode());
      pStock->SetTotalValue(pRTData->GetTotalValue());
      pStock->SetCurrentValue(pRTData->GetCurrentValue());
      pStock->SetHighLimit(pRTData->GetHighLimit());
      pStock->SetLowLimit(pRTData->GetLowLimit());
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
// 大约每100毫秒调度一次
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  // 抓取实时数据(新浪、腾讯和网易）。每400毫秒申请一次，即可保证在3秒中内遍历一遍全体活跃股票。
  if (m_fGetRTData && (m_iCountDownSlowReadingRTData <= 0)) {
    TaskGetRTDataFromWeb();
    TaskProcessWebRTDataGetFromSinaServer();
    // 如果要求慢速读取实时数据，则设置读取速率为每分钟一次
    if (!m_fStartReceivingData && IsSystemReady()) m_iCountDownSlowReadingRTData = 300; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
    else m_iCountDownSlowReadingRTData = 3;  // 计数4次,即每400毫秒申请一次实时数据
  }
  m_iCountDownSlowReadingRTData--;

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast);
    s_timeLast = GetMarketTime();
  }

  // 系统准备好了之后需要完成的各项工作
  if (IsSystemReady()) {
    if (!m_fTodayTempDataLoaded) { // 此工作仅进行一次。
      LoadTodayTempDB();
      m_fTodayTempDataLoaded = true;
    }
    TaskProcessWebRTDataGetFromTengxunServer();
    TaskProcessWebRTDataGetFromNeteaseServer();
    TaskGetNeteaseDayLineFromWeb();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 从新浪、网易或者腾讯实时行情数据服务器读取实时数据。使用其中之一即可。
//
/////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskGetRTDataFromWeb(void) {
  static int siCountDownTengxunNumber = 5;
  static int siCountDownNeteaseNumber = 5;

  if (IsUsingSinaRTDataReceiver()) {
    gl_WebInquirer.GetSinaRTData(); // 每400毫秒(100X4)申请一次实时数据。新浪的实时行情服务器响应时间不超过100毫秒（30-70之间），且没有出现过数据错误。
  }

  if (IsSystemReady()) {
    // 网易实时数据有大量的缺失字段，且前缀后缀也有时缺失，暂时停止使用。
    // 网易实时数据有时还发送没有要求过的股票，不知为何。
    if (IsUsingNeteaseRTDataReceiver()) {
      if (siCountDownNeteaseNumber <= 0) {
        // 读取网易实时行情数据。估计网易实时行情与新浪的数据源相同，故而两者可互换，使用其一即可。
        gl_WebInquirer.GetNeteaseRTData(); // 目前不使用此功能。
        siCountDownNeteaseNumber = 0;
      }
      else siCountDownNeteaseNumber--;
    }
    // 读取腾讯实时行情数据。 由于腾讯实时行情的股数精度为手，没有零股信息，导致无法与新浪实时行情数据对接（新浪精度为股），故而暂时不用
    if (IsUsingTengxunRTDataReceiver()) {
      if (siCountDownTengxunNumber <= 0) {
        gl_WebInquirer.GetTengxunRTData();// 只有当系统准备完毕后，方可执行读取腾讯实时行情数据的工作。目前不使用此功能
        siCountDownTengxunNumber = 5;
      }
      else siCountDownTengxunNumber--; // 新浪实时数据读取五次，腾讯才读取一次。因为腾讯的挂单股数采用的是每手标准，精度不够
    }
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 定时调度函数，每秒一次。
//
// 各种任务之间有可能出现互斥的现象，如存储临时实时数据的工作线程与计算实时数据的工作线程之间就不允许同时运行，
// 故而所有的定时任务，要按照时间间隔从长到短排列，即先执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  static int s_iCountDownProcessWebRTData = 0;
  const long lCurrentTime = GetTime();

  // 各调度程序按间隔时间大小顺序排列，间隔时间长的必须位于间隔时间短的之前。
  SchedulingTaskPerHour(lSecondNumber, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecondNumber, lCurrentTime);
  SchedulingTaskPerMinute(lSecondNumber, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecondNumber, lCurrentTime);

  // 判断是否开始正常收集数据
  TaskCheckStartReceivingData(lCurrentTime);
  // 判断中国股票市场开市状态
  TaskCheckMarketOpen(lCurrentTime);

  if (s_iCountDownProcessWebRTData <= 0) {
    // 将接收到的实时数据分发至各相关股票的实时数据队列中。
    // 由于有多个数据源，故而需要等待各数据源都执行一次后，方可以分发至相关股票处，故而需要每三秒执行一次，以保证各数据源至少都能提供一次数据。
    TaskDistributeSinaRTDataToProperStock();
    TaskProcessTengxunRTData();
    TaskProcessNeteaseRTData();
    s_iCountDownProcessWebRTData = 0;
  }
  else s_iCountDownProcessWebRTData--;

  // 计算实时数据，每秒钟一次。目前个股实时数据为每3秒钟一次更新，故而无需再快了。
  // 此计算任务要在DistributeRTDataReceivedFromWebToProperStock之后执行，以防止出现同步问题。
  if (IsSystemReady() && !gl_ThreadStatus.IsSavingTempData() && IsTodayTempRTDataLoaded()) { // 在系统存储临时数据时不能同时计算实时数据，否则容易出现同步问题。
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      TaskProcessRTData();
      gl_ThreadStatus.SetRTDataNeedCalculate(false);
      gl_ThreadStatus.SetCalculatingRTData(false);
    }
  }

  TaskShowCurrentTransaction();

  // 装载当前股票日线数据
  TaskLoadCurrentStockDayLine();

  return true;
}

bool CChinaMarket::SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime) {
  static int i1HourCounter = 3599; // 一小时一次的计数器

  // 计算每一小时一次的任务
  i1HourCounter -= lSecondNumber;
  if (i1HourCounter < 0) {
    i1HourCounter = 3599;

    return true;
  }
  else
    return false;
}

bool CChinaMarket::SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime) {
  static int i5MinuteCounter = 299; // 五分钟一次的计数器

  // 计算每五分钟一次的任务。
  i5MinuteCounter -= lSecondNumber;
  if (i5MinuteCounter < 0) {
    i5MinuteCounter = 299;

    TaskSaveTempDataIntoDB(lCurrentTime);

    return true;
  } // 每五分钟一次的任务
  else {
    return false;
  }
}

void CChinaMarket::TaskSaveTempDataIntoDB(long lCurrentTime) {
  // 开市时每五分钟存储一次当前状态。这是一个备用措施，防止退出系统后就丢掉了所有的数据，不必太频繁。
  if (m_fSystemReady) {
    if (m_fMarketOpened && !gl_ThreadStatus.IsCalculatingRTData()) {
      if (((lCurrentTime > 93000) && (lCurrentTime < 113600)) || ((lCurrentTime > 130000) && (lCurrentTime < 150600))) { // 存储临时数据严格按照交易时间来确定(中间休市期间和闭市后各要存储一次，故而到11:36和15:06才中止）
        CString str;
        str = _T("存储临时数据");
        gl_systemMessage.PushDayLineInfoMessage(str);
        UpdateTempRTData();
      }
    }
  }
}

bool CChinaMarket::SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  // 计算每分钟一次的任务。所有的定时任务，要按照时间间隔从长到短排列，即先执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
  i1MinuteCounter -= lSecondNumber;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59; // 重置计数器

    TaskResetMarket(lCurrentTime);
    TaskResetMarketAgain(lCurrentTime);

    // 在开市前和中午暂停时查询所有股票池，找到当天活跃股票。
    TaskSetCheckActiveStockFlag(lCurrentTime);

    // 下午三点三分开始处理当日实时数据。
    TaskProcessTodayStock(lCurrentTime);

    TaskSaveChoicedRTData();

    TaskClearChoicedRTDataSet(lCurrentTime);

    TaskUpdateStockCodeDB();
    TaskUpdateOptionDB();

    TaskCheckDayLineDB();

    return true;
  } // 每一分钟一次的任务
  else {
    return false;
  }
}

bool CChinaMarket::TaskSetCheckActiveStockFlag(long lCurrentTime) {
  if (((lCurrentTime >= 91500) && (lCurrentTime < 92900)) || ((lCurrentTime >= 113100) && (lCurrentTime < 125900))) {
    m_fCheckActiveStock = true;
    return true;
  }
  else {
    m_fCheckActiveStock = false;
    return false;
  }
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
  if (IsSystemReady() && (lCurrentTime >= 150400) && !IsTodayStockProcessed()) {
    AfxBeginThread(ThreadProcessCurrentTradeDayStock, nullptr);
    SetTodayStockProcessed(true);
    return true;
  }
  return false;
}

bool CChinaMarket::TaskCheckDayLineDB(void) {
  if (m_fSaveDayLine && (m_iDayLineNeedSave <= 0) && (m_iDayLineNeedUpdate <= 0) && (m_iDayLineNeedProcess <= 0)) {
    if ((m_iDayLineNeedSave < 0) || (m_iDayLineNeedUpdate < 0) || (m_iDayLineNeedProcess < 0)) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("日线历史数据处理过程中程序有瑕疵"));
    }
    m_fSaveDayLine = false;
    TRACE("日线历史数据更新完毕\n");
    CString str;
    str = _T("日线历史数据更新完毕");
    gl_systemMessage.PushInformationMessage(str);
    if (IsDayLineDBUpdated()) {
      // 更新股票池数据库
      SetUpdateStockCodeDB(true);
      ClearDayLineDBUpdatedFlag();
    }
  }
  return true;
}

bool CChinaMarket::TaskCheckStartReceivingData(long lCurrentTime) {
  if (!IsWorkingDay()) { //周六或者周日闭市。结构tm用0--6表示星期日至星期六
    m_fStartReceivingData = false;
    return(m_fStartReceivingData);
  }
  else if ((lCurrentTime < 91200) || (lCurrentTime > 150630) || ((lCurrentTime > 113500) && (lCurrentTime < 125500))) { //下午三点六分三十秒市场交易结束（为了保证最后一个临时数据的存储）
    m_fStartReceivingData = false;

    return(m_fStartReceivingData);
  }
  else m_fStartReceivingData = true;
  return m_fStartReceivingData;
}

bool CChinaMarket::TaskCheckMarketOpen(long lCurrentTime) {
  if (!IsWorkingDay()) { //周六或者周日闭市。结构tm用0--6表示星期日至星期六
    m_fMarketOpened = false;
  }
  else if ((lCurrentTime > 92800) && (lCurrentTime < 150300)) {
    m_fMarketOpened = true;
  }
  else m_fMarketOpened = false;

  return m_fMarketOpened;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此任务必须每分钟调度一次，因其实现机制采用了
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskResetMarket(long lCurrentTime) {
  // 九点十三分重启系统
// 必须在此时间段内重启，如果更早的话容易出现数据不全的问题。
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 91300) && (lCurrentTime < 91400) && IsWorkingDay()) { // 交易日九点十五分重启系统
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      m_fSystemReady = false;
    }
  }
  return true;
}

bool CChinaMarket::TaskResetMarketAgain(long lCurrentTime) {
  // 九点二十五分再次重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 92500)) {
      if ((lCurrentTime <= 93000) && IsWorkingDay()) { // 交易日九点二十五分再次重启系统
        SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
        m_fSystemReady = false;
      }
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

bool CChinaMarket::TaskUpdateStockCodeDB(void) {
  if (IsUpdateStockCodeDB()) {
    AfxBeginThread(ThreadUpdateStockCodeDB, nullptr);
    SetUpdateStockCodeDB(false);
  }
  return true;
}

bool CChinaMarket::TaskUpdateOptionDB(void) {
  if (IsUpdateOptionDB()) {
    AfxBeginThread(ThreadUpdateOptionDB, nullptr);
    SetUpdateOptionDB(false);
  }
  return true;
}

bool CChinaMarket::TaskShowCurrentTransaction(void) {
  // 显示当前交易情况
  CChinaStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStock();

  if (pCurrentStock != nullptr) {
    if (pCurrentStock->IsRTDataCalculated()) {
      if (pCurrentStock->GetCurrentTransationVolume() > 0) pCurrentStock->ReportGuadanTransaction();
      pCurrentStock->ReportGuadan();
      pCurrentStock->SetRTDataCalculated(false);
    }
  }

  return true;
}

bool CChinaMarket::TaskSaveChoicedRTData(void) {
  if (IsSystemReady() && m_fSaveRTData) {
    AfxBeginThread(ThreadSaveRTData, nullptr);
    return true;
  }
  else return false;
}

/////////////////////////////////////////////////////////////////////
//
// 九点二十五分至九点三十分内清除昨日的实时数据。
//
/////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskClearChoicedRTDataSet(long lCurrentTime) {
  if (lCurrentTime > 93100) {
    m_fRTDataSetCleared = true;
  }

  if (!m_fRTDataSetCleared) {
    if ((lCurrentTime > 92900) && (lCurrentTime < 93100)) {
      CSetRealTimeData setRTData;
      setRTData.Open();
      setRTData.m_pDatabase->BeginTrans();
      while (!setRTData.IsEOF()) {
        setRTData.Delete();
        setRTData.MoveNext();
      }
      setRTData.m_pDatabase->CommitTrans();
      setRTData.Close();

      m_fRTDataSetCleared = true;
    }
  }
  return true;
}

bool CChinaMarket::SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime) {
  static int i10SecondsCounter = 9; // 十秒一次的计数器

  // 计算每十秒钟一次的任务
  i10SecondsCounter -= lSecondNumber;
  if (i10SecondsCounter < 0) {
    i10SecondsCounter = 9;
    // do something

    // 将处理日线历史数据的函数改为定时查询，读取和存储采用工作进程。
    if (m_iDayLineNeedProcess > 0) {
      TaskProcessDayLineGetFromNeeteaseServer();
    }

    // 判断是否存储日线库和股票代码库
    if ((m_iDayLineNeedSave > 0)) {
      m_fSaveDayLine = true;
      gl_ChinaStockMarket.SaveDayLineData();
    }
    TaskCheckDayLineDB();
    return true;
  } // 每十秒钟一次的任务
  else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 得到股票的简称
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetStockName(CString strStockCode) {
  try {
    return (m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(strStockCode))->GetStockName());
  }
  catch (exception&) {
    TRACE("GetStockName函数异常\n");
    return _T("");
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码得到股票的索引。
//	如果没找到的话返回值为假。
//
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::GetStockIndex(CString strStockCode, long& lIndex) {
  try {
    lIndex = m_mapChinaMarketAStock.at(strStockCode);
    return true;
  }
  catch (exception&) {
    TRACE("GetStockIndex越界\n");
    lIndex = -1;
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码得到股票的指针。
//	如果没找到的话返回空指针
//
//
////////////////////////////////////////////////////////////////////////////////
CChinaStockPtr CChinaMarket::GetStock(CString strStockCode) {
  try {
    return (m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(strStockCode)));
  }
  catch (exception&) {
    TRACE("GetStock越界, StockCode = %s\n", strStockCode.GetBuffer());
    return nullptr;
  }
}

CChinaStockPtr CChinaMarket::GetStock(long lIndex) {
  try {
    return m_vChinaMarketAStock.at(lIndex);
  }
  catch (exception&) {
    TRACE("GetStock越界\n");
    return nullptr;
  }
}

void CChinaMarket::IncreaseActiveStockNumber(void) {
  m_lTotalActiveStock++;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码和市场代码设置当前选择股票
//
//////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CString strStockCode) {
  CChinaStockPtr pStock = GetStock(strStockCode);
  SetCurrentStock(pStock);
  ASSERT(m_pCurrentStock != NULL);
}

//////////////////////////////////////////////////////////////////////////
//
// 设置当前操作的股票
//
// 设置相应的股票指针，装载其日线数据。
//
/////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CChinaStockPtr pStock) {
  bool fSet = false;
  if (m_pCurrentStock != nullptr) {
    if (!m_pCurrentStock->IsSameStock(pStock)) {
      m_pCurrentStock->SetRecordRTData(false);
      fSet = true;
    }
  }
  else {
    fSet = true;
  }
  if (fSet) {
    pStock->SetRecordRTData(true);
    m_pCurrentStock = pStock;
    m_fCurrentStockChanged = true;
    m_pCurrentStock->SetDayLineLoaded(false); // 这里只是设置标识，实际装载日线由调度程序执行。
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//  此函数由工作线程ThreadDayLineSaveProc调用，尽量不要使用全局变量。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SaveDayLineData(void) {
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;

  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
      if (pStock->GetDayLineSize() > 0) {
        if (pStock->HaveNewDayLineData()) {
          pTransfer = new strTransferSharedPtr; // 此处生成，由线程负责delete
          pTransfer->m_pStock = pStock;
          AfxBeginThread(ThreadSaveDayLineOfOneStock, (LPVOID)pTransfer, THREAD_PRIORITY_LOWEST);
        }
      }
      else { // 此种情况为有股票代码，但此代码尚未上市
        CString str1 = pStock->GetStockCode();
        str1 += _T(" 为未上市股票代码");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
    }
    if (gl_ExitingSystem) {
      break; // 如果程序正在退出，则停止存储。
    }
  }

  return(true);
}

bool CChinaMarket::ClearDayLineContainer(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->ClearDayLineContainer();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将实时数据存入数据库．默认数据库为空。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SaveRTData(void) {
  CSetRealTimeData setRTData;
  CRTDataPtr pRTData = nullptr;
  long lTotal = m_qRTData.size();

  if (lTotal > 0) {
    setRTData.m_strFilter = _T("[ID] = 1");
    setRTData.Open();
    setRTData.m_pDatabase->BeginTrans();
    for (int i = 0; i < lTotal; i++) {
      pRTData = m_qRTData.front();
      m_qRTData.pop(); // 抛掉最前面这个数据
      pRTData->AppendData(setRTData);
    }
    setRTData.m_pDatabase->CommitTrans();
    setRTData.Close();
  }
  return(true);
}

bool CChinaMarket::IsDayLineNeedSaving(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedSaving()) {
      ASSERT(pStock->IsActive());
      return true;
    }
  }
  return false;
}

bool CChinaMarket::IsDayLineNeedUpdate(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedUpdate()) return true;
  }
  return false;
}

bool CChinaMarket::TaskProcessDayLineGetFromNeeteaseServer(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedProcess()) {
      pStock->ProcessNeteaseDayLineData();
      pStock->SetDayLineNeedProcess(false);
    }
  }
  return true;
}

bool CChinaMarket::TaskLoadCurrentStockDayLine(void) {
  if (m_pCurrentStock != nullptr) {
    if (!m_pCurrentStock->IsDayLineLoaded()) {
      AfxBeginThread(ThreadLoadDayLine, 0);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// 处理当前交易日的实时数据，生成日线各基本数据（相对强度、进攻性买卖盘）。
//
// 只有下载完日线历史数据后，方可执行处理实时数据，否则可能误判股票代码存在与否。
//
// long lCurrentTradeDay 当前交易日。由于存在周六和周日，故而此日期并不一定就是当前日期，而可能时周五
//
//////////////////////////////////////////////////////////////////////////////////
long CChinaMarket::ProcessCurrentTradeDayStock(long lCurrentTradeDay) {
  char buffer[20];
  CString strDay;
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;
  long iCount = 0;

  CString str;
  str = _T("开始处理");
  str += GetDayString(lCurrentTradeDay);
  str += _T("日的实时数据");
  gl_systemMessage.PushInformationMessage(str);

  // 存储当前交易日的数据
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayLine.m_strFilter = _T("[Day] =");
  setDayLine.m_strFilter += strDay;
  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  while (!setDayLine.IsEOF()) {
    setDayLine.Delete();
    setDayLine.MoveNext();
  }
  setDayLine.m_pDatabase->CommitTrans();

  setDayLine.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    iCount++;
    pStock->SetDayLineEndDay(lCurrentTradeDay);
    pStock->SetIPOStatus(__STOCK_IPOED__); // 再设置一次。防止新股股票代码由于没有历史数据而被误判为不存在。
    setDayLine.AddNew();
    pStock->SaveBasicInfo(setDayLine);
    setDayLine.Update();
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // 存储今日生成的数据于DayLineInfo表中。
  setDayLineInfo.m_strFilter = _T("[Day] =");
  setDayLineInfo.m_strFilter += strDay;
  setDayLineInfo.Open();
  setDayLineInfo.m_pDatabase->BeginTrans();
  while (!setDayLineInfo.IsEOF()) {
    setDayLineInfo.Delete();
    setDayLineInfo.MoveNext();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();

  setDayLineInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    setDayLineInfo.AddNew();
    pStock->SaveCalculatedInfo(setDayLineInfo);
    setDayLineInfo.Update();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();
  setDayLineInfo.Close();

  str = GetDayString(lCurrentTradeDay);
  str += _T("日实时数据处理完毕");
  gl_systemMessage.PushInformationMessage(str);
  return iCount;
}

//////////////////////////////////////////////////////////////////////////////////
//
// 将当日处理好的数据储存于数据库中，以备万一系统崩溃时重新装入。
// 似乎应该以一个定时工作线程的形式存在。
//
// 研究之。
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateTodayTempDB(void) {
  CSetDayLineToday setDayLineToday;
  CString str;
  // 存储今日生成的数据于DayLineToday表中。
  setDayLineToday.Open();
  setDayLineToday.m_pDatabase->BeginTrans();
  while (!setDayLineToday.IsEOF()) {
    setDayLineToday.Delete();
    setDayLineToday.MoveNext();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
      str = pStock->GetStockCode();
      str += _T(" 股数不正确");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
    setDayLineToday.AddNew();
    pStock->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.Close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入暂存的今日当前数据状态，重置分析的初始态。这样当在开市时系统退出时，不至于损失掉所有已分析的数据
//
// 在设置m_lUnknownVolume为记录集中的m_UnknownVolume - m_Volume，这是因为第一次计算时只是初始化系统。
// 需要设置m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
// 而第一次执行计算实时数据时，只是初始化系统环境，其中设置m_lUnknownVolume += pRTData->GetVolume
// 故而此处这样计算。
/////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::LoadTodayTempDB(void) {
  CChinaStockPtr pStock = nullptr;
  CSetDayLineToday setDayLineToday;
  CRTDataPtr pRTData;

  ASSERT(!m_fTodayTempDataLoaded);
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // 执行此初始化工作时，计算实时数据的工作线程必须没有运行。
  // 读取今日生成的数据于DayLineToday表中。
  setDayLineToday.Open();
  if (!setDayLineToday.IsEOF()) {
    if (setDayLineToday.m_Day == GetDay()) { // 如果是当天的行情，则载入，否则放弃
      while (!setDayLineToday.IsEOF()) {
        if ((pStock = GetStock(setDayLineToday.m_StockCode)) != nullptr) {
          ASSERT(!pStock->HaveFirstRTData()); // 确保没有开始计算实时数据
          pStock->LoadTempInfo(setDayLineToday);
        }
        setDayLineToday.MoveNext();
      }
    }
  }
  setDayLineToday.Close();

  return true;
}

bool CChinaMarket::CalculateRelativeStrong(long lStartCalculatingDay) {
  AfxBeginThread(ThreadCalculateDayLineRS, (LPVOID)lStartCalculatingDay);
  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// 计算lDay的日线相对强度, lDay的格式为：YYYYMMDD,如 19990605.
// 将日线按涨跌排列后,其相对强弱即其在队列中的位置
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CalculateOneDayRelativeStrong(long lDay) {
  vector<CChinaStockPtr> vStock;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0;
  CTime ctTime;
  CSetDayLine setDayLine;
  const long lYear = lDay / 10000;
  const long lMonth = lDay / 100 - lYear * 100;
  const long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];

  sprintf_s(pch, _T("%08d"), lDay);
  strDay = pch;
  setDayLine.m_strSort = _T("[UpDownRate]");
  setDayLine.m_strFilter = _T("[Day] =");
  setDayLine.m_strFilter += strDay;
  setDayLine.Open();
  if (setDayLine.IsEOF()) { // 数据集为空，表明此日没有交易
    setDayLine.Close();
    CString str = strDay;
    str += _T("日数据集为空，没有计算相对强度");
    gl_systemMessage.PushDayLineInfoMessage(str);    // 采用同步机制报告信息
    return false;
  }
  setDayLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayLine.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayLine.m_StockCode);
      vStock.push_back(m_vChinaMarketAStock.at(lIndex));
      vIndex.push_back(iStockNumber); // 将A股的索引记录在容器中。
      iTotalAShare++;
    }
    iStockNumber++;
    setDayLine.MoveNext();
  }

  setDayLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // 只计算活跃股票的相对强度
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // 根据索引去更改数据库,跨过不是A股的股票
      setDayLine.MoveNext();
    }
    setDayLine.Edit();
    double dLastClose = atof(setDayLine.m_LastClose);
    double dLow = atof(setDayLine.m_Low);
    double dHigh = atof(setDayLine.m_High);
    double dClose = atof(setDayLine.m_Close);
    if (((dLow / dLastClose) < 0.88)
        || ((dHigh / dLastClose) > 1.12)) { // 除权、新股上市等
      setDayLine.m_RelativeStrong = ConvertValueToString(50); // 新股上市或者除权除息，不计算此股
    }
    else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) { // 涨停板
      setDayLine.m_RelativeStrong = ConvertValueToString(100);
    }
    else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) { // 跌停板
      setDayLine.m_RelativeStrong = ConvertValueToString(0);
    }
    else {
      setDayLine.m_RelativeStrong = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
    }
    setDayLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayLine.MoveNext(); // 移到下一个数据。
    iBefore++; // 计数器也同时加一。
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  vStock.clear();
  vIndex.clear();
  vRelativeStrong.clear();

  sprintf_s(buffer, "%4d年%2d月%2d日的股票相对强度计算完成", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  TRACE("处理今日相对强度\n");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // 采用同步机制报告信息

  return(true);
}

bool CChinaMarket::UpdateStockCodeDB(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->AppendStockCodeDB(setStockCode);
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

void CChinaMarket::LoadStockCodeDB(void) {
  CSetStockCode setStockCode;
  char buffer[30];
  CString str;

  setStockCode.Open();
  // 装入股票代码数据库
  while (!setStockCode.IsEOF()) {
    CChinaStockPtr pStock = GetStock(setStockCode.m_StockCode);
    pStock->LoadStockCodeDB(setStockCode);
    setStockCode.MoveNext();
  }
  if (gl_ChinaStockMarket.m_iDayLineNeedUpdate > 0) {
    int i = gl_ChinaStockMarket.m_iDayLineNeedUpdate;
    if (GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage(_T("每星期一复查退市股票日线"));
    TRACE("尚余%d个股票需要检查日线数据\n", i);
    _itoa_s(i, buffer, 10);
    str = buffer;
    str += _T("个股票需要检查日线数据");
    gl_systemMessage.PushInformationMessage(str);
  }
  setStockCode.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// 更新选项数据库
//
//
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = GetDay();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = GetDay();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return true;
}

void CChinaMarket::LoadOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_ChinaStockMarket.SetRelativeStrongStartDay(__CHINA_MARKET_BEGIN_DAY__);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(__CHINA_MARKET_BEGIN_DAY__);
    gl_ChinaStockMarket.SetLastLoginDay(__CHINA_MARKET_BEGIN_DAY__);
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(setOption.m_RelativeStrongEndDay);
      if (gl_ChinaStockMarket.GetRelativeStrongEndDay() > __CHINA_MARKET_BEGIN_DAY__) {
        // 当日线历史数据库中存在旧数据时，采用单线程模式存储新数据。使用多线程模式时，MySQL会出现互斥区Exception，估计是数据库重入时发生同步问题）。
        // 故而修补数据时同时只运行一个存储线程，其他都处于休眠状态。此种问题不会出现于生成所有日线数据时，故而新建日线数据时可以使用多线程（目前为4个）。
        gl_SaveOneStockDayLine.SetMaxCount(1);
        gl_cMaxSavingOneDayLineThreads = 2;
      }
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(setOption.m_RalativeStrongStartDay);
    }
    if (setOption.m_LastLoginDay == 0) {
      gl_ChinaStockMarket.SetLastLoginDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      gl_ChinaStockMarket.SetLastLoginDay(setOption.m_LastLoginDay);
    }
  }

  setOption.Close();
}

bool CChinaMarket::UpdateTempRTData(void) {
  if (!gl_ThreadStatus.IsSavingTempData()) {
    gl_ThreadStatus.SetSavingTempData(true);
    AfxBeginThread(ThreadSaveTempRTData, nullptr);
  }

  return true;
}