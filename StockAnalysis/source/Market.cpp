//////////////////////////////////////////////////////////////////////////////////////////////////

//#include"stdafx.h"

#include"globedef.h"
#include"Thread.h"

#include"Market.h"

#include"SetDayLineInfo.h"
#include"SetDayLineToday.h"
#include"SetOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket(void) : CObject() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("市场变量只允许存在一个实例\n");
    ASSERT(0);
  }

  m_fPermitResetSystem = true; // 允许系统被重置标识，唯独此标识不允许系统重置。初始时设置为真：允许重置系统。

  Reset();
}

CMarket::~CMarket() {
}

void CMarket::Reset(void)
{
  m_mapActiveStockToIndex.clear();
  m_vActiveStock.clear();
  m_lTotalActiveStock = 0; // 初始时股票池中的股票数量为零

  m_fLoadedSelectedStock = false;
  m_fSystemReady = false;    // 市场初始状态为未设置好。
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_pCurrentStock = nullptr;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  time_t ttime;
  time(&ttime);
  tm tm_;
  localtime_s(&tm_, &ttime);
  if (tm_.tm_hour >= 15) { // 中国股票市场已经闭市
    m_fTodayStockCompiled = true; // 闭市后才执行本系统，则认为已经处理过今日股票数据了。
  }
  else m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = 19900101;

  m_lCountLoopRTDataInquiring = 0;

  m_fResetm_ItStock = true;

  m_fTodayTempDataLoaded = false;

  m_fCheckTodayActiveStock = true;  //检查当日活跃股票，必须为真。

  m_fUpdatedStockCodeDataBase = false;

  m_fCalculatingRS = false;

  m_fGetRTStockData = true;
  m_fReadingTengxunRTData = false; // 默认状态下不读取腾讯实时行情
  m_iCountDownDayLine = 3;    // 400ms延时（100ms每次）
  m_iCountDownSlowReadingRTData = 3; // 400毫秒每次

  // 生成股票代码池
  CreateTotalStockContainer();
}

#ifdef _DEBUG
void CMarket::AssertValid() const
{
  CObject::AssertValid();
}

void CMarket::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CMarket的初始函数调用一次。
// 这个函数需要其他全局变量初始化的支持，故而gl_ChinaStockMarket实例需要放在所有全局变量的最后。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateTotalStockContainer(void)
{
  char buffer[10]{};

  StockIDPtr pStockID = nullptr;
  int iCount = 0;

  // 清空之前的数据（如果有的话。在Reset时，这两个容器中就存有数据）。
  m_vChinaMarketAStock.clear();
  m_mapChinaMarketAStock.clear();

  // 生成上海股票代码
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // 上海主板
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // 使用下标生成新的映射
  }

  // 生成三版股票代码
  for (int i = 603000; i < 604000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // 上海三板
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // 使用下标生成新的映射
  }

  // 生成科创版股票代码
  for (int i = 688000; i < 689000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // 上海科创板
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // 使用下标生成新的映射
  }

  // 生成B股股票代码
  for (int i = 900000; i < 901000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // 上海B股
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // 使用下标生成新的映射
  }

  // 生成上海指数代码
  for (int i = 0; i < 1000; i++) {
    CString str = _T("sh");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // 上海指数
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // 使用下标生成新的映射
  }

  ///////////////////////////////////////////////
  // 生成深圳主板股票代码
  for (int i = 0; i < 2000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // 深圳主板
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// 使用下标生成新的映射
  }

  // 生成深圳中小板股票代码
  for (int i = 2000; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // 深圳中小板
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// 使用下标生成新的映射
  }

  // 生成B股股票代码
  for (int i = 200000; i < 201000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // 深圳B股
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// 使用下标生成新的映射
  }

  // 生成创业板股票代码
  for (int i = 300000; i < 301000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // 深圳创业板
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// 使用下标生成新的映射
  }

  // 生成深圳指数
  for (int i = 399000; i < 400000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // 深圳指数
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// 使用下标生成新的映射
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
// 新浪实时制式
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CMarket::CreateSinaRTDataInquiringStr(CString& str) {
  static bool fCreateStr = false;
  static int siCounter = 0;

  const long siTotalStock = m_vChinaMarketAStock.size();

  str = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
    return true; // 查询到头了
  }

  for (int i = 1; i < 900; i++) {  // 每次轮询900个股票.
    if (siCounter == siTotalStock) {
      siCounter = 0;
      return true; // 查询到头了
    }
    str += _T(",");
    str += m_vChinaMarketAStock.at(siCounter)->GetStockCode();
    siCounter++;
  }
  siCounter -= 2; // 后退两格。为了防止边缘数据错误，故边缘数据查询两遍(现在这个没必要了，实时数据基本没出错过）。

  return false; // 尚未遍历所有股票
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
// 腾讯实时制式
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateTengxunRTDataInquiringStr(CString& str) {
  static bool fCreateStr = false;
  static int siCounter = 0;

  const long siTotalStock = m_vChinaMarketAStock.size();

  str = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
    return true; // 查询到头了
  }

  for (int i = 1; i < 900; i++) {  // 每次轮询900个股票.
    if (siCounter == siTotalStock) {
      siCounter = 0;
      return true; // 查询到头了
    }
    str += _T(",");
    str += m_vChinaMarketAStock.at(siCounter)->GetStockCode();
    siCounter++;
  }
  siCounter -= 2; // 后退两格。为了防止边缘数据错误，故边缘数据查询两遍(现在这个没必要了，实时数据基本没出错过）。

  return false; // 尚未遍历所有股票
}

