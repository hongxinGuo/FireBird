/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ����ӿڶ�ȡ�̡߳�
//
// ��Ѷ�ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"WebInquirer.h"

UINT ThreadReadTengxunRTData(CTengxunRTWebInquiry* pTengxunRTWebData) {
  // ��Ѷʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  if (pTengxunRTWebData->ReadWebData(100, 30, 20)) {
    ASSERT(!pTengxunRTWebData->IsReadingWebData());
    CWebDataPtr pWebDataReceived = pTengxunRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushTengxunRTData(pWebDataReceived);
    }
  }
  else ASSERT(!pTengxunRTWebData->IsReadingWebData());
  gl_ThreadStatus.DecreaseNumberOfRunningThread();

  return 2;
}