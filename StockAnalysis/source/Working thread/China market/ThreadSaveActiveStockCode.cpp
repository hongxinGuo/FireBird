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

using namespace std;
#include<thread>

UINT ThreadSaveStockSection(not_null<CChinaMarket*> pMarket) {
	pMarket->SaveStockSection();

	return 35;
}