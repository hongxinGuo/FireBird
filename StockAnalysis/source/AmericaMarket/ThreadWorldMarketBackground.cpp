/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ChinaMarket�еĺ�ʱ�������������˴���
//
// ĿǰΪ��
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
		Sleep(50); // ���ټ��50ms
	}
	gl_ThreadStatus.SetWorldMarketBackground(false);
	return 201;
}