bool CMarket::GetSinaStockRTData(void)
{
  static int iCountUp = 0;
  char* pCurrentPos = nullptr;
  CStockRTDataPtr pRTData = nullptr;
  long i = 0;
  INT64 iTotalNumber = 0;

  if (!gl_ThreadStatus.IsSinaRTDataReadingInProcess()) {
    if (gl_ThreadStatus.IsSinaRTDataReceived()) {
      if (gl_stSinaRTDataInquire.fError == false) { //网络通信一切顺利？
        iTotalNumber = gl_stSinaRTDataInquire.lByteRead;
        pCurrentPos = gl_stSinaRTDataInquire.buffer;
        long  iCount = 0;
        while (iCount < iTotalNumber) { // 新浪实时数据基本没有错误，不需要抛掉最后一组数据了。
          pRTData = make_shared<CStockRTData>();
          if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
            i++;
            gl_systemDequeData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
          }
          else {
            TRACE("新浪实时数据有误,抛掉不用\n");
            CString str;
            str = _T("新浪实时数据有误");
            gl_systemMessage.PushInformationMessage(str);
            iCount = iTotalNumber; // 后面的数据可能出问题，抛掉不用。
          }
        }
        TRACE("读入%d个新浪实时数据\n", i);
        // 处理接收到的实时数据
        ProcessRTData();
      }
      else {  // 网络通信出现错误
        TRACE("Error reading http file ：hq.sinajs.cn\n");
        CString str;
        str = _T("Error reading http file ：hq.sinajs.cn");
        gl_systemMessage.PushInformationMessage(str);
      }
    }

    CString strTemp = _T("");

    // 申请下一批次新浪股票实时数据
    if (m_fCheckTodayActiveStock || !SystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTStockSource; // 设置查询新浪实时数据的字符串头
      if (CreateSinaRTDataInquiringStr(strTemp)) {
        if (++m_lCountLoopRTDataInquiring >= 3) {  // 遍历三遍全体股票池
          if (!SystemReady()) { // 如果系统尚未设置好，则显示系统准备
            gl_systemMessage.PushInformationMessage(_T("完成系统初始化"));
          }
          SetSystemReady(true); // 所有的股票实时数据都轮询一遍，当日活跃股票集已经建立，故而可以接受日线数据了。
        }
      }
      gl_stSinaRTDataInquire.strInquire += strTemp;
    }
    else { // 开市时使用今日活跃股票池
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTStockSource;
      GetInquiringStockStr(gl_stSinaRTDataInquire.strInquire);
    }
    gl_ThreadStatus.SetSinaRTDataReceived(false);
    gl_ThreadStatus.SetSinaRTDataReadingInProcess(true);  // 在此先设置一次，以防重入（线程延迟导致）
    AfxBeginThread(ThreadReadingSinaRTDataProc, nullptr);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时行情读取函数。只有当系统准备完毕后，方可执行此函数。
//
// 目前腾讯实时行情数据中的成交股数精度为每手，无法达到新浪实时数据的每股精度，故而只使用其他合适的数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetTengxunStockRTData(void)
{
  static int iCountUp = 0;
  char* pCurrentPos = nullptr;
  CStockRTDataPtr pRTData = nullptr;
  long i = 0;
  INT64 iTotalNumber = 0;

  if (!gl_ThreadStatus.IsTengxunRTDataReadingInProcess()) {
    if (gl_ThreadStatus.IsTengxunRTDataReceived()) {
      if (gl_stTengxunRTDataInquire.fError == false) { //网络通信一切顺利？
        iTotalNumber = gl_stTengxunRTDataInquire.lByteRead;
        pCurrentPos = gl_stTengxunRTDataInquire.buffer;
        long  iCount = 0;
        /*
        while (iCount < iTotalNumber) { // 腾讯实时数据基本没有错误，不需要抛掉最后一组数据了。
          pRTData = make_shared<CStockRTData>();
          if (pRTData->ReadTengxunData(pCurrentPos, iCount)) {
            i++;
            gl_systemDequeData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
          }
          else {
            TRACE("腾讯实时数据有误,抛掉不用\n");
            CString str;
            str = _T("腾讯实时数据有误");
            gl_systemMessage.PushInformationMessage(str);
            iCount = iTotalNumber; // 后面的数据可能出问题，抛掉不用。
          }
        }
        */
        TRACE("读入%i字节腾讯实时数据\n", iTotalNumber);
        // 处理接收到的实时数据
        //ProcessRTData();
      }
      else {  // 网络通信出现错误
        TRACE("Error reading http file ：qt.gtimg.cn\n");
        CString str;
        str = _T("腾讯实时数据通信错误");
        gl_systemMessage.PushInformationMessage(str);
      }
    }

    CString strTemp = _T("");

    // 申请下一批腾讯股票实时数据
    gl_stTengxunRTDataInquire.strInquire = m_strTengxunRTStockSource;
    GetInquiringStockStr(gl_stTengxunRTDataInquire.strInquire);
    gl_ThreadStatus.SetTengxunRTDataReceived(false);
    gl_ThreadStatus.SetTengxunRTDataReadingInProcess(true);  // 在此先设置一次，以防重入（线程延迟导致）
    AfxBeginThread(ThreadReadingTengxunRTDataProc, nullptr);
  }

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
bool CMarket::CreateNeteaseDayLineInquiringStr(CString& str, CString& strStartDay) {
  static int iStarted = 0;
  static int siCounter = 0;

  long lTotalStock;

  StockIDPtr pStockID;

  lTotalStock = m_vChinaMarketAStock.size();

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount < 1000)) {
    if (!m_vChinaMarketAStock.at(siCounter)->IsDayLineNeedUpdate()) { // 日线数据不需要更新。在系统初始时，设置此m_fDayLineNeedUpdate标识
      // TRACE("%S 日线数据无需更新\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetIPOStatus() == __STOCK_NULL__) {	// 尚未使用过的股票代码无需查询日线数据
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
      // TRACE("无效股票代码：%S, 无需查询日线数据\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      iCount++;
      siCounter++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetDayLineEndDay() >= gl_systemTime.GetLastTradeDay()) { // 上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
      // TRACE("%S 日线数据本日已更新\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else {
      fFound = true;
    }
  }

  if (iCount >= 1000) { //  没有找到需要申请日线的股票
    TRACE("未找到需更新日线历史数据的股票\n");
    return false;
  }

  // 找到了需申请日线历史数据的股票（siCounter为索引）
  StockIDPtr pID = m_vChinaMarketAStock.at(siCounter);
  switch (pID->GetMarket()) { // 转换成网易日线数据申请制式（上海为‘0’，深圳为‘1’）
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
  case __SHENZHEN_3BAN__: // 山镇中小板
  case __SHENZHEN_B_SHARE__: // 深圳B股
  case __SHENZHEN_CHUANGYE__: // 深圳创业板
  case __SHENZHEN_MAIN__: // 深圳主板
    str += '1'; // 深圳市场标识
    break;
  default: // 越界
    ASSERT(0);
  }
  char buffer[30];
  SetDownLoadingStockCodeStr(pID->GetStockCode());
  str += pID->GetStockCode().Right(6); // 取股票代码的右边六位数字。
  tm tm_;
  tm_.tm_year = pID->GetDayLineEndDay() / 10000 - 1900;
  tm_.tm_mon = pID->GetDayLineEndDay() / 100 - (tm_.tm_year + 1900) * 100 - 1;
  tm_.tm_mday = pID->GetDayLineEndDay() - (tm_.tm_year + 1900) * 10000 - (tm_.tm_mon + 1) * 100;
  tm_.tm_hour = 12;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  time_t tTime;
  tTime = mktime(&tm_);
  tTime += 3600 * 24; // 增加一天。
  localtime_s(&tm_, &tTime);
  const long lDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday; // 日线的起始日期是最新日期的后一天。
  sprintf_s(buffer, "%8d", lDay);
  strStartDay = buffer;
  siCounter++;
  if (siCounter == lTotalStock) {
    siCounter = 0;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 采用网易日线申请接口，读取日线数据。
// 每次读取一个股票。
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetNetEaseStockDayLineData(void)
{
  CString strRead;
  static bool sfFoundStock = true;
  CDayLinePtr pDayLine = nullptr;

  if (!gl_ThreadStatus.IsNeteaseDayLineReadingInProcess()) {
    if (sfFoundStock) {
      if ((gl_stDayLineInquire.fError == false) && gl_ThreadStatus.IsDayLineDataReady()) { //网络通信一切顺利？
        TRACE("股票%s日线数据为%d字节\n", GetDownLoadingStockCodeStr(), gl_stDayLineInquire.lByteRead);
        ASSERT(gl_stDayLineInquire.lByteRead < 2048 * 1024);
        // 处理当前股票日线数据
        ProcessDayLineData(gl_stDayLineInquire.buffer, gl_stDayLineInquire.lByteRead);
      }
      else {
        if (gl_stDayLineInquire.lByteRead > 0) {
          TRACE("Error reading http file ：quotes.money.163.com/service/\n");
          CString str;
          str = _T("Error reading http file ：quotes.money.163.com/service/");
          gl_systemMessage.PushInformationMessage(str);
          gl_stDayLineInquire.lByteRead = 0;
        }
      }
    }

    CString strTemp;
    char buffer2[200];
    CString strStartDay;

    // 准备网易日线数据申请格式
    strRead = m_strNeteaseDayLineStockSource;
    sfFoundStock = CreateNeteaseDayLineInquiringStr(strRead, strStartDay);
    if (sfFoundStock) {
      strRead += _T("&start=");
      strRead += strStartDay;
      strRead += _T("&end=");
      sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
      strRead += buffer2;
      strRead += m_strNeteaseDayLinePostfix;

      gl_stDayLineInquire.strInquire = strRead;
      gl_ThreadStatus.SetDayLineDataReady(false);
      gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(true); // 这里多设置一次(线程内也设置），以防线程由于唤醒延迟导致再次进入（线程退出时会清除此标识）
      // 这个线程的启动可以采用唤醒模式而不是这样直接调用
      AfxBeginThread(ThreadReadingNeteaseDayLineProc, nullptr);
      return true;
    }
    else return false;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////
//
//	得到分时线偏移量。09:30为0，15:00为240,步长为1分钟
//
//
////////////////////////////////////////////////////////////////////////
long CMarket::GetMinLineOffset(CStockID sID, time_t Time) {
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
bool CMarket::IsAStock(CStockPtr pStock) {
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
bool CMarket::IsAStock(CString strStockCode) {
  if ((strStockCode[0] == 's') && (strStockCode[1] == 'h') && (strStockCode[2] == '6') && (strStockCode[3] == '0')) {
    if ((strStockCode[4] == '0') || (strStockCode[4] == '1')) {
      return true;
    }
  }
  else {
    if ((strStockCode[0] == 's') && (strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
      if ((strStockCode[4] == '0') || (strStockCode[4] == '2')) {
        return true;
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
bool CMarket::IsStock(CString strStockCode, CStockPtr& pStock) {
  if ((pStock = GetStockPtr(strStockCode)) != nullptr) {
    return(true);
  }
  else {
    pStock = NULL;
    return(false);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackBuyAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vActiveStock) {
    if (IsAStock(pStock)) {
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
INT64 CMarket::GetTotalAttackSellAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vActiveStock) {
    if (IsAStock(pStock)) {
      lAmount += pStock->GetAttackSellAmount();
    }
  }
  return(lAmount);
}

void CMarket::SetReadingSinaRTDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingSinaRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingSinaRTDataTime = tt;
    singleLock.Unlock();
  }
}

clock_t CMarket::GetReadingSinaRTDataTime(void)
{
  CSingleLock singleLock(&m_ReadingSinaRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingSinaRTDataTime;
    singleLock.Unlock();
    return tt;
  }
}

void CMarket::SetReadingTengxunRTDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingTengxunRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingTengxunRTDataTime = tt;
    singleLock.Unlock();
  }

}

clock_t CMarket::GetReadingTengxunRTDataTime(void)
{
  CSingleLock singleLock(&m_ReadingTengxunRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingTengxunRTDataTime;
    singleLock.Unlock();
    return tt;
  }
}

void CMarket::SetReadingNeteaseDayDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingNeteaseDayDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingNeteaseDayDataTime = tt;
    singleLock.Unlock();
  }
}

clock_t CMarket::GetReadingNeteaseDayDataTime(void)
{
  CSingleLock singleLock(&m_ReadingNeteaseDayDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingNeteaseDayDataTime;
    singleLock.Unlock();
    return tt;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，由SchedulingTask函数调用。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。如果遇到新的股票代码则生成相应的新股票。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数用到大量的全局变量，还是放在主线程为好。工作线程目前还是只做计算个股票的挂单情况。
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessRTData(void)
{
  // 处理读入的实时数据，生成当日的活跃股票市场
  CStockPtr pStock;
  CStockRTDataPtr pRTDataCompact = nullptr;
  const long lTotalNumber = gl_systemDequeData.GetRTDataDequeSize();

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CStockRTDataPtr pRTData = gl_systemDequeData.PopRTData();
    //ASSERT(pRTData->IsActive());  // 此数据应该是永远有效的。
    if (pRTData->IsActive()) { // 此实时数据有效？
      long lIndex = 0;
      if (m_mapActiveStockToIndex.find(pRTData->GetStockCode()) == m_mapActiveStockToIndex.end()) { // 新的股票代码？
        pStock = make_shared<CStock>();
        pStock->SetActive(true);
        pStock->SetMarket(pRTData->GetMarket());
        pStock->SetStockCode(pRTData->GetStockCode());
        pStock->SetStockName(pRTData->GetStockName());
        pStock->SetCode(pRTData->GetCode());
        pStock->SetLastClose(pRTData->GetLastClose());
        pStock->SetOpen(pRTData->GetOpen());
        AddStockToMarket(pStock); // 添加此股入容器，其索引就是目前的m_lTotalActiveStaock的值。
        ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
        pStock->PushRTData(pRTData);
        lIndex = m_mapChinaMarketAStock[pStock->GetStockCode()];
        m_vChinaMarketAStock.at(lIndex)->SetStockName(pStock->GetStockName());
        m_vChinaMarketAStock.at(lIndex)->SetActive(true); // 本日接收到了数据，
        long lIndex2 = m_mapActiveStockToIndex.at(pRTData->GetStockCode());
        m_vActiveStock.at(lIndex2)->SetTransactionTime(pRTData->GetTransactionTime());  // 设置最新接受到实时数据的时间
        m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(true);
        // 如果此股票代码尚未使用过，则设置为已使用。
        // 停牌后的股票，也能接收到实时数据，只是其内容只有收盘价，其他都为零。考虑清除这种无效数据。
        m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__);
        ASSERT(m_vChinaMarketAStock.at(lIndex)->GetStockCode().Compare(pStock->GetStockCode()) == 0);
      }
      else {
        lIndex = m_mapActiveStockToIndex.at(pRTData->GetStockCode());
        ASSERT(lIndex <= m_lTotalActiveStock);
        if (pRTData->GetTransactionTime() > m_vActiveStock.at(lIndex)->GetTransactionTime()) { // 新的数据？
          m_vActiveStock.at(lIndex)->PushRTData(pRTData); // 存储新的数据至数据池
          m_vActiveStock.at(lIndex)->SetTransactionTime(pRTData->GetTransactionTime());   // 设置最新接受到实时数据的时间
        }
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // 设置接收到实时数据标识

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询实时股票数据的字符串
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetInquiringStockStr(CString& str)
{
  int iCount = 1;

  if (m_fResetm_ItStock) {
    m_itStock = m_vActiveStock.begin();
    m_fResetm_ItStock = false;
  }

  str += (*m_itStock++)->GetStockCode();  // 得到第一个股票代码
  while ((m_itStock != m_vActiveStock.end()) && (iCount < 900)) { // 每次最大查询量为900个股票
    iCount++;
    str += ',';
    str += (*m_itStock++)->GetStockCode();
  }

  if (m_itStock == m_vActiveStock.end()) {
    m_itStock = m_vActiveStock.begin();
  }
  else {
    // 退一格，这样能够覆盖住边缘
    m_itStock--;
  }
  return iCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 由工作线程ThreadCalculatingRTDataProc调用，注意全局变量的使用
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateRTData(void)
{
  for (auto pStock : m_vActiveStock) {
    ASSERT(pStock != nullptr);
    pStock->CalculateRTData();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessDayLineData(char* buffer, long lLength) {
  long iCount = 0;
  char* pCurrentPos = buffer;
  char* pTestPos = buffer;
  vector<CDayLinePtr> vTempDayLine;

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
  long lIndex = -1;

  pTestPos = buffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  if (iCount == lLength) {// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天
    lIndex = m_mapChinaMarketAStock.at(m_strCurrentStockDownLoading);
    // ASSERT(!m_vChinaMarketAStock[lIndex]->m_fActive); 当一个股票IPO后但尚未上市时，股票代码存在但没有日线数据。取消此断言判断。
    // 有些股票在上市后出现被收购或其他情况，导致日线数据不再更新。此种情况不能设置此股票为无效代码
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() == 19900101) { // 如果初始日线结束日期从来没有变更过，则此股票代码尚未被使用过
      m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_NULL__);   // 此股票代码尚未使用。
      //TRACE("无效股票代码：%s\n", static_cast<LPCWSTR>(m_strCurrentStockDownLoading));
    }
    else { // 已经退市的股票
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() + 100 < gl_systemTime.GetDay()) {
        m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_DELISTED__);   // 此股票代码已经退市。
      }
      //TRACE("%S 没有可更新的日线数据\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false); // 都不需要更新日线数据
    return false;
  }

  lIndex = 0;
  long iTemp = 0;
  CString strTemp;
  pTestPos = buffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  while (iCount < lLength) {
    pDayLine = make_shared<CDayLine>();
    if (!ProcessOneItemDayLineData(pDayLine, pCurrentPos, iTemp)) { // 处理一条日线数据
      TRACE("%s 日线数据出错\n", pDayLine->GetStockCode());
      // 清除已暂存的日线数据
      vTempDayLine.clear();
      return false; // 数据出错，放弃载入
    }
    iCount += iTemp;
    pTestPos = buffer;
    pTestPos += iCount;
    ASSERT(*pTestPos == *pCurrentPos);
    if (m_mapActiveStockToIndex.find(pDayLine->GetStockCode()) == m_mapActiveStockToIndex.end()) { // 新的股票代码？
      // 生成新股票
      auto pStock = make_shared<CStock>();
      pStock->SetActive(true);
      pStock->SetDayLineLoaded(false);
      pStock->SetMarket(pDayLine->GetMarket());
      pStock->SetStockCode(pDayLine->GetStockCode());
      pStock->SetStockName(pDayLine->GetStockName());
      strTemp = pStock->GetStockCode().Right(6); // 截取股票代码右边的六个数字
      pStock->SetCode(atoi(strTemp.GetBuffer()));
      AddStockToMarket(pStock);
      lIndex = m_lTotalActiveStock - 1;
      ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
      // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
      long lIndexTemp = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      m_vChinaMarketAStock.at(lIndexTemp)->SetStockCode(pStock->GetStockCode());
      m_vChinaMarketAStock.at(lIndexTemp)->SetStockName(pStock->GetStockName());
    }
    else lIndex = m_mapActiveStockToIndex.at(pDayLine->GetStockCode());
    //if ((pDayLine->m_lClose != 0) && (pDayLine->m_lOpen != 0)) { // 如果数据有效，则存储此日线数据（退市后的日线数据是无效的）。
    vTempDayLine.push_back(pDayLine); // 暂存于临时vector中，因为网易日线数据的时间顺序是颠倒的，最新的在最前面
  /*}
  else {
    int i111 = 0;
  }*/
  }
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("日线下载完成.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetDayLineNeedUpdate(false); // 日线数据下载完毕，不需要申请新数据了。
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_systemTime.GetDay()) { // 提取到的股票日线数据其最新日不是上个月的这个交易日（退市了或相似情况），给一个月的时间观察。
    m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_DELISTED__); // 已退市或暂停交易。
  }
  else {
    m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_IPOED__); // 正常交易股票
  }
  ASSERT(lIndex >= 0);
  m_vActiveStock.at(lIndex)->SetDayLineLoaded(true);
  m_vActiveStock.at(lIndex)->SetDayLineNeedSavingFlag(true); // 设置存储日线标识
  m_vActiveStock.at(lIndex)->m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->GetDay() < gl_systemTime.GetDay()) { // 不要存储今日日线数据（今日日线数据由实时数据生成）.
      // 当新股第一天上市时，其日线只有一天，而且在这里扔掉了，导致其日线容器为空。处理时注意。
      // 由于是调取gl_lLastTradeDay及之前的日线数据，故而新股的日线容器肯定为空。
      m_vActiveStock.at(lIndex)->m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取一条日线数据。采用网易日线历史数据格式。
//
// 与实时数据相类似，各种价格皆放大一千倍后以长整型存储。存入数据库时以DECIMAL(10,3)类型存储。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessOneItemDayLineData(CDayLinePtr pDayLine, char*& pCurrentPos, long& lLength) {
  long iCount = 0;
  static char buffer2[200], buffer3[100];
  long i = 0;
  tm tm_;
  int year = 0, month = 0, day = 0;
  long lDay = 0;
  CString str;
  i = 0;
  while (*pCurrentPos != 0x2c) {
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false; // 数据出错，放弃载入
    }
    buffer3[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer3[i] = 0x00;
  sscanf_s(buffer3, "%04d-%02d-%02d", &year, &month, &day);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = 15;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  tm_.tm_isdst = 0;
  pDayLine->SetTime(mktime(&tm_));
  lDay = year * 10000 + month * 100 + day;
  pDayLine->SetDay(lDay);
  //TRACE("%d %d %d\n", year, month, day);

  if (*pCurrentPos != 0x27) return(false); // 数据出错，放弃载入
  pCurrentPos++;
  iCount++;

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockCode(m_strCurrentStockDownLoading);
  str = m_strCurrentStockDownLoading.Left(2);
  if (str == _T("sh")) {
    pDayLine->SetMarket(__SHANGHAI_MARKET__);
  }
  else if (str == _T("sz")) {
    pDayLine->SetMarket(__SHENZHEN_MARKET__);
  }
  else {
    ASSERT(0); // 股票代码制式出错
    return false;
  }
  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockName(str);

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetClose(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetHigh(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLow(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetOpen(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLastClose(atol(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  if (pDayLine->GetOpen() == 0) {
    //ASSERT(strcmp(buffer2, "None") == 0);
    pDayLine->SetUpDown(0);
  }
  else pDayLine->SetUpDown(atof(buffer2));

  if (pDayLine->GetLastClose() == 0) { // 设置涨跌幅。
    pDayLine->SetUpDownRate(0); // 如果昨日收盘价为零（没交易），则涨跌幅也设为零。
  }
  else {
    // 需要放大1000 * 100倍。收盘价比实际值大1000倍，记录的是百分比，也要增大100倍。
    pDayLine->SetUpDownRate(((double)(pDayLine->GetUpDown() * 100000.0)) / pDayLine->GetLastClose());
  }

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetChangeHandRate(atof(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetVolume(atoll(buffer2)); // 读入的是股数

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetAmount(atoll(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetTotalValue(atoll(buffer2)); // 总市值的单位为：元

  i = 0;
  while (*pCurrentPos != 0x0d) {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // 数据出错，放弃载入
    buffer2[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer2[i] = 0x000;
  pDayLine->SetCurrentValue(atoll(buffer2)); // 流通市值的单位为：元。
  if (*pCurrentPos++ != 0x0a) return false; // 数据出错，放弃载入
  iCount++;

  lLength = iCount;

  return true;
}

bool CMarket::ReadOneValue(char*& pCurrentPos, char* buffer, long& iReadNumber)
{
  int i = 0;

  while (*pCurrentPos != 0x2c) { // 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false; // 数据出错，放弃载入
    }
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  iReadNumber += i;
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入浮点数，小数点后保留三位，不足就加上0.，多于三位就抛弃。读入的数值放大一千倍。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ReadOneValueExceptperiod(char*& pCurrentPos, char* buffer, long& lCounter)
{
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  while ((*pCurrentPos != ',') && (iCount < 3)) {
    if (fFoundPoint) iCount++;
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    if (*pCurrentPos == '.') {
      fFoundPoint = true;
      pCurrentPos++;
    }
    else buffer[i++] = *pCurrentPos++;
  }

  if (fFoundPoint && (iCount < 3)) {
    int jCount = i;
    for (int j = iCount; j < 3; j++) {
      buffer[jCount++] = '0';
    }
    buffer[jCount] = 0x000;
  }
  else {
    buffer[i] = 0x000;
  }

  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    i++;
    pCurrentPos++;
  }
  pCurrentPos++;
  i++;
  if (fFoundPoint) i++;
  lCounter += i; // 多加1，是需要加上少算的逗号

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
// 大约每100毫秒调度一次
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SchedulingTask(void)
{
  static time_t s_timeLast = 0;

  gl_systemTime.CalculateTime();      // 计算系统各种时间

  //根据时间，调度各项定时任务.每秒调度一次
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }

  // 抓取实时数据(新浪和/或腾讯）
  if (!gl_fExiting && m_fGetRTStockData && (m_iCountDownSlowReadingRTData <= 0)) {
    GetSinaStockRTData(); // 每400毫秒(100X4)申请一次实时数据。新浪的实时行情服务器响应时间不超过100毫秒（30-70之间），且没有出现过数据错误。

    // 读取腾讯实时行情数据。 由于腾讯实时行情的股数精度为手，没有零股信息，导致无法与新浪实时行情数据对接（新浪精度为股），故而暂时不用
    if (m_fReadingTengxunRTData && SystemReady()) {
      GetTengxunStockRTData();  // 只有当系统准备完毕后，方可执行读取腾讯实时行情数据的工作
    }

    // 如果要求慢速读取实时数据，则设置读取速率为每分钟一次
    if (!m_fMarketOpened && SystemReady()) m_iCountDownSlowReadingRTData = 1000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
    else m_iCountDownSlowReadingRTData = 3;  // 计数4次

  }
  m_iCountDownSlowReadingRTData--;

  // 系统准备好了之后需要完成的各项工作
  if (SystemReady()) {
    // 装入之前存储的系统今日数据（如果有的话）
    if (!m_fTodayTempDataLoaded) { // 此工作仅进行一次。
      LoadTodayTempData();
      m_fTodayTempDataLoaded = true;
    }

    // 抓取日线数据
    if (!gl_fExiting && m_fGetDayLineData) {
      GetNetEaseStockDayLineData();
    }
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 定时调度函数，每秒一次。
//
// 各种任务之间有可能出现互斥的现象，如存储临时实时数据的工作线程与计算实时数据的工作线程之间就不允许同时运行，
// 故而所有的定时任务，要按照时间间隔从长到短排列，即现执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SchedulingTaskPerSecond(long lSecondNumber)
{
  static int i10SecondsCounter = 9; // 十秒一次的计数器
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器
  static int i5MinuteCounter = 299; // 五分钟一次的计数器
  const long lTime = gl_systemTime.GetTime();

  // 计算每五分钟一次的任务。
  if (i5MinuteCounter <= 0) {
    i5MinuteCounter = 299;

    // 午夜过后重置各种标识
    if (lTime <= 1500 && !m_fPermitResetSystem) {  // 在零点到零点十五分，重置系统标识
      m_fPermitResetSystem = true;
      CString str;
      str = _T(" 重置系统重置标识");
      gl_systemMessage.PushInformationMessage(str);
    }

    // 开市时每五分钟存储一次当前状态。这是一个备用措施，防止退出系统后就丢掉了所有的数据，不必太频繁。
    if (m_fMarketOpened && m_fSystemReady && !gl_ThreadStatus.IsCalculatingRTData()) {
      if (((lTime > 93000) && (lTime < 113600)) || ((lTime > 130000) && (lTime < 150000))) { // 存储临时数据严格按照交易时间来确定(中间休市期间要存储一次，故而到11:36才中止）
        CString str;
        str = _T(" 存储临时数据");
        gl_systemMessage.PushInformationMessage(str);
        UpdateTempRTData();
      }
    }
  } // 每五分钟一次的任务
  else i5MinuteCounter -= lSecondNumber;

  // 计算每分钟一次的任务。所有的定时任务，要按照时间间隔从长到短排列，即现执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 59; // 重置计数器

    // 九点十三分重启系统
    // 必须在此时间段内重启，如果更早的话容易出现数据不全的问题。
    if (m_fPermitResetSystem) { // 如果允许重置系统
      if ((lTime >= 91300) && (lTime <= 91400) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // 交易日九点十五分重启系统
        gl_fResetSystem = true;     // 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
        m_fSystemReady = false;
      }
    }

    // 九点二十五分再次重启系统
    if (m_fPermitResetSystem) { // 如果允许重置系统
      if ((lTime >= 92500) && (lTime <= 93000) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // 交易日九点十五分重启系统
        gl_fResetSystem = true;     // 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
        m_fSystemReady = false;
        m_fPermitResetSystem = false; // 今天不再允许重启系统。
      }
    }

    // 判断中国股票市场开市状态
    if ((lTime < 91500) || (lTime > 150130) || ((lTime > 113500) && (lTime < 125500))) { //下午三点零分三十秒市场交易结束，
      m_fMarketOpened = false;
    }
    else if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) { //周六或者周日闭市。结构tm用0--6表示星期日至星期六
      m_fMarketOpened = false;
    }
    else m_fMarketOpened = true;

    // 在开市前和中午暂停时查询所有股票池，找到当天活跃股票。
    if (((lTime >= 91500) && (lTime < 92900)) || ((lTime >= 113100) && (lTime < 125900))) {
      m_fCheckTodayActiveStock = true;
    }
    else m_fCheckTodayActiveStock = false;

    // 下午三点一分开始处理当日实时数据。
    if ((lTime >= 150100) && !IsTodayStockCompiled()) {
      if (SystemReady()) {
        AfxBeginThread(ThreadCompileTodayStocks, nullptr);
        SetTodayStockCompiledFlag(true);
      }
    }

    // 判断是否结束下载日线历史数据
    if (IsTotalStockDayLineChecked() && !m_fUpdatedStockCodeDataBase) { // 如果所有股票都检查过且存储日线进数据库的线程已经运行结束
      if (!gl_ThreadStatus.IsSavingDayLineInProcess()) { // 如果更新日线数据库线程不是活跃状态，则停止日线数据查询。
        // 更新日线数据库线程处于活跃中时，尚有数据没有存储，不能停止查询过程（查询过程能够激活更新线程）
        m_fUpdatedStockCodeDataBase = true;
        TRACE("日线历史数据更新完毕\n");
        CString str;
        str = _T("日线历史数据更新完毕");
        gl_systemMessage.PushInformationMessage(str);
        SaveStockCodeDataBase();  // 更新股票池数据库
        ASSERT(m_setSavingDayLineOnly.IsOpen());
        m_setSavingDayLineOnly.Close(); // 关闭日线历史数据存储记录集
        m_fGetDayLineData = false; // 所有需要更新的日线资料都更新过了，不再执行更新日线资料任务
      }
    }
    else {
      if (!gl_ThreadStatus.IsSavingDayLineInProcess() && m_fGetDayLineData) {
        gl_ThreadStatus.SetSavingDayLineInProcess(true);
        AfxBeginThread(ThreadSavingDayLineProc, nullptr);
      }
    }
  } // 每一分钟一次的任务
  else i1MinuteCounter -= lSecondNumber;

  // 计算每十秒钟一次的任务
  if (i10SecondsCounter <= 0) {
    i10SecondsCounter = 9;
    // do something
  } // 每十秒钟一次的任务
  else i10SecondsCounter -= lSecondNumber;

  // 计算实时数据，每秒钟一次。目前个股实时数据为每3秒钟一次更新，故而无需再快了。
  if (SystemReady() && !gl_ThreadStatus.IsSavingTempData()) { // 在系统存储临时数据时不能同时计算实时数据，否则容易出现同步问题。
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      AfxBeginThread(ThreadCalculatingRTDataProc, nullptr);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 得到股票的简称
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName(CString strStockCode) {
  if (m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end()) {
    long lIndex = m_mapActiveStockToIndex.at(strStockCode);
    return (m_vActiveStock.at(lIndex)->GetStockName());
  }
  else return _T("");
}

////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码得到股票的索引。
//	如果没找到的话返回值为假。
//
//
////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetStockIndex(CString strStockCode, long& lIndex) {
  if (m_mapActiveStockToIndex.find(strStockCode) == m_mapActiveStockToIndex.end()) {
    lIndex = -1;
    return false;
  }
  else lIndex = m_mapActiveStockToIndex.at(strStockCode);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码得到股票的指针。
//	如果没找到的话返回空指针
//
//
////////////////////////////////////////////////////////////////////////////////
CStockPtr CMarket::GetStockPtr(CString strStockCode) {
  long lIndex = -1;

  if (m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end()) {
    lIndex = m_mapActiveStockToIndex[strStockCode];
    return (m_vActiveStock.at(lIndex));
  }
  else return nullptr;
}

CStockPtr CMarket::GetStockPtr(long lIndex) {
  ASSERT((lIndex >= 0) && (lIndex < m_lTotalActiveStock));
  return m_vActiveStock.at(lIndex);
}

void CMarket::AddStockToMarket(CStockPtr pStock) {
  m_mapActiveStockToIndex[pStock->GetStockCode()] = m_lTotalActiveStock++;
  m_vActiveStock.push_back(pStock);
}

bool CMarket::GetStockIDPtr(CString strStockCode, StockIDPtr& pStockIDPtr)
{
  long lIndex = 0;
  if (m_mapChinaMarketAStock.find(strStockCode) == m_mapChinaMarketAStock.end()) { // 未找到
    pStockIDPtr = nullptr;
    return false;
  }
  lIndex = m_mapChinaMarketAStock.at(strStockCode);
  pStockIDPtr = m_vChinaMarketAStock.at(lIndex);
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// 设置当前操作的股票
//
// 设置相应的股票指针，装载其日线数据。
//
/////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CStockPtr pStock)
{
  if (m_pCurrentStock != pStock) {
    m_pCurrentStock = pStock;
    m_fCurrentStockChanged = true;
    m_pCurrentStock->SetDayLineLoaded(false);
    AfxBeginThread(ThreadLoadDayLineProc, 0);
  }
}

bool CMarket::IsCurrentStockChanged(void) {
  if (m_fCurrentStockChanged) {
    m_fCurrentStockChanged = false;
    return true;
  }
  else return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码和市场代码设置当前选择股票
//
//////////////////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CString strStockCode) {
  m_pCurrentStock = GetStockPtr(strStockCode);
  ASSERT(m_pCurrentStock != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLine(CSetDayLine* psetDayLine, CSetStockCode* psetStockCode, CStockPtr pStock, vector<CDayLinePtr>& vectorDayLine, bool fReversed) {
  long lIndex = 0;
  psetDayLine->m_pDatabase->BeginTrans();
  if (fReversed) {
    for (int i = vectorDayLine.size() - 1; i >= 0; i--) { // 数据是倒序存储的，需要从尾部开始存储
      auto pDayLine = vectorDayLine.at(i);
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // 存储过的日线数据不用存储
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  else {
    for (int i = 0; i < vectorDayLine.size(); i++) { // 数据是正序存储的，需要从头部开始存储
      auto pDayLine = vectorDayLine.at(i);
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // 存储过的日线数据不用存储
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  psetDayLine->m_pDatabase->CommitTrans();

  // 更新最新日线日期和起始日线日期
  bool fSaved = false;
  if (fReversed) {
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(0)->GetDay()) {
      m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(0)->GetDay());
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSaved = true;
    }
  }
  else {
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(vectorDayLine.size() - 1)->GetDay()) {
      m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(0)->GetDay());
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSaved = true;
    }
  }

  if (fSaved) {
    psetStockCode->m_pDatabase->BeginTrans();
    psetStockCode->AddNew();
    psetStockCode->m_Counter = m_vChinaMarketAStock.at(lIndex)->GetIndex();
    psetStockCode->m_StockType = m_vChinaMarketAStock.at(lIndex)->GetMarket();
    psetStockCode->m_StockCode = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
    psetStockCode->m_StockName = m_vChinaMarketAStock.at(lIndex)->GetStockName();
    psetStockCode->m_DayLineStartDay = m_vChinaMarketAStock.at(lIndex)->GetDayLineStartDay();
    psetStockCode->m_DayLineEndDay = m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay();
    psetStockCode->m_IPOed = m_vChinaMarketAStock.at(lIndex)->GetIPOStatus();
    psetStockCode->Update();
    psetStockCode->m_pDatabase->CommitTrans();
  }

  return true;
}

bool CMarket::SaveOneRecord(CSetDayLine* psetDayLine, CDayLinePtr pDayLine) {
  psetDayLine->AddNew();
  psetDayLine->m_Time = pDayLine->GetDay();
  psetDayLine->m_Market = pDayLine->GetMarket();
  psetDayLine->m_StockCode = pDayLine->GetStockCode();
  psetDayLine->m_StockName = pDayLine->GetStockName();
  psetDayLine->m_LastClose = (double)pDayLine->GetLastClose() / 1000;
  psetDayLine->m_High = (double)pDayLine->GetHigh() / 1000;
  psetDayLine->m_Low = (double)pDayLine->GetLow() / 1000;
  psetDayLine->m_Open = (double)pDayLine->GetOpen() / 1000;
  psetDayLine->m_Close = (double)pDayLine->GetClose() / 1000;
  psetDayLine->m_Volume = pDayLine->GetVolume();
  psetDayLine->m_Amount = pDayLine->GetAmount();
  psetDayLine->m_UpAndDown = pDayLine->GetUpDown();
  psetDayLine->m_UpDownRate = pDayLine->GetUpDownRate();
  psetDayLine->m_ChangeHandRate = pDayLine->GetChangeHandRate();
  psetDayLine->m_TotalValue = pDayLine->GetTotalValue();
  psetDayLine->m_CurrentValue = pDayLine->GetCurrentValue();

  psetDayLine->m_RelativeStrong = pDayLine->GetRelativeStrong();

  /*
  psetDayLine->m_TransactionNumber = pDayLine->GetTransactionNumber();
  psetDayLine->m_TransactionNumberBelow5000 = pDayLine->GetTransactionNumberBelow5000();
  psetDayLine->m_TransactionNumberBelow50000 = pDayLine->GetTransactionNumberBelow50000();
  psetDayLine->m_TransactionNumberBelow200000 = pDayLine->GetTransactionNumberBelow200000();
  psetDayLine->m_TransactionNumberAbove200000 = pDayLine->GetTransactionNumberAbove200000();

  psetDayLine->m_OrdinaryBuyVolume = pDayLine->GetOrdinaryBuyVolume();
  psetDayLine->m_OrdinarySellVolume = pDayLine->GetOrdinarySellVolume();
  psetDayLine->m_AttackBuyVolume = pDayLine->GetAttackBuyVolume();
  psetDayLine->m_AttackSellVolume = pDayLine->GetAttackSellVolume();
  psetDayLine->m_StrongBuyVolume = pDayLine->GetStrongBuyVolume();
  psetDayLine->m_StrongSellVolume = pDayLine->GetStrongSellVolume();
  psetDayLine->m_UnknownVolume = pDayLine->GetUnknownVolume();
  psetDayLine->m_CancelBuyVolume = pDayLine->GetCancelBuyVolume();
  psetDayLine->m_CancelSellVolume = pDayLine->GetCancelSellVolume();

  psetDayLine->m_AttackBuyBelow50000 = pDayLine->GetAttackBuyBelow50000();
  psetDayLine->m_AttackBuyBelow200000 = pDayLine->GetAttackBuyBelow200000();
  psetDayLine->m_AttackBuyAbove200000 = pDayLine->GetAttackBuyAbove200000();
  psetDayLine->m_AttackSellBelow50000 = pDayLine->GetAttackSellBelow50000();
  psetDayLine->m_AttackSellBelow200000 = pDayLine->GetAttackSellBelow200000();
  psetDayLine->m_AttackSellAbove200000 = pDayLine->GetAttackSellAbove200000();
  */
  psetDayLine->Update();

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//  此函数由工作线程ThreadDayLineSaveProc调用，尽量不要使用全局变量。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLineData(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  long lIndex = 0;

  for (auto pStock : m_vActiveStock) {
    if (pStock->IsDayLineNeedSaving()) {
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (pStock->m_vDayLine.size() > 0) { // 新股第一天上市时，由于只存储早于今天的日线数据，导致其容器是空的，故而需要判断一下
        SaveDayLine(&m_setSavingDayLineOnly, &setStockCode, pStock, pStock->m_vDayLine, false);
      }
      else {
        CString str1 = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
        str1 += _T(" 新股上市,没有日线资料");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
      pStock->m_vDayLine.clear();
      pStock->SetDayLineLoaded(false);
      CString str = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
      str += _T("日线资料存储完成");
      gl_systemMessage.PushDayLineInfoMessage(str);
      pStock->SetDayLineNeedSavingFlag(false);
    }
    if (gl_fExiting) {
      break; // 如果程序正在退出，则停止存储。
    }
  }

  setStockCode.Close();

  return(true);
}

bool CMarket::ClearAllDayLineVector(void)
{
  for (auto pStock : m_vActiveStock) {
    pStock->m_vDayLine.clear();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将实时数据存入数据库．默认数据库为空。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveRTData(void) {
  CSetRealTimeData setRTData;
  setRTData.m_strFilter = _T("[ID] = 1");

  setRTData.Open();
  setRTData.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if (pStock != nullptr) {
      if (pStock->IsActive()) {
        pStock->SaveRealTimeData(&setRTData);
      }
    }
  }
  setRTData.m_pDatabase->CommitTrans();
  setRTData.Close();
  return(true);
}

bool CMarket::IsTotalStockDayLineChecked(void) {
  for (auto pStockID : m_vChinaMarketAStock) {
    if (pStockID->IsDayLineNeedUpdate()) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// 处理今日接收到的实时数据，生成日线各基本数据（相对强度、进攻性买卖盘）。
//
// 只有下载完日线历史数据后，方可执行处理实时数据，否则可能误判股票代码存在与否。
//
//
//////////////////////////////////////////////////////////////////////////////////
long CMarket::CompileCurrentTradeDayStocks(long lCurrentTradeDay) {
  long lClose = 0, lLastClose = 0;
  char buffer[20];
  CString strDay;
  CSetDayLine setDayKLine;
  CSetDayLineInfo setDayLineInfo;
  long lIndex = 0;
  long iCount = 0;

  CString str;
  str = _T("开始处理最新交易日的实时数据");
  gl_systemMessage.PushInformationMessage(str);

  // 存储当前交易日的数据
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  setDayKLine.m_pDatabase->BeginTrans();
  while (!setDayKLine.IsEOF()) {
    setDayKLine.Delete();
    setDayKLine.MoveNext();
  }
  setDayKLine.m_pDatabase->CommitTrans();

  setDayKLine.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if (pStock == nullptr) {
      gl_systemMessage.PushInformationMessage(_T("当前活跃股票中存在nullptr"));
      continue; // 空置位置。应该不存在。
    }
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    iCount++;
    lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
    m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(lCurrentTradeDay);
    m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__); // 再设置一次。防止新股股票代码由于没有历史数据而被误判为不存在。
    setDayKLine.AddNew();
    setDayKLine.m_Time = lCurrentTradeDay;
    setDayKLine.m_Market = pStock->GetMarket();
    setDayKLine.m_StockName = pStock->GetStockName();
    setDayKLine.m_StockCode = pStock->GetStockCode();
    lLastClose = pStock->GetLastClose();
    setDayKLine.m_LastClose = static_cast<double>(pStock->GetLastClose()) / 1000;
    setDayKLine.m_Open = static_cast<double>(pStock->GetOpen()) / 1000;
    setDayKLine.m_High = static_cast<double>(pStock->GetHigh()) / 1000;
    setDayKLine.m_Low = static_cast<double>(pStock->GetLow()) / 1000;
    lClose = pStock->GetNew();
    setDayKLine.m_Close = static_cast<double>(pStock->GetNew()) / 1000;
    setDayKLine.m_UpAndDown = static_cast<double>(lClose - lLastClose) / 1000;
    if (lLastClose == 0) { // 新上市第一天的股票
      setDayKLine.m_UpDownRate = 0;
    }
    else {
      setDayKLine.m_UpDownRate = (static_cast<double>(lClose - lLastClose)) * 100.0 / lLastClose;
    }

    setDayKLine.m_Volume = pStock->GetVolume();
    setDayKLine.m_Amount = pStock->GetAmount();
    setDayKLine.m_TotalValue = 0;
    setDayKLine.m_CurrentValue = 0;
    setDayKLine.m_RelativeStrong = pStock->GetRelativeStrong();
    /*
    setDayKLine.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayKLine.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayKLine.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayKLine.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayKLine.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayKLine.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayKLine.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayKLine.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayKLine.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayKLine.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayKLine.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayKLine.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayKLine.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayKLine.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayKLine.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayKLine.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayKLine.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayKLine.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayKLine.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayKLine.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
    */
    setDayKLine.Update();
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  // 存储今日生成的数据于DayLineInfo表中。
  setDayLineInfo.m_strFilter = _T("[Time] =");
  setDayLineInfo.m_strFilter += strDay;
  setDayLineInfo.Open();
  setDayLineInfo.m_pDatabase->BeginTrans();
  while (!setDayLineInfo.IsEOF()) {
    setDayLineInfo.Delete();
    setDayLineInfo.MoveNext();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();

  setDayLineInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    setDayLineInfo.AddNew();
    setDayLineInfo.m_Day = lCurrentTradeDay;
    setDayLineInfo.m_StockCode = pStock->GetStockCode();

    setDayLineInfo.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayLineInfo.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayLineInfo.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayLineInfo.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayLineInfo.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayLineInfo.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayLineInfo.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayLineInfo.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayLineInfo.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayLineInfo.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayLineInfo.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayLineInfo.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayLineInfo.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayLineInfo.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayLineInfo.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayLineInfo.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayLineInfo.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayLineInfo.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayLineInfo.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayLineInfo.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
    setDayLineInfo.Update();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();
  setDayLineInfo.Close();

  gl_systemMessage.PushInformationMessage(_T("最新交易日实时数据处理完毕"));
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
bool CMarket::SaveTodayTempData(void) {
  CSetDayLineToday setDayLineToday;

  // 存储今日生成的数据于DayLineToday表中。
  setDayLineToday.Open();
  setDayLineToday.m_pDatabase->BeginTrans();
  while (!setDayLineToday.IsEOF()) {
    setDayLineToday.Delete();
    setDayLineToday.MoveNext();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    ASSERT(pStock->GetVolume() == pStock->GetOrdinaryBuyVolume() + pStock->GetOrdinarySellVolume() + pStock->GetAttackBuyVolume()
      + pStock->GetAttackSellVolume() + pStock->GetStrongBuyVolume() + pStock->GetStrongSellVolume() + pStock->GetUnknownVolume());
    setDayLineToday.AddNew();
    setDayLineToday.m_Time = gl_systemTime.GetDay();
    setDayLineToday.m_Market = pStock->GetMarket();
    setDayLineToday.m_StockName = pStock->GetStockName();
    setDayLineToday.m_StockCode = pStock->GetStockCode();

    setDayLineToday.m_Volume = pStock->GetVolume();
    setDayLineToday.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayLineToday.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayLineToday.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayLineToday.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayLineToday.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayLineToday.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayLineToday.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayLineToday.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayLineToday.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayLineToday.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayLineToday.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayLineToday.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayLineToday.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayLineToday.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayLineToday.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayLineToday.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayLineToday.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayLineToday.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayLineToday.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayLineToday.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
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
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::LoadTodayTempData(void) {
  CStockPtr pStock = nullptr;
  CSetDayLineToday setDayLineToday;
  CStockRTDataPtr pRTData;

  ASSERT(!m_fTodayTempDataLoaded);
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // 执行此初始化工作时，计算实时数据的工作线程必须没有运行。
  // 读取今日生成的数据于DayLineToday表中。
  setDayLineToday.Open();
  if (!setDayLineToday.IsEOF()) {
    if (setDayLineToday.m_Time == gl_systemTime.GetDay()) { // 如果是当天的行情，则载入，否则放弃
      while (!setDayLineToday.IsEOF()) {
        if ((pStock = GetStockPtr(setDayLineToday.m_StockCode)) != nullptr) {
          // 需要设置m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
          // 而第一次执行计算实时数据时，只是初始化系统环境，其中设置m_lUnknownVolume += pRTData->GetVolume
          // 故而此处这样计算。
          ASSERT(!pStock->IsStartCalculating()); // 确保没有开始计算实时数据
          pStock->SetUnknownVolume(setDayLineToday.m_UnknownVolume - setDayLineToday.m_Volume);  // 需要如此设置m_lUnknownVolume

          pStock->SetTransactionNumber(setDayLineToday.m_TransactionNumber);
          pStock->SetTransactionNumberBelow5000(setDayLineToday.m_TransactionNumberBelow5000);
          pStock->SetTransactionNumberBelow50000(setDayLineToday.m_TransactionNumberBelow50000);
          pStock->SetTransactionNumberBelow200000(setDayLineToday.m_TransactionNumberBelow200000);
          pStock->SetTransactionNumberAbove200000(setDayLineToday.m_TransactionNumberAbove200000);
          pStock->SetCancelBuyVolume(setDayLineToday.m_CancelBuyVolume);
          pStock->SetCancelSellVolume(setDayLineToday.m_CancelSellVolume);
          pStock->SetAttackBuyVolume(setDayLineToday.m_AttackBuyVolume);
          pStock->SetAttackSellVolume(setDayLineToday.m_AttackSellVolume);
          pStock->SetStrongBuyVolume(setDayLineToday.m_StrongBuyVolume);
          pStock->SetStrongSellVolume(setDayLineToday.m_StrongSellVolume);
          pStock->SetOrdinaryBuyVolume(setDayLineToday.m_OrdinaryBuyVolume);
          pStock->SetOrdinarySellVolume(setDayLineToday.m_OrdinarySellVolume);
          pStock->SetAttackBuyBelow50000(setDayLineToday.m_AttackBuyBelow50000);
          pStock->SetAttackBuyBelow200000(setDayLineToday.m_AttackBuyBelow200000);
          pStock->SetAttackBuyAbove200000(setDayLineToday.m_AttackBuyAbove200000);
          pStock->SetAttackSellBelow50000(setDayLineToday.m_AttackSellBelow50000);
          pStock->SetAttackSellBelow200000(setDayLineToday.m_AttackSellBelow200000);
          pStock->SetAttackBuyAbove200000(setDayLineToday.m_AttackSellAbove200000);
        }
        setDayLineToday.MoveNext();
      }
    }
  }
  setDayLineToday.Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// 计算lDay的日线相对强度, lDay的格式为：YYYYMMDD,如 19990605.
// 将日线按涨跌排列后,其相对强弱即其在队列中的位置
//
//////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateOneDayRelativeStrong(long lDay) {
  vector<StockIDPtr> vStockID;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0, j = 0;
  CTime ctTime;
  CSetDayLine setDayKLine;
  const long lYear = lDay / 10000;
  const long lMonth = lDay / 100 - lYear * 100;
  const long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];

  for (j = 0; j < 30; j++) pch[j] = 0x000;

  _ltoa_s(lDay, pch, 10);
  strDay = pch;
  setDayKLine.m_strSort = _T("[UpDownRate]");
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  if (setDayKLine.IsEOF()) { // 数据集为空，表明此日没有交易
    setDayKLine.Close();
    return false;
  }
  setDayKLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayKLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayKLine.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayKLine.m_StockCode);
      vStockID.push_back(m_vChinaMarketAStock.at(lIndex));
      vIndex.push_back(iStockNumber); // 将A股的索引记录在容器中。
      iTotalAShare++;
    }
    iStockNumber++;
    setDayKLine.MoveNext();
  }

  setDayKLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // 只计算活跃股票的相对强度
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // 根据索引去更改数据库,跨过不是A股的股票
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    if (((static_cast<double>(setDayKLine.m_Low) / setDayKLine.m_LastClose) < 0.88)
      || ((static_cast<double>(setDayKLine.m_High) / setDayKLine.m_LastClose) > 1.12)) { // 除权、新股上市等
      setDayKLine.m_RelativeStrong = 50; // 新股上市或者除权除息，不计算此股
    }
    else if ((fabs(setDayKLine.m_High - setDayKLine.m_Close) < 0.0001)
      && ((static_cast<double>(setDayKLine.m_Close) / setDayKLine.m_LastClose) > 1.095)) { // 涨停板
      setDayKLine.m_RelativeStrong = 100;
    }
    else if ((fabs(setDayKLine.m_Close - setDayKLine.m_Low) < 0.0001)
      && ((static_cast<double>(setDayKLine.m_Close) / setDayKLine.m_LastClose) < 0.905)) { // 跌停板
      setDayKLine.m_RelativeStrong = 0;
    }
    else {
      setDayKLine.m_RelativeStrong = (static_cast<double>(iCount) * 100) / iTotalAShare;
    }
    setDayKLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayKLine.MoveNext(); // 移到下一个数据。
    iBefore++; // 计数器也同时加一。
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  vStockID.clear();
  vIndex.clear();
  vRelativeStrong.clear();

  sprintf_s(buffer, "%4d年%2d月%2d日的股票相对强度计算完成", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // 采用同步机制报告信息

  return(true);
}

bool CMarket::SaveStockCodeDataBase(void)
{
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStockID : gl_ChinaStockMarket.m_vChinaMarketAStock) {
    setStockCode.AddNew();
    CString str;
    setStockCode.m_Counter = pStockID->GetIndex();
    setStockCode.m_StockType = pStockID->GetMarket();
    setStockCode.m_StockCode = pStockID->GetStockCode();
    if (pStockID->GetStockName() != _T("")) {   // 如果此股票ID有了新的名字，
      setStockCode.m_StockName = pStockID->GetStockName(); // 则存储新的名字
    }
    if (pStockID->GetIPOStatus() == __STOCK_IPOED__) { // 如果此股票是活跃股票
      if (pStockID->GetDayLineEndDay() < (gl_systemTime.GetDay() - 100)) { // 如果此股票的日线历史数据已经早于一个月了，则设置此股票状态为已退市
        setStockCode.m_IPOed = __STOCK_DELISTED__;
      }
      else {
        setStockCode.m_IPOed = pStockID->GetIPOStatus();
      }
    }
    else {
      setStockCode.m_IPOed = pStockID->GetIPOStatus();
    }
    setStockCode.m_DayLineStartDay = pStockID->GetDayLineStartDay();
    setStockCode.m_DayLineEndDay = pStockID->GetDayLineEndDay();
    setStockCode.Update();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

void CMarket::LoadStockCodeDataBase(void)
{
  CSetStockCode setStockCode;

  setStockCode.Open();
  // 装入股票代码数据库
  while (!setStockCode.IsEOF()) {
    long lIndex = 1;
    lIndex = m_mapChinaMarketAStock.at(setStockCode.m_StockCode);
    if (setStockCode.m_StockCode != _T("")) {
      m_vChinaMarketAStock.at(lIndex)->SetStockCode(setStockCode.m_StockCode);
    }
    if (setStockCode.m_StockName != _T("")) {
      CString str = setStockCode.m_StockName; // 用str中间过渡一下，就可以读取UniCode制式的m_StockName了。
      m_vChinaMarketAStock.at(lIndex)->SetStockName(str);
    }
    if (setStockCode.m_IPOed != __STOCK_NOT_CHECKED__) { // 如果此股票代码已经被检查过，则设置股票目前状态。否则不设置。
      m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(setStockCode.m_IPOed);
    }
    m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // 有时一个股票会有多个记录，以最后的日期为准。
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
    }
    // 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
    if (gl_systemTime.GetLastTradeDay() <= m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay()) { // 最新日线数据为今日或者上一个交易日的数据。
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false); // 日线数据不需要更新
    }
    if (setStockCode.m_IPOed == __STOCK_NULL__) { // 无效代码不需更新日线数据
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    if (setStockCode.m_IPOed == __STOCK_DELISTED__) { // 退市股票不需更新日线数据
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    setStockCode.MoveNext();
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
bool CMarket::UpdateOptionDataBase(void)
{
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return false;
}

void CMarket::LoadOptionDataBase(void)
{
  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetLastLoginDay(19900101);
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(setOption.m_RelativeStrongEndDay);
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(setOption.m_RalativeStrongStartDay);
    }
    if (setOption.m_LastLoginDay == 0) {
      gl_ChinaStockMarket.SetLastLoginDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetLastLoginDay(setOption.m_LastLoginDay);
    }
  }

  // 判断是否需要读取日线历史数据
  if (m_lLastLoginDay >= gl_systemTime.GetLastTradeDay()) m_fGetDayLineData = false;
  else m_fGetDayLineData = true;

  setOption.Close();
}

bool CMarket::UpdateTempRTData(void)
{
  if (!gl_ThreadStatus.IsSavingTempData()) {
    gl_ThreadStatus.SetSavingTempData(true);
    AfxBeginThread(ThreadSavingTempRTDataProc, nullptr);
  }

  return false;
}

bool CMarket::OpenSavingDayLineRecord(void)
{
  // 更新日线历史数据的记录集永远处于打开状态（为了加速)
  if (!m_setSavingDayLineOnly.IsOpen()) {
    CString str = _T("[ID] = 1"); // 采用主键作为搜索Index。
    m_setSavingDayLineOnly.m_strFilter = str; // 必须设置，否则会把所有的数据读入，浪费时间
    m_setSavingDayLineOnly.Open(); // 永远打开，用于存储接收到的日线历史数据。
  }

  return true;
}