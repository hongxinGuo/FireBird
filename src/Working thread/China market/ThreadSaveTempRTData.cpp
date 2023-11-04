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

/// <summary>
/// 
/// ���߳������ʵʱ���ݵ��̻߳���
/// 
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

	gl_UpdateChinaMarketDB.acquire();
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->SaveTempRTData();
	gl_ProcessChinaMarketRTData.release();
	gl_UpdateChinaMarketDB.release();

	return 13;
}

UINT ThreadLoadTempRTData(not_null<CChinaMarket*> pMarket, long lTheDate) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->LoadTempRTData(lTheDate);
	gl_ProcessChinaMarketRTData.release();

	return 19;
}

/// <summary>
/// ���߳���洢ʵʱ���ݵ��̻߳��⡣
///
/// ��Ϊ����ʵʱ���ݵĽ��̣���Ҫ���������ٶȣ��������⻯����õ�DistributeAndCalculateRTData���ʶ������Դ˽���
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadDistributeAndCalculateRTData(not_null<CChinaMarket*> pMarket) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->DistributeAndCalculateRTData();
	gl_ProcessChinaMarketRTData.release();
	return 108;
}
