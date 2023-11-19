//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。
//
// 智能指针是否可以以参数的形式传递至线程中，现在尚不清楚。测试一下。目前使用vector来间接传递智能指针。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"RSReference.h"

#include"ClassDeclaration.h"
#include "FinnhubForexSymbol.h"
#include "FinnhubCryptoSymbol.h"

#include<gsl/gsl>
using gsl::not_null;

// 网络读取任务
//

// 数据库处理任务

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadBuildDayLineRS(not_null<CChinaMarketPtr> pMarket, long startCalculatingDate); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarketPtr> pMarket, long lDate); // 此工作线程返回值也为12
// 存储临时系统状态线程
UINT ThreadSaveTempRTData(not_null<CChinaMarketPtr> pMarket); // 此工作线程返回值为13
// 装载临时存储的实时数据线程
UINT ThreadLoadTempRTData(not_null<CChinaMarketPtr> pMarket, long lTheDate); // 此线程返回值为19
// 计算和存储当前交易日数据线程
UINT ThreadProcessTodayStock(not_null<CChinaMarketPtr> pMarket); // 此工作线程返回值为14
// 存储一个股票的日线历史数据。
UINT ThreadSaveDayLineBasicInfo(not_null<CChinaStockPtr> pStock); // 此工作线程返回值为15, 参数为携带智能指针的一个结构指针
// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLine(not_null<CChinaStockPtr> pStockInput); // 此工作线程返回值为16
// 维护日线数据库线程（尚未实现）
UINT ThreadMaintainDayLineDataBase(); // 此工作线程返回值为17
//更新股票代码数据库线程
UINT ThreadUpdateChinaStockProfileDB(not_null<CChinaMarketPtr> pMarket); // 此线程返回值为18
// 更新选项数据库
UINT ThreadUpdateOptionDB(not_null<CChinaMarketPtr> pMarket); // 此线程返回值为20
// 更新选项数据库
UINT ThreadAppendChosenStockDB(CChinaMarketPtr pMarket); // 此线程返回值为22
// 生成周线历史数据
UINT ThreadBuildWeekLine(not_null<CChinaMarketPtr> pMarket, long lStartDate); // 此线程的返回值为25
// 生成股票的周线历史数据
UINT ThreadBuildWeekLineOfStock(not_null<CChinaStockPtr> pStockInput, long lStartDate); /// 此线程的返回值为26
// 从数据库读取周线历史数据
UINT ThreadLoadWeekLine(not_null<CChinaStockPtr> pStockInput); // 此线程返回值为29
// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDate执行具体任务，最多生成8个工作线程。
UINT ThreadBuildWeekLineRS(not_null<CChinaMarketPtr> pMarket, long startCalculatingDate); // 此工作线程返回值为30, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarketPtr> pMarket, long lDate); // 此工作线程返回值也为31
// 计算本周的周线
UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarketPtr> pMarket); // 此工作线程返回值为32
// 重建当前周的周线数据表
UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarketPtr> pMarket); // 此工作线程返回值为33
// 存储证券代码段索引
UINT ThreadSaveStockSection(not_null<CChinaMarketPtr> pMarket); // 此工作线程返回值为35

// 下面为美国股票市场的工作线程
// 更新美股日线起止日期线程
UINT ThreadUpdateWorldMarketStockProfileDB(CWorldMarketPtr pMarket); // 此工作线程返回值为37
// 更新Forex日线数据
UINT ThreadUpdateForexDayLineDB(not_null<CForexSymbolPtr> pSymbol); // 此工作线程的返回值未38
// 更新Forex代码表
UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为39
// 更新CountryList代码表
UINT ThreadUpdateCountryListDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为40
// 更新EPSSurprise表
UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStockPtr> pStockInput); // 此工作线程返回值为41
// 更新美国股票的日线历史数据。
UINT ThreadUpdateWorldStockDayLineDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为42
// 从日线数据中重建起止日期
UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarketPtr pMarket); // 此工作线程返回值为43
// 更新Tiingo股票集。
UINT ThreadUpdateTiingoStockDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为44
// 更新Tiingo工业分类。
UINT ThreadUpdateTiingoIndustry(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为45
// 更新SIC工业分类。
UINT ThreadUpdateSICIndustry(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为46
// 更新Naics工业分类。
UINT ThreadUpdateNaicsIndustry(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为47
// 更新InsiderTransaction
UINT ThreadUpdateInsiderTransactionDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为48
// 更新Forex Exchange
UINT ThreadUpdateForexExchangeDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为49
// 更新Economic Calender
UINT ThreadUpdateEconomicCalendarDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为50
// 更新Crypto Exchange
UINT ThreadUpdateCryptoExchangeDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为51
// 更新Crypto代码表
UINT ThreadUpdateFinnhubCryptoSymbolDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为52
// 更新Crypto日线数据
UINT ThreadUpdateCryptoDayLineDB(not_null<CFinnhubCryptoSymbolPtr> pSymbol); // 此工作线程的返回值未53
// 更新Tiingo crypto代码集。
UINT ThreadUpdateTiingoCryptoSymbolDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为54
// 更新Finnhub basic financial
UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为55
// 更新InsiderSentiment
UINT ThreadUpdateInsiderSentimentDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为58
// 更新公司新闻
UINT ThreadUpdateCompanyNewsDB(not_null<CWorldMarketPtr> pMarket); // 此工作线程返回值为59

//各种计算用工作线程
// 计算10日强股票集（两个月内只出现一次强势峰）
UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarketPtr> pMarket); // 此线程返回值为101
// 计算10日强股票集（两次出现强势峰）
UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarketPtr> pMarket); // 此线程返回值为102
// 计算10日强股票集（使用外部pRef提供的参数）
UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarketPtr> pMarket, CRSReference* pRef, int iIndex); // 此线程返回值为103
// 计算股票的10日强势与否
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStockPtr> pStock); // 此线程返回值为104
// 计算股票的10日强势与否1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock); // 此线程返回值为105
// 计算股票的10日强势与否2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock); // 此线程返回值为106
// 分配并计算chinaMarket的实时数据
UINT ThreadDistributeAndCalculateRTData(not_null<CChinaMarketPtr> pMarket); // 此线程返回值为108
