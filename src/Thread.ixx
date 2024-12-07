#include"pch.h"
module;
#undef max
#include"concurrencpp/concurrencpp.h"
export module FireBird.Thread;

class CChinaStock;
using CChinaStockPtr = std::shared_ptr<CChinaStock>;
class CChinaMarket;
using CChinaMarketPtr = std::shared_ptr<CChinaMarket>;

import FireBird.RelativeStrongReference;

export {
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
}