///////////////////////////////////////////////////////////////////////////////////
//
// �洢���ҵ���֤ȯ���������ݿ�
// ���̵߳���ThreadBuildDayLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadSaveActiveStakeCode(not_null<CChinaMarket*> pMarket) {
  pMarket->SaveActiveStakeCode();

  return 34;
}

UINT ThreadSaveStakeSection(not_null<CChinaMarket*> pMarket) {
  pMarket->SaveStakeSection();

  return 35;
}