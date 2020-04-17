#pragma once

#include"stdafx.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

#include"ChinaStock.h"
class CChinaMarket;
#include"PotenDailyBriefingMarket.h"

// 各工作线程声明
// 线程无法作为类函数放入类中（无法被afxBeginThread()调用），故而各线程都只能采用全局函数形式放于此处。
// 工作线程用于处理耗时任务。为了简化程序起见，目前只将两种任务作为线程：一种是处理数据库的任务，另一种是远程异步接收网络数据的任务。
// 其他工作都放入主线程（以减少同步问题的发生）。

// 网络读取任务

// 读取新浪实时数据线程。网址：http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(CSinaRTWebInquiry* pSinaRTWebData);    // 此工作线程返回值为1，参数为pSinaWebRTData
// 读取腾讯实时数据线程。网址：http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(CTengxunRTWebInquiry* pTengxunRTWebData);    // 此工作线程返回值为2，参数为pTengxunWebRTData
// 读取网易实时数据线程，网址：http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(CNeteaseRTWebInquiry* pNeteaseRTWebData); // 此线程返回值为3，参数为pNeteaseWebRTData
// 读取网易日线历史数据线程, pParam为调用此线程的变量的指针。网址：http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(CNeteaseDayLineWebInquiry* pNeteaseDayLineWebData);      // 此工作线程返回值为4，参数为pNeteaseWebDayLineData
// 读取crweber.com网站上的油运指数线程。网址：http://www.crweber.com
UINT ThreadReadCrweberIndex(CCrweberIndexWebInquiry* pCrweberIndexWebData); // 此工作线程返回值为5，参数为pCrweberIndexWebData
// 读取Poten Daily Briefing.网址：http://energy.poten.com/poten-daily-briefing-webpage-04/11/2018
UINT ThreadReadPotenDailyBriefing(CPotenDailyBriefingWebInquiry* pPotenDailyBriefingWebData); // 此工作线程返回值为6.
// 数据库处理任务

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadCalculateDayLineRS(CChinaMarket* pMarket, long startCalculatingDay); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRelativeStrong线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadCalculateThisDayRS(CChinaMarket* pMarket, long thisDay); // 此工作线程返回值也为12
// 存储临时系统状态线程
UINT ThreadSaveTempRTData(CChinaMarket* pMarket);     // 此工作线程返回值为13
// 计算和存储当前交易日数据线程
UINT ThreadProcessTodayStock(CChinaMarket* pMarket);     // 此工作线程返回值为14
// 存储一个股票的日线历史数据。
UINT ThreadSaveDayLineOfOneStock(CChinaStockPtr pStock); // 此工作线程返回值为15, 参数为携带智能指针的一个结构指针
// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLine(CChinaStockPtr pStock);        // 此工作线程返回值为16
// 维护日线数据库线程（尚未实现）
UINT ThreadMaintainDayLineDataBase(void);   // 此工作线程返回值为17
//更新股票代码数据库线程
UINT ThreadUpdateStockCodeDB(CChinaMarket* pMarket); // 此线程返回值为18
//存储所选股票实时数据线程
UINT ThreadSaveRTData(CChinaMarket* pMarket); // 此线程返回值为19
// 更新选项数据库
UINT ThreadUpdateOptionDB(CChinaMarket* pMarket); // 此线程返回值为20
// 存储Poten.com新的数据
UINT ThreadSavePotenData(CPotenDailyBriefingMarket* pMarket); // 此线程返回值为21
// 更新选项数据库
UINT ThreadAppendChoicedStockDB(CChinaMarket* pMarket); // 此线程返回值为22
