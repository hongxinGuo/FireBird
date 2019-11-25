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

UINT ThreadReadNeteaseDayLine(LPVOID pParam) {
  CNeteaseDayLineWebData* pNeteaseDayLineWebData = (CNeteaseDayLineWebData*)pParam;
  static int siDelayTime = 600;
  static bool fStarted = false;
  static atomic_int siCount = 0;   // ��ʼ������������ִ���˼��δ��̡߳������趨�ӳ�ʱ��
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pNeteaseDayLineWebData->GetBufferAddr();
  CString str;

  const clock_t tt = clock();
  ASSERT(pNeteaseDayLineWebData->IsReadingWebData());    // ���ô��߳�ʱ�Ѿ������˴˱�ʶ
  try {
    pNeteaseDayLineWebData->SetReadingWebData(true);
    pNeteaseDayLineWebData->SetReadingSucceed(true);
    pNeteaseDayLineWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pNeteaseDayLineWebData->GetInquiringString()));
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseDayLineWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ȥ�������Σ������������
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pNeteaseDayLineWebData->AddByteReaded(iCount);
      }
      else {
        Sleep(30); // �ȴ�30�����������Σ�ȷ��û�������ݺ�ŷ��أ������������
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseDayLineWebData->AddByteReaded(iCount);
        }
        else fDone = true;
      }
    }
    *pChar = 0x000; // ������һ��������0X000
    pNeteaseDayLineWebData->SetWebDataReceived(true);

    char* p = pNeteaseDayLineWebData->GetBufferAddr();
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(pNeteaseDayLineWebData->GetDownLoadingStockCode());
    if (pStock->m_pDayLineBuffer != nullptr) delete pStock->m_pDayLineBuffer;
    pStock->m_pDayLineBuffer = new char[pNeteaseDayLineWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    char* pbuffer = pStock->m_pDayLineBuffer;
    for (int i = 0; i < pNeteaseDayLineWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    pStock->m_lDayLineBufferLength = pNeteaseDayLineWebData->GetByteReaded();
    pStock->SetDayLineNeedProcess(true);
    gl_ChinaStockMarket.m_iDayLineNeedProcess++;
  }
  catch (CInternetException * e) {  // ���ִ���Ļ����򵥱����ɣ����账��
    e->Delete();
    pNeteaseDayLineWebData->SetReadingSucceed(false);
    pNeteaseDayLineWebData->SetWebDataReceived(false);
    pNeteaseDayLineWebData->SetByteReaded(0);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pNeteaseDayLineWebData->SetReadingWebData(false);
  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_ChinaStockMarket.SetReadingNeteaseDayDataTime(clock() - tt);

  return 2; // ���߳���������ֵΪ2
}