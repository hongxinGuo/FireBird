///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
// WebData意指从网络上得到的原始数据。
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

extern CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CMockSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry; // 新浪实时数据采集
extern CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。
extern CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6; // 网易日线历史数据
extern CMockFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
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

extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CSinaStakeRTWebInquiryPtr gl_pSinaStakeRTWebInquiry; // 新浪实时数据采集
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。
extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry3; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry4; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry5; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry6; // 网易日线历史数据
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
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
  CQueueWebData m_qSinaRTWebData; // 新浪股票网络数据暂存队列
  CQueueWebData m_qSinaStakeRTWebData; // 新浪证券网络数据暂存队列
  CQueueWebData m_qTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_qNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_qCrweberWebData; // crweber.com网络数据暂存队列
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com网络数据暂存队列
  CQueueWebData m_qFinnhubData; // Poten.com网络数据暂存队列
  CQueueWebData m_qQuandlData; // Poten.com网络数据暂存队列
  CQueueWebData m_qTiingoData; // Poten.com网络数据暂存队列
};

extern CWebInquirer gl_WebInquirer; //
