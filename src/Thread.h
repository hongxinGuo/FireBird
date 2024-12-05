//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。
//
// 智能指针是否可以以参数的形式传递至线程中，现在尚不清楚。测试一下。目前使用vector来间接传递智能指针。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#undef max //Note 包含concurrencpp.h之前，需要undefined max
#include"concurrencpp/concurrencpp.h"

#include"RSReference.h"

class CChinaStock;
typedef shared_ptr<CChinaStock> CChinaStockPtr;
class CChinaMarket;
typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

using std::vector;
using namespace concurrencpp;

extern concurrencpp::runtime gl_runtime;
extern concurrencpp::runtime gl_backgroundRuntime;
extern int gl_concurrency_level; // 并行计算允许最大数量

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadBuildDayLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDate执行具体任务，最多生成8个工作线程。
UINT ThreadBuildWeekLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // 此工作线程返回值为30, 参数为当前最后计算日期

//各种计算用工作线程
// 计算10日强股票集（使用外部pRef提供的参数）
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // 此线程返回值为103
// 计算股票的10日强势与否
UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock); // 此线程返回值为104
// 计算股票的10日强势与否1
UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // 此线程返回值为105
// 计算股票的10日强势与否2
UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // 此线程返回值为106
