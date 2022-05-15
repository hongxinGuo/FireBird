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
#include"globedef.h"
#include"SystemMessage.h"
#include"ThreadStatus.h"
#include"ThreadStatus.h"
#include"ChinaMarket.h"

UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���
	ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �����������̻߳���

	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.SetSavingTempData(true);

	pMarket->UpdateTodayTempDB();

	ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �ٴ�ȷ��һ��
	gl_ThreadStatus.SetSavingTempData(false);
	gl_ThreadStatus.DecreaseSavingThread();

	return 13;
}