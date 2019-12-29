#pragma once

#include"stdafx.h"
#include<thread>

// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。

// 网络读取任务

// 读取新浪实时数据线程。网址：http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(LPVOID pParam);    // 此工作线程返回值为1
// 读取腾讯实时数据线程。网址：http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(LPVOID pParam);    // 此工作线程返回值为2
// 读取网易实时数据线程，网址：http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(LPVOID pParam); // 此线程返回值为3
// 读取网易日线历史数据线程, pParam为调用此线程的变量的指针。网址：http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(LPVOID pParam);      // 此工作线程返回值为4
// 读取crweber.com网站上的油运指数线程。网址：www.crweber.com
UINT ThreadReadCrweberIndex(LPVOID pParam); // 此工作线程返回值为5

// 数据库处理任务

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadCalculateRS(LPVOID pParam); // 此工作线程返回值为11
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRelativeStrong线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadCalculateThisDayRS(LPVOID pParam); // 此工作线程返回值也为12
// 存储临时系统状态线程
UINT ThreadSaveTempRTData(LPVOID pParam);     // 此工作线程返回值为13
// 计算和存储当前交易日数据线程
UINT ThreadCompileCurrentTradeDayStock(LPVOID pParam);     // 此工作线程返回值为14
// 存储一个股票的日线历史数据。
UINT ThreadSaveDayLineOfOneStock(LPVOID pParam); // 此工作线程返回值为15
// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLine(LPVOID pParam);        // 此工作线程返回值为16
// 维护日线数据库线程（尚未实现）
UINT ThreadMaintainDayLineDataBase(LPVOID pParam);   // 此工作线程返回值为17
//更新股票代码数据库线程
UINT ThreadUpdateStockCodeDB(LPVOID pParam); // 此线程返回值为18
