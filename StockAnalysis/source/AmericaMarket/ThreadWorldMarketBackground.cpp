/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将ChinaMarket中的耗时计算任务移至此处。
//
// 目前为空
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"
#include"WebInquirer.h"

#include"Parse.h"

UINT ThreadWorldMarketBackground(void) {
	gl_ThreadStatus.SetWorldMarketBackground(true);
	while (!gl_fExitingSystem) {
		Sleep(50); // 最少间隔50ms
	}
	gl_ThreadStatus.SetWorldMarketBackground(false);
	return 201;
}