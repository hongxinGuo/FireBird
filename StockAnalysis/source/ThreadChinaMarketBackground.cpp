/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将ChinaMarket中的耗时计算任务移至此处。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"

UINT ThreadChinaMarketBackground(void) {
	gl_ThreadStatus.SetChinaMarketBackground(true);
	while (!gl_fExitingSystem) {
		// ChinaMarket后台任务
		gl_pChinaMarket->TaskParseWebRTDataGetFromSinaServer(); // 解析新浪实时数据
		gl_pChinaMarket->TaskParseWebRTDataGetFromNeteaseServer(); // 解析网易实时数据
		gl_pChinaMarket->TaskParseWebRTDataGetFromTengxunServer(); // 解析腾讯实时数据

		Sleep(100); // 最少间隔100ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
}