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
using namespace concurrencpp;

extern concurrencpp::runtime gl_runtime;
extern concurrencpp::thread_pool_executor gl_webInquiryExecutor; // 网络申请线程池工作线程运行调度器

extern long gl_concurrency_level; // 并行计算允许最大数量

// 装载当前股票日线
UINT TaskLoadSelectedStockHistoryData(); // 此线程返回值为111
