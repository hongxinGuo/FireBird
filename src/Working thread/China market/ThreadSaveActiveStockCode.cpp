///////////////////////////////////////////////////////////////////////////////////
//
// �洢���ҵ���֤ȯ���������ݿ�
// ���̵߳���ThreadBuildDayLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"
#include"ChinaMarket.h"

UINT ThreadSaveStockSection(not_null<CChinaMarketPtr> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->SaveStockSection();
	gl_UpdateChinaMarketDB.release();

	return 35;
}
