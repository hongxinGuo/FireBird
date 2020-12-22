///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
// WebData��ָ�������ϵõ���ԭʼ���ݡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"
#include"CrweberIndex.h"

extern int gl_cMaxSavingOneDayLineThreads;

#ifdef __GOOGLEMOCK__
#include"MockSinaRTWebInquiry.h"
#include"MockSinaStakeRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
#include"MockFinnhubWebInquiry.h"
using namespace testing;

extern CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CMockSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
extern CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�
extern CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // ����������ʷ����
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3; // ����������ʷ����
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4; // ����������ʷ����
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5; // ����������ʷ����
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6; // ����������ʷ����
extern CMockFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
#else
#include"SinaRTWebInquiry.h"
#include"SinaStakeRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�
extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6; // ����������ʷ����
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
#endif

class CWebInquirer : public CObject {
public:
  CWebInquirer(void);
  virtual ~CWebInquirer(void);

  void Initialize(void);
public:
  bool GetQuandlData(void) { return gl_pQuandlWebInquiry->GetWebData(); }
  bool GetFinnhubData(void) { return gl_pFinnhubWebInquiry->GetWebData(); }
  bool GetSinaRTData(void) { return gl_pSinaRTWebInquiry->GetWebData(); }
  bool GetSinaStakeRTData(void) { return gl_pSinaStakeRTWebInquiry->GetWebData(); }
  bool GetTengxunRTData(void) { return gl_pTengxunRTWebInquiry->GetWebData(); }
  bool GetNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return gl_pCrweberIndexWebInquiry->GetWebData(); }
  bool IsReadingCrweberIndex(void) noexcept { return gl_pCrweberIndexWebInquiry->IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return gl_pPotenDailyBriefingWebInquiry->GetWebData(); }
  bool IsReadingPotenDailyBriefing(void)  noexcept { return gl_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

  bool IsReadingWebThreadRunning(void) noexcept { return gl_pSinaRTWebInquiry->IsReadingWebThreadRunning(); }

public:
  size_t GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetDataSize(); }
  void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
  CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }
  size_t GetSinaStakeRTDataSize(void) { return m_qSinaStakeRTWebData.GetDataSize(); }
  void PushSinaStakeRTData(CWebDataPtr pData) { m_qSinaStakeRTWebData.PushData(pData); }
  CWebDataPtr PopSinaStakeRTData(void) { return m_qSinaStakeRTWebData.PopData(); }
  size_t GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetDataSize(); }
  void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
  CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }
  size_t GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetDataSize(); }
  void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
  CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }
  size_t GetCrweberDataSize(void) { return m_qCrweberWebData.GetDataSize(); }
  void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushData(pData); }
  CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopData(); }

  size_t GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetDataSize(); }
  void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushData(pData); }
  CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopData(); }

  size_t GetFinnhubDataSize(void) { return m_qFinnhubData.GetDataSize(); }
  void PushFinnhubData(CWebDataPtr pData) { m_qFinnhubData.PushData(pData); }
  CWebDataPtr PopFinnhubData(void) { return m_qFinnhubData.PopData(); }

  size_t GetQuandlDataSize(void) { return m_qQuandlData.GetDataSize(); }
  void PushQuandlData(CWebDataPtr pData) { m_qQuandlData.PushData(pData); }
  CWebDataPtr PopQuandlData(void) { return m_qQuandlData.PopData(); }

  size_t GetTiingoDataSize(void) { return m_qTiingoData.GetDataSize(); }
  void PushTiingoData(CWebDataPtr pData) { m_qTiingoData.PushData(pData); }
  CWebDataPtr PopTiingoData(void) { return m_qTiingoData.PopData(); }

protected:
  CQueueWebData m_qSinaRTWebData; // ���˹�Ʊ���������ݴ����
  CQueueWebData m_qSinaStakeRTWebData; // ����֤ȯ���������ݴ����
  CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
  CQueueWebData m_qFinnhubData; // Poten.com���������ݴ����
  CQueueWebData m_qQuandlData; // Poten.com���������ݴ����
  CQueueWebData m_qTiingoData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebInquirer; //
