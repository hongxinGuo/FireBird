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

UINT ThreadSaveStockSection() {
	gl_UpdateChinaMarketDB.acquire();
	gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
	gl_UpdateChinaMarketDB.release();

	return 35;
}
