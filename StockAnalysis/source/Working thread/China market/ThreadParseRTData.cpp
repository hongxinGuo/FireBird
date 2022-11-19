/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将ChinaMarket中的耗时计算任务移至此处。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"SaveAndLoad.h"

#include"ThreadStatus.h"
#include"JsonParse.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"
#include"WebInquirer.h"

#include"WebRTDataContainer.h"

using namespace std;
#include<chrono>

UINT ThreadChinaMarketBackground(void) {
	gl_ThreadStatus.SetChinaMarketBackground(true);
	while (!gl_systemStatus.IsExitingSystem()) {
		ASSERT(gl_WebInquirer.SinaRTDataSize() == 0);
		ASSERT(gl_WebInquirer.NeteaseRTDataSize() == 0);
		ASSERT(gl_WebInquirer.TengxunRTDataSize() == 0);
		ASSERT(gl_WebInquirer.NeteaseDayLineDataSize() == 0);
		// 下面的函数都移至相应的Product中，不再使用了。
		// 网易实时数据的接收加上处理时间超过了400毫秒，导致接收频率降低。故而将处理任务移至此工作线程中。
		// 此四个任务比较费时，尤其是网易实时数据解析时需要使用json解析器，故而放在此独立线程中。
		// 分析计算具体挂单状况的函数，也应该移至此工作线程中。研究之。
		//ParseSinaData(); // 解析新浪实时数据
		//ParseNeteaseRTDataWithNlohmannJSon(); // 使用nlohmann json解析网易实时数据
		//ParseNeteaseRTDataWithPTree(); // 使用 perproty tree解析网易实时数据
		//ParseTengxunRTData(); // 解析腾讯实时数据
		//ParseDayLineGetFromNeteaseServer();
		Sleep(50); // 最少间隔50ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
}