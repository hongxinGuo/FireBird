///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"
#include"CrweberIndex.h"

extern int gl_cMaxSavingOneDayLineThreads;

#ifdef __GOOGLEMOCK__
#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockCrweberIndexWebInquiry.h"
using namespace Testing;

extern CMockSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CMockTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CMockNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth; // 网易日线历史数据
extern CMockNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth; // 网易日线历史数据
extern CMockPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。
extern CMockCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
#else
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth; // 网易日线历史数据
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。
extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
#endif

class CWebInquirer : public CObject {
public:
  CWebInquirer(void);
  virtual ~CWebInquirer(void);

  void Initialize(void);
public:
  bool GetSinaRTData(void) { return gl_pSinaRTWebInquiry->GetWebData(); }
  bool GetTengxunRTData(void) { return gl_pTengxunRTWebInquiry->GetWebData(); }
  bool GetNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return gl_pCrweberIndexWebInquiry->GetWebData(); }
  bool IsReadingCrweberIndex(void) { return gl_pCrweberIndexWebInquiry->IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return gl_pPotenDailyBriefingWebInquiry->GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return gl_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

  bool IsReadingWebThreadRunning(void) noexcept { return gl_pSinaRTWebInquiry->IsReadingWebThreadRunning(); }

public:
  long GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetWebDataSize(); }
  void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushWebData(pData); }
  CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushWebData(pData); }
  CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushWebData(pData); }
  CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_qCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushWebData(pData); }
  CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopWebData(); }

  long GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetWebDataSize(); }
  void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushWebData(pData); }
  CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopWebData(); }

protected:
  CQueueWebData m_qSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_qTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_qNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_qCrweberWebData; // crweber.com网络数据暂存队列
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com网络数据暂存队列
};

extern CWebInquirer gl_WebInquirer; //
