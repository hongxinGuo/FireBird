//////////////////////////////////////////////////////////////////////////////////////
//
// �洢��ʱʵʱ���ݹ����߳�
//
// ���߳������ʵʱ�����̻߳��⣬��Ҫע��֮��
//
// ����ֵΪ13
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"

UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

	gl_ProcessChinaMarketRTData.acquire();
	pMarket->SaveTempRTData();
	gl_ProcessChinaMarketRTData.release();

	return 13;
}

/// <summary>
/// ���߳���洢ʵʱ���ݵ��̻߳��⡣
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadProcessRTData(not_null<CChinaMarket*> pMarket) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->ProcessRTData();
	pMarket->ProcessTengxunRTData();
	gl_ProcessChinaMarketRTData.release();
	return 107;
}
