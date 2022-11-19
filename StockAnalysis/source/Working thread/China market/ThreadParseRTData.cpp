/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ChinaMarket�еĺ�ʱ�������������˴���
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
		// ����ĺ�����������Ӧ��Product�У�����ʹ���ˡ�
		// ����ʵʱ���ݵĽ��ռ��ϴ���ʱ�䳬����400���룬���½���Ƶ�ʽ��͡��ʶ����������������˹����߳��С�
		// ���ĸ�����ȽϷ�ʱ������������ʵʱ���ݽ���ʱ��Ҫʹ��json���������ʶ����ڴ˶����߳��С�
		// �����������ҵ�״���ĺ�����ҲӦ�������˹����߳��С��о�֮��
		//ParseSinaData(); // ��������ʵʱ����
		//ParseNeteaseRTDataWithNlohmannJSon(); // ʹ��nlohmann json��������ʵʱ����
		//ParseNeteaseRTDataWithPTree(); // ʹ�� perproty tree��������ʵʱ����
		//ParseTengxunRTData(); // ������Ѷʵʱ����
		//ParseDayLineGetFromNeteaseServer();
		Sleep(50); // ���ټ��50ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
}