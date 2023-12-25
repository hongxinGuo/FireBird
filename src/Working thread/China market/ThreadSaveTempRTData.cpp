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
#include "HighPerformanceCounter.h"

/// <summary>
/// 
/// ���߳������ʵʱ���ݵ��̻߳���
/// 
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadSaveTempRTData() {
	gl_UpdateChinaMarketDB.acquire();
	gl_ProcessChinaMarketRTData.acquire();
	gl_containerChinaStock.SaveTempRTData();
	gl_ProcessChinaMarketRTData.release();
	gl_UpdateChinaMarketDB.release();

	return 13;
}

UINT ThreadLoadTempRTData(const CChinaMarketPtr& pMarket, long lTheDate) {
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
UINT ThreadDistributeAndCalculateRTData(const CChinaMarketPtr& pMarket) {
	gl_ProcessChinaMarketRTData.acquire();
	CHighPerformanceCounter counter;
	counter.start();

	pMarket->DistributeRTData();
	pMarket->CalculateRTData();

	counter.stop();
	pMarket->SetDistributeAndCalculateTime(counter.GetElapsedMillisecond());
	gl_ProcessChinaMarketRTData.release();
	return 108;
}
