/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ChinaMarket�еĺ�ʱ�������������˴���
//
// ĿǰΪ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"
#include"WebInquirer.h"

UINT ThreadWorldMarketBackground(void) {
	gl_ThreadStatus.SetWorldMarketBackground(true);
	while (!gl_systemStatus.IsExitingSystem()) {
		Sleep(50); // ���ټ��50ms
	}
	gl_ThreadStatus.SetWorldMarketBackground(false);
	return 201;
}