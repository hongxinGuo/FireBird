/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ChinaMarket�еĺ�ʱ�������������˴���
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
		// ChinaMarket��̨����
		gl_pChinaMarket->TaskParseWebRTDataGetFromSinaServer(); // ��������ʵʱ����
		gl_pChinaMarket->TaskParseWebRTDataGetFromNeteaseServer(); // ��������ʵʱ����
		gl_pChinaMarket->TaskParseWebRTDataGetFromTengxunServer(); // ������Ѷʵʱ����

		Sleep(100); // ���ټ��100ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
}