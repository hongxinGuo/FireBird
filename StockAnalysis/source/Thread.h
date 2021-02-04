#pragma once

#include"stdafx.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"ChinaStock.h"
class CChinaMarket;
#include"WorldMarket.h"
#include"WorldStock.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"

#include<gsl/gsl>
using namespace gsl;

// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。

// 网络读取任务

// 读取新浪实时数据线程。网址：http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(not_null<CSinaRTWebInquiry*> pSinaRTWebData);    // 此工作线程返回值为1，参数为pSinaWebRTData
// 读取腾讯实时数据线程。网址：http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(not_null<CTengxunRTWebInquiry*> pTengxunRTWebData);    // 此工作线程返回值为2，参数为pTengxunWebRTData
// 读取网易实时数据线程，网址：http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(not_null<CNeteaseRTWebInquiry*> pNeteaseRTWebData); // 此线程返回值为3，参数为pNeteaseWebRTData
// 读取网易日线历史数据线程, pParam为调用此线程的变量的指针。网址：http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(not_null<CNeteaseDayLineWebInquiry*> pNeteaseDayLineWebData);      // 此工作线程返回值为4，参数为pNeteaseWebDayLineData
// 读取crweber.com网站上的油运指数线程。网址：http://www.crweber.com
UINT ThreadReadCrweberIndex(not_null<CCrweberIndexWebInquiry*> pCrweberIndexWebData); // 此工作线程返回值为5，参数为pCrweberIndexWebData
// 读取Poten Daily Briefing.网址：http://energy.poten.com/poten-daily-briefing-webpage-04/11/2018
UINT ThreadReadPotenDailyBriefing(not_null<CPotenDailyBriefingWebInquiry*> pPotenDailyBriefingWebData); // 此工作线程返回值为6.
// 读取Finnhub.io网络数据线程。 网址：https://finnhub.io/api/v1/
UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebInquiry); // 此工作线程返回值为7。
// 读取api.tiingo.com
UINT ThreadReadTiingoData(not_null<CTiingoWebInquiry*> pTiingoWebData); // 此工作线程的返回值为8
// 读取quandl
UINT ThreadReadQuandlData(not_null<CQuandlWebInquiry*> pQuandlWebData); // 此工作线程的返回值为9

// 数据库处理任务

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadBuildDayLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDay); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // 此工作线程返回值也为12
// 存储临时系统状态线程
UINT ThreadSaveTempRTData(not_null < CChinaMarket*> pMarket);     // 此工作线程返回值为13
// 计算和存储当前交易日数据线程
UINT ThreadProcessTodayStock(not_null<CChinaMarket*> pMarket);     // 此工作线程返回值为14
// 存储一个股票的日线历史数据。
UINT ThreadSaveDayLineBasicInfoOfStock(not_null<CChinaStock*> pStake); // 此工作线程返回值为15, 参数为携带智能指针的一个结构指针
// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLine(not_null<CChinaStock*> pStake);        // 此工作线程返回值为16
// 维护日线数据库线程（尚未实现）
UINT ThreadMaintainDayLineDataBase(void);   // 此工作线程返回值为17
//更新股票代码数据库线程
UINT ThreadUpdateStakeCodeDB(not_null<CChinaMarket*> pMarket); // 此线程返回值为18
//存储所选股票实时数据线程
UINT ThreadSaveRTData(not_null<CChinaMarket*> pMarket); // 此线程返回值为19
// 更新选项数据库
UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket); // 此线程返回值为20
// 存储Poten.com新的数据
UINT ThreadSavePotenData(not_null<CPotenDailyBriefingMarket*> pMarket); // 此线程返回值为21
// 更新选项数据库
UINT ThreadAppendChoicedStockDB(not_null<CChinaMarket*> pMarket); // 此线程返回值为22
// 存储crweber当日数据
UINT ThreadSaveCrweberDB(not_null<CCrweberIndexMarket*> pMarket, not_null<CCrweberIndexPtr> pCrweberIndex); // 此线程返回值为23
// 整理Crweber数据库
UINT ThreadMaintainCrweberDB(not_null<CCrweberIndexMarket*> pMarket); // 此线程返回值为24
// 生成周线历史数据
UINT ThreadBuildWeekLine(not_null<CChinaMarket*> pMarket, long lStartDate); // 此线程的返回值为25
// 生成股票的周线历史数据
UINT ThreadBuildWeekLineOfStock(not_null<CChinaStock*> pStake, long lStartDate); /// 此线程的返回值为26
// 从数据库读取周线历史数据
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStake); // 此线程返回值为29
// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDate执行具体任务，最多生成8个工作线程。
UINT ThreadBuildWeekLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate); // 此工作线程返回值为30, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // 此工作线程返回值也为31
// 计算本周的周线
UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket); // 此工作线程返回值为32
// 重建当前周的周线数据表
UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket); // 此工作线程返回值为33
// 存储证券代码段索引
UINT ThreadSaveStakeSection(not_null<CChinaMarket*> pMarket); // 此工作线程返回值为35

// 下面为美国股票市场的工作线程
// 更新美股日线起止日期线程
UINT ThreadUpdateStakeDB(not_null<CWorldMarket*> pMarket); // 此工作线程返回值为37
// 更新Forex日线数据
UINT ThreadUpdateForexDayLineDB(not_null<CFinnhubForexSymbol*> pSymbol); // 此工作线程的返回值未38
// 更新Forex代码表
UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket); // 此工作线程返回值为39
// 更新CountryList代码表
UINT ThreadUpdateCountryListDB(not_null<CWorldMarket*> pMarket); // 此工作线程返回值为40
// 更新EPSSurprise表
UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStake); // 此工作线程返回值为41
// 更新美国股票的日线历史数据。
UINT ThreadUpdateWorldStockDayLineDB(not_null<CWorldMarket*> pMarket); // 此工作线程返回值为42
// 从日线数据中重建起止日期
UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket); // 此工作线程返回值为43

//各种计算用工作线程
// 计算10日强股票集（两个月内只出现一次强势峰）
UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarket*> pMarket); // 此线程返回值为101
// 计算10日强股票集（两次出现强势峰）
UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarket*> pMarket); // 此线程返回值为102
// 计算10日强股票集（使用外部pRef提供的参数）
UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarket*> pMarket, CRSReference* pRef, int iIndex); // 此线程返回值为103
// 计算股票的10日强势与否
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStockPtr> pStake); // 此线程返回值为104
// 计算股票的10日强势与否1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStake); // 此线程返回值为105
// 计算股票的10日强势与否2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStake); // 此线程返回值为105
