////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ����������ʷ���ݵ��̡߳�
//
// ��ȡ����ʱ���Լ��Σ���ֹ�����ӳٵ������ݽضϡ�Ŀǰ�ڶ������ݺ��ڲ������Σ���û�����ݺ�ŷ��ء�
//
// ��ʱfReading == false�Ķ��Ի�ʧ�ܣ���������ֻ���������ô˱������ʹ����ǲ�ͬ�߳�ͬʱ������ԭ����Ҫ��Ϊ
// ͬ���¼�ģʽ���Ѵ��߳�Ϊ�á��о�֮�����ڵ��ô��߳�ǰ�����ã��Ͳ�����ֹ����ˣ��ɼ�ʱ�����߳�ʱ�������ʱ���£���
//
// ���߳����յĹ��ܣ�Ҫ������ֻ����ȡ����Ҫ��һЩ��Ӧ�ü������ʹ洢���ܣ��о�֮�����ܶ���漰���������ͬ�����⣬��ҪȨ�⣩��
// �༴���̵߳Ĺ���Ϊ��
// 1.�ӹܵ�����ȡ��Ҫ��ȡ������ʷ���ݵĹ�Ʊ���룬���ô��봦����������߷�������Ҫ��ĸ�ʽ��Ȼ���͸����߷�������
// 2.�ȴ�һ��ʱ���100ms����ʼ�ӷ��������������ݡ�
// 3.�����յ������ݽ��룬�ɹ��Ļ�������Ӧ��Ʊ�����������У����������Ӧ�ı�ʶ��
//
// ���̵߳���������ֵΪ2
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadNeteaseDayLine(LPVOID) {
  static int siDelayTime = 600;
  static bool fStarted = false;
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_NeteaseDayLineWebData.GetBufferAddr();
  CString str;

  const clock_t tt = clock();
  ASSERT(gl_NeteaseDayLineWebData.IsReadingWebData());    // ���ô��߳�ʱ�Ѿ������˴˱�ʶ
  try {
    gl_NeteaseDayLineWebData.SetReadingWebData(true);
    gl_NeteaseDayLineWebData.SetReadingSucceed(true);
    gl_NeteaseDayLineWebData.SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_NeteaseDayLineWebData.GetInquiringString()));
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_NeteaseDayLineWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ȥ�������Σ������������
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_NeteaseDayLineWebData.AddByteReaded(iCount);
      }
      else {
        Sleep(30); // �ȴ�30�����������Σ�ȷ��û�������ݺ�ŷ��أ������������
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_NeteaseDayLineWebData.AddByteReaded(iCount);
        }
        else fDone = true;
      }
    }
    *pChar = 0x000;
    gl_NeteaseDayLineWebData.SetWebDataReceived(true);
  }
  catch (CInternetException * e) {  // ���ִ���Ļ����򵥱����ɣ����账��
    e->Delete();
    gl_NeteaseDayLineWebData.SetReadingSucceed(false);
    gl_NeteaseDayLineWebData.SetWebDataReceived(false);
    gl_NeteaseDayLineWebData.SetByteReaded(0);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_NeteaseDayLineWebData.SetReadingWebData(false);
  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_ChinaStockMarket.SetReadingNeteaseDayDataTime(clock() - tt);

  return 2; // ���߳���������ֵΪ2
}