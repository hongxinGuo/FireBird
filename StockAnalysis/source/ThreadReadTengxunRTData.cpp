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
#include"Market.h"

UINT ThreadReadTengxunRTData(LPVOID pParam) {
  CTengxunRTWebData* pTengxunWebRTData = (CTengxunRTWebData*)pParam;

  // ��Ѷʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  if (pTengxunWebRTData->ReadWebData(100, 50, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pTengxunWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_queueWebInquire.PushTengxunRTData(pWebDataReceived);
    }
  }

  return 2;
}