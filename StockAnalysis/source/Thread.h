#pragma once

#include"stdafx.h"

// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。

// 读取新浪实时数据线程。网址：http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(LPVOID pParam);    // 此工作线程返回值为1

// 读取网易日线历史数据线程, pParam为调用此线程的变量的指针。网址：http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(LPVOID pParam);      // 此工作线程返回值为2

// 计算实时数据线程
UINT ThreadCalculateRTData(LPVOID pParam);  // 此工作线程返回值为3

// 存储临时系统状态线程
UINT ThreadSaveTempRTData(LPVOID pParam);     // 此工作线程返回值为4

// 计算和存储当前交易日数据线程
UINT ThreadCompileCurrentTradeDayStock(LPVOID pParam);     // 此工作线程返回值为5

// 存储一个股票的日线历史数据。此线程被ThreadSaveDayLine线程间接调用.尚未实现
UINT ThreadSaveDayLineOfOneStock(LPVOID pParam); // 此工作线程返回值为6

// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLine(LPVOID pParam);        // 此工作线程返回值为7

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadCalculateRS(LPVOID pParam); // 此工作线程返回值为8

// 更新数据库线程（尚未实现）
UINT ThreadUpdateDataBase(LPVOID pParam);   // 此工作线程返回值为9

// 读取腾讯实时数据线程。网址：http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(LPVOID pParam);    // 此工作线程返回值为10

// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRelativeStrongProcx线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadCalculateThisDayRS(LPVOID pParam); // 此工作线程返回值为11

// 读取crweber.com网站上的油运指数线程。网址：www.crweber.com
UINT ThreadReadCrweberIndex(LPVOID pParam); // 此工作线程返回值为12

// 读取网易实时数据线程，网址：http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(LPVOID pParam); // 此线程返回值为13
