#include"pch.h"

#include "VirtualWebInquiry.h"
//#include"ThreadStatus.h"
//#include"Thread.h"

//#include"InfoReport.h"
//#include"HighPerformanceCounter.h"

//#include"ChinaMarket.h"
#include"curl/curl.h"

#include<thread>

using std::thread;

bool CVirtualWebInquiry::GetWebData2(void) {
	// todo 使用libcurl的curl_multi_perform(),看看速度如何
	return true;
}
