//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。
//
// 智能指针是否可以以参数的形式传递至线程中，现在尚不清楚。测试一下。目前使用vector来间接传递智能指针。
//
// todo 使用concurrencpp改写工作线程
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <concurrencpp/runtime/runtime.h>

#include"RSReference.h"

#include"ClassDeclaration.h"

using gsl::not_null;

extern concurrencpp::runtime gl_runtime;

// 网络读取任务
//

// 数据库处理任务

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadBuildDayLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildDayLineRSOfDate(long lDate); // 此工作线程返回值也为12
// 生成周线历史数据
UINT ThreadBuildWeekLine(const not_null<CChinaMarketPtr>& pMarket, long lStartDate); // 此线程的返回值为25
// 生成股票的周线历史数据
UINT ThreadBuildWeekLineOfStock(const not_null<CChinaStockPtr>& pStockInput, long lStartDate); /// 此线程的返回值为26
// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDate执行具体任务，最多生成8个工作线程。
UINT ThreadBuildWeekLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // 此工作线程返回值为30, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRS线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadBuildWeekLineRSOfDate(long lDate); // 此工作线程返回值也为31
// 计算本周的周线
UINT ThreadBuildWeekLineOfCurrentWeek(const not_null<CChinaMarketPtr>& pMarket); // 此工作线程返回值为32
// 重建当前周的周线数据表
UINT ThreadBuildCurrentWeekWeekLineTable(const not_null<CChinaMarketPtr>& pMarket); // 此工作线程返回值为33

// 下面为美国股票市场的工作线程
// 更新Tiingo工业分类。
UINT ThreadUpdateTiingoIndustry(const not_null<CWorldMarketPtr>& pMarket); // 此工作线程返回值为45
// 更新SIC工业分类。
UINT ThreadUpdateSICIndustry(const not_null<CWorldMarketPtr>& pMarket); // 此工作线程返回值为46
// 更新Naics工业分类。
UINT ThreadUpdateNaicsIndustry(const not_null<CWorldMarketPtr>& pMarket); // 此工作线程返回值为47
// 更新Forex Exchange
UINT ThreadUpdateForexExchangeDB(); // 此工作线程返回值为49

//各种计算用工作线程
// 计算10日强股票集（使用外部pRef提供的参数）
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // 此线程返回值为103
// 计算股票的10日强势与否
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const CRSReference* pRef, const not_null<CChinaStockPtr>& pStock); // 此线程返回值为104
// 计算股票的10日强势与否1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // 此线程返回值为105
// 计算股票的10日强势与否2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // 此线程返回值为106
