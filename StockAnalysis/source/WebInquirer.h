///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"

#include"CrweberIndex.h"

extern int gl_cMaxSavingOneDayLineThreads;

class CWebInquirer : public CObject {
public:
  CWebInquirer(void);
  virtual ~CWebInquirer(void);

public:
  bool GetSinaRTData(void) { return m_pSinaRTWebInquiry->GetWebData(); }
  bool GetTengxunRTData(void) { return m_pTengxunRTWebInquiry->GetWebData(); }
  bool GetNeteaseRTData(void) { return m_pNeteaseRTWebInquiry->GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return m_pCrweberIndexWebInquiry->GetWebData(); }
  bool IsReadingCrweberIndex(void) { return m_pCrweberIndexWebInquiry->IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return m_pPotenDailyBriefingWebInquiry->GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return m_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

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

  bool IsReadingWebThreadRunning(void) noexcept { return m_pSinaRTWebInquiry->IsReadingWebThreadRunning(); }

protected:
  CSinaRTWebInquiryPtr m_pSinaRTWebInquiry; // 新浪实时数据采集
  CTengxunRTWebInquiryPtr m_pTengxunRTWebInquiry; // 腾讯实时数据采集
  CNeteaseRTWebInquiryPtr m_pNeteaseRTWebInquiry; // 网易实时数据采集
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiry; // 网易日线历史数据
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquirySecond; // 网易日线历史数据
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryThird; // 网易日线历史数据
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryFourth; // 网易日线历史数据
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryFifth; // 网易日线历史数据
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquirySixth; // 网易日线历史数据
  CCrweberIndexWebInquiryPtr m_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
  CPotenDailyBriefingWebInquiryPtr m_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。

  CQueueWebData m_qSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_qTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_qNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_qCrweberWebData; // crweber.com网络数据暂存队列
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com网络数据暂存队列
};

extern CWebInquirer gl_WebInquirer; //
