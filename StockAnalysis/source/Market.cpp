//////////////////////////////////////////////////////////////////////////////////////////////////


//#include"stdafx.h"

#include"globedef.h"
#include"accessory.h"
#include"ClientThread.h"

#include"Market.h"

#include"SetDayLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket ( void ) : CObject() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("市场变量只允许存在一个实例\n");
    ASSERT(0);
  }
  
  m_fLoadedSelectedStock = false;
	m_fMarketReady = false;    // 市场初始状态为未设置好。
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_pCurrentStock = nullptr;

	m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  m_lTotalActiveStock = 0; // 初始时股票池中的股票数量为零

  m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = 19900101;
}

CMarket::~CMarket( ) {

   
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

/////////////////////////////////////////////////////////////////////////
//
//	得到分时线偏移量。09:30为0，15:00为240,步长为1分钟
//
//
////////////////////////////////////////////////////////////////////////
long CMarket::GetMinLineOffset( CStockID sID, time_t Time ) {
	ASSERT( Time >= 0 );
	tm tmTemp;
	time_t t = 0;
	long lIndex = 0;

	localtime_s( &tmTemp, &Time );
	tmTemp.tm_hour = (9 - 8);			// time_t, tm使用的是国际标准时(UTC),故北京时间09：30即UTC的01：30。要减去8小时
	tmTemp.tm_min = 30;
	tmTemp.tm_sec = 0;
	t = mktime( &tmTemp );
	lIndex = (Time - t)/60;
	if ( lIndex < 0 ) lIndex = 0;
	if ( (lIndex >= 120) && (lIndex < 209) ) lIndex = 119;
	if ( lIndex >= 210 ) lIndex -= 90;
	if ( lIndex >= 240 ) lIndex = 239;
	
	ASSERT( (lIndex >= 0) && (lIndex < 240) );
	return( lIndex );
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

  if ((pStock->GetStockCode()[0] == 's') && (pStock->GetStockCode()[1] == 'h') && (pStock->GetStockCode()[2] == '6')) {
    return true;
  }
  else {
    if ((pStock->GetStockCode()[0] == 's') && (pStock->GetStockCode()[1] == 'z') && (pStock->GetStockCode()[2] == '0') && (pStock->GetStockCode()[3] == '0')) {
      return true;
    }
  }
  return(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		判断strStockCode是否为沪深A股的股票代码。
//		沪市A股代码以6开头，深市A股代码以00开头。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::IsAStock(CString strStockCode) {

  if ((strStockCode[0] == 's') && (strStockCode[1] == 'h') && (strStockCode[2] == '6')) {
    return true;
  }
  else {
    if ((strStockCode[0] == 's') && (strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
      return true;
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
bool CMarket::IsStock( CString strStockCode, CStockPtr & pStock ) {
	if ( (pStock = gl_ChinaStockMarket.GetStockPtr( strStockCode )) != nullptr ) {
		return( true );
	}
	else {
		pStock = NULL;
		return( false );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
long long CMarket::GetTotalAttackBuyAmount( void ) {
	long long lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->GetAttackBuyAmount();
		}
	}
	return( lAmount );
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
long long CMarket::GetTotalAttackSellAmount( void ) {
	long long lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return( lAmount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，由CMainFrame的OnTimer函数调用。
//
// 此函数用到大量的全局变量，还是放在主线程为好。工作线程目前还是只做计算个股票的挂单情况。
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessRTData(void)
{
  // 处理读入的实时数据，生成当日的活跃股票市场
  CStockPtr pStock;
	CStockRTDataPtr pRTDataCompact = nullptr;
  long lTotalNumber = gl_systemDequeData.GetRTDataDequeSize();

  for (int i = 0; i < lTotalNumber; i++) {
    CStockRTDataPtr pRTData = gl_systemDequeData.PopRTData();
    if (pRTData->m_fActive) { // 此实时数据有效？
      long lIndex = 0;
      if (m_mapActiveStockToIndex.find(pRTData->m_strStockCode) == m_mapActiveStockToIndex.end()) { // 新的股票代码？
        m_mapActiveStockToIndex[pRTData->m_strStockCode] = m_lTotalActiveStock++; // 建立新的映射
        pStock = make_shared<CStock>();
				pStock->SetActive(true);
				pStock->SetMarket(pRTData->m_wMarket);
				pStock->SetStockCode(pRTData->m_strStockCode);
				pStock->SetStockName(pRTData->m_strStockName);
				pStock->SetCode(pRTData->m_iStockCode);
				pStock->SetLastClose(pRTData->m_lLastClose);
				pStock->SetOpen(pRTData->m_lOpen);
				pStock->UpdataCurrentStatus(pRTData);

        m_vActiveStock.push_back(pStock); // 添加此股入容器，其索引就是目前的m_lTotalActiveStaock的值。
        ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
				pStock->PushRTStockData(pRTData);
        pStock->UpdataCurrentStatus(pRTData);
				lIndex = gl_mapTotalStockToIndex[pStock->GetStockCode()];
				gl_vTotalStock.at(lIndex)->m_strStockName = pStock->GetStockName();
				gl_vTotalStock.at(lIndex)->m_fActive = true; // 本日接收到了数据，
        gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = true;
        // 如果此股票代码尚未使用过，则设置为已使用。
        // 停牌后的股票，也能接收到实时数据，只是其内容只有收盘价，其他都为零。考虑清除这种无效数据。
        gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_IPOED__;
				ASSERT(gl_vTotalStock.at(lIndex)->m_strStockCode.Compare(pStock->GetStockCode()) == 0);
      }
      else {
        lIndex = m_mapActiveStockToIndex.at(pRTData->m_strStockCode);
        ASSERT(lIndex <= m_lTotalActiveStock);
        if (pRTData->m_time > m_vActiveStock.at(lIndex)->GetTime()) { // 新的数据？
          m_vActiveStock.at(lIndex)->UpdataCurrentStatus(pRTData);
          m_vActiveStock.at(lIndex)->PushRTStockData(pRTData); // 存储新的数据至数据池
        }
      }
    }
  }
  
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
  static bool sfFirstTime = true;
  int iCount = 1;

  if (sfFirstTime) {
    m_itStock = m_vActiveStock.begin();
    sfFirstTime = false;
  }

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
// 由工作线程ClientThreadCalculatingRTDataProc调用，注意全局变量的使用
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateRTData(void)
{
  for ( auto pStock : m_vActiveStock) {
    if (pStock != nullptr) {
      pStock->CalculateRTData();
      // 显示当前交易情况
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(pStock->GetStockCode()) == 0) {
          gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDanTransaction();
        }
      }
      // 显示当前取消挂单的情况
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(pStock->GetStockCode()) == 0) {
          gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDan();
        }
      }
    }
    else {
      TRACE(_T("警告：当日活跃股票池中发现nulltr, %s\n"), (LPCSTR)pStock->GetStockCode());
    }
    

    if (gl_fExiting) return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessDayLineData(char * buffer, long lLength) {
	long iCount = 0;
	char * pCurrentPos = buffer;
	char * pTestPos = buffer;
	vector<CDayLinePtr> vTempDayLine;

	if (lLength == 0) { // 没有数据读入？此中状态是查询的股票为无效（不存在）号码
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
		lIndex = gl_mapTotalStockToIndex.at(m_strCurrentStockDownLoading);
		// ASSERT(!gl_vTotalStock[lIndex]->m_fActive); 当一个股票IPO后但尚未上市时，股票代码存在但没有日线数据。取消此断言判断。
    // 有些股票在上市后出现被收购或其他情况，导致日线数据不再更新。此种情况不能设置此股票为无效代码
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay == 19900101) { // 如果初始日线结束日期从来没有变更过，则此股票代码尚未被使用过
      gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_NULL__;   // 此股票代码尚未使用。
      //TRACE("无效股票代码：%s\n", static_cast<LPCWSTR>(m_strCurrentStockDownLoading));
    }
    else { // 已经退市的股票
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay + 100 < gl_lToday) {
        gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_DELISTED__;   // 此股票代码已经退市。
      }
      //TRACE("%S 没有可更新的日线数据\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = false; // 都不需要更新日线数据
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
			TRACE("%s 日线数据出错\n", (LPCSTR)(pDayLine->GetStockCode()));
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
			m_mapActiveStockToIndex[pDayLine->GetStockCode()] = m_lTotalActiveStock++;
			auto pStock = make_shared<CStock>();
			pStock->SetActive(true);
			pStock->SetDayLineLoaded(false);
			pStock->SetMarket(pDayLine->GetMarket());
			pStock->SetStockCode(pDayLine->GetStockCode());
			pStock->SetStockName(pDayLine->GetStockName());
			strTemp = pStock->GetStockCode().Right(6); // 截取股票代码右边的六个数字
			pStock->SetCode(atoi(strTemp.GetBuffer()));
			lIndex = m_lTotalActiveStock - 1;
			m_vActiveStock.push_back(pStock);
      ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
      // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
      long lIndexTemp = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      gl_vTotalStock.at(lIndexTemp)->m_strStockCode = pStock->GetStockCode();
      gl_vTotalStock.at(lIndexTemp)->m_strStockName = pStock->GetStockName();
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
  gl_systemMessage.PushOutputMessage(strTemp);
  gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_fDayLineNeedUpdated = false; // 日线数据下载完毕，不需要申请新数据了。
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_lToday) { // 提取到的股票日线数据其最新日不是上个月的这个交易日（退市了或相似情况），给一个月的时间观察。
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_lIPOed = __STOCK_DELISTED__; // 已退市或暂停交易。
  }
  else {
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_lIPOed = __STOCK_IPOED__; // 正常交易股票
  }
	ASSERT(lIndex >= 0);
	m_vActiveStock.at(lIndex)->SetDayLineLoaded(true);
  m_vActiveStock.at(lIndex)->SetDayLineNeedSavingFlag(true); // 设置存储日线标识 
  m_vActiveStock.at(lIndex)->m_vDayLine.clear(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->GetDay() < gl_lToday) { // 不要存储今日日线数据（今日日线数据由实时数据生成）.
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
bool CMarket::ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength) {
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
		pDayLine->SetMarket(1);
	}
	else if (str == _T("sz")) {
		pDayLine->SetMarket(2);
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
	pDayLine->SetVolume(atoi(buffer2)); // 读入的是股数

	if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
	pDayLine->SetAmount(atof(buffer2));

	if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
	pDayLine->SetTotalValue(atof(buffer2)); // 总市值的单位为：元

	i = 0;
	while (*pCurrentPos != 0x0d) {
		if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // 数据出错，放弃载入
		buffer2[i++] = *pCurrentPos++;
		iCount++;
	}
	pCurrentPos++;
	iCount++;
	buffer2[i] = 0x000;
	pDayLine->SetCurrentValue(atof(buffer2)); // 流通市值的单位为：元。
	if(*pCurrentPos++ != 0x0a) return false; // 数据出错，放弃载入
	iCount++;

	lLength = iCount;

	return true;
}

bool CMarket::ReadOneValue(char *& pCurrentPos, char * buffer, long & iReadNumber)
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
bool CMarket::ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter)
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

bool CMarket::SchedulingTask(void)
{
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 得到股票的简称
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName( CString strStockCode ) {
	long lIndex = m_mapActiveStockToIndex[strStockCode];
	return ( m_vActiveStock.at(lIndex)->GetStockName());
}

////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码得到股票的索引。
//	如果没找到的话返回值为假。
//
//
////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetStockIndex(CString strStockCode, long & lIndex) {

  if (m_mapActiveStockToIndex.find(strStockCode) == m_mapActiveStockToIndex.end()) {
    return false;
  }
  else lIndex = m_mapActiveStockToIndex[strStockCode];

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

	if ( m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end() ) {
    lIndex = m_mapActiveStockToIndex[strStockCode];
		return (m_vActiveStock.at(lIndex));
	}
	else return nullptr;
}

CStockPtr CMarket::GetStockPtr( long lIndex ) { 
	ASSERT( (lIndex >= 0) && (lIndex < m_lTotalActiveStock) );
	return m_vActiveStock.at(lIndex); 
}

bool CMarket::GetStockIDPtr(CString strStockCode, StockIDPtr& pStockIDPtr)
{
  long lIndex = 0;
  if (gl_mapTotalStockToIndex.find(strStockCode) == gl_mapTotalStockToIndex.end()) { // 未找到
    return false;
  }
  lIndex = gl_mapTotalStockToIndex.at(strStockCode);
  pStockIDPtr = gl_vTotalStock.at(lIndex);
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
    AfxBeginThread(ClientthreadLoadDayLineProc, 0);
  }
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码和市场代码设置当前选择股票
//
//////////////////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock( CString strStockCode ) {
  m_pCurrentStock = GetStockPtr( strStockCode );
	ASSERT(m_pCurrentStock != NULL );
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode, CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed) {

  long lIndex = 0;
  psetDayLine->m_pDatabase->BeginTrans();
  if (fReversed) {
    for (int i = vectorDayLine.size() - 1; i >= 0; i--) { // 数据是倒序存储的，需要从尾部开始存储
      auto pDayLine = vectorDayLine.at(i);
      lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay >= pDayLine->GetDay()) continue; // 存储过的日线数据不用存储
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  else {
    for (int i = 0; i < vectorDayLine.size(); i++) { // 数据是倒序存储的，需要从尾部开始存储
      auto pDayLine = vectorDayLine.at(i);
      lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay >= pDayLine->GetDay()) continue; // 存储过的日线数据不用存储
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  psetDayLine->m_pDatabase->CommitTrans();

  // 更新最新日线日期和起始日线日期
  bool fSave = false;
  if (fReversed) {
    gl_vTotalStock.at(lIndex)->m_lDayLineStartDay = vectorDayLine.at(vectorDayLine.size()-1)->GetDay();
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay < vectorDayLine.at(0)->GetDay()) {
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      fSave = true;
    }
  }
  else {
    gl_vTotalStock.at(lIndex)->m_lDayLineStartDay = vectorDayLine.at(0)->GetDay();
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay < vectorDayLine.at(vectorDayLine.size() - 1)->GetDay()) {
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      fSave = true;
    }
  }

  if (fSave) {
    psetStockCode->m_pDatabase->BeginTrans();
    psetStockCode->AddNew();
    psetStockCode->m_Counter = gl_vTotalStock.at(lIndex)->m_nIndex;
    psetStockCode->m_StockType = gl_vTotalStock.at(lIndex)->m_wMarket;
    psetStockCode->m_StockCode = gl_vTotalStock.at(lIndex)->m_strStockCode;
    psetStockCode->m_StockName = gl_vTotalStock.at(lIndex)->m_strStockName;
    psetStockCode->m_DayLineStartDay = gl_vTotalStock.at(lIndex)->m_lDayLineStartDay;
    psetStockCode->m_DayLineEndDay = gl_vTotalStock.at(lIndex)->m_lDayLineEndDay;
    psetStockCode->m_NewestDayLineDay = gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay;
    psetStockCode->m_IPOed = gl_vTotalStock.at(lIndex)->m_lIPOed;
    psetStockCode->Update();
    psetStockCode->m_pDatabase->CommitTrans();
  }
  
  return true;
}

bool CMarket::SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine) {
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

  psetDayLine->Update();

  return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//  此函数由工作线程ClientThreadDayLineSaveProc调用，尽量不要使用全局变量。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLineData(void) {
  CSetStockCode setStockCode;
  CSetDayLine setSavingDayLine;

  setStockCode.Open();
  long lIndex = 0;

	for (auto pStock : m_vActiveStock) {
		if (pStock->IsDayLineNeedSaving()) {
 			lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (pStock->m_vDayLine.size() > 0) { // 新股第一天上市时，由于只存储早于今天的日线数据，导致其容器是空的，故而需要判断一下
        SaveDayLine(&gl_setSavingDayLineOnly, &setStockCode, pStock, pStock->m_vDayLine, false);
      }
      else {
        CString str1 = gl_vTotalStock.at(lIndex)->m_strStockCode;
        str1 += _T(" 新股上市,没有日线资料");
        gl_systemMessage.PushOutputMessage(str1);

      }
      pStock->m_vDayLine.clear();
      pStock->SetDayLineLoaded(false);
      CString str = gl_vTotalStock.at(lIndex)->m_strStockCode;
      str += _T("日线资料存储完成");
      gl_systemMessage.PushOutputMessage(str);
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
bool CMarket::SaveRTData( void ) {
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
  for (auto pStockID : gl_vTotalStock) {
    if (pStockID->m_fDayLineNeedUpdated) return false;
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
bool CMarket::CompileCurrentTradeDayStocks(long lCurrentTradeDay) {
  long lClose = 0, lLastClose = 0;
  char buffer[20];
  CString strDay;
  CSetDayLine setDayKLine;
  CSetDayLineInfo setDayLineInfo;
  long lIndex = 0;
  gl_systemMessage.PushFindMessage(_T("开始处理最新交易日的实时数据"));

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
  for (auto pStock : m_vActiveStock ) {
    if (pStock == nullptr) continue; // 空置位置。应该不存在。
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // 此股票今天停牌,所有的数据皆为零,不需要存储.
      continue;
    }
    lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
    gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = lCurrentTradeDay; // 更新最新接收到日线数据日期。
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay == gl_lLastTradeDay) { // 如果日线最新历史数据日期就是上一个交易日，则更新此日期
      // 此判断有缺陷：当出现平常日期也是节假日时，就不会更新了。研究之。
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = lCurrentTradeDay;
    }
    gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_IPOED__; // 再设置一次。防止新股股票代码由于没有历史数据而被误判为不存在。
    setDayKLine.AddNew();
    setDayKLine.m_Time = lCurrentTradeDay;
    setDayKLine.m_Market = pStock->GetMarket();
    setDayKLine.m_StockName = pStock->GetStockName();
    setDayKLine.m_StockCode = pStock->GetStockCode();
    lLastClose = pStock->GetLastClose();
    setDayKLine.m_LastClose = (double)pStock->GetLastClose() / 1000;
    setDayKLine.m_Open = (double)pStock->GetOpen() / 1000;
    setDayKLine.m_High = (double)pStock->GetHigh() / 1000;
    setDayKLine.m_Low = (double)pStock->GetLow() / 1000;
    lClose = pStock->GetNew();
    setDayKLine.m_Close = (double)pStock->GetNew() / 1000;
    setDayKLine.m_UpAndDown = ((double)(lClose - lLastClose)) / 1000;
    if (lLastClose == 0) { // 新上市第一天的股票
      setDayKLine.m_UpDownRate = 0;
    }
    else {
      setDayKLine.m_UpDownRate = (((double)(lClose - lLastClose)) * 100.0) / lLastClose;
    }
    
    setDayKLine.m_Volume = pStock->GetVolume();
    setDayKLine.m_Amount = pStock->GetAmount();
    setDayKLine.m_TotalValue = 0;
    setDayKLine.m_CurrentValue = 0;
    setDayKLine.m_RelativeStrong = pStock->GetRelativeStrong();
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
    setDayLineInfo.m_Time = lCurrentTradeDay;
    setDayLineInfo.m_Market = pStock->GetMarket();
    setDayLineInfo.m_StockName = pStock->GetStockName();
    setDayLineInfo.m_StockCode = pStock->GetStockCode();

    setDayLineInfo.m_RelativeStrong = pStock->GetRelativeStrong();
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

  gl_systemMessage.PushFindMessage(_T("最新交易日实时数据处理完毕"));
  return true;
}
