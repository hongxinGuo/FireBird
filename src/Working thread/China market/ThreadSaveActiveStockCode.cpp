///////////////////////////////////////////////////////////////////////////////////
//
// �洢���ҵ���֤ȯ���������ݿ�
// ���̵߳���ThreadBuildDayLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

UINT ThreadSaveStockSection() {
	gl_UpdateChinaMarketDB.acquire();
	gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
	gl_UpdateChinaMarketDB.release();

	return 35;
}
